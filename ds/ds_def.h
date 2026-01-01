// .h
// Data Structure Declarations
// by Kyle Furey

/**
 * ds_def.h
 *
 * This file is used by the ds.h library for declarations.
 * Settings, default parameters, and repeated functionality are defined here.
 *
 * ds_malloc, ds_calloc, ds_realloc, and ds_free are ds.h's default allocator functions.
 * ds_memcpy, ds_memmove, ds_memset are ds.h's default memory functions.
 * ds_strlen, ds_tolower, ds_toupper, ds_isspace are ds.h's default string functions.
 *
 * ds_ARENA_ALIGN is a macro used to align an arena's memory.
 * ds_ARENA_LEAK_ASSERT is whether arena_delete() will assert if memory is "leaked".
 *
 * ds_VECTOR_EXPANSION is a multiplier applied to a vector's capacity to make room.
 * ds_VECTOR_TRUNC_ASSERT is whether vectors and strings will assert when implicitly truncating elements.
 *
 * ds_MAP_LOAD_FACTOR_NUM / ds_MAP_LOAD_FACTOR_DEN is the maximum percentage a map can be filled.
 * When the map's capacity is greater than this fraction, it will rehash its values.
 *
 * ds_DEFAULT_COMPARE can replace x_y_comparer for trivial numeric comparisons.
 * ds_REVERSE_COMPARE is the exact opposite of ds_DEFAULT_COMPARE.
 *
 * ds_DEFAULT_EQUALS can replace x_y_equals for trivial numeric equality.
 *
 * ds_DEFAULT_HASH calls ds_hashify() on any key to get its hash number. Replaces key_hasher.
 * ds_INT_HASH uses an integer type as a hash number. Replaces key_hasher.
 * ds_STRING_HASH calls ds_hashify() on a string with strlen(). Replaces key_hasher.
 *
 * ds_NULL is a sentinel value used to indicate an invalid pointer.
 * ds_NOT_FOUND is a sentinel value used to indicate an invalid index.
 * ds_SIZE_MAX is the maximum value of ds_size.
 *
 * ds_assert() is used to make assertions clear and clean with zero runtime overhead.
 *
 * ds_API is a tag in front of each generated ds.h function.
 *
 * ds_false and ds_true are boolean values used internally.
 *
 * ds_BUCKET_EMPTY indicates a bucket is empty.
 * ds_BUCKET_OCCUPIED indicates a bucket is full.
 * ds_BUCKET_SKIP indicates a bucket was once full.
 *
 * ds_bool, ds_byte, ds_int, ds_uint, ds_size, and ds_diff are type aliases used internally.
 *
 * ds_void_deleter() is a no-op deleter function used for data structures with trivial types.
 *
 * ds_hashify() is a generic Fowler-Noll-Vo implementation for hashing keys.
 */

#ifndef DS_DEF_H
#define DS_DEF_H

#include "ds_std.h"

/** The default data structure allocator functions. */
#define ds_malloc   malloc
#define ds_calloc   calloc
#define ds_realloc  realloc
#define ds_free     free

/** The default data structure memory functions. */
#define ds_memcpy   memcpy
#define ds_memmove  memmove
#define ds_memset   memset

/** The default data structure string functions. */
#define ds_strlen   strlen
#define ds_tolower  tolower
#define ds_toupper  toupper
#define ds_isspace  isspace

/** Aligns a size for an arena. */
#define ds_ARENA_ALIGN(size, alignment) (((size) + ((alignment) - 1)) & ~((alignment) - 1))

/** Whether to assert when a deleted arena leaks memory. */
#define ds_ARENA_LEAK_ASSERT 1

/** The rate to expand vectors at. */
#define ds_VECTOR_EXPANSION 2

/** Whether to assert when vectors and strings implicitly truncate elements. */
#define ds_VECTOR_TRUNC_ASSERT 1

/** The maximum fill capacity before rehashing a map. */
#define ds_MAP_LOAD_FACTOR_NUM 1
#define ds_MAP_LOAD_FACTOR_DEN 2

/** Data structure parameters for trivial types. */
#define ds_DEFAULT_COMPARE x > y
#define ds_REVERSE_COMPARE x <= y
#define ds_DEFAULT_EQUALS  x == y
#define ds_DEFAULT_HASH    ds_hashify(sizeof(key), &key)
#define ds_INT_HASH        key
#define ds_STRING_HASH     ds_hashify(ds_strlen(key), key)

/** A value of a pointer with no data. */
#define ds_NULL ((void *) 0)

/** An index indicating something was not found. */
#define ds_NOT_FOUND ((ds_size) -1)

/** The maximum value of ds_size. */
#define ds_SIZE_MAX SIZE_MAX

/** Asserts used in data structures. */
#ifdef NDEBUG
#define ds_assert(cond) ((void) 0)
#else
#define ds_assert(cond)\
do if (!(cond)) fprintf(stderr, \
"ds.h - ASSERTION FAILED\nFunction:\t%s()\nLine:\t\t%d\nCondition:\t%s\n", \
__func__, __LINE__, (#cond)), abort(); while (ds_false)
#endif

/** A tag for generated functions indicating they are from the ds.h library. */
#define ds_API

/** Boolean literals. */
enum {
    ds_false = 0,
    ds_true = 1,
};

/** Each state in a hash map bucket. */
enum {
    ds_BUCKET_EMPTY = 0,
    ds_BUCKET_OCCUPIED = 1,
    ds_BUCKET_SKIP = 2,
};

/** OS-aligned types for data structures. */
typedef     bool            ds_bool;
typedef     uint8_t         ds_byte;
typedef     signed int      ds_int;
typedef     unsigned int    ds_uint;
typedef     size_t          ds_size;
typedef     ptrdiff_t       ds_diff;

/** A no-op deleter function for trivial types. */
ds_API static inline void ds_void_deleter(void *self) {
    (void) self;
}

/** Hashes any data as an array of bytes. */
ds_API static inline ds_size ds_hashify(ds_size size, const void *data) {
    ds_assert(data != ds_NULL);
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
