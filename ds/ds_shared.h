// .h
// Generic Shared Reference Data Structure
// by Kyle Furey

/**
 * ds_shared.h
 *
 * This is a reference counted pointer that ensures memory is freed when shared_count is 0.
 * Shared references create slight indirection for ensuring shared ownership of memory.
 *
 * When memory is accessed from multiple contexts in a program, a shared reference ensures
 * the memory is freed only after all owners are done with it.
 *
 * shared       shared_new              ( T data )
 *
 * shared       shared_copy             ( shared* shared )
 *
 * uint         shared_shared_count     ( const shared* self )
 *
 * uint         shared_weak_count       ( const shared* self )
 *
 * T*           shared_get              ( shared* self )
 *
 * const T*     shared_get_const        ( const shared* self )
 *
 * void         shared_reset            ( shared* self, T data )
 *
 * void         shared_delete           ( shared* self )
 */

#ifndef DS_SHARED_H
#define DS_SHARED_H

#include "ds_def.h"

/** Declares a named shared pointer for the given type. */
#define DECLARE_SHARED_NAMED(name, T, deleter)\
\
typedef struct {\
    ds_uint shared_count;\
    ds_uint weak_count;\
    T *data;\
} ds__##name##_control_block;\
\
typedef struct {\
    ds__##name##_control_block *control_block;\
} name;\
\
DS_API static inline name name##_new(T data) {\
    ds__##name##_control_block *control_block =\
    (ds__##name##_control_block *) ds_malloc(sizeof(ds__##name##_control_block));\
    ds_assert(control_block != NULL);\
    control_block->shared_count = 1;\
    control_block->weak_count = 0;\
    control_block->data = (T *) ds_malloc(sizeof(T));\
    ds_assert(control_block->data != NULL);\
    *control_block->data = data;\
    return (name) {\
        control_block,\
    };\
}\
\
DS_API static inline name name##_copy(name *shared) {\
    ds_assert(shared != NULL);\
    ds__##name##_control_block *control_block = shared->control_block;\
    ds_assert(control_block != NULL);\
    ds_assert(control_block->shared_count > 0);\
    ds_assert(control_block->data != NULL);\
    ++control_block->shared_count;\
    return *shared;\
}\
\
DS_API static inline ds_uint name##_shared_count(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->control_block != NULL);\
    return self->control_block->shared_count;\
}\
\
DS_API static inline ds_uint name##_weak_count(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->control_block != NULL);\
    return self->control_block->weak_count;\
}\
\
DS_API static inline T *name##_get(name *self) {\
    ds_assert(self != NULL);\
    ds__##name##_control_block *control_block = self->control_block;\
    ds_assert(control_block != NULL);\
    ds_assert(control_block->shared_count > 0);\
    ds_assert(control_block->data != NULL);\
    return control_block->data;\
}\
\
DS_API static inline const T *name##_get_const(const name *self) {\
    ds_assert(self != NULL);\
    const ds__##name##_control_block *control_block = self->control_block;\
    ds_assert(control_block != NULL);\
    ds_assert(control_block->shared_count > 0);\
    ds_assert(control_block->data != NULL);\
    return control_block->data;\
}\
\
DS_API static inline void name##_reset(name *self, T data) {\
    ds_assert(self != NULL);\
    ds__##name##_control_block *control_block = self->control_block;\
    ds_assert(control_block != NULL);\
    ds_assert(control_block->shared_count > 0);\
    ds_assert(control_block->data != NULL);\
    deleter(control_block->data);\
    *control_block->data = data;\
}\
\
DS_API static inline void name##_delete(name *self) {\
    ds_assert(self != NULL);\
    ds__##name##_control_block *control_block = self->control_block;\
    ds_assert(control_block != NULL);\
    ds_assert(control_block->shared_count > 0);\
    --control_block->shared_count;\
    if (control_block->shared_count == 0) {\
        deleter(control_block->data);\
        ds_free(control_block->data);\
        control_block->data = NULL;\
        if (control_block->weak_count == 0) {\
            ds_free(control_block);\
        }\
    }\
    *self = (name) {0};\
}

/** Declares a shared pointer for the given type. */
#define DECLARE_SHARED(T, deleter) DECLARE_SHARED_NAMED(shared_##T, T, deleter)

#endif // DS_SHARED_H
