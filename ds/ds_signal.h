// .h
// Generic Multicast Event Data Structure
// by Kyle Furey

/**
 * ds_signal.h
 *
 * signal           signal_new          ( size_t capacity )
 *
 * signal           signal_copy         ( const signal* signal )
 *
 * size_t           signal_count        ( const signal* self )
 *
 * size_t           signal_empty        ( const signal* self )
 *
 * bool             signal_bound        ( const signal* self, signal_handle handle )
 *
 * signal_handle    signal_bind         ( signal* self, T* target, signal_func func )
 *
 * void             signal_unbind       ( signal* self, signal_handle handle )
 *
 * macro            signal_invoke       ( signal* self, args... )
 *
 * void             signal_clear        ( signal* self )
 *
 * void             signal_delete       ( signal* self )
 */

#ifndef DS_SIGNAL_H
#define DS_SIGNAL_H

#include "ds_slab.h"

/** Declares a named multicast event for the given function signature. */
#define DECLARE_SIGNAL_NAMED(name, T, R, ...)\
\
typedef R(*name##_func)(T *, ##__VA_ARGS__);\
\
typedef struct {\
    T *target;\
    name##_func func;\
} ds__##name##_binding;\
\
DECLARE_SLAB_NAMED(ds__##name##_slab, ds__##name##_binding, void_deleter)\
\
typedef ds__##name##_slab_id name##_handle;\
\
typedef struct {\
    ds__##name##_slab bindings;\
} name;\
\
static inline name name##_new(ds_size capacity) {\
    ds_assert(capacity > 0);\
    return (name) {\
        ds__##name##_slab_new(capacity),\
    };\
}\
\
static inline name name##_copy(const name *signal) {\
    ds_assert(signal != NULL);\
    return (name) {\
        ds__##name##_slab_copy(&signal->bindings),\
    };\
}\
\
static inline ds_size name##_count(const name *self) {\
    ds_assert(self != NULL);\
    return self->bindings.count;\
}\
\
static inline ds_size name##_empty(const name *self) {\
    ds_assert(self != NULL);\
    return self->bindings.count == 0;\
}\
\
static inline bool name##_bound(const name *self, name##_handle handle) {\
    ds_assert(self != NULL);\
    return ds__##name##_slab_valid(&self->bindings, (ds__##name##_slab_id) handle);\
}\
\
static inline name##_handle name##_bind(name *self, T *target, name##_func func) {\
    ds_assert(self != NULL);\
    ds_assert(target != NULL);\
    ds_assert(func != NULL);\
    return (name##_handle ) ds__##name##_slab_borrow(\
        &self->bindings,\
        (ds__##name##_binding) {\
            target,\
            func,\
        }\
    );\
}\
\
static inline void name##_unbind(name *self, name##_handle handle) {\
    ds_assert(self != NULL);\
    ds_assert(name##_bound(self, handle));\
    ds__##name##_slab_return(&self->bindings, (ds__##name##_slab_id) handle);\
}\
\
static inline void name##_clear(name *self) {\
    ds_assert(self != NULL);\
    ds__##name##_slab_clear(&self->bindings);\
}\
\
static inline void name##_delete(name *self) {\
    ds_assert(self != NULL);\
    ds__##name##_slab_delete(&self->bindings);\
    *self = (name) {0};\
}

/** Invokes a signal with the given arguments. */
#define signal_invoke(self, ...)\
do {\
    ds_assert((self) != NULL);\
    ds_assert((self)->bindings.count <= (self)->bindings.buckets.count);\
    ds_assert((self)->bindings.buckets.array != NULL);\
    ds_size remaining = (self)->bindings.count;\
    for (ds_size i = 0; remaining > 0 && i < (self)->bindings.buckets.count; ++i) {\
        if ((self)->bindings.buckets.array[i].age == 0) {\
            continue;\
        }\
        ds_assert((self)->bindings.buckets.array[i].data.target != NULL);\
        ds_assert((self)->bindings.buckets.array[i].data.func != NULL);\
        (self)->bindings.buckets.array[i].data.func((self)->bindings.buckets.array[i].data.target, ##__VA_ARGS__);\
        --remaining;\
    }\
    ds_assert(remaining == 0);\
} while (false)

/** Declares a multicast event for the given function signature.  */
#define DECLARE_SIGNAL(T, R, ...) DECLARE_SIGNAL_NAMED(T##_signal, T, R, ##__VA_ARGS__)

#endif // DS_SIGNAL_H
