// .h
// Data Structure Declarations
// by Kyle Furey

/**
 *
 */

#ifndef DS_DEF_H
#define DS_DEF_H

#include "ds_std.h"

/** Asserts used in data structures. */
#ifdef NDEBUG
#define ds_assert(cond) ((void) 0)
#else
#define ds_assert(cond)\
do if (!(cond)) fprintf(stderr, \
"ds.h - ASSERTION FAILED\nFunction:\t%s()\nLine:\t\t%d\nCondition:\t%s\n", \
__func__, __LINE__, (#cond)), abort(); while (false)
#endif

/** The default data structure allocators. */
#define ds_malloc   malloc
#define ds_calloc   calloc
#define ds_realloc  realloc
#define ds_free     free

/** Aligns a size for an allocator. */
#define ALLOCATOR_ALIGN(size, alignment) (((size) + ((alignment) - 1)) & ~((alignment) - 1))

/** Whether to assert when a deleted allocator leaks memory. */
#define ALLOCATOR_LEAK_ASSERT 1

/** The rate to expand vectors at. */
#define VECTOR_EXPANSION 2

/** The maximum fill capacity before rehashing a map. */
#define MAP_LOAD_FACTOR ((double) MAP_LOAD_FACTOR_NUM / (double) MAP_LOAD_FACTOR_DEN)
#define MAP_LOAD_FACTOR_NUM 1
#define MAP_LOAD_FACTOR_DEN 2

/** Data structure parameters for trivial types. */
#define DEFAULT_COMPARE     x > y
#define REVERSE_COMPARE     x <= y
#define DEFAULT_EQUALS      x == y
#define DEFAULT_HASH        hashify(sizeof(key), &key)
#define INT_HASH            key
#define STRING_HASH         hashify(strlen(key), key)

/** An index indicating something was not found. */
#define NOT_FOUND ((size_t) -1)

/** Each state in a hash map bucket. */
typedef enum {
    BUCKET_EMPTY = 0,
    BUCKET_OCCUPIED = 1,
    BUCKET_SKIP = 2,
} ds_bucket_state;

/** OS-aligned integer types for data structures. */
typedef     uint8_t         ds_byte;
typedef     signed int      ds_int;
typedef     unsigned int    ds_uint;
typedef     size_t          ds_size;

/** A no-op deleter function for trivial types. */
static inline void void_deleter(void *self) {
    (void) self;
}

/** Hashes any data as an array of bytes. */
static inline size_t hashify(size_t size, const void *data) {
    ds_assert(data != NULL);
    // FNV-1a
    const ds_byte *memory = (ds_byte *) data;
    size_t hash = 2166136261u; // FNV offset
    for (size_t i = 0; i < size; ++i) {
        hash ^= memory[i];
        hash *= 16777619u; // FNV prime
    }
    return hash;
}

#endif // DS_DEF_H
