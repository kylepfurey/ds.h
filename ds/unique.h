// .h
// Generic Unique Reference Data Structure
// by Kyle Furey

#ifndef DS_UNIQUE_H
#define DS_UNIQUE_H

#include "std.h"

/** Declares a named unique pointer for the given type. */
#define DECLARE_UNIQUE_NAMED(name, T, deleter)\
// TODO

/** Declares a unique pointer for the given type. */
#define DECLARE_UNIQUE(T, deleter) DECLARE_UNIQUE_NAMED(unique_##T, T, deleter)

#endif // DS_UNIQUE_H
