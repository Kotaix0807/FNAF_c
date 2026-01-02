#ifndef SOUND_H
#define SOUND_H

#include <SDL_mixer.h>
#include <SDL.h>
#include <stdbool.h>

typedef struct sounds_{
    Mix_Chunk **chunks;
    int n;
}sfx;

typedef struct songs{
    Mix_Music **music;
    int n;
}music;

bool initAudio(void);
void quitAudio(void);

void playAndFreeSfx(const char *sound);

sfx *initSfxLib(char *path);
music *initMusicLib(char *path);
void freeSfxLib(sfx *cur);
void freeMusicLib(music *cur);

#endif
