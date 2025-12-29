// .h
// Generic Hash Map Data Structure
// by Kyle Furey

#ifndef DS_MAP_H
#define DS_MAP_H

#include "vector.h"

/** Declares a named hash map of the given types. */
#define DECLARE_MAP_NAMED(name, K, V, key_hasher, deleter)\
// TODO

/** Declares a hash map of the given types. */
#define DECLARE_MAP(K, V, key_hasher, deleter) DECLARE_MAP_NAMED(K##_map, K, V, key_hasher, deleter)

#endif // DS_MAP_H
