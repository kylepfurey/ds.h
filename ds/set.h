// .h
// Generic Sorted Set Data Structure
// by Kyle Furey

#ifndef DS_SET_H
#define DS_SET_H

#include "std.h"

/** Declares a named sorted set of the given type. */
#define DECLARE_SET_NAMED(name, T, deleter, x_y_comparer)\
// TODO

/** Declares a sorted set of the given type. */
#define DECLARE_SET(T, deleter, comparer) DECLARE_SET_NAMED(T##_set, T, deleter, x_y_comparer)

#endif // DS_SET_H
