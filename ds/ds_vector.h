// .h
// ds.h Dynamic Array Data Structure
// by Kyle Furey

/**
 * ds_vector.h
 *
 * ds_DECLARE_VECTOR_NAMED(
 *      name,               - The name of the data structure and function prefix.
 *
 *      T,                  - The type to generate this data structure with.
 *
 *      deleter,            - The name of the function used to deallocate T.
 *                            ds_void_deleter may be used for trivial types.
 * )
 *
 * Vectors are dynamic arrays. They own an expanding buffer to hold new elements.
 * Vectors also cache their size and capacity for runtime checks when accessing memory.
 *
 * Vectors are excellent general-use data structures for storing multiple elements.
 * Indexing into vectors provides the fastest random access to their elements.
 * Inserting and removing into the back of a vector is also very fast.
 *
 * * Returns a new vector with a current capacity of <capacity> elements.
 * * <capacity> must be greater than 0.
 * * This data structure must be deleted with vector_delete().
 *
 *   vector       vector_new              ( size_t capacity )
 *
 * * Returns a new vector copied from <vector>.
 * * The new vector owns its own memory and must be deleted with vector_delete().
 *
 *   vector       vector_copy             ( const vector* vector )
 *
 * * Returns the number of elements in the vector.
 *
 *   size_t       vector_count            ( const vector* self )
 *
 * * Returns the current maximum number of elements that can be contained in the vector.
 *
 *   size_t       vector_capacity         ( const vector* self )
 *
 * * Returns whether the vector is empty.
 *
 *   bool         vector_empty            ( const vector* self )
 *
 * * Returns a pointer to the element in the vector at <index>.
 * * <index> must be a valid index.
 *
 *   T*           vector_get              ( vector* self, size_t index )
 *
 * * Returns a pointer to the element in the vector at <index>.
 * * <index> must be a valid index.
 *
 *   const T*     vector_get_const        ( const vector* self, size_t index )
 *
 * * Returns a pointer to the vector's array.
 *
 *   T*           vector_array            ( vector* self )
 *
 * * Returns a pointer to the vector's array.
 *
 *   const T*     vector_array_const      ( const vector* self )
 *
 * * Resizes the vector to contain <capacity> number of elements.
 * * If ds_VECTOR_TRUNC_ASSERT is true, this will assert if elements are deleted.
 *
 *   void         vector_resize           ( vector* self, size_t capacity )
 *
 * * Inserts <data> at <index> into the vector.
 * * <index> must be a valid index or the end of the vector.
 *
 *   void         vector_insert           ( vector* self, size_t index, T data )
 *
 * * Erases the element at <index> from the vector.
 * * <index> must be a valid index.
 *
 *   void         vector_erase            ( vector* self, size_t index )
 *
 * * Inserts <data> at the end of the vector.
 *
 *   void         vector_push             ( vector* self, T data )
 *
 * * Deletes the element at the end of the vector.
 * * The vector must not be empty.
 *
 *   void         vector_pop              ( vector* self )
 *
 * * Reverses and returns the vector.
 * * Returns a pointer to the vector's array.
 *
 *   T*           vector_reverse          ( vector* self )
 *
 * * Deletes all elements in a vector.
 *
 *   void         vector_clear            ( vector* self )
 *
 * * Iterates and mutates the vector.
 * * This replaces each element after calling <transform>.
 * * Elements in the vector are not deleted, only updated.
 * * Returns the vector's data.
 *
 *   T*           vector_map              ( vector* self, T(*transform)(T) )
 *
 * * Iterates and mutates the vector.
 * * This deletes elements that return false in <predicate>.
 * * Returns the new size of the vector.
 *
 *   size_t       vector_filter           ( vector* self, bool(*predicate)(T) )
 *
 * * Iterates the vector and computes a value.
 * * Each element is "applied" to <start> using <accumulator>:
 * * foreach elem => start = accumulator(start, elem)
 * * Returns the accumulated value.
 *
 *   T            vector_reduce           ( vector* self, T start, T(*accumulator)(T, T) )
 *
 * * Iterates the vector calling <action> on each element.
 *
 *   void         vector_foreach          ( const vector* self, void(*action)(T) )
 *
 * * Safely deletes a vector.
 *
 *   void         vector_delete           ( vector* self )
 */

#ifndef DS_VECTOR_H
#define DS_VECTOR_H

#include "ds_def.h"

/** Declares a named dynamic array of the given type. */
#define ds_DECLARE_VECTOR_NAMED(name, T, deleter)\
\
typedef struct {\
    ds_size count;\
    ds_size capacity;\
    T *array;\
} name;\
\
ds_API static inline name name##_new(ds_size capacity) {\
    ds_assert(capacity > 0);\
    T *array = (T *) ds_malloc(sizeof(T) * capacity);\
    ds_assert(array != ds_NULL);\
    return (name) {\
        0,\
        capacity,\
        array,\
    };\
}\
\
ds_API static inline name name##_copy(const name *vector) {\
    ds_assert(vector != ds_NULL);\
    name self = name##_new(vector->capacity);\
    self.count = vector->count;\
    ds_memcpy(self.array, vector->array, sizeof(T) * self.count);\
    return self;\
}\
\
ds_API static inline ds_size name##_count(const name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count <= self->capacity);\
    return self->count;\
}\
\
ds_API static inline ds_size name##_capacity(const name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count <= self->capacity);\
    return self->capacity;\
}\
\
ds_API static inline ds_bool name##_empty(const name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count <= self->capacity);\
    return self->count == 0;\
}\
\
ds_API static inline T *name##_get(name *self, ds_size index) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != ds_NULL);\
    ds_assert(index < self->count);\
    return self->array + index;\
}\
\
ds_API static inline const T *name##_get_const(const name *self, ds_size index) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != ds_NULL);\
    ds_assert(index < self->count);\
    return self->array + index;\
}\
\
ds_API static inline T *name##_array(name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != ds_NULL);\
    return self->array;\
}\
\
ds_API static inline const T *name##_array_const(const name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != ds_NULL);\
    return self->array;\
}\
\
ds_API static inline void name##_resize(name *self, ds_size capacity) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != ds_NULL);\
    ds_assert(capacity > 0);\
    if (capacity == self->capacity) {\
        return;\
    }\
    if (ds_VECTOR_TRUNC_ASSERT) {\
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
    ds_assert(array != ds_NULL);\
    self->array = array;\
}\
\
ds_API static inline void name##_insert(name *self, ds_size index, T data) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != ds_NULL);\
    ds_assert(index <= self->count);\
    ds_assert(self->capacity <= ds_SIZE_MAX / (sizeof(T) * ds_VECTOR_EXPANSION));\
    if (self->count == self->capacity) {\
        name##_resize(self, self->capacity * ds_VECTOR_EXPANSION);\
    }\
    ds_memmove(self->array + index + 1, self->array + index, sizeof(T) * (self->count - index));\
    ++self->count;\
    self->array[index] = data;\
}\
\
ds_API static inline void name##_erase(name *self, ds_size index) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != ds_NULL);\
    ds_assert(index < self->count);\
    deleter(self->array + index);\
    ds_memmove(self->array + index, self->array + index + 1, sizeof(T) * (self->count - index - 1));\
    --self->count;\
}\
\
ds_API static inline void name##_push(name *self, T data) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->capacity <= ds_SIZE_MAX / (sizeof(T) * ds_VECTOR_EXPANSION));\
    if (self->count == self->capacity) {\
        name##_resize(self, self->capacity * ds_VECTOR_EXPANSION);\
    }\
    self->array[self->count] = data;\
    ++self->count;\
}\
\
ds_API static inline void name##_pop(name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != ds_NULL);\
    ds_assert(self->count > 0);\
    --self->count;\
    deleter(&self->array[self->count]);\
}\
\
ds_API static inline T *name##_reverse(name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != ds_NULL);\
    ds_size count = self->count / 2;\
    for (ds_size i = 0; i < count; ++i) {\
        T temp = self->array[i];\
        self->array[i] = self->array[self->count - i - 1];\
        self->array[self->count - i - 1] = temp;\
    }\
    return self->array;\
}\
\
ds_API static inline void name##_clear(name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != ds_NULL);\
    for (ds_size i = 0; i < self->count; ++i) {\
        deleter(self->array + i);\
    }\
    self->count = 0;\
}\
\
ds_API static inline T *name##_map(name *self, T(*transform)(T)) {\
    ds_assert(self != ds_NULL);\
    ds_assert(transform != ds_NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != ds_NULL);\
    for (ds_size i = 0; i < self->count; ++i) {\
        self->array[i] = transform(self->array[i]);\
    }\
    return self->array;\
}\
\
ds_API static inline ds_size name##_filter(name *self, ds_bool(*predicate)(T)) {\
    ds_assert(self != ds_NULL);\
    ds_assert(predicate != ds_NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != ds_NULL);\
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
ds_API static inline T name##_reduce(const name *self, T start, T(*accumulator)(T, T)) {\
    ds_assert(self != ds_NULL);\
    ds_assert(accumulator != ds_NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != ds_NULL);\
    for (ds_size i = 0; i < self->count; ++i) {\
        start = accumulator(start, self->array[i]);\
    }\
    return start;\
}\
\
ds_API static inline void name##_foreach(const name *self, void(*action)(T)) {\
    ds_assert(self != ds_NULL);\
    ds_assert(action != ds_NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != ds_NULL);\
    for (ds_size i = 0; i < self->count; ++i) {\
        action(self->array[i]);\
    }\
}\
\
ds_API static inline void name##_delete(name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count <= self->capacity);\
    ds_assert(self->array != ds_NULL);\
    name##_clear(self);\
    ds_free(self->array);\
    *self = (name) {0};\
}

/** Declares a dynamic array of the given type. */
#define ds_DECLARE_VECTOR(T, deleter)\
        ds_DECLARE_VECTOR_NAMED(T##_vector, T, deleter)

#endif // DS_VECTOR_H
