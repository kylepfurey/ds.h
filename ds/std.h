// .h
// Data Structure Standard Library Includes
// by Kyle Furey

#ifndef DS_STD_H
#define DS_STD_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

/** A placeholder for trivially comparable values (greatest to smallest). */
#define DEFAULT_COMPARE x > y

/** Checks values for equality. */
#define DEFAULT_EQUALS x == y

/** A placeholder for a trivially hashable value. */
#define DEFAULT_HASH hashify(sizeof(key), key)

/** A placeholder for a no-op deleter function. */
static inline void void_deleter(void *self) {
    (void) self;
}

/** Generates a unique hash code from the given data. */
static inline size_t hashify(size_t size, const void *data) {
    assert(data != NULL);
    // FNV-1a
    const uint8_t *memory = (uint8_t *) data;
    size_t hash = 2166136261u; // FNV offset
    for (size_t i = 0; i < size; ++i) {
        hash ^= memory[i];
        hash *= 16777619u; // FNV prime
    }
    return hash;
}

#endif // DS_STD_H
