// .h
// Generic Dynamic Array Data Structure
// by Kyle Furey

/**
 * ds_vector.h
 *
 * Vectors are dynamic arrays. They own an expanding buffer to hold new elements.
 * Vectors also cache their size and capacity for runtime checks when accessing memory.
 *
 * Vectors are excellent general-use data structures for storing multiple elements.
 * Indexing into vectors provide the fastest random access to their elements.
 * Inserting and removing into the back of a vector is also very fast.
 *
 * vector       vector_new          ( size_t capacity )
 *
 * vector       vector_copy         ( const vector* vector )
 *
 * size_t       vector_count        ( const vector* self )
 *
 * size_t       vector_capacity     ( const vector* self )
 *
 * bool         vector_empty        ( const vector* self )
 *
 * T*           vector_get          ( vector* self, size_t index )
 *
 * const T*     vector_get_const    ( const vector* self, size_t index )
 *
 * void         vector_resize       ( vector* self, size_t capacity )
 *
 * void         vector_insert       ( vector* self, size_t index, T data )
 *
 * void         vector_erase        ( vector* self, size_t index )
 *
 * void         vector_push         ( vector* self, T data )
 *
 * void         vector_pop          ( vector* self )
 *
 * T*           vector_reverse      ( vector* self )
 *
 * void         vector_clear        ( vector* self )
 *
 * T*           vector_map          ( vector* self, T(*transform)(T) )
 *
 * size_t       vector_filter       ( vector* self, bool(*predicate)(T) )
 *
 * T            vector_reduce       ( vector* self, T start, T(*accumulator)(T, T) )
 *
 * void         vector_foreach      ( const vector* self, void(*action)(T) )
 *
 * void         vector_delete       ( vector* self )
 */

#ifndef DS_VECTOR_H
#define DS_VECTOR_H

#include "ds_def.h"

/** Declares a named dynamic array of the given type. */
#define DECLARE_VECTOR_NAMED(name, T, deleter)\
\
typedef struct {\
    ds_size count;\
    ds_size capacity;\
    T *array;\
} name;\
\
DS_API static inline name name##_new(ds_size capacity) {\
    ds_assert(capacity > 0);\
    T *array = (T *) ds_malloc(sizeof(T) * capacity);\
    ds_assert(array != NULL);\
    return (name) {\
        0,\
        capacity,\
        array,\
    };\
}\
\
DS_API static inline name name##_copy(const name *vector) {\
    ds_assert(vector != NULL);\
    name self = name##_new(vector->capacity);\
    self.count = vector->count;\
    memcpy(self.array, vector->array, sizeof(T) * self.count);\
    return self;\
}\
\
DS_API static inline ds_size name##_count(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->count <= self->capacity);\
    return self->count;\
}\
\
DS_API static inline ds_size name##_capacity(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->count <= self->capacity);\
    return self->capacity;\
}\
\
DS_API static inline bool name##_empty(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->count <= self->capacity);\
    return self->count == 0;\
}\
\
DS_API static inline T *name##_get(name *self, ds_size index) {\
    ds_assert(self != NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != NULL);\
    ds_assert(index < self->count);\
    return self->array + index;\
}\
\
DS_API static inline const T *name##_get_const(const name *self, ds_size index) {\
    ds_assert(self != NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != NULL);\
    ds_assert(index < self->count);\
    return self->array + index;\
}\
\
DS_API static inline void name##_resize(name *self, ds_size capacity) {\
    ds_assert(self != NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != NULL);\
    ds_assert(capacity > 0);\
    if (capacity == self->capacity) {\
        return;\
    }\
    if (VECTOR_TRUNC_ASSERT) {\
        ds_assert(capacity >= self->count);\
    }\
    if (capacity < self->count) {\
        for (ds_size i = capacity; i < self->count; ++i) {\
            deleter(self->array + i);\
        }\
        self->count = capacity;\
    }\
    self->capacity = capacity;\
    T *array = (T *) ds_realloc(self->array, sizeof(T) * capacity);\
    ds_assert(array != NULL);\
    self->array = array;\
}\
\
DS_API static inline void name##_insert(name *self, ds_size index, T data) {\
    ds_assert(self != NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != NULL);\
    ds_assert(index <= self->count);\
    ds_assert(self->capacity <= SIZE_MAX / (sizeof(T) * VECTOR_EXPANSION));\
    if (self->count == self->capacity) {\
        name##_resize(self, self->capacity * VECTOR_EXPANSION);\
    }\
    memmove(self->array + index + 1, self->array + index, sizeof(T) * (self->count - index));\
    ++self->count;\
    self->array[index] = data;\
}\
\
DS_API static inline void name##_erase(name *self, ds_size index) {\
    ds_assert(self != NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != NULL);\
    ds_assert(index < self->count);\
    deleter(self->array + index);\
    memmove(self->array + index, self->array + index + 1, sizeof(T) * (self->count - index - 1));\
    --self->count;\
}\
\
DS_API static inline void name##_push(name *self, T data) {\
    ds_assert(self != NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->capacity <= SIZE_MAX / (sizeof(T) * VECTOR_EXPANSION));\
    if (self->count == self->capacity) {\
        name##_resize(self, self->capacity * VECTOR_EXPANSION);\
    }\
    self->array[self->count] = data;\
    ++self->count;\
}\
\
DS_API static inline void name##_pop(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != NULL);\
    ds_assert(self->count > 0);\
    --self->count;\
    deleter(&self->array[self->count]);\
}\
\
DS_API static inline T *name##_reverse(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != NULL);\
    ds_size count = self->count / 2;\
    for (ds_size i = 0; i < count; ++i) {\
        T temp = self->array[i];\
        self->array[i] = self->array[self->count - i - 1];\
        self->array[self->count - i - 1] = temp;\
    }\
    return self->array;\
}\
\
DS_API static inline void name##_clear(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != NULL);\
    for (ds_size i = 0; i < self->count; ++i) {\
        deleter(self->array + i);\
    }\
    self->count = 0;\
}\
\
DS_API static inline T *name##_map(name *self, T(*transform)(T)) {\
    ds_assert(self != NULL);\
    ds_assert(transform != NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != NULL);\
    for (ds_size i = 0; i < self->count; ++i) {\
        self->array[i] = transform(self->array[i]);\
    }\
    return self->array;\
}\
\
DS_API static inline ds_size name##_filter(name *self, bool(*predicate)(T)) {\
    ds_assert(self != NULL);\
    ds_assert(predicate != NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != NULL);\
    ds_size total = 0;\
    for (ds_size i = 0; i < self->count; ++i) {\
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
DS_API static inline T name##_reduce(name *self, T start, T(*accumulator)(T, T)) {\
    ds_assert(self != NULL);\
    ds_assert(accumulator != NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != NULL);\
    for (ds_size i = 0; i < self->count; ++i) {\
        start = accumulator(start, self->array[i]);\
    }\
    return start;\
}\
\
DS_API static inline void name##_foreach(const name *self, void(*action)(T)) {\
    ds_assert(self != NULL);\
    ds_assert(action != NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != NULL);\
    for (ds_size i = 0; i < self->count; ++i) {\
        action(self->array[i]);\
    }\
}\
\
DS_API static inline void name##_delete(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != NULL);\
    name##_clear(self);\
    ds_free(self->array);\
    *self = (name) {0};\
}

/** Declares a dynamic array of the given type. */
#define DECLARE_VECTOR(T, deleter) DECLARE_VECTOR_NAMED(T##_vector, T, deleter)

#endif // DS_VECTOR_H
