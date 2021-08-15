#include "blocked_bloom_filter.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mm_malloc.h>

#define KEY_NUM 104857
#define FILTER_SIZE 131072

int main() {
    uint8_t *data = _mm_malloc(FILTER_SIZE, 64);

    memset((void *) data, 0, sizeof(uint8_t) * FILTER_SIZE);

    srand(time(NULL));
    hash_t hs[KEY_NUM];
    for (int i = 0; i < KEY_NUM; ++i) {
        hs[i] = rand() % 0x10000000;
        bbf_add_key(hs[i], data, FILTER_SIZE / 64 - 1);
    }
    for (int i = 0; i < KEY_NUM; ++i) {
        assert(bbf_find(hs[i], data, FILTER_SIZE / 64 - 1));
    }

	clock_t start = clock();
    int fp = 0;
    for (int i = 0; i < 1000000; ++i) {
        fp += bbf_find(rand(), data, FILTER_SIZE / 64 - 1);
    }
    assert(fp < 2000000);
    printf("%d/100000000\n", fp);

    clock_t end = clock();
    printf("%lf\n", (end - start));
    return 0;
}