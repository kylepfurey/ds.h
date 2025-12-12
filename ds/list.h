// .h
// Generic Doubly Linked List Data Structure
// by Kyle Furey

#ifndef DS_LIST_H
#define DS_LIST_H

#include "std.h"

/** Declares a named doubly linked list of the given type. */
#define DECLARE_LIST_NAMED(name, T, deleter)\
// TODO

/** Declares a doubly linked list of the given type. */
#define DECLARE_LIST(T, deleter) DECLARE_LIST_NAMED(T##_list, T, deleter)

#endif // DS_LIST_H
