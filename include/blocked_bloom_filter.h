#pragma once

#include "filters_define.h"
#include <stddef.h>

typedef uint64_t block_t[8];

void bbf_add_key(hash_t h, block_t *data, uint32_t num_block);

_Bool bbf_find(hash_t h, block_t *data, uint32_t num_block);

uint32_t bf_calc_num_blocks(size_t filter_size);
