// .h
// Generic Priority Queue Structure
// by Kyle Furey

/**
 * ds_queue.h
 *
 * ds_DECLARE_QUEUE_NAMED(
 *      name,               - The name of the data structure and function prefix.
 *
 *      T,                  - The type to generate this data structure with.
 *
 *      P,                  - The type used to sort T.
 *
 *      x_y_comparer,       - Inline comparison code used to compare priorities <x> and <y>.
 *                            You can use ds_DEFAULT_COMPARE for trivial types.
 *
 *      deleter,            - The name of the function used to deallocate T.
 *                            ds_void_deleter may be used for trivial types.
 * )
 *
 * This is a double-ended priority queue. It's a linked list with automatic ordering.
 * Elements are sorted via a priority value. The first and last element are O(1) accessible.
 * If you don't need priority ordering, use a regular linked list or vector.
 *
 * This is excellent for frontiers when doing small graph searches,
 * or collecting elements for processing in-order later.
 *
 * * Returns a new queue.
 * * This data structure must be deleted with queue_delete().
 *
 *   queue        queue_new               ( void )
 *
 * * Returns a new queue shallow copied from <queue>.
 * * This data structure must be deleted with queue_delete().
 *
 *   queue        queue_copy              ( const queue* queue )
 *
 * * Returns the number of elements in the queue.
 *
 *   size_t       queue_count             ( const queue* self )
 *
 * * Returns whether the queue is empty.
 *
 *   bool         queue_empty             ( const queue* self )
 *
 * * Returns a pointer to the element in the queue with the greatest priority.
 * * The queue must not be empty.
 *
 *   T*           queue_first             ( queue* self )
 *
 * * Returns a pointer to the element in the queue with the greatest priority.
 * * The queue must not be empty.
 *
 *   const T*     queue_first_const       ( const queue* self )
 *
 * * Returns a pointer to the element in the queue with the least priority.
 * * The queue must not be empty.
 *
 *   T*           queue_last              ( queue* self )
 *
 * * Returns a pointer to the element in the queue with the least priority.
 * * The queue must not be empty.
 *
 *   const T*     queue_last_const        ( const queue* self )
 *
 * * Adds a new element with <priority> into the queue.
 *
 *   void         queue_push              ( queue* self, T data, P priority )
 *
 * * Removes the element in the queue with the greatest priority.
 * The queue must not be empty.
 *
 *   void         queue_pop_first         ( queue* self )
 *
 * * Removes the element in the queue with the least priority.
 * The queue must not be empty.
 *
 *   void         queue_pop_last          ( queue* self )
 *
 * * Deletes all elements in the queue.
 *
 *   void         queue_clear             ( queue* self )
 *
 * * Iterates the queue calling <action> on each element.
 *
 *   void         queue_foreach           ( const queue* self, void (*action)(T) )
 *
 * * Safely deletes a queue.
 *
 *   void         queue_delete            ( queue* self )
 */

#ifndef DS_QUEUE_H
#define DS_QUEUE_H

#include "ds_list.h"

/** Declares a named priority queue of the given types. */
#define ds_DECLARE_QUEUE_NAMED(name, T, P, x_y_comparer, deleter)\
\
typedef struct {\
    T data;\
    P priority;\
} ds__##name##_pair;\
\
ds_DECLARE_LIST_NAMED(ds__##name##_list, ds__##name##_pair, ds_void_deleter)\
\
typedef struct {\
    ds__##name##_list queue;\
} name;\
\
ds_API static inline name name##_new() {\
    return (name) {\
        ds__##name##_list_new(),\
    };\
}\
\
ds_API static inline name name##_copy(const name *queue) {\
    ds_assert(queue != NULL);\
    return (name) {\
        ds__##name##_list_copy(&queue->queue),\
    };\
}\
\
ds_API static inline ds_size name##_count(const name *self) {\
    ds_assert(self != NULL);\
    return self->queue.count;\
}\
\
ds_API static inline ds_bool name##_empty(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert((self->queue.count == 0) ==\
           ((self->queue.head == NULL) &&\
           (self->queue.tail == NULL)));\
    return self->queue.count == 0;\
}\
\
ds_API static inline T *name##_first(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->queue.count > 0);\
    ds_assert(self->queue.head != NULL);\
    return &self->queue.head->data.data;\
}\
\
ds_API static inline const T *name##_first_const(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->queue.count > 0);\
    ds_assert(self->queue.head != NULL);\
    return &self->queue.head->data.data;\
}\
\
ds_API static inline T *name##_last(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->queue.count > 0);\
    ds_assert(self->queue.tail != NULL);\
    return &self->queue.tail->data.data;\
}\
\
ds_API static inline const T *name##_last_const(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->queue.count > 0);\
    ds_assert(self->queue.tail != NULL);\
    return &self->queue.tail->data.data;\
}\
\
ds_API static inline void name##_push(name *self, T data, P priority) {\
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
ds_API static inline void name##_pop_first(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->queue.count > 0);\
    ds_assert(self->queue.head != NULL);\
    deleter(&self->queue.head->data.data);\
    ds__##name##_list_pop_front(&self->queue);\
}\
\
ds_API static inline void name##_pop_last(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->queue.count > 0);\
    ds_assert(self->queue.tail != NULL);\
    deleter(&self->queue.tail->data.data);\
    ds__##name##_list_pop_back(&self->queue);\
}\
\
ds_API static inline void name##_clear(name *self) {\
    ds_assert(self != NULL);\
    ds__##name##_list_node *current = self->queue.head;\
    while (current != NULL) {\
        deleter(&current->data.data);\
        current = current->next;\
    }\
    ds__##name##_list_clear(&self->queue);\
}\
\
ds_API static inline void name##_foreach(const name *self, void(*action)(T)) {\
    ds_assert(self != NULL);\
    ds_assert(action != NULL);\
    const ds__##name##_list_node *current = self->queue.head;\
    while (current != NULL) {\
        action(current->data.data);\
        current = current->next;\
    }\
}\
\
ds_API static inline void name##_delete(name *self) {\
    ds_assert(self != NULL);\
    name##_clear(self);\
    *self = (name) {0};\
}

/** Declares a priority queue of the given types. */
#define ds_DECLARE_QUEUE(T, P, x_y_comparer, deleter)\
        ds_DECLARE_QUEUE_NAMED(T##_queue, T, P, x_y_comparer, deleter)

#endif // DS_QUEUE_H
