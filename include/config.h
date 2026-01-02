#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

#define FRAME_TIME_MS(a) (1000 / a)
#define ASSETS_DIR "assets/"

//Directories
#define CONFIG_DIR ASSETS_DIR "config/"
#define SFX_DIR ASSETS_DIR "sfx/"
#define MUSIC_DIR ASSETS_DIR "music/"
#define SPRITES_DIR ASSETS_DIR "sprites/"
#define PATH_SIZE(a) sizeof(SFX_DIR) + sizeof(MUSIC_DIR) + sizeof(a)

#define CFG_FILE "hd.ini"

typedef struct {
    char name[128];
    int WIN_W;
    int WIN_H;
    bool fullscreen;
    bool vsync;
    int fps;
    
    int master_volume;
    int music_volume;
    int sfx_volume;
    int audio_frequency;

    bool show_fps;
    bool debug_mode;
} GameConfig;

extern GameConfig config;

bool loadConfig(GameConfig *cfg, char *cfg_name);
void printConfig(GameConfig *cfg);

#endif