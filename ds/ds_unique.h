// .h
// Generic Unique Reference Data Structure
// by Kyle Furey

/**
 * ds_unique.h
 *
 * ds_DECLARE_UNIQUE_NAMED(
 *      name,               - The name of the data structure and function prefix.
 *
 *      T,                  - The type to generate this data structure with.
 *
 *      deleter,            - The name of the function used to deallocate T.
 *                            ds_void_deleter may be used for trivial types.
 * )
 *
 * This is a wrapper over heap memory that aims to explicitly state who owns it.
 * Unique references must always be freed at the end of the scope they are created.
 *
 * This a cleaner way of indicating who owns memory over raw pointers.
 * The wrapper over the actual pointer leaves zero overhead with clear ownership.
 *
 * * Returns a new unique reference containing <data>.
 * * This data structure must be deleted with unique_delete().
 *
 *   unique       unique_new          ( T data )
 *
 * * Returns a pointer to <self>'s data.
 *
 *   T*           unique_get          ( unique* self )
 *
 * * Returns a pointer to <self>'s data.
 *
 *   const T*     unique_get_const    ( const unique* self )
 *
 * * Resets the value in <self> with <data>.
 *
 *   void         unique_reset        ( unique* self, T data )
 *
 * * Safely deletes a unique reference.
 *
 *   void         unique_delete       ( unique* self )
 */

#ifndef DS_UNIQUE_H
#define DS_UNIQUE_H

#include "ds_def.h"

/** Declares a named unique pointer for the given type. */
#define ds_DECLARE_UNIQUE_NAMED(name, T, deleter)\
\
typedef struct {\
    T *data;\
} name;\
\
ds_API static inline name name##_new(T data) {\
    T *self = (T *) ds_malloc(sizeof(T));\
    ds_assert(self != NULL);\
    *self = data;\
    return (name) {\
        self,\
    };\
}\
\
ds_API static inline T *name##_get(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->data != NULL);\
    return self->data;\
}\
\
ds_API static inline const T *name##_get_const(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->data != NULL);\
    return self->data;\
}\
\
ds_API static inline void name##_reset(name *self, T data) {\
    ds_assert(self != NULL);\
    ds_assert(self->data != NULL);\
    deleter(self->data);\
    *self->data = data;\
}\
\
ds_API static inline void name##_delete(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->data != NULL);\
    deleter(self->data);\
    ds_free(self->data);\
    *self = (name) {0};\
}

/** Declares a unique pointer for the given type. */
#define ds_DECLARE_UNIQUE(T, deleter)\
        ds_DECLARE_UNIQUE_NAMED(unique_##T, T, deleter)

#endif // DS_UNIQUE_H
