#pragma once

#include <stdint.h>

#define likely(expr) __builtin_expect(!!(expr), 1)
#define unlikely(expr) __builtin_expect(!!(expr), 0)

typedef uint64_t bitset_t;

unsigned int test_bit(const bitset_t *bitset, unsigned int idx);
unsigned int set_bit_nonatomic(bitset_t *bitset, unsigned int idx);
unsigned int unset_bit_nonatomic(bitset_t *bitset, unsigned int idx);

int64_t search_lowest_bit(const bitset_t *bitset, unsigned int start_idx,
                          unsigned int last_idx);
int64_t consume_lowest_bit(bitset_t *bitset, unsigned int start_idx,
                           unsigned int last_idx);
int64_t search_lowest_common_bit(const bitset_t *bitset,
                                 const bitset_t *bitset2,
                                 unsigned int start_idx, unsigned int last_idx);
