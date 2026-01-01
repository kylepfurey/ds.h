// .h
// Generic Weak Reference Data Structure
// by Kyle Furey

/**
 * ds_weak.h
 *
 * ds_DECLARE_WEAK_NAMED(
 *      name,               - The name of the data structure and function prefix.
 *
 *      shared_name,        - The name of the shared reference data structure to extend.
 * )
 *
 * Weak references are a counterpart to shared references that don't own the memory.
 * These references act as a view into the memory only when needed. They have their own weak_count.
 * Weak references are "weak" because the memory may already be deleted by shared references.
 *
 * While shared references are good for distributing pointers across multiple contexts,
 * weak references are great for managing memory internally while allowing external
 * parties to have temporary access to it. This is great for breaking reference cycles.
 *
 * * Returns a new weak reference from <shared>.
 * * This data structure must be deleted with weak_delete().
 *
 *   weak     weak_new            ( const shared* shared )
 *
 * * Returns a new weak reference with the same address as <weak>.
 * * The new weak reference increments the weak count and must be deleted with weak_delete().
 *
 *   weak     weak_copy           ( const weak* weak )
 *
 * * Returns the number of shared references to <self>'s data.
 *
 *   uint     weak_shared_count   ( const weak* self )
 *
 * * Returns the number of weak references to <self>'s data.
 *
 *   uint     weak_weak_count     ( const weak* self )
 *
 * * Returns whether the weak reference is still valid.
 *
 *   bool     weak_valid          ( const weak* self )
 *
 * * Returns a new shared reference with the same address as <self>.
 * * <self> must be valid.
 * * This data structure must be deleted with shared_delete().
 *
 *   shared   weak_upgrade        ( weak* self )
 *
 * * Safely deletes a weak reference.
 *
 *   void     weak_delete         ( weak* self )
 */

#ifndef DS_WEAK_H
#define DS_WEAK_H

#include "ds_def.h"

/** Declares a named weak pointer for the given type. */
#define ds_DECLARE_WEAK_NAMED(name, shared_name)\
\
typedef struct {\
    ds__##shared_name##_control_block *control_block;\
} name;\
\
ds_API static inline name name##_new(const shared_name *shared) {\
    ds_assert(shared != ds_NULL);\
    ds_assert(shared->control_block != ds_NULL);\
    ds__##shared_name##_control_block *control_block = shared->control_block;\
    ds_assert(control_block->shared_count > 0);\
    ds_assert(control_block->data != ds_NULL);\
    ++control_block->weak_count;\
    return (name) {\
        control_block,\
    };\
}\
\
ds_API static inline name name##_copy(const name *weak) {\
    ds_assert(weak != ds_NULL);\
    ds_assert(weak->control_block != ds_NULL);\
    ds__##shared_name##_control_block *control_block = weak->control_block;\
    ds_assert(control_block->weak_count > 0);\
    ++control_block->weak_count;\
    return (name) {\
        control_block,\
    };\
}\
\
ds_API static inline ds_uint name##_shared_count(const name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->control_block != ds_NULL);\
    return self->control_block->shared_count;\
}\
\
ds_API static inline ds_uint name##_weak_count(const name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->control_block != ds_NULL);\
    return self->control_block->weak_count;\
}\
\
ds_API static inline ds_bool name##_valid(const name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->control_block != ds_NULL);\
    ds__##shared_name##_control_block *control_block = self->control_block;\
    ds_assert(control_block->weak_count > 0);\
    ds_assert((control_block->shared_count > 0) != (control_block->data == ds_NULL));\
    return control_block->shared_count > 0;\
}\
\
ds_API static inline shared_name name##_upgrade(name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->control_block != ds_NULL);\
    ds__##shared_name##_control_block *control_block = self->control_block;\
    ds_assert(control_block->shared_count > 0);\
    ds_assert(control_block->weak_count > 0);\
    ds_assert(control_block->data != ds_NULL);\
    ++control_block->shared_count;\
    return (shared_name) {\
        control_block,\
    };\
}\
\
ds_API static inline void name##_delete(name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->control_block != ds_NULL);\
    ds__##shared_name##_control_block *control_block = self->control_block;\
    ds_assert(control_block->weak_count > 0);\
    --control_block->weak_count;\
    if (control_block->weak_count == 0 && control_block->shared_count == 0) {\
        ds_assert(control_block->data == ds_NULL);\
        ds_free(control_block);\
    }\
    *self = (name) {0};\
}

/** Declares a weak pointer for the given type. */
#define ds_DECLARE_WEAK(T)\
        ds_DECLARE_WEAK_NAMED(weak_##T, shared_##T)

#endif // DS_WEAK_H
