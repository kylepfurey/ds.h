// .h
// Generic Sorted Set Data Structure
// by Kyle Furey

#ifndef DS_SET_H
#define DS_SET_H

#include "def.h"

/** Declares a named sorted set of the given type. */
#define DECLARE_SET_NAMED(name, T, x_y_comparer, x_y_equals, deleter)\
\
typedef struct ds_##name##_node {\
    T data;\
    struct ds_##name##_node *left;\
    struct ds_##name##_node *right;\
} ds_##name##_node;\
\
typedef struct {\
    ds_size count;\
    ds_##name##_node *root;\
} name;\
\
static inline name name##_new() {\
    return (name) {\
        0,\
        NULL,\
    };\
}\
\
static inline ds_size name##_count(const name *self) {\
    assert(self != NULL);\
    return self->count;\
}\
\
static inline bool name##_empty(const name *self) {\
    assert(self != NULL);\
    return self->count == 0;\
}\
\
static inline const T *name##_find(const name *self, T data) {\
}\
\
static inline const T *name##_left(const name *self) {\
}\
\
static inline const T *name##_middle(const name *self) {\
}\
\
static inline const T *name##_right(const name *self) {\
}\
\
static inline bool name##_contains(const name *self, T data) {\
}\
\
static inline bool name##_insert(name *self, T data) {\
}\
\
static inline name name##_copy(const name *set) {\
}\
\
static inline bool name##_erase(name *self, T data) {\
}\
\
static inline bool name##_subset(const name *self, const name *set) {\
}\
\
static inline name *name##_union(name *self, const name *set) {\
}\
\
static inline name *name##_intersect(name *self, const name *set) {\
}\
\
static inline name *name##_difference(name *self, const name *set) {\
}\
\
static inline void name##_clear(name *self) {\
}\
\
static inline void name##_foreach(const name *self, void(*action)(T)) {\
}\
\
static inline void name##_delete(name *self) {\
}

/** Declares a sorted set of the given type. */
#define DECLARE_SET(T, x_y_comparer, x_y_equals, deleter) DECLARE_SET_NAMED(T##_set, T, x_y_comparer, x_y_equals, deleter)

#endif // DS_SET_H
