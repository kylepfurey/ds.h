// .h
// Generic Hash Map Data Structure
// by Kyle Furey

#ifndef DS_MAP_H
#define DS_MAP_H

#include "std.h"

/** Declares a named hash map of the given types. */
#define DECLARE_MAP_NAMED(name, K, V, deleter, hasher)\
// TODO

/** Declares a hash map of the given types. */
#define DECLARE_MAP(K, V, deleter, hasher) DECLARE_MAP_NAMED(K##_map, K, V, deleter, hasher)

#endif // DS_MAP_H
