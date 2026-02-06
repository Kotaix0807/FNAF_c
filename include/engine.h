#ifndef ENGINE_H
#define ENGINE_H

#include "SDL_pixels.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdbool.h>

extern bool INSTANCE;
extern int last_frame;
extern float deltatime;
extern SDL_Renderer *render;
extern SDL_Window *window;
extern SDL_Color renderColor;

extern int MouseX;
extern int MouseY;

extern TTF_Font *font;

bool Game_Init();
void Game_Setup();
void Game_KeyboardInput();
void Game_UpdateFrame();
void Game_Render();
void Game_Destroy();

#endif