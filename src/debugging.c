#include <stdbool.h>
#include <SDL_render.h>

#include "SDL_video.h"
#include "text.h"
#include "debugging.h"
#include "engine.h"
#include "config.h"
#include "img.h"

#include "gui.h"

FrameDebugState fdState = FDEBUG_OFF;
int inputImageNum = 0;
int inputFrameW = 16;
int inputFrameH = 16;

// Definiciones de variables globales declaradas en sprites.h
//bool frameDebug = false;
SDL_Rect *framePointer = NULL;
texture sprites = {0};
Text txtXY;

// Configuracion original de la ventana (para restaurar al salir del debug)
static int originalWinW = 0;
static int originalWinH = 0;

static void activateFrameDebug(void)
{
    framePointer = malloc(sizeof(SDL_Rect));
    if (!framePointer)
        return;

    framePointer->x = 0;
    framePointer->y = 0;
    framePointer->w = inputFrameW;
    framePointer->h = inputFrameH;

    txtXY = Text_Create("X: 0, Y: 0", 10, 40);

    SDL_SetWindowSize(window, config.WIN_W, config.WIN_H);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED_DISPLAY(config.defaultMonitor), SDL_WINDOWPOS_CENTERED_DISPLAY(config.defaultMonitor));
    //SDL_RenderSetLogicalSize(render, config.WIN_W, config.WIN_H);

    fdState = FDEBUG_ACTIVE;
}

void loadFrameDebug()
{
    if (fdState != FDEBUG_OFF)
    {
        exitFrameDebug();
        return;
    }

    sprites = initTextureLib(SPRITES_DIR);
    if (sprites.n <= 0)
        return;

    inputImageNum = 0;
    inputFrameW = 16;
    inputFrameH = 16;

    fdState = FDEBUG_SETUP;
}

void keyEventFrameDebug(SDL_Keycode key)
{
    if (key == SDLK_F1)
    {
        loadFrameDebug();
        return;
    }
    if (fdState != FDEBUG_ACTIVE)
        return;

    int maxX = sprites.rects[inputImageNum]->w - framePointer->w;
    int maxY = sprites.rects[inputImageNum]->h - framePointer->h;

    if(key == SDLK_LEFT)
        framePointer->x -= framePointer->w;
    if(key == SDLK_RIGHT)
        framePointer->x += framePointer->w;
    if(key == SDLK_UP)
        framePointer->y -= framePointer->h;
    if(key == SDLK_DOWN)
        framePointer->y += framePointer->h;

    // Wrap-around para X
    if(framePointer->x < 0)
        framePointer->x = maxX;
    else if(framePointer->x > maxX)
        framePointer->x = 0;

    // Wrap-around para Y
    if(framePointer->y < 0)
        framePointer->y = maxY;
    else if(framePointer->y > maxY)
        framePointer->y = 0;
}

void exitFrameDebug()
{
    if (framePointer)
    {
        free(framePointer);
        framePointer = NULL;
    }
    freeTextureLib(&sprites);

    if (fdState == FDEBUG_ACTIVE)
        Text_Free(&txtXY);

    // Restaurar configuracion original
    if (originalWinW > 0 && originalWinH > 0)
    {
        config.WIN_W = originalWinW;
        config.WIN_H = originalWinH;
        SDL_RestoreWindow(window);
        SDL_SetWindowSize(window, config.WIN_W, config.WIN_H);
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED_DISPLAY(config.defaultMonitor), SDL_WINDOWPOS_CENTERED_DISPLAY(config.defaultMonitor));
        SDL_RenderSetLogicalSize(render, config.WIN_W, config.WIN_H);
    }

    fdState = FDEBUG_OFF;
}

void renderFrameDebug()
{
    if (fdState == FDEBUG_OFF)
        return;

    if (fdState == FDEBUG_SETUP)
    {
        struct nk_context *ctx = GUI_GetContext();
        if (nk_begin(ctx, "Frame Debug Setup",
                     nk_rect(0, 0, 400, 320),
                     NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_TITLE | NK_WINDOW_CLOSABLE | NK_WINDOW_SCALABLE))
        {
            nk_layout_row_dynamic(ctx, 30, 1);
            nk_property_int(ctx, "Image #:", 0, &inputImageNum, sprites.n - 1, 1, 1);

            nk_layout_row_dynamic(ctx, 30, 1);
            nk_property_int(ctx, "Frame W:", 1, &inputFrameW, 512, 1, 1);

            nk_layout_row_dynamic(ctx, 30, 1);
            nk_property_int(ctx, "Frame H:", 1, &inputFrameH, 512, 1, 1);
            if (nk_button_label(ctx, "Start"))
                activateFrameDebug();
        }
        else
        {
            // Ventana cerrada con X
            freeTextureLib(&sprites);
            fdState = FDEBUG_OFF;
        }
        nk_end(ctx);
        return;
    }

    // FDEBUG_ACTIVE: dibujar la imagen y el rectangulo
    drawImage(0, 0, -1, -1, sprites.textures_array[inputImageNum]);
    renderRect(framePointer, 255, 0, 0, 255);

    char buffer[32];
    if (framePointer != NULL) {
        snprintf(buffer, sizeof(buffer), "X: %d, Y: %d",
                 framePointer->x / framePointer->w,
                 framePointer->y / framePointer->h);
        Text_Set(&txtXY, buffer);
    }
    Text_Draw(&txtXY);
}