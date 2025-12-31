// .h
// Data Structure Declarations
// by Kyle Furey

/**
 * ds_def.h
 *
 * This file is used by the ds.h library for declarations.
 * Settings, default parameters, and repeated functionality are defined here.
 *
 * ds_assert() is used to make assertions clear and clean with zero release overhead.
 *
 * ds_malloc, ds_calloc, ds_realloc, and ds_free are ds.h's default allocator functions.
 *
 * ARENA_ALIGN() is a macro used to align an arena's memory.
 * ARENA_LEAK_ASSERT is whether arena_delete() will assert if memory is "leaked".
 *
 * VECTOR_EXPANSION is a multiplier applied to a vector's capacity to make room.
 *
 * MAP_LOAD_FACTOR_NUM / MAP_LOAD_FACTOR_DEN is the maximum percentage a map can be filled.
 * When the map's capacity is greater than this fraction, it will rehash its values.
 *
 * DEFAULT_COMPARE can replace x_y_comparer for trivial numeric comparisons.
 * REVERSE_COMPARE is the exact opposite of DEFAULT_COMPARE.
 *
 * DEFAULT_EQUALS can replace x_y_equals for trivial numeric equality.
 *
 * DEFAULT_HASH calls ds_hashify() on any key to get its hash number. Replaces key_hasher.
 * INT_HASH uses an integer type as a hash number. Replaces key_hasher.
 * STRING_HASH calls ds_hashify() on a string with strlen(). Replaces key_hasher.
 *
 * NOT_FOUND is a sentinel value used to indicate an invalid index.
 *
 * BUCKET_EMPTY indicates a bucket is empty.
 * BUCKET_OCCUPIED indicates a bucket is full.
 * BUCKET_SKIP indicates a bucket was once full.
 *
 * ds_byte, ds_int, ds_uint, ds_size, ds_diff are type aliases used internally.
 *
 * void_deleter() is a noop deleter function used for data structures with trivial types.
 *
 * ds_hashify() is a generic Fowler-Noll-Vo implementation for hashing keys.
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

/** Aligns a size for an arena. */
#define ARENA_ALIGN(size, alignment) (((size) + ((alignment) - 1)) & ~((alignment) - 1))

/** Whether to assert when a deleted arena leaks memory. */
#define ARENA_LEAK_ASSERT 1

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
#define DEFAULT_HASH        ds_hashify(sizeof(key), &key)
#define INT_HASH            key
#define STRING_HASH         ds_hashify(strlen(key), key)

/** An index indicating something was not found. */
#define NOT_FOUND ((ds_size) -1)

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
typedef     ptrdiff_t       ds_diff;

/** A no-op deleter function for trivial types. */
static inline void void_deleter(void *self) {
    (void) self;
}

/** Hashes any data as an array of bytes. */
static inline ds_size ds_hashify(ds_size size, const void *data) {
    ds_assert(data != NULL);
    // FNV-1a
    const ds_byte *memory = (ds_byte *) data;
    ds_size hash = 2166136261u; // FNV offset
    for (ds_size i = 0; i < size; ++i) {
        hash ^= memory[i];
        hash *= 16777619u; // FNV prime
    }
    return hash;
}

#endif // DS_DEF_H
