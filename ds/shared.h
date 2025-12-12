// .h
// Generic Shared Reference Data Structure
// by Kyle Furey

#ifndef DS_SHARED_H
#define DS_SHARED_H

#include "std.h"

/** Declares a named shared pointer for the given type. */
#define DECLARE_SHARED_NAMED(name, T, deleter)\
// TODO

/** Declares a shared pointer for the given type. */
#define DECLARE_SHARED(T, deleter) DECLARE_SHARED_NAMED(shared_##T, T, deleter)

#endif // DS_SHARED_H
