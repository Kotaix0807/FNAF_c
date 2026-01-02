#ifndef TOOLS_H
#define TOOLS_H

#define ARRAY_L(a) (sizeof(a) / sizeof(a[0]))
#define CAT(a, b) a ## b

typedef enum
{   
    IMAGE = 0,
    SOUND
}valid_type;

int binarySearch(int arr[], int left, int right, int key);
int filesInDir(char *path);
char **getFilesFromDir(char *path, int *outCount, const char **extensions, int extCount, valid_type type);
void freeStringArray(char **array, int n);
void printDebug(char *error, ...);
char *typeAdmited(valid_type type);

#endif