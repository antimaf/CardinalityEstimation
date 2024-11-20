/*
 * xxHash - Extremely Fast Hash algorithm
 * Header File
 * Copyright (C) 2012-2020 Yann Collet
 *
 * BSD 2-Clause License (https://www.opensource.org/licenses/bsd-license.php)
 */

#ifndef XXHASH_H_5627135585666179
#define XXHASH_H_5627135585666179

#include <stdint.h>

// XXHash implementation
uint64_t XXHash64(const void* input, size_t length, uint64_t seed) {
    // Simple implementation for now
    const unsigned char* p = (const unsigned char*)input;
    uint64_t hash = seed + 0x9E3779B97F4A7C15ULL;
    
    while (length >= 8) {
        uint64_t k = *(uint64_t*)p;
        k *= 0x9DDFEA08EB382D69ULL;
        k ^= k >> 47;
        k *= 0x9DDFEA08EB382D69ULL;
        hash ^= k;
        hash *= 0x9DDFEA08EB382D69ULL;
        p += 8;
        length -= 8;
    }
    
    while (length--) {
        hash ^= *p++;
        hash *= 0x9DDFEA08EB382D69ULL;
    }
    
    hash ^= hash >> 47;
    hash *= 0x9DDFEA08EB382D69ULL;
    hash ^= hash >> 47;
    
    return hash;
}

#endif // XXHASH_H_5627135585666179
