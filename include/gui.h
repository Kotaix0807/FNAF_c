#ifndef GUI_H
#define GUI_H

#include <SDL.h>
#include <stdbool.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "nuklear.h"
#pragma GCC diagnostic pop

struct nk_context;

bool GUI_Init(SDL_Window *win, SDL_Renderer *ren, const char *font_path, float font_size);
int GUI_HandleEvent(SDL_Event *event);
void GUI_InputBegin(void);
void GUI_InputEnd(void);
void GUI_Render(void);
void GUI_Destroy(void);
struct nk_context* GUI_GetContext(void);

#endif
