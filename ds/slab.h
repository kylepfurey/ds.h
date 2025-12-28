// .h
// Generic Slab Allocator Data Structure
// by Kyle Furey

#ifndef DS_SLAB_H
#define DS_SLAB_H

#include "vector.h"

/** Declares a named slab allocator of the given type. */
#define DECLARE_SLAB_NAMED(name, T, deleter)\
// TODO

/** Declares a slab allocator of the given type. */
#define DECLARE_SLAB(T, deleter) DECLARE_SLAB_NAMED(T##_slab, T, deleter)

#endif // DS_SLAB_H
