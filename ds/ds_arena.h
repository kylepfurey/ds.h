// .h
// Generic Block Allocator Data Structure
// by Kyle Furey

/**
 * ds_arena.h
 *
 * ds_DECLARE_ARENA_NAMED(
 *      name,               - The name of the data structure and function prefix.
 *
 *      alignment,          - The alignment of allocated arena memory.
 * )
 *
 * The "arena" struct is automatically generated with the default word size for alignment.
 *
 * This is a simple block allocator. It allocates a buffer and is divided per allocation.
 * This allocator is type-agnostic and operates with raw memory to create new objects.
 * Allocations are made at runtime using arena_malloc().
 *
 * This is a fantastic alternative to malloc() if you want deterministic heap memory.
 *
 * * Returns a new arena with at least <size> bytes of aligned memory.
 * * This data structure must be deleted with arena_delete().
 *
 *   arena    arena_new       ( size_t size )
 *
 * * Allocates at least <size> number of bytes.
 * * Returns a new pointer or NULL.
 *
 *   void*    arena_malloc    ( arena* self, size_t size )
 *
 * * Allocates a <count> * <size> array of zeroed memory.
 * * Returns a new pointer or NULL.
 *
 *   void*    arena_calloc    ( arena* self, size_t count, size_t size )
 *
 * * Reallocates <ptr> to have at least <size> bytes.
 * * Returns a new pointer or NULL.
 *
 *   void*    arena_realloc   ( arena* self, void *ptr, size_t size )
 *
 * * Frees <ptr> from the arena so it may be reused again.
 * * <ptr> can be NULL.
 *
 *   void     arena_free      ( arena* self, void* ptr )
 *
 * * Safely deletes an arena.
 * * If ds_ARENA_LEAK_ASSERT is true, this will assert if the arena leaked memory.
 *
 *   void     arena_delete    ( arena* self )
 */

#ifndef DS_ARENA_H
#define DS_ARENA_H

#include "ds_def.h"

/** Declares a named block allocator with the given alignment. */
#define ds_DECLARE_ARENA_NAMED(name, alignment)\
\
typedef struct ds__##name##_block {\
    ds_size size;\
    struct ds__##name##_block *next;\
} ds__##name##_block;\
\
enum {\
    ds__##name##_block_SIZE = ds_ARENA_ALIGN(sizeof(ds__##name##_block), alignment),\
};\
\
typedef struct {\
    void *start;\
    void *end;\
    ds__##name##_block *free;\
} name;\
\
ds_API static inline name name##_new(ds_size size) {\
    ds_assert(size >= ds__##name##_block_SIZE + (alignment));\
    size = ds_ARENA_ALIGN(size, (alignment));\
    size += ds__##name##_block_SIZE;\
    void *start = ds_malloc(size);\
    ds_assert(start != ds_NULL);\
    name self = (name) {\
        start,\
        (ds_byte *) start + size,\
        (ds__##name##_block *) start,\
    };\
    self.free->size = size;\
    self.free->next = ds_NULL;\
    return self;\
}\
\
ds_API static inline void *name##_malloc(name *self, ds_size size) {\
    ds_assert(self != ds_NULL);\
    if (size == 0) {\
        return ds_NULL;\
    }\
    size = ds_ARENA_ALIGN(size, (alignment));\
    ds__##name##_block **previous = &self->free;\
    ds__##name##_block *current = self->free;\
    while (current != ds_NULL) {\
        if (current->size >= ds__##name##_block_SIZE + size) {\
            ds__##name##_block *block = (ds__##name##_block *)\
                ((ds_byte *) current + size + ds__##name##_block_SIZE);\
            block->size = current->size - size - ds__##name##_block_SIZE;\
            block->next = current->next;\
            *previous = block;\
            current->size = size;\
            current->next = ds_NULL;\
            return (ds_byte *) current + ds__##name##_block_SIZE;\
        }\
        previous = &current->next;\
        current = current->next;\
    }\
    return ds_NULL;\
}\
\
ds_API static inline void name##_free(name *self, void *ptr) {\
    ds_assert(self != ds_NULL);\
    ds_assert(ptr == ds_NULL || (ptr >= self->start && ptr < self->end));\
    if (ptr == ds_NULL) {\
        return;\
    }\
    ds__##name##_block* block = (ds__##name##_block *)\
        ((ds_byte *) ptr - ds__##name##_block_SIZE);\
    ds__##name##_block *previous = ds_NULL;\
    ds__##name##_block *current = self->free;\
    while (current != ds_NULL && current < block) {\
        previous = current;\
        current = current->next;\
    }\
    block->next = current;\
    if (previous != ds_NULL) {\
        previous->next = block;\
    } else {\
        self->free = block;\
    }\
    if (current != ds_NULL && (ds_byte *) block + block->size + ds__##name##_block_SIZE == (ds_byte *) current) {\
        block->size += ds__##name##_block_SIZE + current->size;\
        block->next = current->next;\
    }\
    if (previous != ds_NULL && (ds_byte *) previous + previous->size + ds__##name##_block_SIZE == (ds_byte *) block) {\
        previous->size += block->size + ds__##name##_block_SIZE;\
        previous->next = block->next;\
    }\
}\
\
ds_API static inline void *name##_calloc(name *self, ds_size count, ds_size size) {\
    ds_assert(self != ds_NULL);\
    ds_assert(size == 0 || count <= ds_SIZE_MAX / size);\
    ds_size total = count * size;\
    void *ptr = name##_malloc(self, total);\
    if (ptr != ds_NULL) {\
        ds_memset(ptr, 0, total);\
    }\
    return ptr;\
}\
\
ds_API static inline void *name##_realloc(name *self, void *ptr, ds_size size) {\
    ds_assert(self != ds_NULL);\
    ds_assert(ptr == ds_NULL || (ptr >= self->start && ptr < self->end));\
    if (ptr == ds_NULL) {\
        return name##_malloc(self, size);\
    }\
    if (size == 0) {\
        name##_free(self, ptr);\
        return ds_NULL; \
    }\
    ds__##name##_block* block = (ds__##name##_block *)\
        ((ds_byte *) ptr - ds__##name##_block_SIZE);\
    if (block->size >= size) {\
        return ptr;\
    }\
    void *new_ptr = name##_malloc(self, size);\
    if (new_ptr == ds_NULL) {\
        return ds_NULL;\
    }\
    ds_memcpy(new_ptr, ptr, block->size);\
    name##_free(self, ptr);\
    return new_ptr;\
}\
\
ds_API static inline void name##_delete(name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->start != ds_NULL);\
    if (ds_ARENA_LEAK_ASSERT) {\
        ds_assert(\
            self->free != ds_NULL &&\
            self->free->next == ds_NULL &&\
            (void *) self->free == self->start &&\
            self->free->size == ((ds_byte *) self->end - (ds_byte *) self->start)\
        );\
    }\
    ds_free(self->start);\
    *self = (name) {0};\
}

/** Declares a block allocator with the alignment of the given type. */
#define ds_DECLARE_ARENA(T_aligned)\
        ds_DECLARE_ARENA_NAMED(T_aligned##_arena, sizeof(T_aligned))

/** Declares the default OS-aligned block allocator type. */
ds_DECLARE_ARENA_NAMED(arena, sizeof(void *))

#endif // DS_ARENA_H
