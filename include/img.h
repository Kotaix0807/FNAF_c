#ifndef IMG_H
#define IMG_H

#include <SDL_image.h>
#include <stdbool.h>

typedef struct texture_{
    SDL_Texture **textures_array;
    SDL_Rect **rects;
    int n;
}texture;

/**
 * @brief Inicializar uso de librerias de texturas.
 * 
 * @return true Exito inicializando.
 * @return false Error inicializando.
 */
bool initTexture(void);
/**
 * @brief Cerrar uso de libreria de texturas.
 */
void quitTexture(void);

/**
 * @brief Inicializar libreria de textura.
 * 
 * @param path Directorio.
 * @return texture Devuelve la libreria cargada.
 */
texture initTextureLib(char *path);
/**
 * @brief Librerar libreria de texturas.
 * 
 * @param txr Libreria a liberar.
 */
void freeTextureLib(texture *txr);
/**
 * @brief Asigna un rectangulo al tamanho de una textura.
 * 
 * @param texture Textura.
 * @param rect Rectangulo entero a textura.
 */
void assignRectToTexture(SDL_Texture *texture, SDL_Rect *rect);
/**
 * @brief Dibujar una imagen con rectangulos de tipo float
 * 
 * @param x 
 * @param y 
 * @param w 
 * @param h 
 * @param texture 
 */
void drawImageF(float x, float y, float w, float h, SDL_Texture *texture);
/**
 * @brief Dibujar una imagen con rectangulos de tipo int.
 * 
 * @param x 
 * @param y 
 * @param w 
 * @param h 
 * @param texture 
 */
void drawImage(int x, int y, int w, int h, SDL_Texture *texture);
/**
 * @brief Reenderiza las aristas de un rectangulo.
 * 
 * @param rect 
 * @param r 
 * @param g 
 * @param b 
 * @param a 
 */
void renderRect(SDL_Rect *rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

#endif
