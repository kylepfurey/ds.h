// .h
// Generic Weak Reference Data Structure
// by Kyle Furey

#ifndef DS_WEAK_H
#define DS_WEAK_H

#include "std.h"

/** Declares a named weak pointer for the given type. */
#define DECLARE_WEAK_NAMED(name, shared_name)\
// TODO

/** Declares a weak pointer for the given type. */
#define DECLARE_WEAK(T) DECLARE_WEAK_NAMED(weak_##T, shared_##T)

#endif // DS_WEAK_H
