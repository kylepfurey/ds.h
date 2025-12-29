// .h
// Generic Doubly Linked List Data Structure
// by Kyle Furey

#ifndef DS_LIST_H
#define DS_LIST_H

#include "std.h"

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
    size_t count;\
    struct name##_node *head;\
    struct name##_node *tail;\
} name;\
\
static inline name name##_new() {\
    return (name) {\
        0,\
        NULL,\
        NULL,\
    };\
}\
\
static inline size_t name##_count(const name *self) {\
    assert(self != NULL);\
    return self->count;\
}\
\
static inline bool name##_valid(const name *self, size_t index) {\
    assert(self != NULL);\
    return index < self->count;\
}\
\
static inline name##_node *name##_get(name *self, size_t index) {\
    assert(self != NULL);\
    assert(index < self->count);\
    name##_node *node;\
    if (index <= self->count / 2) {\
        node = self->head;\
        assert(node != NULL);\
        for (size_t i = 0; i != index; ++i) {\
            node = node->next;\
            assert(node != NULL);\
        }\
    } else {\
        node = self->tail;\
        assert(node != NULL);\
        for (size_t i = self->count - 1; i != index; --i) {\
            node = node->previous;\
            assert(node != NULL);\
        }\
    }\
    return node;\
}\
\
static inline const name##_node *name##_get_const(const name *self, size_t index) {\
    assert(self != NULL);\
    assert(index < self->count);\
    const name##_node *node;\
    if (index <= self->count / 2) {\
        node = self->head;\
        assert(node != NULL);\
        for (size_t i = 0; i != index; ++i) {\
            node = node->next;\
            assert(node != NULL);\
        }\
    } else {\
        node = self->tail;\
        assert(node != NULL);\
        for (size_t i = self->count - 1; i != index; --i) {\
            node = node->previous;\
            assert(node != NULL);\
        }\
    }\
    return node;\
}\
\
static inline name##_node *name##_insert_before(name *self, name##_node *node, T data) {\
    assert(self != NULL);\
    assert(node != NULL);\
    assert(self->count > 0);\
    ++self->count;\
    name##_node *new_node = (name##_node *) malloc(sizeof(name##_node));\
    assert(new_node != NULL);\
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
static inline name##_node *name##_insert_after(name *self, name##_node *node, T data) {\
    assert(self != NULL);\
    assert(node != NULL);\
    assert(self->count > 0);\
    ++self->count;\
    name##_node *new_node = (name##_node *) malloc(sizeof(name##_node));\
    assert(new_node != NULL);\
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
static inline void name##_erase(name *self, name##_node *node) {\
    assert(self != NULL);\
    assert(node != NULL);\
    assert(self->count > 0);\
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
    free(node);\
}\
\
static inline name##_node *name##_push_front(name *self, T data) {\
    assert(self != NULL);\
    ++self->count;\
    name##_node *node = (name##_node *) malloc(sizeof(name##_node));\
    assert(node != NULL);\
    node->data = data;\
    node->previous = NULL;\
    if (self->head == NULL) {\
        assert(self->tail == NULL);\
        node->next = NULL;\
        self->tail = node;\
    } else {\
        assert(self->tail != NULL);\
        node->next = self->head;\
        self->head->previous = node;\
    }\
    self->head = node;\
    return node;\
}\
\
static inline name##_node *name##_push_back(name *self, T data) {\
    assert(self != NULL);\
    ++self->count;\
    name##_node *node = (name##_node *) malloc(sizeof(name##_node));\
    assert(node != NULL);\
    node->data = data;\
    node->next = NULL;\
    if (self->tail == NULL) {\
        assert(self->head == NULL);\
        node->previous = NULL;\
        self->head = node;\
    } else {\
        assert(self->head != NULL);\
        node->previous = self->tail;\
        self->tail->next = node;\
    }\
    self->tail = node;\
    return node;\
}\
\
static inline void name##_pop_front(name *self) {\
    assert(self != NULL);\
    assert(self->count > 0);\
    --self->count;\
    name##_node *node = self->head;\
    assert(node != NULL);\
    self->head = node->next;\
    if (self->head == NULL) {\
        self->tail = NULL;\
        assert(self->count == 0);\
    }\
    deleter(&node->data);\
    free(node);\
    if (self->head != NULL) {\
        self->head->previous = NULL;\
    }\
}\
\
static inline void name##_pop_back(name *self) {\
    assert(self != NULL);\
    assert(self->count > 0);\
    --self->count;\
    name##_node *node = self->tail;\
    assert(node != NULL);\
    self->tail = node->previous;\
    if (self->tail == NULL) {\
        self->head = NULL;\
        assert(self->count == 0);\
    }\
    deleter(&node->data);\
    free(node);\
    if (self->tail != NULL) {\
        self->tail->next = NULL;\
    }\
}\
\
static inline void name##_clear(name *self) {\
    assert(self != NULL);\
    name##_node *current = self->head;\
    while (current != NULL) {\
        name##_node *next = current->next;\
        deleter(&current->data);\
        free(current);\
        current = next;\
    }\
    self->count = 0;\
    self->head = NULL;\
    self->tail = NULL;\
}\
\
static inline bool name##_foreach(name *self, bool (*action)(T *)) {\
    assert(self != NULL);\
    assert(action != NULL);\
    name##_node *current = self->head;\
    while (current != NULL) {\
        if (!action(&current->data)) {\
            return false;\
        }\
        current = current->next;\
    }\
    return true;\
}\
\
static inline bool name##_foreach_const(const name *self, bool (*action)(const T *)) {\
    assert(self != NULL);\
    assert(action != NULL);\
    const name##_node *current = self->head;\
    while (current != NULL) {\
        if (!action(&current->data)) {\
            return false;\
        }\
        current = current->next;\
    }\
    return true;\
}\
\
static inline void name##_free(name *self) {\
    assert(self != NULL);\
    name##_clear(self);\
}

/** Declares a doubly linked list of the given type. */
#define DECLARE_LIST(T, deleter) DECLARE_LIST_NAMED(T##_list, T, deleter)

#endif // DS_LIST_H
