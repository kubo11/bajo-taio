
#ifndef BAJO_TAIO_HELPERS_H
#define BAJO_TAIO_HELPERS_H

#include <stdio.h>
#include <stdlib.h>

void copy_array(int* source, int* destination, int size);
void copy_2d_array(int** source, int** destination, int size);
void free_2d_array(int** array, int size);

#endif // BAJO_TAIO_HELPERS_H