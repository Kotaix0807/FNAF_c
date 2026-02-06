#include "img.h"
#include "engine.h"
#include "tools.h"
#include <stdio.h>
#include <stdlib.h>

//#define IMG_DEBUG

bool initTexture(void)
{
    if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		printDebug("No se pudo iniciar IMG: %s\n", SDL_GetError());
		return false;
	}
    return true;
}

static const char *imageExtensions[] = {".png", ".jpg", ".jpeg", ".bmp"};

texture initTextureLib(char *path)
{
	texture current = {0};
	int n = 0;

	char **textures_array = getFilesFromDir(path, &n, imageExtensions, ARRAY_L(imageExtensions), IMAGE);
	if(!textures_array || n <= 0)
	{
		printDebug("No se pudo crear la libreria de texturas en '%s'\n", path);
		return current;
	}

	current.textures_array = malloc(n * sizeof(SDL_Texture *));
	current.rects = malloc(n * sizeof(SDL_Rect *));
	if(!current.textures_array)
	{
		printDebug("No se pudo asignar memoria para texturas\n");
		freeStringArray(textures_array, n);
		return current;
	}
	if(!current.rects)
	{
		printDebug("No se pudo asignar memoria para rectangulos\n");
		freeStringArray(textures_array, n);
		return current;
	}
	current.n = n;

	for (int i = 0; i < n; i++)
	{	
		char image_path[strlen(path) + strlen(textures_array[i]) + 1];
		snprintf(image_path, sizeof(image_path), "%s%s", path, textures_array[i]);
		SDL_Surface *srf = IMG_Load(image_path);
		if(!srf)
		{
			printDebug("No se pudo cargar la imagen '%s'\n", textures_array[i]);
			freeTextureLib(&current);
			break;
		}
		current.textures_array[i] = SDL_CreateTextureFromSurface(render, srf);
		current.rects[i] = malloc(sizeof(SDL_Rect));
		if(!current.rects[i])
		{
			printDebug("No se pudo asignar memoria para el rectangulo %d\n", i);
			SDL_FreeSurface(srf);
			freeTextureLib(&current);
			break;
		}
		assignRectToTexture(current.textures_array[i], current.rects[i]);
		SDL_FreeSurface(srf);
	}
	freeStringArray(textures_array, n);
	return current;
}

void freeTextureLib(texture *txr)
{
	if(!txr)
		return;
	for(int i = 0; i < txr->n; i++)
	{
		if(txr->textures_array && txr->textures_array[i])
			SDL_DestroyTexture(txr->textures_array[i]);
		if(txr->rects && txr->rects[i])
			free(txr->rects[i]);
	}
	free(txr->textures_array);
	free(txr->rects);
	txr->textures_array = NULL;
	txr->rects = NULL;
	txr->n = 0;
}

void quitTexture(void)
{
	IMG_Quit();
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void assignRectToTexture(SDL_Texture *texture, SDL_Rect *rect)
{
	int w = 0, h = 0;
	if(!rect || !texture)
	{
		printDebug("Error asignando tamanho al rectangulo\n");
		return;
	}
	GetTextureSize(texture, &w, &h);
	if(w <= 0 || h <= 0)
	{
		printDebug("Error, el tamanho del rectangulo a asignar no es valido\n");
		return;
	}
	rect->x = 0;
	rect->y = 0;
	rect->w = w;
	rect->h = h;
}

void drawImageF(float x, float y, float w, float h, SDL_Texture *texture)
{
	if(!texture)
	{
		printDebug("Error, no se pudo dibujar la textura\n");
		return;
	}
	int w_, h_;
	GetTextureSize(texture, &w_, &h_);
	if(w <= 0 || h <= 0)
	{
		w = w_;
		h = h_;
	}
	SDL_Rect srcrect = {
		.x = 0,
		.y = 0,
		.w = w_,
		.h = h_
	};
	SDL_FRect destrect;
	destrect.w = w;
	destrect.h = h;
	destrect.x = x;
	destrect.y = y;
	SDL_RenderCopyF(render, texture, &srcrect, &destrect);
}

void drawImage(int x, int y, int w, int h, SDL_Texture *texture)
{
	if(!texture)
	{
		printDebug("Error, no se pudo dibujar la textura\n");
		return;
	}
	int w_, h_;
	GetTextureSize(texture, &w_, &h_);
	if(w <= 0 || h <= 0)
	{
		w = w_;
		h = h_;
	}
	SDL_Rect srcrect = {
		.x = 0,
		.y = 0,
		.w = w_,
		.h = h_
	};
	SDL_Rect destrect;
	destrect.w = w;
	destrect.h = h;
	destrect.x = x;
	destrect.y = y;
	SDL_RenderCopy(render, texture, &srcrect, &destrect);
}

void renderRect(SDL_Rect *rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(render, r, g, b, a);
	SDL_RenderDrawRect(render, rect);
}


#ifdef IMG_DEBUG
int main()
{

	return 0;
}
#endif