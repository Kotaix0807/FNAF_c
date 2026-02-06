#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <SDL.h>
#include <wchar.h>

#include "config.h"
#include "tools.h"

//#define TOOLS_DEBUG

int recBinarySearch(int arr[], int left, int right, int key)
{
    if (right >= left)
    {
        int mid = left + (right - left) / 2;

        if (arr[mid] == key)
            return mid;

        if (arr[mid] > key)
            return recBinarySearch(arr, left, mid - 1, key);
        
        return recBinarySearch(arr, mid + 1, right, key);
    }
    return -1;
}

int filesInDir(char *path)
{
    DIR *dr = opendir(path);
    struct dirent *de;
    if (dr == NULL)
    {
        printDebug("No se pudo abirir '%s'\n", path);
        return -1;
    }

    int count = 0;
    while ((de = readdir(dr)) != NULL)
    {
        if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
            count++;
        #ifdef TOOLS_DEBUG
        printf("[ARCHIVOS DETECTADOS]\n");
        printf("%s\n", de->d_name);
        #endif
    }
    closedir(dr);
    return count;
}

static int hasValidExtension(const char *filename, const char **extensions, int extCount, valid_type type)
{
    if (extensions == NULL || extCount <= 0)
        return 1;

    const char *ext = strrchr(filename, '.');
    if (ext == NULL)
        return 0;

    for (int i = 0; i < extCount; i++)
    {
        if (strcmp(ext, extensions[i]) == 0)
            return 1;
    }
    printDebug("El archivo '%s' no es %s\n", filename, typeAdmited(type));
    return 0;
}

char **getFilesFromDir(char *path, int *outCount, const char **extensions, int extCount, valid_type type)
{
    DIR *dr = opendir(path);
    struct dirent *de;
    if (dr == NULL)
    {
        printDebug("No se pudo abrir '%s'\n", path);
        if (outCount)
            *outCount = 0;
        return NULL;
    }

    int capacity = 16;
    int count = 0;
    char **fileArray = malloc(sizeof(char *) * capacity);
    if (fileArray == NULL)
    {
        printDebug("Error al asignar memoria\n");
        closedir(dr);
        if (outCount)
            *outCount = 0;
        return NULL;
    }

    while ((de = readdir(dr)) != NULL)
    {
        if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0 &&
            hasValidExtension(de->d_name, extensions, extCount, type))
        {
            if (count >= capacity)
            {
                capacity *= 2;
                char **tmp = realloc(fileArray, sizeof(char *) * capacity);
                if (tmp == NULL)
                {
                    printDebug("Error al reasignar memoria\n");
                    freeStringArray(fileArray, count);
                    closedir(dr);
                    if (outCount)
                        *outCount = 0;
                    return NULL;
                }
                fileArray = tmp;
            }

            fileArray[count] = strdup(de->d_name);
            if (fileArray[count] == NULL)
            {
                printDebug("Error al copiar nombre de archivo\n");
                for (int i = 0; i < count; i++)
                    free(fileArray[i]);
                free(fileArray);
                closedir(dr);
                if (outCount)
                    *outCount = 0;
                return NULL;
            }
            count++;
        }
    }
    closedir(dr);

    if (outCount)
        *outCount = count;
    return fileArray;
}

void freeStringArray(char **array, int n)
{
    for (int i = 0; i < n; i++)
    {
        if(array[i])
            free(array[i]);
    }
    if(array)
        free(array);
}

void printDebug(char *error, ...)
{
    if(config.debug_mode)
    {
        va_list args;
        va_start(args, error);
        vfprintf(stderr, error, args);
        va_end(args);
    }
}

char *typeAdmited(valid_type type)
{
    switch(type)
    {
        case IMAGE:
            return "imagen";
        case SOUND:
            return "sonido";
        default:
            return "desconocido";
    }
}

void GetTextureSize(SDL_Texture* texture, int* width, int* height)
{
    SDL_QueryTexture(texture, NULL, NULL, width, height);
}

SDL_Color setColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_Color color = {
        .r = r,
        .g = g,
        .b = b,
        .a = a
    };
    return color;
}

int largestStr(char *arr[], int n)
{
    if(n <= 0)
    {
        perror("No array length");
        exit(1);
    }
    int max = 0, count = 0;
    for(int i = 0; i < n; i++)
    {
        count = (int)strlen(arr[i]);
        if(count > max)
            max = count;
    }
    return max;
}

int largestStr_bra(char *arr[], int n)
{
    if(n <= 0)
    {
        perror("No array length");
        exit(1);
    }
    int max = 0, count = 0;
    for(int i = 0; i < n; i++)
    {
        count = (int)u8_len(arr[i]);
        if(count > max)
            max = count;
    }
    return max;
}

int largestOpt(char *choices[], int n_choice, const char *title)
{
    int length = largestStr(choices, n_choice);
    if(title && length < (int)strlen(title))
        return (int)strlen(title) + 4;
    else
        return length + 6;
}

unsigned long fileLines(const char *file, int opt)
{
    FILE *txt = fopen(file, "r");
    if(!txt)
    {
        printf("Error: file '%s' not found\n", file);
        return 0;
    }
    unsigned long lines_count = 0, char_count = 0, WMAX = 0;
    int t;
    while ((t = fgetc(txt)) != EOF)
    {
        if (t == '\n' || t == '\r')
        {
            if(char_count > WMAX)
                WMAX = char_count;
            lines_count++;
            char_count = 0;
        }
        else
        {
            char_count++;
        }
    }
    if(lines_count > 1)
        lines_count++; //Linea adicional porque no existe \n inicial
    fclose(txt);
    if (opt == 0)
        return lines_count;
    else
        return WMAX;
}

char **readText(const char *file)
{
    unsigned long lines = fileLines(file, 0);
    unsigned long chars = fileLines(file, 1);
    if (lines == 0 || chars == 0)
        return NULL;

    FILE *txt = fopen(file, "r");
    if (!txt)
        return NULL;

    char **arr = calloc(lines, sizeof(char *));
    if (!arr) {
        fclose(txt);
        return NULL;
    }

    for (unsigned long i = 0; i < lines; i++) {
        arr[i] = calloc(chars + 2, sizeof(char));
        if (!arr[i]) {
            fclose(txt);
            for (unsigned long j = 0; j < i; j++)
                free(arr[j]);
            free(arr);
            return NULL;
        }
        if (!fgets(arr[i], (int)(chars + 2), txt))
            arr[i][0] = '\0';
        arr[i][strcspn(arr[i], "\r\n")] = '\0';
    }
    fclose(txt);
    return arr;
}

size_t u8_len(const char *s)
{
    mbstate_t st = {0};
    size_t count = 0;
    const char *p = s;
    wchar_t wc;
    while (*p)
    {
        size_t r = mbrtowc(&wc, p, MB_CUR_MAX, &st);
        if (r == (size_t)-1 || r == (size_t)-2)
            break;
        if (r == 0)
            break;
        count++;
        p += r;
    }
    return count;
}

int replace_fmt(char **arr, int idx, const char *arg)
{
    if (!arr || idx < 0 || !arg)
        return -1;

    const char *tmpl = arr[idx];
    if (!tmpl)
        return -1;

    size_t needed = (size_t)snprintf(NULL, 0, tmpl, arg);
    char *buf = malloc(needed + 1);
    if (!buf)
        return -1;

    snprintf(buf, needed + 1, tmpl, arg);
    arr[idx] = buf;
    return 0;
}

#ifdef TOOLS_DEBUG

GameConfig config = {0};  // Definición temporal para el test
int main()
{   
    config.debug_mode = true;
    printError("Error: %d, %d\n", 5, 3);
    return 0;
}

#endif
