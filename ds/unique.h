// .h
// Generic Unique Reference Data Structure
// by Kyle Furey

#ifndef DS_UNIQUE_H
#define DS_UNIQUE_H

#include "std.h"

/** Declares a named unique pointer for the given type. */
#define DECLARE_UNIQUE_NAMED(name, T, deleter)\
\
typedef struct {\
    T *data;\
} name;\
\
static inline name name##_new(T data) {\
    T *ptr = (T *) malloc(sizeof(T));\
    assert(ptr != NULL);\
    *ptr = data;\
    return (name) {\
        ptr,\
    };\
}\
\
static inline void name##_free(name *self) {\
    assert(self != NULL);\
    assert(self->data != NULL);\
    deleter(self->data);\
    free(self->data);\
    self->data = NULL;\
}\
\
static inline T *name##_get(name *self) {\
    assert(self != NULL);\
    assert(self->data != NULL);\
    return self->data;\
}\
\
static inline const T *name##_get_const(const name *self) {\
    assert(self != NULL);\
    assert(self->data != NULL);\
    return self->data;\
}

/** Declares a unique pointer for the given type. */
#define DECLARE_UNIQUE(T, deleter) DECLARE_UNIQUE_NAMED(unique_##T, T, deleter)

#endif // DS_UNIQUE_H
