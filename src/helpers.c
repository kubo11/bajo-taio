#include "helpers.h"

void copy_array(int* source, int* destination, int size) {
    for (int i = 0; i < size; i++)
    {
        destination[i] = source[i];
    }
}

void copy_2d_array(int** source, int** destination, int size) {
    for (int i = 1; i < size; i++)
    {
        for (int j = 1; j < size; j++)
        {
            destination[i][j] = source[i][j];
        }
    }
}

void free_2d_array(int** array, int size) {
    for (int i = 0; i < size; i++)
    {
        free(array[i]);
    }
    free(array);
}