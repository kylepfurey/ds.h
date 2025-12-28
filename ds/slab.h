// .h
// Generic Slab Allocator Data Structure
// by Kyle Furey

#ifndef DS_SLAB_H
#define DS_SLAB_H

#include "vector.h"

/** Declares a named slab allocator of the given type. */
#define DECLARE_SLAB_NAMED(name, T, deleter)\
\
typedef struct {\
    uint32_t index;\
    uint32_t age;\
} name##_id;\
\
typedef struct {\
    uint32_t age;\
    T data;\
} __##name##_block;\
\
DECLARE_VECTOR_NAMED(__##name##_vector, __##name##_block, void_deleter)\
\
typedef struct {\
    size_t count;\
    name##_id next;\
    __##name##_vector vector;\
} name;\
\
static inline name name##_new(size_t capacity) {\
    assert(capacity > 0);\
    return (name) {\
        0,\
        (name##_id) {\
            0,\
            1,\
        },\
        __##name##_vector_new(capacity),\
    };\
}\
\
static inline bool name##_valid(const name *self, name##_id id) {\
    assert(self != NULL);\
    const __##name##_vector *vector = &self->vector;\
    assert(self->count <= vector->count);\
    assert(vector->array != NULL);\
    if (self->count == 0 || id.index >= vector->count) {\
        return false;\
    }\
    uint32_t age = vector->array[id.index].age;\
    return age != 0 && age == id.age;\
}\
\
static inline T *name##_get(name *self, name##_id id) {\
    assert(self != NULL);\
    assert(name##_valid(self, id));\
    __##name##_vector *vector = &self->vector;\
    assert(self->count <= vector->count);\
    assert(vector->array != NULL);\
    assert(id.index < vector->count);\
    return &vector->array[id.index].data;\
}\
\
static inline const T *name##_get_const(const name *self, name##_id id) {\
    assert(self != NULL);\
    assert(name##_valid(self, id));\
    const __##name##_vector *vector = &self->vector;\
    assert(self->count <= vector->count);\
    assert(vector->array != NULL);\
    assert(id.index < vector->count);\
    return &vector->array[id.index].data;\
}\
\
static inline name##_id name##_borrow(name *self, T data) {\
    assert(self != NULL);\
    __##name##_vector *vector = &self->vector;\
    assert(self->count <= vector->count);\
    assert(vector->array != NULL);\
    name##_id id = self->next;\
    if (id.index == vector->count) {\
        __##name##_vector_push(vector, (__##name##_block) { id.age, data, });\
        assert(self->count < vector->count);\
        ++self->next.index;\
    } else {\
        vector->array[id.index].age = id.age;\
        vector->array[id.index].data = data;\
        do {\
            ++self->next.index;\
        } while (self->next.index < vector->count &&\
                 vector->array[self->next.index].age != 0);\
    }\
    ++self->count;\
    ++self->next.age;\
    assert(self->next.age > 0);\
    return id;\
}\
\
static inline void name##_return(name *self, name##_id id) {\
    assert(self != NULL);\
    assert(self->count > 0);\
    assert(name##_valid(self, id));\
    __##name##_vector *vector = &self->vector;\
    assert(self->count <= vector->count);\
    assert(vector->array != NULL);\
    assert(id.index < vector->count);\
    --self->count;\
    if (self->next.index > id.index) {\
        self->next.index = id.index;\
    }\
    deleter(&vector->array[id.index].data);\
    vector->array[id.index].age = 0;\
}\
\
static inline void name##_clear(name *self) {\
    assert(self != NULL);\
    __##name##_vector *vector = &self->vector;\
    assert(self->count <= vector->count);\
    assert(vector->array != NULL);\
    if (self->count == 0) {\
        return;\
    }\
    for (size_t i = 0; i < vector->count; ++i) {\
        if (vector->array[i].age == 0) {\
            continue;\
        }\
        vector->array[i].age = 0;\
        deleter(&vector->array[i].data);\
        --self->count;\
        if (self->count == 0) {\
            break;\
        }\
    }\
    assert(self->count == 0);\
    self->next = (name##_id) { 0, self->next.age };\
}\
\
static inline bool name##_foreach(name *self, bool(*action)(T *)) {\
    assert(self != NULL);\
    assert(action != NULL);\
    __##name##_vector *vector = &self->vector;\
    assert(self->count <= vector->count);\
    assert(vector->array != NULL);\
    size_t remaining = self->count;\
    if (remaining == 0) {\
        return true;\
    }\
    for (size_t i = 0; i < vector->count; ++i) {\
        if (vector->array[i].age == 0) {\
            continue;\
        }\
        if (!action(&vector->array[i].data)) {\
            return false;\
        }\
        --remaining;\
        if (remaining == 0) {\
            break;\
        }\
    }\
    assert(remaining == 0);\
    return true;\
}\
\
static inline bool name##_foreach_const(const name *self, bool(*action)(const T *)) {\
    assert(self != NULL);\
    assert(action != NULL);\
    const __##name##_vector *vector = &self->vector;\
    assert(self->count <= vector->count);\
    assert(vector->array != NULL);\
    size_t remaining = self->count;\
    if (remaining == 0) {\
        return true;\
    }\
    for (size_t i = 0; i < vector->count; ++i) {\
        if (vector->array[i].age == 0) {\
            continue;\
        }\
        if (!action(&vector->array[i].data)) {\
            return false;\
        }\
        --remaining;\
        if (remaining == 0) {\
            break;\
        }\
    }\
    assert(remaining == 0);\
    return true;\
}\
\
static inline void name##_free(name *self) {\
    assert(self != NULL);\
    name##_clear(self);\
    __##name##_vector_free(&self->vector);\
}

/** Declares a slab allocator of the given type. */
#define DECLARE_SLAB(T, deleter) DECLARE_SLAB_NAMED(T##_slab, T, deleter)

#endif // DS_SLAB_H
