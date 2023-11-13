#ifndef BAJO_TAIO_BITSET_H
#define BAJO_TAIO_BITSET_H

#include <stdint.h>
#include <stdio.h>

#define WORD_BITS (__CHAR_BIT__ * sizeof(unsigned int))

typedef uint8_t bool;
#define true (uint8_t)1;
#define false (uint8_t)0;

typedef struct Bitset{
  size_t size;
  size_t data_length;
  unsigned int* data;
} Bitset;

Bitset* create_bitset(size_t size);

void destroy_bitset(Bitset* bitset);

void set_bit(Bitset* bitset, uint32_t bit);

void set_all_bits(Bitset* bitset);

void unset_bit(Bitset* bitset, uint32_t bit);

void unset_all_bits(Bitset* bitset);

bool get_bit(Bitset* bitset, uint32_t bit);

bool check_if_all_unset(Bitset* bitset);

Bitset* copy_bitset(Bitset* bitset);

Bitset* bitset_intersection(Bitset* bitset1, Bitset* bitset2);

uint32_t count_set_bits(Bitset* bitset);

#endif // BAJO_TAIO_BITSET_H