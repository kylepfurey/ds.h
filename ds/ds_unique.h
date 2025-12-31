// .h
// Generic Unique Reference Data Structure
// by Kyle Furey

/**
 * ds_unique.h
 *
 * unique       unique_new          ( T data )
 *
 * unique       unique_copy         ( const unique* unique )
 *
 * T*           unique_get          ( unique* self )
 *
 * const T*     unique_get_const    ( const unique* self )
 *
 * void         unique_reset        ( unique* self, T data )
 *
 * void         unique_delete       ( unique* self )
 */

#ifndef DS_UNIQUE_H
#define DS_UNIQUE_H

#include "ds_def.h"

/** Declares a named unique pointer for the given type. */
#define DECLARE_UNIQUE_NAMED(name, T, deleter)\
\
typedef struct {\
    T *data;\
} name;\
\
static inline name name##_new(T data) {\
    T *self = (T *) ds_malloc(sizeof(T));\
    ds_assert(self != NULL);\
    *self = data;\
    return (name) {\
        self,\
    };\
}\
\
static inline name name##_copy(const name *unique) {\
    T *self = (T *) ds_malloc(sizeof(T));\
    ds_assert(self != NULL);\
    *self = *unique->data;\
    return (name) {\
        self,\
    };\
}\
\
static inline T *name##_get(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->data != NULL);\
    return self->data;\
}\
\
static inline const T *name##_get_const(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->data != NULL);\
    return self->data;\
}\
\
static inline void name##_reset(name *self, T data) {\
    ds_assert(self != NULL);\
    ds_assert(self->data != NULL);\
    deleter(self->data);\
    *self->data = data;\
}\
\
static inline void name##_delete(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->data != NULL);\
    deleter(self->data);\
    ds_free(self->data);\
    *self = (name) {0};\
}

/** Declares a unique pointer for the given type. */
#define DECLARE_UNIQUE(T, deleter) DECLARE_UNIQUE_NAMED(unique_##T, T, deleter)

#endif // DS_UNIQUE_H
