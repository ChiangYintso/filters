#pragma once

#include "filters_define.h"
#include <stddef.h>

void bf_add_key(hash_t h, uint8_t *data, size_t len);

_Bool bf_find(hash_t h, uint8_t* data, size_t len);
