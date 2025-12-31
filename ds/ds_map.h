// .h
// Generic Key-Value Hash Map Data Structure
// by Kyle Furey

/**
 * ds_map.h
 *
 * ds_DECLARE_MAP_NAMED(
 *      name,           - The name of the data structure and function prefix.
 *
 *      K,              - The key type used to index V.
 *
 *      V,              - The value type to generate this data structure with.
 *
 *      key_hasher,     - Inline hashing code used to hash a key named <key>.
 *                        You can use ds_DEFAULT_HASH, ds_INT_HASH, ds_STRING_HASH, or a custom hasher.
 *
 *      x_y_equals,     - Inline comparison code used to equate keys <x> and <y>.
 *                        You can use ds_DEFAULT_EQUALS for trivial types.
 *
 *      value_deleter,  - The name of the function used to deallocate V.
 *                        ds_void_deleter may be used for trivial types.
 * )
 *
 * This is a simple open addressing key-value hash map.
 * Values are stored in buckets in an underlying vector.
 * Values are indexed by hashing their key type into a number for near O(1) operations.
 * This number is used to probe for finding where an element may be quickly.
 *
 * It's one of the fastest options for storing, finding, and removing key-value pairs.
 *
 * * Returns a new map with <capacity> number of buckets.
 * * <capacity> must be greater than 0.
 * * This data structure must be deleted with map_delete().
 *
 *   map          map_new             ( size_t capacity )
 *
 * * Returns a new map shallow copied from <map>.
 * * This data structure must be deleted with map_delete().
 *
 *   map          map_copy            ( const map* map )
 *
 * * Returns the number of elements in the map.
 *
 *   size_t       map_count           ( const map* self )
 *
 * * Returns the number of the buckets in the map.
 *
 *   size_t       map_capacity        ( const map* self )
 *
 * * Returns whether the map is empty.
 *
 *   bool         map_empty           ( const map* self )
 *
 * * Returns a pointer to a value that matches <key> in the map.
 * * Returns NULL if no key is found.
 *
 *   V*           map_find            ( map* self, K key )
 *
 * * Returns a pointer to a value that matches <key> in the map.
 * * Returns NULL if no key is found.
 *
 *   const V*     map_find_const      ( const map* self, K key )
 *
 * * Returns whether the map contains <key>.
 *
 *   bool         map_contains        ( const map* self, K key )
 *
 * * Sets the number of buckets in the map.
 * * This must not be less than the number of elements.
 *
 *   void         map_resize          ( map* self, size_t capacity )
 *
 * * Inserts a new key-value pair into the map.
 * * Returns whether a value was overwritten.
 *
 *   bool         map_insert          ( map* self, K key, V value )
 *
 * * Deletes the value that matches <key>.
 * * Returns whether <key> was found.
 *
 *   bool         map_erase           ( map* self, K key )
 *
 * * Deletes all pairs in the map.
 *
 *   void         map_clear           ( map* self )
 *
 * * Iterates the map calling <action> on each key and value.
 *
 *   void         map_foreach         ( const map* self, void (*action)(K, V) )
 *
 * * Iterates the map calling <action> on each key.
 *
 *   void         map_foreach_key     ( const map* self, void (*action)(K) )
 *
 * * Iterates the map calling <action> on each value.
 *
 *   void         map_foreach_value   ( const map* self, void (*action)(V) )
 *
 * * Safely deletes a map.
 *
 *   void         map_delete          ( map* self )
 */

#ifndef DS_MAP_H
#define DS_MAP_H

#include "ds_vector.h"

/** Declares a named key-value hash map of the given types. */
#define ds_DECLARE_MAP_NAMED(name, K, V, key_hasher, x_y_equals, value_deleter)\
\
typedef struct {\
    K key;\
    V value;\
    ds_uint state;\
} ds__##name##_bucket;\
\
ds_DECLARE_VECTOR_NAMED(ds__##name##_vector, ds__##name##_bucket, ds_void_deleter)\
\
typedef struct {\
    ds_size count;\
    ds__##name##_vector buckets;\
} name;\
\
ds_API static inline name name##_new(ds_size capacity) {\
    ds_assert(capacity > 0);\
    name self = (name) {\
        0,\
        ds__##name##_vector_new(capacity),\
    };\
    ds_memset(self.buckets.array, 0, sizeof(ds__##name##_bucket) * capacity);\
    return self;\
}\
\
ds_API static inline name name##_copy(const name *map) {\
    ds_assert(map != NULL);\
    name self = (name) {\
        map->count,\
        ds__##name##_vector_copy(&map->buckets),\
    };\
    ds_memcpy(self.buckets.array, map->buckets.array, sizeof(ds__##name##_bucket) * map->buckets.capacity);\
    return self;\
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
ds_API static inline V *name##_find(name *self, K key) {\
    ds_assert(self != NULL);\
    ds_assert(self->buckets.array != NULL);\
    ds_size hash = (key_hasher);\
    ds_size capacity = self->buckets.capacity;\
    hash %= capacity;\
    ds_size remaining = self->count;\
    for (ds_size i = 0; remaining > 0 && i < capacity; ++i) {\
        ds__##name##_bucket *bucket = self->buckets.array + ((hash + i) % capacity);\
        if (bucket->state == ds_BUCKET_EMPTY) {\
            return NULL;\
        }\
        if (bucket->state == ds_BUCKET_SKIP) {\
            continue;\
        }\
        K x = key;\
        K y = bucket->key;\
        if ((x_y_equals)) {\
            return &bucket->value;\
        }\
        --remaining;\
    }\
    ds_assert(remaining == 0);\
    return NULL;\
}\
\
ds_API static inline const V *name##_find_const(const name *self, K key) {\
    ds_assert(self != NULL);\
    ds_assert(self->buckets.array != NULL);\
    ds_size hash = (key_hasher);\
    ds_size capacity = self->buckets.capacity;\
    hash %= capacity;\
    ds_size remaining = self->count;\
    for (ds_size i = 0; remaining > 0 && i < capacity; ++i) {\
        const ds__##name##_bucket *bucket = self->buckets.array + ((hash + i) % capacity);\
        if (bucket->state == ds_BUCKET_EMPTY) {\
            return NULL;\
        }\
        if (bucket->state == ds_BUCKET_SKIP) {\
            continue;\
        }\
        K x = key;\
        K y = bucket->key;\
        if ((x_y_equals)) {\
            return &bucket->value;\
        }\
        --remaining;\
    }\
    ds_assert(remaining == 0);\
    return NULL;\
}\
\
ds_API static inline ds_bool name##_contains(const name *self, K key) {\
    ds_assert(self != NULL);\
    return name##_find_const(self, key) != NULL;\
}\
\
ds_API static inline void name##_resize(name *self, ds_size capacity) {\
    ds_assert(self != NULL);\
    ds_assert(capacity >= self->count);\
    if (capacity == self->count) {\
        return;\
    }\
    ds_assert(self->buckets.array != NULL);\
    ds__##name##_bucket *array = (ds__##name##_bucket *) ds_calloc(capacity, sizeof(ds__##name##_bucket));\
    ds_assert(array != NULL);\
    ds_size remaining = self->count;\
    for (ds_size i = 0; remaining > 0 && i < self->buckets.capacity; ++i) {\
        ds__##name##_bucket *bucket = self->buckets.array + i;\
        if (bucket->state != ds_BUCKET_OCCUPIED) {\
            continue;\
        }\
        K key = bucket->key;\
        ds_size hash = (key_hasher);\
        hash %= capacity;\
        for (ds_size j = 0; j < capacity; ++j) {\
            ds__##name##_bucket *target = array + ((hash + j) % capacity);\
            if (target->state == ds_BUCKET_OCCUPIED) {\
                continue;\
            }\
            *target = *bucket;\
            break;\
        }\
        --remaining;\
    }\
    ds_assert(remaining == 0);\
    self->buckets.capacity = capacity;\
    ds_free(self->buckets.array);\
    self->buckets.array = array;\
}\
\
ds_API static inline ds_bool name##_insert(name *self, K key, V value) {\
    ds_assert(self != NULL);\
    ds_assert(self->buckets.array != NULL);\
    if (ds_MAP_LOAD_FACTOR_DEN * (self->count + 1) > ds_MAP_LOAD_FACTOR_NUM * self->buckets.capacity) {\
        ds_size new_capacity = ds_VECTOR_EXPANSION * self->buckets.capacity;\
        ds_assert(new_capacity > self->buckets.capacity);\
        name##_resize(self, new_capacity);\
    }\
    ds__##name##_bucket new_bucket = (ds__##name##_bucket) {\
        key,\
        value,\
        ds_BUCKET_OCCUPIED,\
    };\
    ds_size hash = (key_hasher);\
    while (ds_true) {\
        ds_size capacity = self->buckets.capacity;\
        hash %= capacity;\
        ds__##name##_bucket *target = NULL;\
        for (ds_size i = 0; i < capacity; ++i) {\
            ds__##name##_bucket *bucket = self->buckets.array + ((hash + i) % capacity);\
            if (bucket->state == ds_BUCKET_EMPTY) {\
                ++self->count;\
                if (target == NULL) {\
                    target = bucket;\
                }\
                *target = new_bucket;\
                return ds_false;\
            }\
            if (bucket->state == ds_BUCKET_SKIP) {\
                if (target == NULL) {\
                    target = bucket;\
                }\
                continue;\
            }\
            K x = key;\
            K y = bucket->key;\
            if ((x_y_equals)) {\
                value_deleter(&bucket->value);\
                bucket->value = value;\
                return ds_true;\
            }\
        }\
        ds_size new_capacity = ds_VECTOR_EXPANSION * self->buckets.capacity;\
        ds_assert(new_capacity > self->buckets.capacity);\
        name##_resize(self, new_capacity);\
    }\
}\
\
ds_API static inline ds_bool name##_erase(name *self, K key) {\
    ds_assert(self != NULL);\
    ds_assert(self->buckets.array != NULL);\
    ds_size hash = (key_hasher);\
    ds_size capacity = self->buckets.capacity;\
    hash %= capacity;\
    ds_size remaining = self->count;\
    for (ds_size i = 0; remaining > 0 && i < capacity; ++i) {\
        ds__##name##_bucket *bucket = self->buckets.array + ((hash + i) % capacity);\
        if (bucket->state == ds_BUCKET_EMPTY) {\
            return ds_false;\
        }\
        if (bucket->state == ds_BUCKET_SKIP) {\
            continue;\
        }\
        K x = key;\
        K y = bucket->key;\
        if ((x_y_equals)) {\
            --self->count;\
            value_deleter(&bucket->value);\
            bucket->state = ds_BUCKET_SKIP;\
            return ds_true;\
        }\
        --remaining;\
    }\
    ds_assert(remaining == 0);\
    return ds_false;\
}\
\
ds_API static inline void name##_clear(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->buckets.array != NULL);\
    for (ds_size i = 0; self->count > 0 && i < self->buckets.capacity; ++i) {\
        ds__##name##_bucket *bucket = self->buckets.array + i;\
        if (bucket->state != ds_BUCKET_OCCUPIED) {\
            continue;\
        }\
        --self->count;\
        value_deleter(&bucket->value);\
    }\
    ds_memset(self->buckets.array, 0, sizeof(ds__##name##_bucket) * self->buckets.capacity);\
}\
\
ds_API static inline void name##_foreach(const name *self, void(*action)(K, V)) {\
    ds_assert(self != NULL);\
    ds_assert(action != NULL);\
    ds_assert(self->buckets.array != NULL);\
    ds_size remaining = self->count;\
    for (ds_size i = 0; remaining > 0 && i < self->buckets.capacity; ++i) {\
        const ds__##name##_bucket *bucket = self->buckets.array + i;\
        if (bucket->state != ds_BUCKET_OCCUPIED) {\
            continue;\
        }\
        action(bucket->key, bucket->value);\
        --remaining;\
    }\
    ds_assert(remaining == 0);\
}\
\
ds_API static inline void name##_foreach_key(const name *self, void(*action)(K)) {\
    ds_assert(self != NULL);\
    ds_assert(action != NULL);\
    ds_assert(self->buckets.array != NULL);\
    ds_size remaining = self->count;\
    for (ds_size i = 0; remaining > 0 && i < self->buckets.capacity; ++i) {\
        const ds__##name##_bucket *bucket = self->buckets.array + i;\
        if (bucket->state != ds_BUCKET_OCCUPIED) {\
            continue;\
        }\
        action(bucket->key);\
        --remaining;\
    }\
    ds_assert(remaining == 0);\
}\
\
ds_API static inline void name##_foreach_value(const name *self, void(*action)(V)) {\
    ds_assert(self != NULL);\
    ds_assert(action != NULL);\
    ds_assert(self->buckets.array != NULL);\
    ds_size remaining = self->count;\
    for (ds_size i = 0; remaining > 0 && i < self->buckets.capacity; ++i) {\
        const ds__##name##_bucket *bucket = self->buckets.array + i;\
        if (bucket->state != ds_BUCKET_OCCUPIED) {\
            continue;\
        }\
        action(bucket->value);\
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

/** Declares a key-value hash map of the given types. */
#define ds_DECLARE_MAP(K, V, key_hasher, x_y_equals, value_deleter)\
        ds_DECLARE_MAP_NAMED(K##_##V##_map, K, V, key_hasher, x_y_equals, value_deleter)

#endif // DS_MAP_H
