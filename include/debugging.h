#ifndef DEBUGGING_H
#define DEBUGGING_H

#include <SDL_image.h>
#include <stdbool.h>

#include "img.h"

typedef enum {
    FDEBUG_OFF,
    FDEBUG_SETUP,
    FDEBUG_ACTIVE
} FrameDebugState;

extern bool frameDebug;
extern SDL_Rect *framePointer;
/*
    NOTA:
    Si se utiliza un puntero a NULL, son 8 bytes de memoria en stack.
    A la hora de alocar memoria, se utilizan 64 bytes adicionales en heap.
*/
extern texture sprites;
extern int selected;
/**
 * @brief Cargar el frame debug.
 * 
 */
void loadFrameDebug();
/**
 * @brief Gestionar el evento de teclado.
 * 
 * @param key evento de teclado.
 */
void keyEventFrameDebug(SDL_Keycode key);
/**
 * @brief Salir del modo debug de frames.
 * 
 */
void exitFrameDebug();
/**
 * @brief Reenderizar debug de frames.
 * 
 */
void renderFrameDebug();

#endif