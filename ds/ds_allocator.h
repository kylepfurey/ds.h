// .h
// Generic Block Allocator Data Structure
// by Kyle Furey

/**
 *
 */

#ifndef DS_ALLOCATOR_H
#define DS_ALLOCATOR_H

#include "ds_def.h"

/** Declares a named block allocator with the given alignment. */
#define DECLARE_ALLOCATOR_NAMED(name, alignment)\
\
typedef struct ds__##name##_block {\
    ds_size size;\
    struct ds__##name##_block *next;\
} ds__##name##_block;\
\
enum {\
    ds__##name##_block_SIZE = ALLOCATOR_ALIGN(sizeof(ds__##name##_block), alignment),\
};\
\
typedef struct {\
    void *start;\
    void *end;\
    ds__##name##_block *free;\
} name;\
\
static inline name name##_new(ds_size size) {\
    ds_assert(size >= ds__##name##_block_SIZE + (alignment));\
    size = ALLOCATOR_ALIGN(size, (alignment));\
    void *start = ds_malloc(size);\
    ds_assert(start != NULL);\
    name self = (name) {\
        start,\
        (ds_byte *) start + size,\
        (ds__##name##_block *) start,\
    };\
    self.free->size = size;\
    self.free->next = NULL;\
    return self;\
}\
\
static inline void *name##_malloc(name *self, ds_size size) {\
    ds_assert(self != NULL);\
    if (size == 0) {\
        return NULL;\
    }\
    size = ALLOCATOR_ALIGN(size, (alignment));\
    ds__##name##_block **previous = &self->free;\
    ds__##name##_block *current = self->free;\
    while (current != NULL) {\
        if (current->size >= ds__##name##_block_SIZE + size) {\
            ds__##name##_block *block = (ds__##name##_block *)\
                ((ds_byte *) current + size + ds__##name##_block_SIZE);\
            block->size = current->size - size - ds__##name##_block_SIZE;\
            block->next = current->next;\
            *previous = block;\
            current->size = size;\
            current->next = NULL;\
            return (ds_byte *) current + ds__##name##_block_SIZE;\
        }\
        previous = &current->next;\
        current = current->next;\
    }\
    return NULL;\
}\
\
static inline void name##_free(name *self, void *ptr) {\
    ds_assert(self != NULL);\
    ds_assert(ptr == NULL || (ptr >= self->start && ptr < self->end));\
    if (ptr == NULL) {\
        return;\
    }\
    ds__##name##_block* block = (ds__##name##_block *)\
        ((ds_byte *) ptr - ds__##name##_block_SIZE);\
    ds__##name##_block *previous = NULL;\
    ds__##name##_block *current = self->free;\
    while (current != NULL && current < block) {\
        previous = current;\
        current = current->next;\
    }\
    block->next = current;\
    if (previous != NULL) {\
        previous->next = block;\
    } else {\
        self->free = block;\
    }\
    if (current != NULL && (ds_byte *) block + block->size + ds__##name##_block_SIZE == (ds_byte *) current) {\
        block->size += ds__##name##_block_SIZE + current->size;\
        block->next = current->next;\
    }\
    if (previous != NULL && (ds_byte *) previous + previous->size + ds__##name##_block_SIZE == (ds_byte *) block) {\
        previous->size += block->size + ds__##name##_block_SIZE;\
        previous->next = block->next;\
    }\
}\
\
static inline void *name##_calloc(name *self, ds_size count, ds_size size) {\
    ds_assert(self != NULL);\
    ds_assert(size == 0 || count <= SIZE_MAX / size);\
    ds_size total = count * size;\
    void *ptr = name##_malloc(self, total);\
    if (ptr != NULL) {\
        memset(ptr, 0, total);\
    }\
    return ptr;\
}\
\
static inline void *name##_realloc(name *self, void *ptr, ds_size size) {\
    ds_assert(self != NULL);\
    ds_assert(ptr == NULL || (ptr >= self->start && ptr < self->end));\
    if (ptr == NULL) {\
        return name##_malloc(self, size);\
    }\
    if (size == 0) {\
        name##_free(self, ptr);\
        return NULL; \
    }\
    ds__##name##_block* block = (ds__##name##_block *)\
        ((ds_byte *) ptr - ds__##name##_block_SIZE);\
    if (block->size >= size) {\
        return ptr;\
    }\
    void *new_ptr = name##_malloc(self, size);\
    if (new_ptr == NULL) {\
        return NULL;\
    }\
    memcpy(new_ptr, ptr, block->size);\
    name##_free(self, ptr);\
    return new_ptr;\
}\
\
static inline void name##_delete(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->start != NULL);\
    if (ALLOCATOR_LEAK_ASSERT) {\
        ds_assert(\
            self->free != NULL &&\
            self->free->next == NULL &&\
            (void *) self->free == self->start &&\
            self->free->size == ((ds_byte *) self->end - (ds_byte *) self->start)\
        );\
    }\
    ds_free(self->start);\
    *self = (name) {0};\
}

/** Declares the default block allocator type. */
DECLARE_ALLOCATOR_NAMED(allocator, sizeof(ds_size))

#endif // DS_ALLOCATOR_H
