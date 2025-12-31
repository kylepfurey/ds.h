// .h
// Generic Weak Reference Data Structure
// by Kyle Furey

#ifndef DS_WEAK_H
#define DS_WEAK_H

#include "def.h"

/** Declares a named weak pointer for the given type. */
#define DECLARE_WEAK_NAMED(name, shared_name)\
\
typedef struct {\
    ds__##shared_name##_control_block *control_block;\
} name;\
\
static inline name name##_new(shared_name *shared) {\
    ds_assert(shared != NULL);\
    ds_assert(shared->control_block != NULL);\
    ds__##shared_name##_control_block *control_block = shared->control_block;\
    ds_assert(control_block->shared_count > 0);\
    ds_assert(control_block->data != NULL);\
    ++control_block->weak_count;\
    return (name) {\
        control_block,\
    };\
}\
\
static inline name name##_copy(name *weak) {\
    ds_assert(weak != NULL);\
    ds_assert(weak->control_block != NULL);\
    ds__##shared_name##_control_block *control_block = weak->control_block;\
    ds_assert(control_block->weak_count > 0);\
    ++control_block->weak_count;\
    return (name) {\
        control_block,\
    };\
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
static inline bool name##_valid(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->control_block != NULL);\
    ds__##shared_name##_control_block *control_block = self->control_block;\
    ds_assert(control_block->weak_count > 0);\
    ds_assert((control_block->shared_count > 0) != (control_block->data == NULL));\
    return control_block->shared_count > 0;\
}\
\
static inline shared_name name##_upgrade(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->control_block != NULL);\
    ds__##shared_name##_control_block *control_block = self->control_block;\
    ds_assert(control_block->shared_count > 0);\
    ds_assert(control_block->weak_count > 0);\
    ds_assert(control_block->data != NULL);\
    ++control_block->shared_count;\
    return (shared_name) {\
        control_block,\
    };\
}\
\
static inline void name##_delete(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->control_block != NULL);\
    ds__##shared_name##_control_block *control_block = self->control_block;\
    ds_assert(control_block->weak_count > 0);\
    --control_block->weak_count;\
    if (control_block->weak_count == 0 && control_block->shared_count == 0) {\
        ds_assert(control_block->data == NULL);\
        ds_free(control_block);\
    }\
    *self = (name) {0};\
}

/** Declares a weak pointer for the given type. */
#define DECLARE_WEAK(T) DECLARE_WEAK_NAMED(weak_##T, shared_##T)

#endif // DS_WEAK_H
