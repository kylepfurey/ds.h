// .h
// Generic Weak Reference Data Structure
// by Kyle Furey

#ifndef DS_WEAK_H
#define DS_WEAK_H

#include "std.h"

/** Declares a named weak pointer for the given type. */
#define DECLARE_WEAK_NAMED(name, shared_name)\
\
typedef struct {\
__##shared_name##_control_block *control_block;\
} name;\
\
static inline name name##_new(shared_name *shared) {\
    assert(shared != NULL);\
    assert(shared->control_block != NULL);\
    __##shared_name##_control_block *control_block = shared->control_block;\
    assert(control_block->shared_count > 0);\
    assert(control_block->data != NULL);\
    ++control_block->weak_count;\
    return (name) {\
        control_block,\
    };\
}\
\
static inline void name##_free(name *self) {\
    assert(self != NULL);\
    assert(self->control_block != NULL);\
    __##shared_name##_control_block *control_block = self->control_block;\
    assert(control_block->weak_count > 0);\
    --control_block->weak_count;\
    if (control_block->weak_count == 0 && control_block->shared_count == 0) {\
        assert(control_block->data == NULL);\
        free(control_block);\
        self->control_block = NULL;\
    }\
}\
\
static inline bool name##_valid(const name *self) {\
    assert(self != NULL);\
    assert(self->control_block != NULL);\
    __##shared_name##_control_block *control_block = self->control_block;\
    assert(control_block->weak_count > 0);\
    assert((control_block->shared_count > 0) != (control_block->data == NULL));\
    return control_block->shared_count > 0;\
}\
\
static inline shared_name name##_upgrade(name *self) {\
    assert(self != NULL);\
    assert(self->control_block != NULL);\
    __##shared_name##_control_block *control_block = self->control_block;\
    assert(control_block->shared_count > 0);\
    assert(control_block->weak_count > 0);\
    assert(control_block->data != NULL);\
    ++control_block->shared_count;\
    return (shared_name) {\
        control_block,\
    };\
}

/** Declares a weak pointer for the given type. */
#define DECLARE_WEAK(T) DECLARE_WEAK_NAMED(weak_##T, shared_##T)

#endif // DS_WEAK_H
