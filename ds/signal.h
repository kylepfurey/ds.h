// .h
// Generic Event Handler Data Structure
// by Kyle Furey

#ifndef DS_SIGNAL_H
#define DS_SIGNAL_H

#include "slab.h"

/** Declares a named event handler for the given function signature. */
#define DECLARE_SIGNAL_NAMED(name, T, R, ...)\
\
typedef R(*name##_func)(T *, ##__VA_ARGS__);\
\
typedef struct {\
    T *target;\
    name##_func func;\
} __##name##_binding;\
\
DECLARE_SLAB_NAMED(__##name##_slab, __##name##_binding, void_deleter)\
\
typedef __##name##_slab_id name##_handle;\
\
typedef struct {\
    __##name##_slab slab;\
} name;\
\
static inline name name##_new(size_t capacity) {\
    assert(capacity > 0);\
    return (name) {\
        __##name##_slab_new(capacity),\
    };\
}\
\
static inline bool name##_bound(const name *self, name##_handle handle) {\
    assert(self != NULL);\
    return __##name##_slab_valid(&self->slab, (__##name##_slab_id) handle);\
}\
\
static inline name##_handle name##_bind(name *self, T *target, name##_func func) {\
    assert(self != NULL);\
    assert(target != NULL);\
    assert(func != NULL);\
    return (name##_handle ) __##name##_slab_borrow(&self->slab, (__##name##_binding) { target, func, });\
}\
\
static inline void name##_unbind(name *self, name##_handle handle) {\
    assert(self != NULL);\
    assert(name##_bound(self, handle));\
    __##name##_slab_return(&self->slab, (__##name##_slab_id) handle);\
}\
\
static inline void name##_clear(name *self) {\
    assert(self != NULL);\
    __##name##_slab_clear(&self->slab);\
}\
\
static inline void name##_free(name *self) {\
    assert(self != NULL);\
    __##name##_slab_free(&self->slab);\
}

/** Declares an event handler for the given function signature.  */
#define DECLARE_SIGNAL(T, R, ...) DECLARE_SIGNAL_NAMED(T##_signal, T, R, ##__VA_ARGS__)

/** Invokes a signal with the given arguments. */
#define signal_invoke(self, ...)\
\
do {\
    assert((self) != NULL);\
    assert((self)->slab.count <= (self)->slab.vector.count);\
    assert((self)->slab.vector.array != NULL);\
    size_t remaining = (self)->slab.count;\
    if (remaining == 0) {\
        break;\
    }\
    for (size_t i = 0; i < (self)->slab.vector.count; ++i) {\
        if ((self)->slab.vector.array[i].age == 0) {\
            continue;\
        }\
        assert((self)->slab.vector.array[i].data.target != NULL);\
        assert((self)->slab.vector.array[i].data.func != NULL);\
        (self)->slab.vector.array[i].data.func((self)->slab.vector.array[i].data.target, ##__VA_ARGS__);\
        --remaining;\
        if (remaining == 0) {\
            break;\
        }\
    }\
    assert(remaining == 0);\
} while (false);

#endif // DS_SIGNAL_H
