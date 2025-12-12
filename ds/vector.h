// .h
// Generic Dynamic Array Data Structure
// by Kyle Furey

#ifndef DS_VECTOR_H
#define DS_VECTOR_H

#include "std.h"

/** Declares a named dynamic array of the given type. */
#define DECLARE_VECTOR_NAMED(name, T, deleter)\
// TODO

/** Declares a dynamic array of the given type. */
#define DECLARE_VECTOR(T, deleter) DECLARE_VECTOR_NAMED(T##_vector, T, deleter)

#endif // DS_VECTOR_H
