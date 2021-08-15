#pragma once

#include "filters_define.h"
#include <stdint.h>

typedef uint64_t block_t[8];
 
void bbf_add_key(hash_t h, block_t * data, uint32_t block_mask);

_Bool bbf_find(hash_t h, block_t *data, uint32_t block_mask);
