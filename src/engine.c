#include "SDL_video.h"
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <stdio.h>

//#define ARDUINO_ON

#ifdef ARDUINO_ON
#include "arduino.h"
#endif
#include "gui.h"
#include "engine.h"
#include "config.h"
#include "img.h"
#include "sound.h"
#include "tools.h"
#include "debugging.h"
#include "text.h"

bool INSTANCE = true;
int last_frame = 0;
float deltatime = 0.0f;
SDL_Renderer *render = NULL;
SDL_Window *window = NULL;
SDL_Color renderColor = {
	.r = 0,
	.g = 0,
	.b = 0,
	.a = 255
};

int MouseX = 0;
int MouseY = 0;

TTF_Font *font = NULL;

Text txtFPS;
Text txtMouse;

bool Game_Init()
{
	//Cargar configuracion.
	if(loadConfig(&config, CONFIG_DIR CFG_FILE) != true)
		return false;

	Uint32 windowFlags = SDL_WINDOW_RESIZABLE | (config.fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
	//Iniciar SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printDebug("No se pudo iniciar SDL: %s\n", SDL_GetError());
		return false;
	}
	//Iniciar textura y audio.
	initTexture();
    initAudio();
	//Si la cantidad de monitores es menor al monitor que apunta el window, entonces el default es 0
	if(SDL_GetNumVideoDisplays() < config.defaultMonitor)
		config.defaultMonitor = 0;

	//Iniciar ventana.
	window = SDL_CreateWindow(config.name, SDL_WINDOWPOS_CENTERED_DISPLAY(config.defaultMonitor), SDL_WINDOWPOS_CENTERED_DISPLAY(config.defaultMonitor), config.WIN_W, config.WIN_H, windowFlags);
	if (!window)
	{
		printDebug("No se pudo crear ventana: %s\n", SDL_GetError());
		return false;
	}
	//Iniciar render.
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!render)
	{
		printDebug("No se pudo crear render: %s\n", SDL_GetError());
		return false;
	}
	// Establecer resolucion logica para escalado simetrico.
	SDL_RenderSetLogicalSize(render, config.WIN_W, config.WIN_H);
	if (TTF_Init() == -1)
	{
		printDebug("No se pudo iniciar TTF: %s\n", TTF_GetError());
		return false;
	}
	//Iniciar sistema de texto.
	if (!Text_InitSystem(FONTS_DIR LCD_FONT, 24))
		return false;
	//Iniciar GUI (Nuklear).
	if (!GUI_Init(window, render, FONTS_DIR VT_FONT, 30))
	{
		printDebug("No se pudo iniciar GUI\n");
		return false;
	}
	#ifdef ARDUINO_ON
	if (!arduinoConnect())
	{
		printDebug("No se pudo conectar con Arduino (continuando sin el)\n");
	}
	#endif
	return true;
}

void Game_Setup()
{
	txtFPS = Text_Create("FPS: 0", 10, 10);
	txtMouse = Text_Create("Mouse: 0, 0", 1080, 40);
}
void Game_KeyboardInput()
{
	SDL_Event event;
	GUI_InputBegin();
	while (SDL_PollEvent(&event))
	{
		GUI_HandleEvent(&event);
		switch (event.type)
		{
			case(SDL_QUIT):
			{
				INSTANCE = false;
				break;
			}
			case(SDL_KEYDOWN):
			{
				SDL_Keycode KEY = event.key.keysym.sym;
				keyEventFrameDebug(KEY);

				if(KEY == SDLK_ESCAPE)
					INSTANCE = false;
				if(KEY == SDLK_F11)
				{
					config.fullscreen = !config.fullscreen;
					SDL_SetWindowFullscreen(window, config.fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
				}
				break;
			}
			case(SDL_KEYUP):
			{
				break;
			}
			case(SDL_MOUSEWHEEL):
			{
				break;
			}
			default:
				break;
		}
	}
	GUI_InputEnd();
}
void Game_UpdateFrame()
{
	Uint32 actualTime = SDL_GetTicks();
	deltatime = (actualTime - last_frame) / 1000.0f;
	last_frame = actualTime;


	int WaitTime = FRAME_TIME_MS(config.fps) - (SDL_GetTicks() - actualTime);
	if (WaitTime > 0 && WaitTime <= FRAME_TIME_MS(config.fps))
		SDL_Delay(WaitTime);
}

void Game_Render()
{
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderClear(render);

	SDL_GetMouseState(&MouseX, &MouseY);

	renderFrameDebug();

	// Actualizar textos (solo re-renderiza si cambian)
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "FPS: %d", (int)(1.0f / deltatime));
	Text_Set(&txtFPS, buffer);

	snprintf(buffer, sizeof(buffer), "Mouse: %d, %d", MouseX, MouseY);
	Text_Set(&txtMouse, buffer);

	// Dibujar textos (1 linea cada uno)
	Text_Draw(&txtFPS);
	Text_Draw(&txtMouse);

	GUI_Render();
	SDL_RenderPresent(render);
}

void Game_Destroy()
{
	Text_Free(&txtFPS);
	Text_Free(&txtMouse);
	Text_QuitSystem();
	exitFrameDebug();
	#ifdef ARDUINO_ON
	arduinoDisconnect();
	#endif

	GUI_Destroy();

	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);

	quitTexture();
	quitAudio();
	SDL_Quit();
}