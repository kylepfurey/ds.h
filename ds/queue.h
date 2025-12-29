// .h
// Generic Priority Queue Structure
// by Kyle Furey

#ifndef DS_QUEUE_H
#define DS_QUEUE_H

#include "list.h"

/** Declares a named priority queue of the given types. */
#define DECLARE_QUEUE_NAMED(name, T, P, x_y_comparer, deleter)\
\
typedef struct {\
    T data;\
    P priority;\
} __##name##_pair;\
\
DECLARE_LIST_NAMED(__##name##_list, __##name##_pair, void_deleter)\
\
typedef struct {\
    __##name##_list list;\
} name;\
\
static inline name name##_new() {\
    return (name) {\
        __##name##_list_new(),\
    };\
}\
\
static inline size_t name##_count(const name *self) {\
    assert(self != NULL);\
    return self->list.count;\
}\
\
static inline bool name##_empty(const name *self) {\
    assert(self != NULL);\
    assert((self->list.count == 0) ==\
           ((self->list.head == NULL) &&\
           (self->list.tail == NULL)));\
    return self->list.count == 0;\
}\
\
static inline T *name##_next(name *self) {\
    assert(self != NULL);\
    assert(self->list.count > 0);\
    assert(self->list.head != NULL);\
    return &self->list.head->data.data;\
}\
\
static inline const T *name##_next_const(const name *self) {\
    assert(self != NULL);\
    assert(self->list.count > 0);\
    assert(self->list.head != NULL);\
    return &self->list.head->data.data;\
}\
\
static inline T *name##_last(name *self) {\
    assert(self != NULL);\
    assert(self->list.count > 0);\
    assert(self->list.tail != NULL);\
    return &self->list.tail->data.data;\
}\
\
static inline const T *name##_last_const(const name *self) {\
    assert(self != NULL);\
    assert(self->list.count > 0);\
    assert(self->list.tail != NULL);\
    return &self->list.tail->data.data;\
}\
\
static inline void name##_push(name *self, T data, P priority) {\
    assert(self != NULL);\
    if (self->list.head == NULL) {\
        __##name##_list_push_front(&self->list, (__##name##_pair) { data, priority, });\
        assert(self->list.head != NULL);\
        return;\
    }\
    __##name##_list_node *current = self->list.head;\
    while (current != NULL) {\
        P x = priority;\
        P y = current->data.priority;\
        if ((x_y_comparer)) {\
            __##name##_list_insert_before(&self->list, current, (__##name##_pair) { data, priority, });\
            return;\
        }\
        current = current->next;\
    }\
    __##name##_list_push_back(&self->list, (__##name##_pair) { data, priority, });\
}\
\
static inline void name##_pop_first(name *self) {\
    assert(self != NULL);\
    assert(self->list.count > 0);\
    assert(self->list.head != NULL);\
    deleter(&self->list.head->data.data);\
    __##name##_list_pop_front(&self->list);\
}\
\
static inline void name##_pop_last(name *self) {\
    assert(self != NULL);\
    assert(self->list.count > 0);\
    assert(self->list.tail != NULL);\
    deleter(&self->list.tail->data.data);\
    __##name##_list_pop_back(&self->list);\
}\
\
static inline void name##_clear(name *self) {\
    assert(self != NULL);\
    __##name##_list_node *current = self->list.head;\
    size_t remaining = self->list.count;\
    while (current != NULL) {\
        deleter(&current->data.data);\
        current = current->next;\
        --remaining;\
    }\
    assert(remaining == 0);\
    __##name##_list_clear(&self->list);\
}\
\
static inline bool name##_foreach(name *self, bool (*action)(T *)) {\
    assert(self != NULL);\
    assert(action != NULL);\
    __##name##_list_node *current = self->list.head;\
    size_t remaining = self->list.count;\
    while (current != NULL) {\
        if (!action(&current->data.data)) {\
            return false;\
        }\
        current = current->next;\
        --remaining;\
    }\
    assert(remaining == 0);\
    return true;\
}\
\
static inline bool name##_foreach_const(const name *self, bool (*action)(const T *)) {\
    assert(self != NULL);\
    assert(action != NULL);\
    const __##name##_list_node *current = self->list.head;\
    size_t remaining = self->list.count;\
    while (current != NULL) {\
        if (!action(&current->data.data)) {\
            return false;\
        }\
        current = current->next;\
        --remaining;\
    }\
    assert(remaining == 0);\
    return true;\
}\
\
static inline void name##_free(name *self) {\
    assert(self != NULL);\
    name##_clear(self);\
}

/** Declares a priority queue of the given types. */
#define DECLARE_QUEUE(T, P, x_y_comparer, deleter) DECLARE_QUEUE_NAMED(T##_queue, T, P, x_y_comparer, deleter)

#endif // DS_QUEUE_H
