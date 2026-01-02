// .h
// ds.h Doubly Linked List Data Structure
// by Kyle Furey

/**
 * ds_list.h
 *
 * ds_DECLARE_LIST_NAMED(
 *      name,           - The name of the data structure and function prefix.
 *
 *      T,              - The type to generate this data structure with.
 *
 *      deleter,        - The name of the function used to deallocate T.
 *                        ds_void_deleter may be used for trivial types.
 * )
 *
 * This is a doubly linked list. Data is allocated non-contiguously with pointer links.
 * This is the only data structure to publicly expose its node type in this library.
 * This is intentional for O(1) ordered insertions and removals via a node pointer.
 *
 * This is a great data structure for stacks, queues, and deques.
 * Insertion and deletion is fast with direct node references.
 * Iteration is slower than contiguous containers due to pointer chasing and cache misses.
 *
 * * This is the underlying list_node type. You must not modify its pointers.
 *
 *   typedef struct list_node {
 *        T               data;
 *        list_node*      previous;
 *        list_node*      next;
 *   } list_node;
 *
 * * Returns a new list.
 * * This data structure must be deleted with list_delete().
 *
 *   list                 list_new                ( void )
 *
 * * Returns a new list copied from <list>.
 * * The new list owns its own memory and must be deleted with list_delete().
 *
 *   list                 list_copy               ( const list* list )
 *
 * * Returns the number of nodes in the list.
 *
 *   size_t               list_count              ( const list* self )
 *
 * * Returns whether the list is empty.
 *
 *   bool                 list_empty              ( const list* self )
 *
 * * Returns the node at the front of the list.
 * * The list must not be empty.
 *
 *   list_node*           list_front              ( list* self )
 *
 * * Returns the node at the front of the list.
 * * The list must not be empty.
 *
 *   const list_node*     list_front_const        ( const list* self )
 *
 * * Returns the node at the end of the list.
 * * The list must not be empty.
 *
 *   list_node*           list_back               ( list* self )
 *
 * * Returns the node at the end of the list.
 * * The list must not be empty.
 *
 *   const list_node*     list_back_const         ( const list* self )
 *
 * * Traverses the linked list to find and return a node.
 * * <index> must be valid.
 *
 *   list_node*           list_get                ( list* self, size_t index )
 *
 * * Traverses the linked list to find and return a node.
 * * <index> must be valid.
 *
 *   const list_node*     list_get_const          ( const list* self, size_t index )
 *
 * * Prepends and returns a new node containing <data> before <node>.
 * * <node> must be owned by list.
 *
 *   list_node*           list_insert_before      ( list* self, list_node* node, T data )
 *
 * * Appends and returns a new node containing <data> after <node>.
 * * <node> must be owned by list.
 *
 *   list_node*           list_insert_after       ( list* self, list_node* node, T data )
 *
 * * Deletes a <node> from the list.
 * * <node> must be owned by list.
 *
 *   void                 list_erase              ( list* self, list_node* node )
 *
 * * Prepends and returns a new node containing <data> to the front of the list.
 *
 *   list_node*           list_push_front         ( list* self, T data )
 *
 * * Appends and returns a new node containing <data> at the end of the list.
 *
 *   list_node*           list_push_back          ( list* self, T data )
 *
 * * Deletes the node at the front of the list.
 *
 *   void                 list_pop_front          ( list* self )
 *
 * * Deletes the node at the end of the list.
 *
 *   void                 list_pop_back           ( list* self )
 *
 * * Deletes all nodes in the list.
 *
 *   void                 list_clear              ( list* self )
 *
 * * Iterates the list calling <action> on each element.
 *
 *   void                 list_foreach            ( const list* self, void (*action)(T) )
 *
 * * Safely deletes a list.
 *
 *   void                 list_delete             ( list* self )
 */

#ifndef DS_LIST_H
#define DS_LIST_H

#include "ds_def.h"

/** Declares a named doubly linked list of the given type. */
#define ds_DECLARE_LIST_NAMED(name, T, deleter)\
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
ds_API static inline name name##_new(void) {\
    return (name) {\
        0,\
        ds_NULL,\
        ds_NULL,\
    };\
}\
\
ds_API static inline ds_size name##_count(const name *self) {\
    ds_assert(self != ds_NULL);\
    return self->count;\
}\
\
ds_API static inline ds_bool name##_empty(const name *self) {\
    ds_assert(self != ds_NULL);\
    return self->count == 0;\
}\
\
ds_API static inline name##_node *name##_front(name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count > 0);\
    ds_assert(self->head != ds_NULL);\
    return self->head;\
}\
\
ds_API static inline const name##_node *name##_front_const(const name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count > 0);\
    ds_assert(self->head != ds_NULL);\
    return self->head;\
}\
\
ds_API static inline name##_node *name##_back(name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count > 0);\
    ds_assert(self->tail != ds_NULL);\
    return self->tail;\
}\
\
ds_API static inline const name##_node *name##_back_const(const name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count > 0);\
    ds_assert(self->tail != ds_NULL);\
    return self->tail;\
}\
\
ds_API static inline name##_node *name##_get(name *self, ds_size index) {\
    ds_assert(self != ds_NULL);\
    ds_assert(index < self->count);\
    name##_node *node;\
    if (index <= self->count / 2) {\
        node = self->head;\
        ds_assert(node != ds_NULL);\
        for (ds_size i = 0; i != index; ++i) {\
            node = node->next;\
            ds_assert(node != ds_NULL);\
        }\
    } else {\
        node = self->tail;\
        ds_assert(node != ds_NULL);\
        for (ds_diff i = self->count - 1; i != index; --i) {\
            node = node->previous;\
            ds_assert(node != ds_NULL);\
        }\
    }\
    return node;\
}\
\
ds_API static inline const name##_node *name##_get_const(const name *self, ds_size index) {\
    ds_assert(self != ds_NULL);\
    ds_assert(index < self->count);\
    const name##_node *node;\
    if (index <= self->count / 2) {\
        node = self->head;\
        ds_assert(node != ds_NULL);\
        for (ds_size i = 0; i != index; ++i) {\
            node = node->next;\
            ds_assert(node != ds_NULL);\
        }\
    } else {\
        node = self->tail;\
        ds_assert(node != ds_NULL);\
        for (ds_diff i = self->count - 1; i != index; --i) {\
            node = node->previous;\
            ds_assert(node != ds_NULL);\
        }\
    }\
    return node;\
}\
\
ds_API static inline name##_node *name##_insert_before(name *self, name##_node *node, T data) {\
    ds_assert(self != ds_NULL);\
    ds_assert(node != ds_NULL);\
    ds_assert(self->count > 0);\
    ++self->count;\
    name##_node *new_node = (name##_node *) ds_malloc(sizeof(name##_node));\
    ds_assert(new_node != ds_NULL);\
    new_node->data = data;\
    new_node->next = node;\
    new_node->previous = node->previous;\
    if (node->previous != ds_NULL) {\
        node->previous->next = new_node;\
    }\
    node->previous = new_node;\
    if (self->head == node) {\
        self->head = new_node;\
    }\
    return new_node;\
}\
\
ds_API static inline name##_node *name##_insert_after(name *self, name##_node *node, T data) {\
    ds_assert(self != ds_NULL);\
    ds_assert(node != ds_NULL);\
    ds_assert(self->count > 0);\
    ++self->count;\
    name##_node *new_node = (name##_node *) ds_malloc(sizeof(name##_node));\
    ds_assert(new_node != ds_NULL);\
    new_node->data = data;\
    new_node->previous = node;\
    new_node->next = node->next;\
    if (node->next != ds_NULL) {\
        node->next->previous = new_node;\
    }\
    node->next = new_node;\
    if (self->tail == node) {\
        self->tail = new_node;\
    }\
    return new_node;\
}\
\
ds_API static inline void name##_erase(name *self, name##_node *node) {\
    ds_assert(self != ds_NULL);\
    ds_assert(node != ds_NULL);\
    ds_assert(self->count > 0);\
    --self->count;\
    if (self->head == node) {\
        self->head = node->next;\
    }\
    if (self->tail == node) {\
        self->tail = node->previous;\
    }\
    if (node->previous != ds_NULL) {\
        node->previous->next = node->next;\
    }\
    if (node->next != ds_NULL) {\
        node->next->previous = node->previous;\
    }\
    deleter(&node->data);\
    ds_free(node);\
}\
\
ds_API static inline name##_node *name##_push_front(name *self, T data) {\
    ds_assert(self != ds_NULL);\
    ++self->count;\
    name##_node *node = (name##_node *) ds_malloc(sizeof(name##_node));\
    ds_assert(node != ds_NULL);\
    node->data = data;\
    node->previous = ds_NULL;\
    if (self->head == ds_NULL) {\
        ds_assert(self->tail == ds_NULL);\
        node->next = ds_NULL;\
        self->tail = node;\
    } else {\
        ds_assert(self->tail != ds_NULL);\
        node->next = self->head;\
        self->head->previous = node;\
    }\
    self->head = node;\
    return node;\
}\
\
ds_API static inline name##_node *name##_push_back(name *self, T data) {\
    ds_assert(self != ds_NULL);\
    ++self->count;\
    name##_node *node = (name##_node *) ds_malloc(sizeof(name##_node));\
    ds_assert(node != ds_NULL);\
    node->data = data;\
    node->next = ds_NULL;\
    if (self->tail == ds_NULL) {\
        ds_assert(self->head == ds_NULL);\
        node->previous = ds_NULL;\
        self->head = node;\
    } else {\
        ds_assert(self->head != ds_NULL);\
        node->previous = self->tail;\
        self->tail->next = node;\
    }\
    self->tail = node;\
    return node;\
}\
\
ds_API static inline name name##_copy(const name *list) {\
    ds_assert(list != ds_NULL);\
    name self = (name) {\
        0,\
        ds_NULL,\
        ds_NULL,\
    };\
    const name##_node *current = list->head;\
    while (current != ds_NULL) {\
        name##_push_back(&self, current->data);\
        current = current->next;\
    }\
    ds_assert(self.count == list->count);\
    return self;\
}\
\
ds_API static inline void name##_pop_front(name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count > 0);\
    --self->count;\
    name##_node *node = self->head;\
    ds_assert(node != ds_NULL);\
    self->head = node->next;\
    if (self->head == ds_NULL) {\
        self->tail = ds_NULL;\
        ds_assert(self->count == 0);\
    }\
    deleter(&node->data);\
    ds_free(node);\
    if (self->head != ds_NULL) {\
        self->head->previous = ds_NULL;\
    }\
}\
\
ds_API static inline void name##_pop_back(name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->count > 0);\
    --self->count;\
    name##_node *node = self->tail;\
    ds_assert(node != ds_NULL);\
    self->tail = node->previous;\
    if (self->tail == ds_NULL) {\
        self->head = ds_NULL;\
        ds_assert(self->count == 0);\
    }\
    deleter(&node->data);\
    ds_free(node);\
    if (self->tail != ds_NULL) {\
        self->tail->next = ds_NULL;\
    }\
}\
\
ds_API static inline void name##_clear(name *self) {\
    ds_assert(self != ds_NULL);\
    name##_node *current = self->head;\
    while (current != ds_NULL) {\
        name##_node *next = current->next;\
        deleter(&current->data);\
        ds_free(current);\
        current = next;\
    }\
    self->count = 0;\
    self->head = ds_NULL;\
    self->tail = ds_NULL;\
}\
\
ds_API static inline void name##_foreach(const name *self, void(*action)(T)) {\
    ds_assert(self != ds_NULL);\
    ds_assert(action != ds_NULL);\
    const name##_node *current = self->head;\
    while (current != ds_NULL) {\
        action(current->data);\
        current = current->next;\
    }\
}\
\
ds_API static inline void name##_delete(name *self) {\
    ds_assert(self != ds_NULL);\
    name##_clear(self);\
    *self = (name) {0};\
}

/** Declares a doubly linked list of the given type. */
#define ds_DECLARE_LIST(T, deleter)\
        ds_DECLARE_LIST_NAMED(T##_list, T, deleter)

#endif // DS_LIST_H
