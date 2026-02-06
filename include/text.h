#ifndef TEXT_H
#define TEXT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>

#define ARCADE_FONT "PressStart2P-Regular.ttf"
#define LCD_FONT "LcdSolid.ttf"
#define VT_FONT "VT323-Regular.ttf"
// Estructura de texto con cache de textura
typedef struct {
    SDL_Texture *texture;   // Textura cacheada
    SDL_Rect rect;          // Posición y tamaño
    SDL_Color color;        // Color del texto
    TTF_Font *font;         // Fuente usada
    char *content;          // Texto actual (para comparar)
} Text;

// Inicializa el sistema de texto (cargar fuente por defecto)
bool Text_InitSystem(const char *fontPath, int defaultSize);

// Cierra el sistema de texto
void Text_QuitSystem(void);

// Crea un texto en una posición
Text Text_Create(const char *content, int x, int y);

// Crea un texto con color personalizado
Text Text_CreateColored(const char *content, int x, int y, SDL_Color color);

// Actualiza el contenido (solo re-renderiza si cambia)
void Text_Set(Text *text, const char *content);

// Dibuja el texto en pantalla
void Text_Draw(Text *text);

// Libera recursos del texto
void Text_Free(Text *text);

#endif
