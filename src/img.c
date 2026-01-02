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
	if(!textures_array)
	{
		printDebug("No se pudo crear la libreria de texturas en '%s'\n", path);
		return current;
	}

	current.array_textures = malloc(n * sizeof(SDL_Texture *));
	if(!current.array_textures)
	{
		printDebug("No se pudo asignar memoria para texturas\n");
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
		current.array_textures[i] = SDL_CreateTextureFromSurface(render, srf);
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
		if(txr->array_textures[i])
			SDL_DestroyTexture(txr->array_textures[i]);
}

void quitTexture(void)
{
	IMG_Quit();
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

//texture *initTextreLib(char *path);
//void freeTextreLib(texture *cur);

#ifdef IMG_DEBUG
int main()
{

	return 0;
}
#endif