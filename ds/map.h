// .h
// Generic Hash Map Data Structure
// by Kyle Furey

#ifndef DS_MAP_H
#define DS_MAP_H

#include "vector.h"

/** Declares a named hash map of the given types. */
#define DECLARE_MAP_NAMED(name, K, V, key_hasher, deleter)\
\
typedef struct {\
    K key;\
    V value;\
} K##_##V##_pair;\
\
DECLARE_VECTOR_NAMED(__##name##_vector, K##_##V##_pair, void_deleter)\
\
typedef struct {\
    __##name##_vector buckets;\
} name;\
\
static inline name name##_new(size_t capacity) {\
}\
\
static inline name name##_copy(const name *map) {\
}\
\
static inline size_t name##_count(const name *self) {\
}\
\
static inline size_t name##_capacity(const name *self) {\
}\
\
static inline bool name##_empty(const name *self) {\
}\
\
static inline V *name##_find(name *self, K key) {\
}\
\
static inline const V *name##_find_const(const name *self, K key) {\
}\
\
static inline void name##_resize(name *self, size_t capacity) {\
}\
\
static inline bool name##_contains(const name *self, K key) {\
}\
\
static inline bool name##_insert(name *self, K key, V value) {\
}\
\
static inline bool name##_erase(name *self, K key) {\
}\
\
static inline void name##_foreach(const name *self, void(*action)(K##_##V##_pair)) {\
}\
\
static inline void name##_foreach_key(const name *self, void(*action)(K)) {\
}\
\
static inline void name##_foreach_value(const name *self, void(*action)(V)) {\
}\
\
static inline void name##_free(name *self) {\
}

/** Declares a hash map of the given types. */
#define DECLARE_MAP(K, V, key_hasher, deleter) DECLARE_MAP_NAMED(K##_##V##_map, K, V, key_hasher, deleter)

#endif // DS_MAP_H
