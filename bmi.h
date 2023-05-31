#pragma once

#include <stdint.h>

#define likely(expr) __builtin_expect(!!(expr), 1)
#define unlikely(expr) __builtin_expect(!!(expr), 0)

typedef uint64_t bitset_t;

int test_bit(const bitset_t *restrict bitset, uint32_t idx);

int set_bit_nonatomic(bitset_t *restrict bitset, uint32_t idx);
int unset_bit_nonatomic(bitset_t *restrict bitset, uint32_t idx);
int set_bit_atomic(bitset_t *restrict bitset, uint32_t idx);
int unset_bit_atomic(bitset_t *restrict bitset, uint32_t idx);

int64_t search_lowest_bit(const bitset_t *restrict bitset, uint32_t start_idx,
                          uint32_t last_idx);
int64_t consume_lowest_bit_nonatomic(bitset_t *restrict bitset,
                                     uint32_t start_idx, uint32_t last_idx);
int64_t search_lowest_common_bit(const bitset_t *restrict bitset,
                                 const bitset_t *restrict bitset2,
                                 uint32_t start_idx, uint32_t last_idx);
