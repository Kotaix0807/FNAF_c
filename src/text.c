#define _POSIX_C_SOURCE 200809L
#include "text.h"
#include "engine.h"
#include "tools.h"
#include <string.h>
#include <stdlib.h>

static TTF_Font *defaultFont = NULL;
static SDL_Color defaultColor = {255, 255, 255, 255};

bool Text_InitSystem(const char *fontPath, int defaultSize)
{
    defaultFont = TTF_OpenFont(fontPath, defaultSize);
    if (!defaultFont)
    {
        printDebug("No se pudo cargar fuente: %s\n", TTF_GetError());
        return false;
    }
    return true;
}

void Text_QuitSystem(void)
{
    if (defaultFont)
    {
        TTF_CloseFont(defaultFont);
        defaultFont = NULL;
    }
    TTF_Quit();
}

// Función interna para renderizar texto a textura
static void Text_Render(Text *text)
{
    if (text->texture)
    {
        SDL_DestroyTexture(text->texture);
        text->texture = NULL;
    }

    if (!text->content || text->content[0] == '\0')
        return;

    SDL_Surface *surface = TTF_RenderUTF8_Blended(text->font, text->content, text->color);
    if (!surface)
        return;

    text->texture = SDL_CreateTextureFromSurface(render, surface);
    text->rect.w = surface->w;
    text->rect.h = surface->h;
    SDL_FreeSurface(surface);
}

Text Text_Create(const char *content, int x, int y)
{
    return Text_CreateColored(content, x, y, defaultColor);
}

Text Text_CreateColored(const char *content, int x, int y, SDL_Color color)
{
    Text text = {0};
    text.rect.x = x;
    text.rect.y = y;
    text.color = color;
    text.font = defaultFont;

    if (content)
    {
        text.content = strdup(content);
        Text_Render(&text);
    }

    return text;
}

void Text_Set(Text *text, const char *content)
{
    // Si el contenido es igual, no hacer nada (optimización clave)
    if (text->content && content && strcmp(text->content, content) == 0)
        return;

    free(text->content);
    text->content = content ? strdup(content) : NULL;
    Text_Render(text);
}

void Text_Draw(Text *text)
{
    if (text->texture)
        SDL_RenderCopy(render, text->texture, NULL, &text->rect);
}

void Text_Free(Text *text)
{
    if (text->texture)
        SDL_DestroyTexture(text->texture);
    free(text->content);
    text->texture = NULL;
    text->content = NULL;
}
