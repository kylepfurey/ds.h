// .h
// Generic Priority Queue Structure
// by Kyle Furey

#ifndef DS_QUEUE_H
#define DS_QUEUE_H

#include "std.h"

/** Declares a named priority queue of the given types. */
#define DECLARE_QUEUE_NAMED(name, T, P, deleter, comparer)\
// TODO

/** Declares a priority queue of the given types. */
#define DECLARE_QUEUE(T, P, deleter, comparer) DECLARE_QUEUE_NAMED(T##_queue, T, P, deleter, comparer)

#endif // DS_QUEUE_H
