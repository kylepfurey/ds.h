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
} ds__##name##_pair;\
\
DECLARE_LIST_NAMED(ds__##name##_list, ds__##name##_pair, void_deleter)\
\
typedef struct {\
    ds__##name##_list queue;\
} name;\
\
static inline name name##_new() {\
    return (name) {\
        ds__##name##_list_new(),\
    };\
}\
\
static inline name name##_copy(const name *queue) {\
    ds_assert(queue != NULL);\
    return (name) {\
        ds__##name##_list_copy(&queue->queue),\
    };\
}\
\
static inline ds_size name##_count(const name *self) {\
    ds_assert(self != NULL);\
    return self->queue.count;\
}\
\
static inline bool name##_empty(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert((self->queue.count == 0) ==\
           ((self->queue.head == NULL) &&\
           (self->queue.tail == NULL)));\
    return self->queue.count == 0;\
}\
\
static inline T *name##_next(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->queue.count > 0);\
    ds_assert(self->queue.head != NULL);\
    return &self->queue.head->data.data;\
}\
\
static inline const T *name##_next_const(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->queue.count > 0);\
    ds_assert(self->queue.head != NULL);\
    return &self->queue.head->data.data;\
}\
\
static inline T *name##_last(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->queue.count > 0);\
    ds_assert(self->queue.tail != NULL);\
    return &self->queue.tail->data.data;\
}\
\
static inline const T *name##_last_const(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->queue.count > 0);\
    ds_assert(self->queue.tail != NULL);\
    return &self->queue.tail->data.data;\
}\
\
static inline void name##_push(name *self, T data, P priority) {\
    ds_assert(self != NULL);\
    if (self->queue.head == NULL) {\
        ds__##name##_list_push_front(\
            &self->queue,\
            (ds__##name##_pair) {\
                data,\
                priority,\
            }\
        );\
        ds_assert(self->queue.head != NULL);\
        return;\
    }\
    ds__##name##_list_node *current = self->queue.head;\
    while (current != NULL) {\
        P x = priority;\
        P y = current->data.priority;\
        if ((x_y_comparer)) {\
            ds__##name##_list_insert_before(\
                &self->queue,\
                current,\
                (ds__##name##_pair) {\
                    data,\
                    priority,\
                }\
            );\
            return;\
        }\
        current = current->next;\
    }\
    ds__##name##_list_push_back(\
        &self->queue,\
        (ds__##name##_pair) {\
            data,\
            priority,\
        }\
    );\
}\
\
static inline void name##_pop_first(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->queue.count > 0);\
    ds_assert(self->queue.head != NULL);\
    deleter(&self->queue.head->data.data);\
    ds__##name##_list_pop_front(&self->queue);\
}\
\
static inline void name##_pop_last(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->queue.count > 0);\
    ds_assert(self->queue.tail != NULL);\
    deleter(&self->queue.tail->data.data);\
    ds__##name##_list_pop_back(&self->queue);\
}\
\
static inline void name##_clear(name *self) {\
    ds_assert(self != NULL);\
    ds__##name##_list_node *current = self->queue.head;\
    while (current != NULL) {\
        deleter(&current->data.data);\
        current = current->next;\
    }\
    ds__##name##_list_clear(&self->queue);\
}\
\
static inline void name##_foreach(const name *self, void(*action)(T)) {\
    ds_assert(self != NULL);\
    ds_assert(action != NULL);\
    const ds__##name##_list_node *current = self->queue.head;\
    while (current != NULL) {\
        action(current->data.data);\
        current = current->next;\
    }\
}\
\
static inline void name##_delete(name *self) {\
    ds_assert(self != NULL);\
    name##_clear(self);\
    *self = (name) {0};\
}

/** Declares a priority queue of the given types. */
#define DECLARE_QUEUE(T, P, x_y_comparer, deleter) DECLARE_QUEUE_NAMED(T##_queue, T, P, x_y_comparer, deleter)

#endif // DS_QUEUE_H
