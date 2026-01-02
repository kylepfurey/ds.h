// .h
// Generic Multicast Event Data Structure
// by Kyle Furey

/**
 * ds_signal.h
 *
 * ds_DECLARE_SIGNAL_NAMED(
 *      name,               - The name of the data structure and function prefix.
 *
 *      T,                  - The type to generate this data structure with.
 *                            A pointer to T is always the first argument of the function signature.
 *                            You can use void here if you would like multicast signals.
 *
 *      R,                  - The return type of the function signature.
 *
 *      A...,               - Optionally any argument types of the function signature.
 * )
 *
 * Signals are collections of object to function bindings.
 * These functions share the same signature and the signal can call each function at once.
 * The signal will pass the same arguments to each function so each object is updated.
 *
 * This is excellent for the observer pattern. Binding provides a handle for unbinding.
 * Objects can opaquely bind to an event and be notified when the event is triggered.
 * Objects must unbind themselves on destruction to avoid invalid memory access on invoke.
 *
 * * signal_func is an alias for a pointer to the function signature.
 *
 *   typedef R (*signal_func) (T*, A...);
 *
 * * Returns a new signal with a current capacity of <capacity> bindings.
 * * <capacity> must be greater than 0.
 * * This data structure must be deleted with signal_delete().
 *
 *   signal           signal_new          ( size_t capacity )
 *
 * * Returns a new signal copied from <signal>.
 * * The new signal owns its own memory and must be deleted with signal_delete().
 *
 *   signal           signal_copy         ( const signal* signal )
 *
 * * Returns the current number of bindings in the signal.
 *
 *   size_t           signal_count        ( const signal* self )
 *
 * * Returns whether the signal has no bindings.
 *
 *   bool             signal_empty        ( const signal* self )
 *
 * * Returns whether <handle> is bound to the signal.
 *
 *   bool             signal_bound        ( const signal* self, signal_handle handle )
 *
 * * Binds <target> with <func> into the signal.
 * * Returns a handle to the binding for unbinding.
 *
 *   signal_handle    signal_bind         ( signal* self, T* target, signal_func func )
 *
 * * Unbinds <handle> from the signal.
 * * <handle> must be bound to signal.
 *
 *   void             signal_unbind       ( signal* self, signal_handle handle )
 *
 * * This is a macro, not a function.
 * * Invokes <self> with the given arguments.
 * * Bindings are not invoked in any particular order.
 * * <args> must match the function signature and are passed to each binding.
 * * The signal can be mutated while being invoked.
 *
 *   MACRO            signal_invoke       ( signal* self, args... )
 *
 * * Deletes all bindings in a signal.
 *
 *   void             signal_clear        ( signal* self )
 *
 * * Safely deletes a signal.
 *
 *   void             signal_delete       ( signal* self )
 */

#ifndef DS_SIGNAL_H
#define DS_SIGNAL_H

#include "ds_slab.h"

/** Declares a named multicast event for the given function signature. */
#define ds_DECLARE_SIGNAL_NAMED(name, T, R, ...)\
\
typedef R(*name##_func)(T *, ##__VA_ARGS__);\
\
typedef struct {\
    T *target;\
    name##_func func;\
} ds__##name##_binding;\
\
ds_DECLARE_SLAB_NAMED(ds__##name##_slab, ds__##name##_binding, ds_void_deleter)\
\
typedef ds__##name##_slab_id name##_handle;\
\
typedef struct {\
    ds__##name##_slab bindings;\
} name;\
\
ds_API static inline name name##_new(ds_size capacity) {\
    ds_assert(capacity > 0);\
    return (name) {\
        ds__##name##_slab_new(capacity),\
    };\
}\
\
ds_API static inline name name##_copy(const name *signal) {\
    ds_assert(signal != ds_NULL);\
    return (name) {\
        ds__##name##_slab_copy(&signal->bindings),\
    };\
}\
\
ds_API static inline ds_size name##_count(const name *self) {\
    ds_assert(self != ds_NULL);\
    return self->bindings.count;\
}\
\
ds_API static inline ds_bool name##_empty(const name *self) {\
    ds_assert(self != ds_NULL);\
    return self->bindings.count == 0;\
}\
\
ds_API static inline ds_bool name##_bound(const name *self, name##_handle handle) {\
    ds_assert(self != ds_NULL);\
    return ds__##name##_slab_valid(&self->bindings, handle);\
}\
\
ds_API static inline name##_handle name##_bind(name *self, T *target, name##_func func) {\
    ds_assert(self != ds_NULL);\
    ds_assert(target != ds_NULL);\
    ds_assert(func != ds_NULL);\
    return ds__##name##_slab_borrow(\
        &self->bindings,\
        (ds__##name##_binding) {\
            target,\
            func,\
        }\
    );\
}\
\
ds_API static inline void name##_unbind(name *self, name##_handle handle) {\
    ds_assert(self != ds_NULL);\
    ds_assert(name##_bound(self, handle));\
    ds__##name##_slab_return(&self->bindings, handle);\
}\
\
ds_API static inline void name##_clear(name *self) {\
    ds_assert(self != ds_NULL);\
    ds__##name##_slab_clear(&self->bindings);\
}\
\
ds_API static inline void name##_delete(name *self) {\
    ds_assert(self != ds_NULL);\
    ds__##name##_slab_delete(&self->bindings);\
    *self = (name) {0};\
}

/** Invokes a signal with the given arguments. */
#define signal_invoke(self, ...)\
do {\
    ds_assert((self) != ds_NULL);\
    ds_assert((self)->bindings.count <= (self)->bindings.buckets.count);\
    ds_assert((self)->bindings.buckets.array != ds_NULL);\
    ds_size remaining = (self)->bindings.count;\
    for (ds_size i = 0; remaining > 0 && i < (self)->bindings.buckets.count; ++i) {\
        if ((self)->bindings.buckets.array[i].age == 0) {\
            continue;\
        }\
        ds_assert((self)->bindings.buckets.array[i].data.target != ds_NULL);\
        ds_assert((self)->bindings.buckets.array[i].data.func != ds_NULL);\
        (self)->bindings.buckets.array[i].data.func((self)->bindings.buckets.array[i].data.target, ##__VA_ARGS__);\
        --remaining;\
    }\
    ds_assert(remaining == 0);\
} while (ds_false)

/** Declares a multicast event for the given function signature.  */
#define ds_DECLARE_SIGNAL(T, R, ...)\
        ds_DECLARE_SIGNAL_NAMED(T##_signal, T, R, ##__VA_ARGS__)

#endif // DS_SIGNAL_H
