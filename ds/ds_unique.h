// .h
// Generic Unique Reference Data Structure
// by Kyle Furey

/**
 * ds_unique.h
 *
 * This is a wrapper over heap memory that aims to explicitly state who owns it.
 * Unique references must always be freed at the end of the scope they are created.
 *
 * This a cleaner way of indicating who owns memory over raw pointers.
 * The wrapper over the actual pointer leaves zero overhead with clear ownership.
 *
 * unique       unique_new          ( T data )
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
DS_API static inline name name##_new(T data) {\
    T *self = (T *) ds_malloc(sizeof(T));\
    ds_assert(self != NULL);\
    *self = data;\
    return (name) {\
        self,\
    };\
}\
\
DS_API static inline T *name##_get(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->data != NULL);\
    return self->data;\
}\
\
DS_API static inline const T *name##_get_const(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->data != NULL);\
    return self->data;\
}\
\
DS_API static inline void name##_reset(name *self, T data) {\
    ds_assert(self != NULL);\
    ds_assert(self->data != NULL);\
    deleter(self->data);\
    *self->data = data;\
}\
\
DS_API static inline void name##_delete(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->data != NULL);\
    deleter(self->data);\
    ds_free(self->data);\
    *self = (name) {0};\
}

/** Declares a unique pointer for the given type. */
#define DECLARE_UNIQUE(T, deleter) DECLARE_UNIQUE_NAMED(unique_##T, T, deleter)

#endif // DS_UNIQUE_H
