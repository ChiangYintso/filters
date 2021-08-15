#pragma once

#include "filters_define.h"
#include <stdint.h>

typedef uint64_t block_t[8];
 
inline __attribute__((always_inline)) void bbf_add_key(hash_t h, block_t * data, uint32_t block_mask);

inline __attribute__((always_inline)) _Bool bbf_find(hash_t h, block_t *data, uint32_t block_mask);
