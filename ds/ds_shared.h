// .h
// Generic Shared Reference Data Structure
// by Kyle Furey

/**
 *
 */

#ifndef DS_SHARED_H
#define DS_SHARED_H

#include "ds_def.h"

/** Declares a named shared pointer for the given type. */
#define DECLARE_SHARED_NAMED(name, T, deleter)\
\
typedef struct {\
    ds_size shared_count;\
    ds_size weak_count;\
    T *data;\
} ds__##name##_control_block;\
\
typedef struct {\
    ds__##name##_control_block *control_block;\
} name;\
\
static inline name name##_new(T data) {\
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
static inline name name##_copy(name *shared) {\
    ds_assert(shared != NULL);\
    ds__##name##_control_block *control_block = shared->control_block;\
    ds_assert(control_block != NULL);\
    ds_assert(control_block->shared_count > 0);\
    ds_assert(control_block->data != NULL);\
    ++control_block->shared_count;\
    return *shared;\
}\
\
static inline ds_size name##_shared_count(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->control_block != NULL);\
    return self->control_block->shared_count;\
}\
\
static inline ds_size name##_weak_count(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->control_block != NULL);\
    return self->control_block->weak_count;\
}\
\
static inline void name##_reset(name *self, T data) {\
    ds_assert(self != NULL);\
    ds__##name##_control_block *control_block = self->control_block;\
    ds_assert(control_block != NULL);\
    ds_assert(control_block->shared_count > 0);\
    ds_assert(control_block->data != NULL);\
    deleter(control_block->data);\
    *control_block->data = data;\
}\
\
static inline T *name##_get(name *self) {\
    ds_assert(self != NULL);\
    ds__##name##_control_block *control_block = self->control_block;\
    ds_assert(control_block != NULL);\
    ds_assert(control_block->shared_count > 0);\
    ds_assert(control_block->data != NULL);\
    return control_block->data;\
}\
\
static inline const T *name##_get_const(const name *self) {\
    ds_assert(self != NULL);\
    const ds__##name##_control_block *control_block = self->control_block;\
    ds_assert(control_block != NULL);\
    ds_assert(control_block->shared_count > 0);\
    ds_assert(control_block->data != NULL);\
    return control_block->data;\
}\
\
static inline void name##_delete(name *self) {\
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
