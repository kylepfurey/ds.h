// .h
// Generic Slab Allocator Data Structure
// by Kyle Furey

/**
 * ds_slab.h
 *
 * ds_DECLARE_SLAB_NAMED(
 *      name,               - The name of the data structure and function prefix.
 *
 *      T,                  - The type to generate this data structure with.
 *
 *      deleter,            - The name of the function used to deallocate T.
 *                            ds_void_deleter may be used for trivial types.
 * )
 *
 * This is a slab allocator. It is a mapping of IDs to same-size objects in a buffer.
 * Slab allocation is useful for reusing memory when large objects are being reallocated.
 *
 * Lightweight IDs ensure O(1) access to the memory and a way to quickly return it.
 * Borrowing has the potential to resize the buffer, so use IDs to refresh pointers often.
 *
 * * Returns a new slab with a current capacity of <capacity> objects.
 * * <capacity> must be greater than 0.
 * * This data structure must be deleted with slab_delete().
 *
 *   slab         slab_new            ( size_t capacity )
 *
 * * Returns a new slab shallow copied from <slab>.
 * * This data structure must be deleted with slab_delete().
 *
 *   slab         slab_copy           ( const slab* slab )
 *
 * * Returns the number of objects in the slab.
 *
 *   size_t       slab_count          ( const slab* self )
 *
 * * Returns the current maximum number of objects that can be contained in the slab.
 *
 *   size_t       slab_capacity       ( const slab* self )
 *
 * * Returns whether the slab is empty.
 *
 *   bool         slab_empty          ( const slab* self )
 *
 * * Returns whether <id> points to a valid object.
 *
 *   bool         slab_valid          ( const slab* self, slab_id id )
 *
 * * Returns a pointer to an object with <id>.
 * * <id> must be a valid ID.
 *
 *   T*           slab_get            ( slab* self, slab_id id )
 *
 * * Returns a pointer to an object with <id>.
 * * <id> must be a valid ID.
 *
 *   const T*     slab_get_const      ( const slab* self, slab_id id )
 *
 * * Allocates a new object in the slab with <data>.
 * * This may resize the buckets and invalidate pointers, so store the ID.
 * * Returns the objects new ID.
 *
 *   slab_id      slab_borrow         ( slab* self, T data )
 *
 * * Frees the memory for the object with <id>.
 * * <id> must be a valid ID.
 *
 *   void         slab_return         ( slab* self, slab_id id )
 *
 * * Deletes all objects in a slab.
 *
 *   void         slab_clear          ( slab* self )
 *
 * * Iterates the slab calling <action> on each object.
 *
 *   void         slab_foreach        ( const slab* self, void(*action)(T) )
 *
 * * Safely deletes a slab.
 *
 *   void         slab_delete         ( slab* self )
 */

#ifndef DS_SLAB_H
#define DS_SLAB_H

#include "ds_vector.h"

/** Declares a named slab allocator of the given type. */
#define ds_DECLARE_SLAB_NAMED(name, T, deleter)\
\
typedef struct {\
    ds_uint index;\
    ds_uint age;\
} name##_id;\
\
typedef struct {\
    T data;\
    ds_uint age;\
} ds__##name##_block;\
\
ds_DECLARE_VECTOR_NAMED(ds__##name##_vector, ds__##name##_block, ds_void_deleter)\
\
typedef struct {\
    ds_size count;\
    name##_id next;\
    ds__##name##_vector buckets;\
} name;\
\
ds_API static inline name name##_new(ds_size capacity) {\
    ds_assert(capacity > 0);\
    return (name) {\
        0,\
        (name##_id) {\
            0,\
            1,\
        },\
        ds__##name##_vector_new(capacity),\
    };\
}\
\
ds_API static inline name name##_copy(const name *slab) {\
    ds_assert(slab != NULL);\
    return (name) {\
        slab->count,\
        slab->next,\
        ds__##name##_vector_copy(&slab->buckets),\
    };\
}\
\
ds_API static inline ds_size name##_count(const name *self) {\
    ds_assert(self != NULL);\
    return self->count;\
}\
\
ds_API static inline ds_size name##_capacity(const name *self) {\
    ds_assert(self != NULL);\
    return self->buckets.capacity;\
}\
\
ds_API static inline ds_bool name##_empty(const name *self) {\
    ds_assert(self != NULL);\
    return self->count == 0;\
}\
\
ds_API static inline ds_bool name##_valid(const name *self, name##_id id) {\
    ds_assert(self != NULL);\
    const ds__##name##_vector *vector = &self->buckets;\
    ds_assert(self->count <= vector->count);\
    ds_assert(vector->array != NULL);\
    if (self->count == 0 || id.index >= vector->count) {\
        return ds_false;\
    }\
    ds_uint age = vector->array[id.index].age;\
    return age != 0 && age == id.age;\
}\
\
ds_API static inline T *name##_get(name *self, name##_id id) {\
    ds_assert(self != NULL);\
    ds_assert(name##_valid(self, id));\
    ds__##name##_vector *vector = &self->buckets;\
    ds_assert(self->count <= vector->count);\
    ds_assert(vector->array != NULL);\
    ds_assert(id.index < vector->count);\
    return &vector->array[id.index].data;\
}\
\
ds_API static inline const T *name##_get_const(const name *self, name##_id id) {\
    ds_assert(self != NULL);\
    ds_assert(name##_valid(self, id));\
    const ds__##name##_vector *vector = &self->buckets;\
    ds_assert(self->count <= vector->count);\
    ds_assert(vector->array != NULL);\
    ds_assert(id.index < vector->count);\
    return &vector->array[id.index].data;\
}\
\
ds_API static inline name##_id name##_borrow(name *self, T data) {\
    ds_assert(self != NULL);\
    ds__##name##_vector *vector = &self->buckets;\
    ds_assert(self->count <= vector->count);\
    ds_assert(vector->array != NULL);\
    name##_id id = self->next;\
    if (id.index == vector->count) {\
        ds__##name##_vector_push(\
            vector,\
            (ds__##name##_block) {\
                data,\
                id.age,\
            }\
        );\
        ds_assert(self->count < vector->count);\
        ++self->next.index;\
    } else {\
        vector->array[id.index].data = data;\
        vector->array[id.index].age = id.age;\
        do {\
            ++self->next.index;\
        } while (self->next.index < vector->count &&\
                 vector->array[self->next.index].age != 0);\
    }\
    ++self->count;\
    ++self->next.age;\
    ds_assert(self->next.age > 0);\
    return id;\
}\
\
ds_API static inline void name##_return(name *self, name##_id id) {\
    ds_assert(self != NULL);\
    ds_assert(self->count > 0);\
    ds_assert(name##_valid(self, id));\
    ds__##name##_vector *vector = &self->buckets;\
    ds_assert(self->count <= vector->count);\
    ds_assert(vector->array != NULL);\
    ds_assert(id.index < vector->count);\
    --self->count;\
    if (self->next.index > id.index) {\
        self->next.index = id.index;\
    }\
    deleter(&vector->array[id.index].data);\
    vector->array[id.index].age = 0;\
}\
\
ds_API static inline void name##_clear(name *self) {\
    ds_assert(self != NULL);\
    ds__##name##_vector *vector = &self->buckets;\
    ds_assert(self->count <= vector->count);\
    ds_assert(vector->array != NULL);\
    if (self->count == 0) {\
        return;\
    }\
    for (ds_size i = 0; i < vector->count; ++i) {\
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
    ds_assert(self->count == 0);\
    self->next = (name##_id) {\
        0,\
        self->next.age,\
    };\
}\
\
ds_API static inline void name##_foreach(const name *self, void(*action)(T)) {\
    ds_assert(self != NULL);\
    ds_assert(action != NULL);\
    const ds__##name##_vector *vector = &self->buckets;\
    ds_assert(self->count <= vector->count);\
    ds_assert(vector->array != NULL);\
    ds_size remaining = self->count;\
    for (ds_size i = 0; remaining > 0 && i < vector->count; ++i) {\
        if (vector->array[i].age == 0) {\
            continue;\
        }\
        action(vector->array[i].data);\
        --remaining;\
    }\
    ds_assert(remaining == 0);\
}\
\
ds_API static inline void name##_delete(name *self) {\
    ds_assert(self != NULL);\
    name##_clear(self);\
    ds__##name##_vector_delete(&self->buckets);\
    *self = (name) {0};\
}

/** Declares a slab allocator of the given type. */
#define ds_DECLARE_SLAB(T, deleter)\
        ds_DECLARE_SLAB_NAMED(T##_slab, T, deleter)

#endif // DS_SLAB_H
