#include "bloom_filter.h"

#define K 8

void bf_add_key(hash_t h, uint8_t *data, size_t len) {
    const uint32_t delta = (h >> 17) | (h << 15); // Rotate right 17 bits
    for (size_t j = 0; j < K; ++j) {
        const uint32_t bitpos = h % (len * 8);
        data[bitpos / 8] |= (1 << (bitpos % 8));
        h += delta;
    }
}

_Bool bf_find(hash_t h, uint8_t *data, size_t len) {
    const uint32_t delta = (h >> 17) | (h << 15); // Rotate right 17 bits
    for (size_t j = 0; j < K; ++j) {
        const uint32_t bitpos = h % (len * 8);
        if ((data[bitpos / 8] & (1 << (bitpos % 8))) == 0)
            return 0;
        h += delta;
    }
    return 1;
}

#ifdef FILTERS_TEST

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define KEY_NUM 13107
#define FILTER_SIZE 16384

int main() {
    uint8_t *data = malloc(FILTER_SIZE);

    memset((void *)data, 0, sizeof(uint8_t) * FILTER_SIZE);

    srand(time(NULL));
    hash_t hs[KEY_NUM];
    for (int i = 0; i < KEY_NUM; ++i) {
        hs[i] = rand() % 0x10000000;
        bf_add_key(hs[i], data, FILTER_SIZE);
    }
    for (int i = 0; i < KEY_NUM; ++i) {
        assert(bf_find(hs[i], data, FILTER_SIZE));
    }

    int fp = 0;
    for (int i = 0; i < 10000; ++i) {
        fp += bf_find(rand() % 0x10000000 + 0x10000000, data, FILTER_SIZE);
    }
    printf("%d/10000\n", fp);
    assert(fp < 200);
    puts("passed!");
    return 0;
}
#ifdef FILTERS_TEST_MAIN
#endif
#endif