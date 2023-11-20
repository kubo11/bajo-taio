#include "helpers.h"

void copy_2d_array(int** source, int** destination, int size) {
    for (int i = 1; i < size; i++)
    {
        memcpy(destination[i], source[i], size * sizeof(int));
    }
}

void free_2d_array(int** array, int size) {
    for (int i = 0; i < size; i++)
    {
        free(array[i]);
    }
    free(array);
}