// .h
// Generic Key-Value Hash Map Data Structure
// by Kyle Furey

/**
 *
 */

#ifndef DS_MAP_H
#define DS_MAP_H

#include "ds_vector.h"

/** Declares a named key-value hash map of the given types. */
#define DECLARE_MAP_NAMED(name, K, V, key_hasher, x_y_equals, value_deleter)\
\
typedef struct {\
    K key;\
    V value;\
    ds_uint state;\
} ds__##name##_bucket;\
\
DECLARE_VECTOR_NAMED(ds__##name##_vector, ds__##name##_bucket, void_deleter)\
\
typedef struct {\
    ds_size count;\
    ds__##name##_vector buckets;\
} name;\
\
static inline name name##_new(ds_size capacity) {\
    ds_assert(capacity > 0);\
    name self = (name) {\
        0,\
        ds__##name##_vector_new(capacity),\
    };\
    memset(self.buckets.array, 0, sizeof(ds__##name##_bucket) * capacity);\
    return self;\
}\
\
static inline name name##_copy(const name *map) {\
    ds_assert(map != NULL);\
    name self = (name) {\
        map->count,\
        ds__##name##_vector_copy(&map->buckets),\
    };\
    memcpy(self.buckets.array, map->buckets.array, sizeof(ds__##name##_bucket) * map->buckets.capacity);\
    return self;\
}\
\
static inline ds_size name##_count(const name *self) {\
    ds_assert(self != NULL);\
    return self->count;\
}\
\
static inline ds_size name##_capacity(const name *self) {\
    ds_assert(self != NULL);\
    return self->buckets.capacity;\
}\
\
static inline bool name##_empty(const name *self) {\
    ds_assert(self != NULL);\
    return self->count == 0;\
}\
\
static inline V *name##_find(name *self, K key) {\
    ds_assert(self != NULL);\
    ds_assert(self->buckets.array != NULL);\
    ds_size hash = (key_hasher);\
    ds_size capacity = self->buckets.capacity;\
    hash %= capacity;\
    ds_size remaining = self->count;\
    for (ds_size i = 0; remaining > 0 && i < capacity; ++i) {\
        ds__##name##_bucket *bucket = self->buckets.array + ((hash + i) % capacity);\
        if (bucket->state == BUCKET_EMPTY) {\
            return NULL;\
        }\
        if (bucket->state == BUCKET_SKIP) {\
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
static inline const V *name##_find_const(const name *self, K key) {\
    ds_assert(self != NULL);\
    ds_assert(self->buckets.array != NULL);\
    ds_size hash = (key_hasher);\
    ds_size capacity = self->buckets.capacity;\
    hash %= capacity;\
    ds_size remaining = self->count;\
    for (ds_size i = 0; remaining > 0 && i < capacity; ++i) {\
        const ds__##name##_bucket *bucket = self->buckets.array + ((hash + i) % capacity);\
        if (bucket->state == BUCKET_EMPTY) {\
            return NULL;\
        }\
        if (bucket->state == BUCKET_SKIP) {\
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
static inline bool name##_contains(const name *self, K key) {\
    ds_assert(self != NULL);\
    return name##_find_const(self, key) != NULL;\
}\
\
static inline void name##_resize(name *self, ds_size capacity) {\
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
        if (bucket->state != BUCKET_OCCUPIED) {\
            continue;\
        }\
        K key = bucket->key;\
        ds_size hash = (key_hasher);\
        hash %= capacity;\
        for (ds_size j = 0; j < capacity; ++j) {\
            ds__##name##_bucket *target = array + ((hash + j) % capacity);\
            if (target->state == BUCKET_OCCUPIED) {\
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
static inline bool name##_insert(name *self, K key, V value) {\
    ds_assert(self != NULL);\
    ds_assert(self->buckets.array != NULL);\
    if (MAP_LOAD_FACTOR_DEN * (self->count + 1) > MAP_LOAD_FACTOR_NUM * self->buckets.capacity) {\
        ds_size new_capacity = VECTOR_EXPANSION * self->buckets.capacity;\
        ds_assert(new_capacity > self->buckets.capacity);\
        name##_resize(self, new_capacity);\
    }\
    ds__##name##_bucket new_bucket = (ds__##name##_bucket) {\
        key,\
        value,\
        BUCKET_OCCUPIED,\
    };\
    ds_size hash = (key_hasher);\
    while (true) {\
        ds_size capacity = self->buckets.capacity;\
        hash %= capacity;\
        ds__##name##_bucket *target = NULL;\
        for (ds_size i = 0; i < capacity; ++i) {\
            ds__##name##_bucket *bucket = self->buckets.array + ((hash + i) % capacity);\
            if (bucket->state == BUCKET_EMPTY) {\
                ++self->count;\
                if (target == NULL) {\
                    target = bucket;\
                }\
                *target = new_bucket;\
                return false;\
            }\
            if (bucket->state == BUCKET_SKIP) {\
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
                return true;\
            }\
        }\
        ds_size new_capacity = VECTOR_EXPANSION * self->buckets.capacity;\
        ds_assert(new_capacity > self->buckets.capacity);\
        name##_resize(self, new_capacity);\
    }\
}\
\
static inline bool name##_erase(name *self, K key) {\
    ds_assert(self != NULL);\
    ds_assert(self->buckets.array != NULL);\
    ds_size hash = (key_hasher);\
    ds_size capacity = self->buckets.capacity;\
    hash %= capacity;\
    ds_size remaining = self->count;\
    for (ds_size i = 0; remaining > 0 && i < capacity; ++i) {\
        ds__##name##_bucket *bucket = self->buckets.array + ((hash + i) % capacity);\
        if (bucket->state == BUCKET_EMPTY) {\
            return false;\
        }\
        if (bucket->state == BUCKET_SKIP) {\
            continue;\
        }\
        K x = key;\
        K y = bucket->key;\
        if ((x_y_equals)) {\
            --self->count;\
            value_deleter(&bucket->value);\
            bucket->state = BUCKET_SKIP;\
            return true;\
        }\
        --remaining;\
    }\
    ds_assert(remaining == 0);\
    return false;\
}\
\
static inline void name##_clear(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->buckets.array != NULL);\
    for (ds_size i = 0; self->count > 0 && i < self->buckets.capacity; ++i) {\
        ds__##name##_bucket *bucket = self->buckets.array + i;\
        if (bucket->state != BUCKET_OCCUPIED) {\
            continue;\
        }\
        --self->count;\
        value_deleter(&bucket->value);\
    }\
    memset(self->buckets.array, 0, sizeof(ds__##name##_bucket) * self->buckets.capacity);\
}\
\
static inline void name##_foreach(const name *self, void(*action)(K, V)) {\
    ds_assert(self != NULL);\
    ds_assert(action != NULL);\
    ds_assert(self->buckets.array != NULL);\
    ds_size remaining = self->count;\
    for (ds_size i = 0; remaining > 0 && i < self->buckets.capacity; ++i) {\
        const ds__##name##_bucket *bucket = self->buckets.array + i;\
        if (bucket->state != BUCKET_OCCUPIED) {\
            continue;\
        }\
        action(bucket->key, bucket->value);\
        --remaining;\
    }\
    ds_assert(remaining == 0);\
}\
\
static inline void name##_foreach_key(const name *self, void(*action)(K)) {\
    ds_assert(self != NULL);\
    ds_assert(action != NULL);\
    ds_assert(self->buckets.array != NULL);\
    ds_size remaining = self->count;\
    for (ds_size i = 0; remaining > 0 && i < self->buckets.capacity; ++i) {\
        const ds__##name##_bucket *bucket = self->buckets.array + i;\
        if (bucket->state != BUCKET_OCCUPIED) {\
            continue;\
        }\
        action(bucket->key);\
        --remaining;\
    }\
    ds_assert(remaining == 0);\
}\
\
static inline void name##_foreach_value(const name *self, void(*action)(V)) {\
    ds_assert(self != NULL);\
    ds_assert(action != NULL);\
    ds_assert(self->buckets.array != NULL);\
    ds_size remaining = self->count;\
    for (ds_size i = 0; remaining > 0 && i < self->buckets.capacity; ++i) {\
        const ds__##name##_bucket *bucket = self->buckets.array + i;\
        if (bucket->state != BUCKET_OCCUPIED) {\
            continue;\
        }\
        action(bucket->value);\
        --remaining;\
    }\
    ds_assert(remaining == 0);\
}\
\
static inline void name##_delete(name *self) {\
    ds_assert(self != NULL);\
    name##_clear(self);\
    ds__##name##_vector_delete(&self->buckets);\
    *self = (name) {0};\
}

/** Declares a key-value hash map of the given types. */
#define DECLARE_MAP(K, V, key_hasher, x_y_equals, value_deleter) DECLARE_MAP_NAMED(K##_##V##_map, K, V, key_hasher, x_y_equals, value_deleter)

#endif // DS_MAP_H
