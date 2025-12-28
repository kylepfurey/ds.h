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
typedef T##_id name##_handle;\
\
typedef struct {\
    __##name##_slab slab;\
} name;\
\
static inline name name##_new(size_t capacity) {\
}\
\
static inline bool name##_bound(const name *self, name##_handle handle) {\
}\
\
static inline name##_handle name##_bind(name *self, T *target, name##_func func) {\
}\
\
static inline void name##_unbind(name *self, name##_handle handle) {\
}\
\
static inline void name##_clear(name *self) {\
}\
\
static inline void name##_free(name *self) {\
}

/** Declares an event handler for the given function signature.  */
#define DECLARE_SIGNAL(T, R, ...) DECLARE_SIGNAL_NAMED(T##_signal, T, R, ##__VA_ARGS__)

/** Invokes a signal with the given arguments. */
#define signal_invoke(self, ...)\
\
do {\
} while (false);

#endif // DS_SIGNAL_H
