// .h
// Generic Slab Allocator Data Structure
// by Kyle Furey

#ifndef DS_SLAB_H
#define DS_SLAB_H

#include "vector.h"

/** Declares a named slab allocator of the given type. */
#define DECLARE_SLAB_NAMED(name, T, deleter)\
\
typedef struct {\
    uint32_t index;\
    uint32_t age;\
} T##_id;\
\
typedef struct {\
    uint32_t age;\
    T data;\
} __##name##_block;\
\
DECLARE_VECTOR_NAMED(__##name##_vector, __##name##_block, void_deleter)\
\
typedef struct {\
    size_t count;\
    T##_id next;\
    __##name##_vector vector;\
} name;\
\
static inline name name##_new(size_t capacity) {\
}\
\
static inline bool name##_valid(const name *self, T##_id id) {\
}\
\
static inline T *name##_get(name *self, T##_id id) {\
}\
\
static inline const T *name##_get_const(const name *self, T##_id id) {\
}\
\
static inline T##_id name##_borrow(name *self, T data) {\
}\
\
static inline void name##_return(name *self, T##_id id) {\
}\
\
static inline void name##_clear(name *self) {\
}\
\
static inline void name##_free(name *self) {\
}

/** Declares a slab allocator of the given type. */
#define DECLARE_SLAB(T, deleter) DECLARE_SLAB_NAMED(T##_slab, T, deleter)

#endif // DS_SLAB_H
