#pragma once

#include <stdint.h>

#define likely(expr) __builtin_expect(!!(expr), 1)
#define unlikely(expr) __builtin_expect(!!(expr), 0)

unsigned int test_bit(const uint32_t *__restrict bitset,
                      const unsigned int idx);
unsigned int set_bit_nonatomic(uint32_t *__restrict bitset,
                               const unsigned int idx);
unsigned int unset_bit_nonatomic(uint32_t *__restrict bitset,
                                 const unsigned int idx);

int64_t search_lowest_bit(const uint64_t *__restrict bitset,
                          const unsigned int start_idx,
                          const unsigned int last_idx);
int64_t consume_lowest_bit(uint64_t *__restrict bitset,
                           const unsigned int start_idx,
                           const unsigned int last_idx);
int64_t search_lowest_common_bit(const uint64_t *__restrict bitset,
                                 const uint64_t *__restrict bitset2,
                                 const unsigned int start_idx,
                                 const unsigned int last_idx);
