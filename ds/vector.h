// .h
// Generic Dynamic Array Data Structure
// by Kyle Furey

#ifndef DS_VECTOR_H
#define DS_VECTOR_H

#include "std.h"

/** The rate to expand vectors at. */
#define VECTOR_EXPANSION 2

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
static inline name name##_copy(const name *vector) {\
    assert(vector != NULL);\
    name self = name##_new(vector->capacity);\
    self.count = vector->count;\
    memcpy(self.array, vector->array, sizeof(T) * self.count);\
    return self;\
}\
\
static inline size_t name##_count(const name *self) {\
    assert(self != NULL);\
    assert(self->count <= self->capacity);\
    return self->count;\
}\
\
static inline size_t name##_capacity(const name *self) {\
    assert(self != NULL);\
    assert(self->count <= self->capacity);\
    return self->capacity;\
}\
\
static inline bool name##_empty(const name *self) {\
    assert(self != NULL);\
    assert(self->count <= self->capacity);\
    return self->count == 0;\
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
    assert(self->capacity <= SIZE_MAX / (sizeof(T) * VECTOR_EXPANSION));\
    if (self->count == self->capacity) {\
        name##_resize(self, self->capacity * VECTOR_EXPANSION);\
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
    name##_insert(self, self->count, data);\
}\
\
static inline void name##_pop(name *self) {\
    assert(self != NULL);\
    assert(self->count <= self->capacity);\
    assert(self->count > 0);\
    name##_erase(self, self->count - 1);\
}\
\
static inline T *name##_reverse(name *self) {\
    assert(self != NULL);\
    assert(self->count <= self->capacity);\
    assert(self->array != NULL);\
    size_t count = self->count / 2;\
    for (size_t i = 0; i < count; ++i) {\
        T temp = self->array[i];\
        self->array[i] = self->array[self->count - i - 1];\
        self->array[self->count - i - 1] = temp;\
    }\
    return self->array;\
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
static inline T *name##_map(name *self, T(*transform)(T)) {\
    assert(self != NULL);\
    assert(transform != NULL);\
    assert(self->count <= self->capacity);\
    assert(self->array != NULL);\
    for (size_t i = 0; i < self->count; ++i) {\
        self->array[i] = transform(self->array[i]);\
    }\
    return self->array;\
}\
\
static inline size_t name##_filter(name *self, bool(*predicate)(T)) {\
    assert(self != NULL);\
    assert(predicate != NULL);\
    assert(self->count <= self->capacity);\
    assert(self->array != NULL);\
    size_t total = 0;\
    for (size_t i = 0; i < self->count; ++i) {\
        if (predicate(self->array[i])) {\
            self->array[total++] = self->array[i];\
        } else {\
            deleter(&self->array[i]);\
        }\
    }\
    self->count = total;\
    return total;\
}\
\
static inline T name##_reduce(name *self, T start, T(*accumulator)(T, T)) {\
    assert(self != NULL);\
    assert(accumulator != NULL);\
    assert(self->count <= self->capacity);\
    assert(self->array != NULL);\
    for (size_t i = 0; i < self->count; ++i) {\
        start = accumulator(start, self->array[i]);\
    }\
    return start;\
}\
\
static inline void name##_foreach(const name *self, void(*action)(T)) {\
    assert(self != NULL);\
    assert(action != NULL);\
    assert(self->count <= self->capacity);\
    assert(self->array != NULL);\
    for (size_t i = 0; i < self->count; ++i) {\
        action(self->array[i]);\
    }\
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
