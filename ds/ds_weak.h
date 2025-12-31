// .h
// Generic Weak Reference Data Structure
// by Kyle Furey

/**
 * ds_weak.h
 *
 * Weak references are a counterpart to shared references that don't own the memory.
 * These references act as a view into the memory only when needed. They have their own weak_count.
 * Weak references are "weak" because the memory may already be deleted by shared references.
 *
 * While shared references are good for distributing pointers across multiple contexts,
 * weak references are great for managing memory internally while allowing external
 * parties to have temporary access to it. This is great for breaking reference cycles.
 *
 * weak         weak_new            ( shared* shared )
 *
 * weak         weak_copy           ( weak* weak )
 *
 * uint         weak_shared_count   ( const weak* self )
 *
 * uint         weak_weak_count     ( const weak* self )
 *
 * bool         weak_valid          ( const weak* self )
 *
 * shared       weak_upgrade        ( weak* self )
 *
 * void         weak_delete         ( weak* self )
 */

#ifndef DS_WEAK_H
#define DS_WEAK_H

#include "ds_def.h"

/** Declares a named weak pointer for the given type. */
#define DECLARE_WEAK_NAMED(name, shared_name)\
\
typedef struct {\
    ds__##shared_name##_control_block *control_block;\
} name;\
\
DS_API static inline name name##_new(shared_name *shared) {\
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
DS_API static inline name name##_copy(name *weak) {\
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
DS_API static inline bool name##_valid(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->control_block != NULL);\
    ds__##shared_name##_control_block *control_block = self->control_block;\
    ds_assert(control_block->weak_count > 0);\
    ds_assert((control_block->shared_count > 0) != (control_block->data == NULL));\
    return control_block->shared_count > 0;\
}\
\
DS_API static inline shared_name name##_upgrade(name *self) {\
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
DS_API static inline void name##_delete(name *self) {\
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
