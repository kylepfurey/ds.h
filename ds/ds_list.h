// .h
// Generic Doubly Linked List Data Structure
// by Kyle Furey

/**
 * ds_list.h
 *
 * This is a doubly linked list. Data is allocated non-contiguously with pointer links.
 * This is the only data structure to publicly expose its node type in this library.
 * This is intentional for O(1) ordered insertions and removals via a node pointer.
 *
 * This is a great data structure for stacks, queues, and deques.
 * Insertion and deletion is fast with direct node references.
 * Iteration is slower than contiguous containers due to pointer chasing and cache misses.
 *
 * typedef struct {
 *      T                   data;
 *      list_node* const    previous;
 *      list_node* const    next;
 * } list_node;
 *
 * list                 list_new                ( void )
 *
 * list                 list_copy               ( const list* self )
 *
 * size_t               list_count              ( const list* self )
 *
 * bool                 list_empty              ( const list* self )
 *
 * list_node*           list_head               ( list* self )
 *
 * const list_node*     list_head_const         ( const list* self )
 *
 * list_node*           list_tail               ( list* self )
 *
 * const list_node*     list_tail_const         ( const list* self )
 *
 * list_node*           list_get                ( list* self, size_t index )
 *
 * const list_node*     list_get_const          ( const list* self, size_t index )
 *
 * list_node*           list_insert_before      ( list* self, list_node* node, T data )
 *
 * list_node*           list_insert_after       ( list* self, list_node* node, T data )
 *
 * void                 list_erase              ( list* self, list_node* node )
 *
 * list_node*           list_push_front         ( list* self, T data )
 *
 * list_node*           list_push_back          ( list* self, T data )
 *
 * void                 list_pop_front          ( list* self )
 *
 * void                 list_pop_back           ( list* self )
 *
 * void                 list_clear              ( list* self )
 *
 * void                 list_foreach            ( const list* self, void (*action)(T) )
 *
 * void                 list_delete             ( list* self )
 */

#ifndef DS_LIST_H
#define DS_LIST_H

#include "ds_def.h"

/** Declares a named doubly linked list of the given type. */
#define DECLARE_LIST_NAMED(name, T, deleter)\
\
typedef struct name##_node {\
    T data;\
    struct name##_node *previous;\
    struct name##_node *next;\
} name##_node;\
\
typedef struct {\
    ds_size count;\
    struct name##_node *head;\
    struct name##_node *tail;\
} name;\
\
DS_API static inline name name##_new() {\
    return (name) {\
        0,\
        NULL,\
        NULL,\
    };\
}\
\
DS_API static inline ds_size name##_count(const name *self) {\
    ds_assert(self != NULL);\
    return self->count;\
}\
\
DS_API static inline bool name##_empty(const name *self) {\
    ds_assert(self != NULL);\
    return self->count == 0;\
}\
\
DS_API static inline name##_node *name##_head(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->count > 0);\
    ds_assert(self->head != NULL);\
    return self->head;\
}\
\
DS_API static inline const name##_node *name##_head_const(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->count > 0);\
    ds_assert(self->head != NULL);\
    return self->head;\
}\
\
DS_API static inline name##_node *name##_tail(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->count > 0);\
    ds_assert(self->tail != NULL);\
    return self->tail;\
}\
\
DS_API static inline const name##_node *name##_tail_const(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->count > 0);\
    ds_assert(self->tail != NULL);\
    return self->tail;\
}\
\
DS_API static inline name##_node *name##_get(name *self, ds_size index) {\
    ds_assert(self != NULL);\
    ds_assert(index < self->count);\
    name##_node *node;\
    if (index <= self->count / 2) {\
        node = self->head;\
        ds_assert(node != NULL);\
        for (ds_size i = 0; i != index; ++i) {\
            node = node->next;\
            ds_assert(node != NULL);\
        }\
    } else {\
        node = self->tail;\
        ds_assert(node != NULL);\
        for (ds_diff i = self->count - 1; i != index; --i) {\
            node = node->previous;\
            ds_assert(node != NULL);\
        }\
    }\
    return node;\
}\
\
DS_API static inline const name##_node *name##_get_const(const name *self, ds_size index) {\
    ds_assert(self != NULL);\
    ds_assert(index < self->count);\
    const name##_node *node;\
    if (index <= self->count / 2) {\
        node = self->head;\
        ds_assert(node != NULL);\
        for (ds_size i = 0; i != index; ++i) {\
            node = node->next;\
            ds_assert(node != NULL);\
        }\
    } else {\
        node = self->tail;\
        ds_assert(node != NULL);\
        for (ds_diff i = self->count - 1; i != index; --i) {\
            node = node->previous;\
            ds_assert(node != NULL);\
        }\
    }\
    return node;\
}\
\
DS_API static inline name##_node *name##_insert_before(name *self, name##_node *node, T data) {\
    ds_assert(self != NULL);\
    ds_assert(node != NULL);\
    ds_assert(self->count > 0);\
    ++self->count;\
    name##_node *new_node = (name##_node *) ds_malloc(sizeof(name##_node));\
    ds_assert(new_node != NULL);\
    new_node->data = data;\
    new_node->next = node;\
    new_node->previous = node->previous;\
    if (node->previous != NULL) {\
        node->previous->next = new_node;\
    }\
    node->previous = new_node;\
    if (self->head == node) {\
        self->head = new_node;\
    }\
    return new_node;\
}\
\
DS_API static inline name##_node *name##_insert_after(name *self, name##_node *node, T data) {\
    ds_assert(self != NULL);\
    ds_assert(node != NULL);\
    ds_assert(self->count > 0);\
    ++self->count;\
    name##_node *new_node = (name##_node *) ds_malloc(sizeof(name##_node));\
    ds_assert(new_node != NULL);\
    new_node->data = data;\
    new_node->previous = node;\
    new_node->next = node->next;\
    if (node->next != NULL) {\
        node->next->previous = new_node;\
    }\
    node->next = new_node;\
    if (self->tail == node) {\
        self->tail = new_node;\
    }\
    return new_node;\
}\
\
DS_API static inline void name##_erase(name *self, name##_node *node) {\
    ds_assert(self != NULL);\
    ds_assert(node != NULL);\
    ds_assert(self->count > 0);\
    --self->count;\
    if (self->head == node) {\
        self->head = node->next;\
    }\
    if (self->tail == node) {\
        self->tail = node->previous;\
    }\
    if (node->previous != NULL) {\
        node->previous->next = node->next;\
    }\
    if (node->next != NULL) {\
        node->next->previous = node->previous;\
    }\
    deleter(&node->data);\
    ds_free(node);\
}\
\
DS_API static inline name##_node *name##_push_front(name *self, T data) {\
    ds_assert(self != NULL);\
    ++self->count;\
    name##_node *node = (name##_node *) ds_malloc(sizeof(name##_node));\
    ds_assert(node != NULL);\
    node->data = data;\
    node->previous = NULL;\
    if (self->head == NULL) {\
        ds_assert(self->tail == NULL);\
        node->next = NULL;\
        self->tail = node;\
    } else {\
        ds_assert(self->tail != NULL);\
        node->next = self->head;\
        self->head->previous = node;\
    }\
    self->head = node;\
    return node;\
}\
\
DS_API static inline name##_node *name##_push_back(name *self, T data) {\
    ds_assert(self != NULL);\
    ++self->count;\
    name##_node *node = (name##_node *) ds_malloc(sizeof(name##_node));\
    ds_assert(node != NULL);\
    node->data = data;\
    node->next = NULL;\
    if (self->tail == NULL) {\
        ds_assert(self->head == NULL);\
        node->previous = NULL;\
        self->head = node;\
    } else {\
        ds_assert(self->head != NULL);\
        node->previous = self->tail;\
        self->tail->next = node;\
    }\
    self->tail = node;\
    return node;\
}\
\
DS_API static inline name name##_copy(const name *list) {\
    ds_assert(list != NULL);\
    name self = (name) {\
        0,\
        NULL,\
        NULL,\
    };\
    const name##_node *current = list->head;\
    while (current != NULL) {\
        name##_push_back(&self, current->data);\
        current = current->next;\
    }\
    ds_assert(self.count == list->count);\
    return self;\
}\
\
DS_API static inline void name##_pop_front(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->count > 0);\
    --self->count;\
    name##_node *node = self->head;\
    ds_assert(node != NULL);\
    self->head = node->next;\
    if (self->head == NULL) {\
        self->tail = NULL;\
        ds_assert(self->count == 0);\
    }\
    deleter(&node->data);\
    ds_free(node);\
    if (self->head != NULL) {\
        self->head->previous = NULL;\
    }\
}\
\
DS_API static inline void name##_pop_back(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->count > 0);\
    --self->count;\
    name##_node *node = self->tail;\
    ds_assert(node != NULL);\
    self->tail = node->previous;\
    if (self->tail == NULL) {\
        self->head = NULL;\
        ds_assert(self->count == 0);\
    }\
    deleter(&node->data);\
    ds_free(node);\
    if (self->tail != NULL) {\
        self->tail->next = NULL;\
    }\
}\
\
DS_API static inline void name##_clear(name *self) {\
    ds_assert(self != NULL);\
    name##_node *current = self->head;\
    while (current != NULL) {\
        name##_node *next = current->next;\
        deleter(&current->data);\
        ds_free(current);\
        current = next;\
    }\
    self->count = 0;\
    self->head = NULL;\
    self->tail = NULL;\
}\
\
DS_API static inline void name##_foreach(const name *self, void(*action)(T)) {\
    ds_assert(self != NULL);\
    ds_assert(action != NULL);\
    const name##_node *current = self->head;\
    while (current != NULL) {\
        action(current->data);\
        current = current->next;\
    }\
}\
\
DS_API static inline void name##_delete(name *self) {\
    ds_assert(self != NULL);\
    name##_clear(self);\
    *self = (name) {0};\
}

/** Declares a doubly linked list of the given type. */
#define DECLARE_LIST(T, deleter) DECLARE_LIST_NAMED(T##_list, T, deleter)

#endif // DS_LIST_H
