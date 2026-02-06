#ifndef TOOLS_H
#define TOOLS_H

#include <SDL.h>

#define ARRAY_L(a) (sizeof(a) / sizeof(a[0]))
#define CAT(a, b) a ## b
#define UNUSED(x) ((void)(x))

typedef enum
{   
    IMAGE = 0,
    SOUND
}valid_type;

/**
 * @brief Busqueda binaria recursiva
 * 
 * @param arr Arreglo base
 * @param left limite izquierdo
 * @param right Limite derecho
 * @param key Dato a encontrar
 * @return int Index del dato
 */
int recBinarySearch(int arr[], int left, int right, int key);
/**
 * @brief Cuenta la candidad de archivos en un directorio
 * 
 * @param path Directorio
 * @return int Cantidad de archivos
 */
int filesInDir(char *path);
/**
 * @brief Get the Files From Dir object
 * 
 * @param path Directorio
 * @param outCount Contador de archivos
 * @param extensions Extensiones permitidas
 * @param extCount 
 * @param type 
 * @return char** 
 */
char **getFilesFromDir(char *path, int *outCount, const char **extensions, int extCount, valid_type type);
/**
 * @brief Liberar array de strings.
 * 
 * @param array Arreglo.
 * @param n Cantidad de datos a liberar.
 */
void freeStringArray(char **array, int n);
/**
 * @brief Imprime texto en stderr solo si el modo debug esta activado.
 * 
 * @param error 
 * @param ... 
 */
void printDebug(char *error, ...);
/**
 * @brief Devuelve un string del tipo admitido:
 * \sa EJ: valid_type IMAGE -> char* "imagen".
 * 
 * @param type 
 * @return char* 
 */
char *typeAdmited(valid_type type);
/**
 * @brief Obtener el tamanho de una textura.
 * 
 * @param texture Textura.
 * @param width Ancho.
 * @param height Alto.
 */
void GetTextureSize(SDL_Texture* texture, int* width, int* height);
/**
 * @brief Devuelve un color 
 * 
 * @param r 
 * @param g 
 * @param b 
 * @param a 
 * @return SDL_Color 
 */
SDL_Color setColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

int largestStr(char *arr[], int n);
int largestStr_bra(char *arr[], int n);
int largestOpt(char *choices[], int n_choice, const char *title);
unsigned long fileLines(const char *file, int opt);
char **readText(const char *file);
size_t u8_len(const char *s);
int replace_fmt(char **arr, int idx, const char *arg);

#endif