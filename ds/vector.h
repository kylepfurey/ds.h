// .h
// Generic Dynamic Array Data Structure
// by Kyle Furey

#ifndef DS_VECTOR_H
#define DS_VECTOR_H

#include "std.h"

/** Declares a named dynamic array of the given type. */
#define DECLARE_VECTOR_NAMED(name, T, deleter)\
\
typedef struct {\
    size_t count;\
    size_t capacity;\
    T *array;\
} name;\
\
static inline name name##_new(size_t capacity) {\
    assert(capacity > 0);\
    T *array = (T *) malloc(sizeof(T) * capacity);\
    assert(array != NULL);\
    return (name) {\
        0,\
        capacity,\
        array,\
    };\
}\
\
static inline T *name##_get(name *self, size_t index) {\
    assert(self != NULL);\
    assert(self->count <= self->capacity);\
    assert(self->array != NULL);\
    assert(index < self->count);\
    return self->array + index;\
}\
\
static inline const T *name##_get_const(const name *self, size_t index) {\
    assert(self != NULL);\
    assert(self->count <= self->capacity);\
    assert(self->array != NULL);\
    assert(index < self->count);\
    return self->array + index;\
}\
\
static inline void name##_resize(name *self, size_t capacity) {\
    assert(self != NULL);\
    assert(self->count <= self->capacity);\
    assert(self->array != NULL);\
    assert(capacity > 0);\
    if (capacity == self->capacity) {\
        return;\
    }\
    if (capacity < self->count) {\
        for (size_t i = capacity; i < self->count; ++i) {\
            deleter(self->array + i);\
        }\
        self->count = capacity;\
    }\
    self->capacity = capacity;\
    T *array = (T *) realloc(self->array, sizeof(T) * capacity);\
    assert(array != NULL);\
    self->array = array;\
}\
\
static inline void name##_insert(name *self, size_t index, T data) {\
    assert(self != NULL);\
    assert(self->count <= self->capacity);\
    assert(self->array != NULL);\
    assert(index <= self->count);\
    assert(self->capacity <= SIZE_MAX / (sizeof(T) * 2));\
    if (self->count == self->capacity) {\
        name##_resize(self, self->capacity * 2);\
    }\
    memmove(self->array + index + 1, self->array + index, sizeof(T) * (self->count - index));\
    ++self->count;\
    self->array[index] = data;\
}\
\
static inline void name##_erase(name *self, size_t index) {\
    assert(self != NULL);\
    assert(self->count <= self->capacity);\
    assert(self->array != NULL);\
    assert(index < self->count);\
    deleter(self->array + index);\
    memmove(self->array + index, self->array + index + 1, sizeof(T) * (self->count - index - 1));\
    --self->count;\
}\
\
static inline void name##_push(name *self, T data) {\
    assert(self != NULL);\
    assert(self->count <= self->capacity);\
    assert(self->array != NULL);\
    name##_insert(self, self->count, data);\
}\
\
static inline void name##_pop(name *self) {\
    assert(self != NULL);\
    assert(self->count <= self->capacity);\
    assert(self->array != NULL);\
    assert(self->count > 0);\
    name##_erase(self, self->count - 1);\
}\
\
static inline void name##_clear(name *self) {\
    assert(self != NULL);\
    assert(self->count <= self->capacity);\
    assert(self->array != NULL);\
    for (size_t i = 0; i < self->count; ++i) {\
        deleter(self->array + i);\
    }\
    self->count = 0;\
}\
\
static inline bool name##_foreach(name *self, bool(*action)(T *)) {\
    assert(self != NULL);\
    assert(action != NULL);\
    assert(self->count <= self->capacity);\
    assert(self->array != NULL);\
    for (size_t i = 0; i < self->count; ++i) {\
        if (!action(&self->array[i])) {\
            return false;\
        }\
    }\
    return true;\
}\
\
static inline bool name##_foreach_const(const name *self, bool(*action)(const T *)) {\
    assert(self != NULL);\
    assert(action != NULL);\
    assert(self->count <= self->capacity);\
    assert(self->array != NULL);\
    for (size_t i = 0; i < self->count; ++i) {\
        if (!action(&self->array[i])) {\
            return false;\
        }\
    }\
    return true;\
}\
\
static inline void name##_free(name *self) {\
    assert(self != NULL);\
    assert(self->count <= self->capacity);\
    assert(self->array != NULL);\
    name##_clear(self);\
    self->capacity = 0;\
    free(self->array);\
    self->array = NULL;\
}

/** Declares a dynamic array of the given type. */
#define DECLARE_VECTOR(T, deleter) DECLARE_VECTOR_NAMED(T##_vector, T, deleter)

#endif // DS_VECTOR_H
