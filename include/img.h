#ifndef IMG_H
#define IMG_H

#include <SDL_image.h>
#include <stdbool.h>

typedef struct texture_{
    SDL_Texture **array_textures;
    int n;
}texture;

bool initTexture(void);
void quitTexture(void);

texture initTextureLib(char *path);
void freeTextureLib(texture *txr);

#endif
