// .h
// Generic Event Handler Data Structure
// by Kyle Furey

#ifndef DS_SIGNAL_H
#define DS_SIGNAL_H

#include "vector.h"

/** Declares a named event handler for the given function signature. */
#define DECLARE_SIGNAL_NAMED(name, T, deleter, R, ...)\
// TODO

/** Declares an event handler for the given function signature.  */
#define DECLARE_SIGNAL(T, deleter, R, ...) DECLARE_SIGNAL_NAMED(T##_signal, T, deleter, R, ##__VA_ARGS__)

#endif // DS_SIGNAL_H
