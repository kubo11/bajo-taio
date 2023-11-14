#include "bitset.h"
#include "error_handling.h"

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// Bitset is a sequence of bits where each bit can be individually set and checked
// It is implemented over an array of unsigned ints

Bitset* create_bitset(size_t size) {
  Bitset *bitset = (Bitset*)calloc(1, sizeof(Bitset));
  ASSERT(bitset != NULL, "Could not allocate space for bitset.");
  bitset->size = size;
  bitset->data_length = (size - 1) / __CHAR_BIT__ * sizeof(unsigned int) + 1;
  bitset->data = (unsigned int*)calloc(bitset->data_length, sizeof(unsigned int));
  ASSERT(bitset->data != NULL, "Could not allocate space for bitset data.");
  return bitset;
}

void destroy_bitset(Bitset *bitset) {
  ASSERT(bitset != NULL, "Bitset is NULL.");
  free(bitset->data);
  free(bitset);
}

void set_bit(Bitset *bitset, uint32_t bit) {
  ASSERT(bitset != NULL, "Bitset is NULL.");
  ASSERT(bit >= 0, "Bit index too low.");
  ASSERT(bit >= 0, "Bit index too high.");
  bitset->data[bit / __CHAR_BIT__] |= (1 << (bit % WORD_BITS));
}

void set_all_bits(Bitset *bitset) {
  ASSERT(bitset != NULL, "Bitset is NULL.");
  for (int i = 0; i < bitset->data_length; ++i) {
    bitset->data[i] = UINT_MAX;
  }
}

void unset_bit(Bitset *bitset, uint32_t bit) {
  ASSERT(bitset != NULL, "Bitset is NULL.");
  ASSERT(bit >= 0, "Bit index too low.");
  ASSERT(bit >= 0, "Bit index too high.");
  bitset->data[bit / __CHAR_BIT__] = ~bitset->data[bit / __CHAR_BIT__];
  bitset->data[bit / __CHAR_BIT__] |= (1 << (bit % WORD_BITS));
  bitset->data[bit / __CHAR_BIT__] = ~bitset->data[bit / __CHAR_BIT__];
}

void unset_all_bits(Bitset *bitset) {
  ASSERT(bitset != NULL, "Bitset is NULL.");
  for (int i = 0; i < bitset->data_length; ++i) {
    bitset->data[i] = 0;
  }
}

bool get_bit(Bitset *bitset, uint32_t bit) {
  ASSERT(bitset != NULL, "Bitset is NULL.");
  ASSERT(bit >= 0, "Bit index too low.");
  ASSERT(bit >= 0, "Bit index too high.");
  if (bitset->data[bit / __CHAR_BIT__] & (1 << (bit % WORD_BITS)))
    return true;
  return false;
}

bool check_if_all_unset(Bitset *bitset) {
  ASSERT(bitset != NULL, "Bitset is NULL.");
  for (int i = 0; i < bitset->data_length; ++i) {
    if (bitset->data[i]) {
      return false;
    }
  }
  return true;
}

Bitset* copy_bitset(Bitset *bitset) {
  ASSERT(bitset != NULL, "Bitset is NULL.");
  Bitset *copy = create_bitset(bitset->size);
  memcpy(copy->data, bitset->data, bitset->data_length * sizeof(unsigned int));
  return copy;
}

Bitset* bitset_intersection(Bitset *bitset1, Bitset *bitset2) {
  ASSERT(bitset1 != NULL, "Bitset1 is NULL.");
  ASSERT(bitset2 != NULL, "Bitset2 is NULL.");
  size_t size = fmin(bitset1->size, bitset2->size);
  Bitset *intersection = create_bitset(size);

  for (int i = 0; i < intersection->data_length; ++i) {
    intersection->data[i] = bitset1->data[i] & bitset2->data[i];
  }

  return intersection;
}

uint32_t count_set_bits(Bitset *bitset) {
  ASSERT(bitset != NULL, "Bitset is NULL.");
  uint32_t count = 0;
  for (int i = 0; i < bitset->size; ++i) {
    if (get_bit(bitset, i)) {
      count++;
    }
  }
  return count;
}