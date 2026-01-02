#include "sound.h"
#include "config.h"
#include "tools.h"

#define MAX_CHANNELS 16

static const char *audioExtensions[] = {".wav", ".ogg", ".mp3"};

typedef struct {
    Mix_Chunk *chunk;
    bool in_use;
} ChannelData;

static ChannelData channel_chunks[MAX_CHANNELS] = {0};

static void channelDoneCallback(int channel)
{
    if (channel >= 0 && channel < MAX_CHANNELS && channel_chunks[channel].in_use)
    {
        if (channel_chunks[channel].chunk)
        {
            Mix_FreeChunk(channel_chunks[channel].chunk);
            channel_chunks[channel].chunk = NULL;
        }
        channel_chunks[channel].in_use = false;
    }
}

bool initAudio(void)
{
    if (SDL_WasInit(SDL_INIT_AUDIO) == 0)
    {
        if (SDL_Init(SDL_INIT_AUDIO) < 0)
        {
            printDebug("Error SDL_Init: %s\n", SDL_GetError());
            return false;
        }
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printDebug("Error Mix_OpenAudio: %s\n", Mix_GetError());
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return false;
    }
    return true;
}

void quitAudio(void)
{
    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void playAndFreeSfx(const char *sound)
{
    if (!initAudio())
    {
        printDebug("Olvidaste iniciar el audio! o no esta activo...: %s\n", Mix_GetError());
        return;
    }

    char path[PATH_SIZE(sound)];
    snprintf(path, sizeof(path), "%s%s", SFX_DIR, sound);

    Mix_Chunk *sfx_chunk = Mix_LoadWAV(path);
    if(!sfx_chunk) {
        printDebug("Error al cargar %s: %s\n", path, Mix_GetError());
        return;
    }

    Mix_ChannelFinished(channelDoneCallback);

    int channel = Mix_PlayChannel(-1, sfx_chunk, 0);
    if (channel == -1) {
        printDebug("Error al reproducir %s: %s\n", path, Mix_GetError());
        Mix_FreeChunk(sfx_chunk);
        return;
    }

    if (channel >= 0 && channel < MAX_CHANNELS)
    {
        channel_chunks[channel].chunk = sfx_chunk;
        channel_chunks[channel].in_use = true;
    }
}


sfx *initSfxLib(char *path)
{
    int sfx_count = 0;
    char **sounds = getFilesFromDir(path, &sfx_count, audioExtensions, ARRAY_L(audioExtensions), SOUND);
    if(!sounds)
    {
        printDebug("No se pudo inicializar la libreria sfx en la carpeta '%s'\n", path);
        return NULL;
    }
    
    if(sfx_count <= 0)
    {
        printDebug("No se encontraron archivos de audio en '%s'\n", path);
        freeStringArray(sounds, sfx_count);
        return NULL;
    }

    sfx *cur = calloc(1, sizeof(sfx));
    if(!cur)
        return NULL;

    cur->n = sfx_count;
    cur->chunks = calloc((size_t)sfx_count, sizeof(Mix_Chunk *));
    if(!cur->chunks) {
        free(cur);
        return NULL;
    }

    for(int i = 0; i < sfx_count; i++)
    {
        if(!sounds[i])
            continue;
        char fullpath[PATH_SIZE(sounds[i])];
        snprintf(fullpath, sizeof(fullpath), "%s%s", SFX_DIR, sounds[i]);
        cur->chunks[i] = Mix_LoadWAV(fullpath);
        if(!cur->chunks[i])
            printDebug("Error al cargar %s: %s\n", fullpath, Mix_GetError());
        if (sounds[i])
            free(sounds[i]);   
    }
    free(sounds);
    return cur;
}

music *initMusicLib(char *path)
{
    int music_count = 0;
    char **songs = getFilesFromDir(path, &music_count, audioExtensions, ARRAY_L(audioExtensions), SOUND);

    if(!songs)
    {
        printDebug("No se pudo inicializar la libreria de musica en la carpeta '%s'\n", path);
        return NULL;
    }

    if(music_count <= 0)
    {
        printDebug("No se encontraron archivos de audio en '%s'\n", path);
        freeStringArray(songs, music_count);
        return NULL;
    }

    music *cur = calloc(1, sizeof(music));
    if(!cur)
        return NULL;

    cur->n = music_count;
    cur->music = calloc((size_t)music_count, sizeof(Mix_Music *));
    if (!cur->music) {
        free(cur);
        return NULL;
    }

    for(int i = 0; i < music_count; i++)
    {
        if(songs[i])
        {
            char fullpath[PATH_SIZE(songs[i])];
            snprintf(fullpath, sizeof(fullpath), "%s%s", MUSIC_DIR, songs[i]);
            cur->music[i] = Mix_LoadMUS(fullpath);
            if(!cur->music[i])
                printDebug("Error al cargar %s: %s\n", fullpath, Mix_GetError());
            if(songs[i])
                free(songs[i]);
        }
    }
    free(songs);
    return cur;
}

void freeSfxLib(sfx *cur)
{
    if(!cur)
        return;

    if(cur->chunks) {
        for(int i = 0; i < cur->n; i++)
        {
            if(cur->chunks[i])
                Mix_FreeChunk(cur->chunks[i]);
        }
        free(cur->chunks);
    }
    cur->chunks = NULL;
    cur->n = 0;
    free(cur);
}

void freeMusicLib(music *cur)
{
    if(!cur)
        return;

    if(cur->music) {
        for(int i = 0; i < cur->n; i++)
        {
            if(cur->music[i])
                Mix_FreeMusic(cur->music[i]);
        }
        free(cur->music);
    }
    cur->music = NULL;
    cur->n = 0;
    free(cur);
}

//TODO: investigar sobre <dirent.h>