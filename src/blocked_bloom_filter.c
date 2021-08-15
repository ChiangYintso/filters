// Cache-, Hash- and Space-Efficient Bloom Filters
// Reference: https://algo2.iti.kit.edu/documents/cacheefficientbloomfilters-jea.pdf

#include "blocked_bloom_filter.h"
#include <assert.h>
#include <immintrin.h>
#include <stdint.h>

inline __attribute__((always_inline)) void make_mask(hash_t h, __m256i *mask_high, __m256i *mask_low) {
    const __m256i rehash = _mm256_setr_epi32(0x00000001, 0x9e3779b9, 0xe35e67b1, 0x734297e9,
                                             0x35fbe861, 0xdeb7c719, 0x448b211, 0x3459b749);
    __m256i hash_data = _mm256_set1_epi32(h);
    hash_data = _mm256_mullo_epi32(rehash, hash_data);
    *mask_high = _mm256_srli_epi64(hash_data, 58);
    *mask_low = _mm256_slli_epi64(hash_data, 6);
    *mask_low = _mm256_srli_epi64(*mask_low, 58);

    const __m256i ones = _mm256_set1_epi64x(1);

    *mask_high = _mm256_sllv_epi64(ones, *mask_high);
    *mask_low = _mm256_sllv_epi64(ones, *mask_low);
}

inline __attribute__((always_inline)) void bbf_add_key(hash_t h, block_t *data, uint32_t block_mask) {
    assert((unsigned long) data % 64 == 0);
    assert((((block_mask + 1) & 1)) == 0);
    uint32_t block_idx = h & block_mask;

    __m256i mask_high, mask_low;
    make_mask(h, &mask_high, &mask_low);
    __m256i *block = (__m256i *) (data + block_idx);
    _mm256_store_si256(block, _mm256_or_si256(*block, mask_high));
    block += 1;
    _mm256_store_si256(block, _mm256_or_si256(*block, mask_low));
    uint64_t *m = (uint64_t *) &mask_high;
}

inline __attribute__((always_inline)) _Bool bbf_find(hash_t h, block_t *data, uint32_t block_mask) {
    assert((unsigned long) data % 64 == 0);
    assert((((block_mask + 1) & 1)) == 0);
    uint32_t block_idx = h & block_mask;

    __m256i mask_high, mask_low;
    make_mask(h, &mask_high, &mask_low);
    __m256i *block = (__m256i *) (data + block_idx);
    return _mm256_testc_si256(*block, mask_high) && _mm256_testc_si256(*(block + 1), mask_low);
}

#ifdef FILTERS_TEST

#ifdef FILTERS_TEST_MAIN

#include <mm_malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define KEY_NUM 13107
#define FILTER_SIZE 16384

int main() {
    static_assert(sizeof(block_t) == 64);
    uint8_t *data = _mm_malloc(FILTER_SIZE, 64);
    static_assert(((unsigned long) (((block_t *) 0) + 1)) == 64);

    memset((void *) data, 0, sizeof(uint8_t) * FILTER_SIZE);
    assert((unsigned long) (data) % 64 == 0);
    srand(time(NULL));
    hash_t hs[KEY_NUM];
    for (int i = 0; i < KEY_NUM; ++i) {
        hs[i] = rand() % 0x10000000;
        bbf_add_key(hs[i], data, FILTER_SIZE / 64 - 1);
    }
    for (int i = 0; i < KEY_NUM; ++i) {
        assert(bbf_find(hs[i], data, FILTER_SIZE / 64 - 1));
    }
    // for (int i = 0; i < FILTER_SIZE; ++i) {
    //     printf("%lu ", data[i]);
    //     if (i % 32 == 31) {
    //         puts("");
    //     }
    // }
    int fp = 0;
    for (int i = 0; i < 10000; ++i) {
        fp += bbf_find(rand() % 0x10000000 + 0x10000000, data, FILTER_SIZE / 64 - 1);
    }
    printf("%d/10000\n", fp);
    assert(fp < 200);
    puts("passed!");
    return 0;
}
#endif
#endif
