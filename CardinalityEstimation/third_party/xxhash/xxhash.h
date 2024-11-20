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
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// XXHash function declaration
uint64_t XXHash64(const void* input, size_t length, uint64_t seed);

#ifdef __cplusplus
}
#endif

#endif // XXHASH_H_5627135585666179
