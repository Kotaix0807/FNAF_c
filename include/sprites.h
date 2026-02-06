#ifndef SPRITES_H
#define SPRITES_H

#include <SDL_image.h>
#include <stdbool.h>

typedef struct {
    SDL_Rect *frames;
    int frame_count;
    int current_frame;
    float animation_timer;
    float frame_duration;
} animation;

typedef struct {
    SDL_Texture *texture;
    SDL_Rect src_rect;
    int width;
    int height;
} sprite;

typedef struct {
    SDL_Texture *sheets;
    bool loaded;
} SpriteManager;

extern SpriteManager sprite_manager;
/*
typedef struct {
    SDL_Rect hitbox;
    sprite main;
    animation run;
}obj;
*/
#endif
