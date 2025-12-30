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
} ds_##name##_binding;\
\
DECLARE_SLAB_NAMED(ds_##name##_slab, ds_##name##_binding, void_deleter)\
\
typedef ds_##name##_slab_id name##_handle;\
\
typedef struct {\
    ds_##name##_slab bindings;\
} name;\
\
static inline name name##_new(ds_size capacity) {\
    assert(capacity > 0);\
    return (name) {\
        ds_##name##_slab_new(capacity),\
    };\
}\
\
static inline name name##_copy(const name *signal) {\
    assert(signal != NULL);\
    return (name) {\
        ds_##name##_slab_copy(&signal->bindings),\
    };\
}\
\
static inline ds_size name##_count(const name *self) {\
    assert(self != NULL);\
    return self->bindings.count;\
}\
\
static inline ds_size name##_empty(const name *self) {\
    assert(self != NULL);\
    return self->bindings.count == 0;\
}\
\
static inline bool name##_bound(const name *self, name##_handle handle) {\
    assert(self != NULL);\
    return ds_##name##_slab_valid(&self->bindings, (ds_##name##_slab_id) handle);\
}\
\
static inline name##_handle name##_bind(name *self, T *target, name##_func func) {\
    assert(self != NULL);\
    assert(target != NULL);\
    assert(func != NULL);\
    return (name##_handle ) ds_##name##_slab_borrow(\
        &self->bindings,\
        (ds_##name##_binding) {\
            target,\
            func,\
        }\
    );\
}\
\
static inline void name##_unbind(name *self, name##_handle handle) {\
    assert(self != NULL);\
    assert(name##_bound(self, handle));\
    ds_##name##_slab_return(&self->bindings, (ds_##name##_slab_id) handle);\
}\
\
static inline void name##_clear(name *self) {\
    assert(self != NULL);\
    ds_##name##_slab_clear(&self->bindings);\
}\
\
static inline void name##_delete(name *self) {\
    assert(self != NULL);\
    ds_##name##_slab_delete(&self->bindings);\
    *self = (name) {0};\
}

/** Declares an event handler for the given function signature.  */
#define DECLARE_SIGNAL(T, R, ...) DECLARE_SIGNAL_NAMED(T##_signal, T, R, ##__VA_ARGS__)

/** Invokes a signal with the given arguments. */
#define signal_invoke(self, ...)\
\
do {\
    assert((self) != NULL);\
    assert((self)->bindings.count <= (self)->bindings.vector.count);\
    assert((self)->bindings.vector.array != NULL);\
    ds_size remaining = (self)->bindings.count;\
    for (ds_size i = 0; remaining > 0 && i < (self)->bindings.vector.count; ++i) {\
        if ((self)->bindings.vector.array[i].age == 0) {\
            continue;\
        }\
        assert((self)->bindings.vector.array[i].data.target != NULL);\
        assert((self)->bindings.vector.array[i].data.func != NULL);\
        (self)->bindings.vector.array[i].data.func((self)->bindings.vector.array[i].data.target, ##__VA_ARGS__);\
        --remaining;\
    }\
    assert(remaining == 0);\
} while (false);

#endif // DS_SIGNAL_H
