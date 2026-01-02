// .h
// ds.h Optional Value Data Structure
// by Kyle Furey

/**
 * ds_optional.h
 *
 * ds_DECLARE_OPTIONAL_NAMED(
 *      name,                   - The name of the data structure and function prefix.
 *
 *      T,                      - The type to generate this data structure with.
 *
 *      deleter,                - The name of the function used to deallocate T.
 *                                ds_void_deleter may be used for trivial types.
 * )
 *
 * An optional value is just an abstraction for a value that may or may not exist.
 *
 * Unlike shared references which guarantee access and ownership of memory, optionals
 * provide functionality for explicitly handling NULL values in a safe and elegant way.
 *
 * * Returns a valid optional containing <data>.
 * * Optionals are just wrappers over data and do not allocate memory.
 * * This data structure must be deleted with optional_delete().
 *
 *   optional            optional_new               ( T data )
 *
 * * Returns an empty optional indicating no value is present.
 * * Optionals are just wrappers over data and do not allocate memory.
 * * This data structure must be deleted with optional_delete().
 *
 *   optional            optional_none              ( void )
 *
 * * Returns whether the optional has a value.
 *
 *   bool                optional_valid             ( const optional* self )
 *
 * * Returns whether the optional does not have a value.
 *
 *   bool                optional_empty             ( const optional* self )
 *
 * * Releases and returns the optional's value.
 * * The optional must not be empty.
 *
 *   T                   optional_take              ( optional* self )
 *
 * * Releases and returns the optional's value or <data> if it is empty.
 * * This does not set the optional.
 *
 *   T                   optional_take_or           ( optional* self, T data )
 *
 * * Returns a pointer to the optional's value.
 * * The optional must not be empty.
 *
 *   T*                  optional_borrow            ( optional* self )
 *
 * * Returns a pointer to the optional's value.
 * * The optional must not be empty.
 *
 *   const T*            optional_borrow_const      ( const optional* self )
 *
 * * Resets the optional's value.
 * * This will overwrite the current value, if any.
 *
 *   void                optional_reset             ( optional* self, T data )
 *
 * * Deletes the optional's current value, if any.
 *
 *   void                optional_clear             ( optional* self )
 *
 * * Mutates the optional only if it is valid.
 * * This replaces the optional's value after calling <transform>.
 * * The optional's value is not deleted, only updated.
 * * Returns <self>.
 *
 *   optional*           optional_map               ( optional* self, T(*transform)(T) )
 *
 * * Mutates the optional only if it is valid.
 * * This deletes the optional if <predicate> returns false.
 * * Returns <self>.
 *
 *   optional*           optional_filter            ( optional* self, bool(*predicate)(T) )
 *
 * * Mutates the optional only if it is valid.
 * * <data> is "applied" to the optional's value using <accumulator>:
 * * if value => value = accumulator(value, data)
 * * Returns <self>.
 *
 *   optional*           optional_reduce            ( optional* self, T(*accumulator)(T, T), T data )
 *
 * * Calls <action> with the optional's value only if it is valid.
 * * Returns <self>.
 *
 *   const optional*     optional_foreach           ( const optional* self, void(*action)(T) )
 *
 * * Safely deletes the optional's value only if it is valid.
 *
 *   void                optional_delete            ( optional* self )
 */

#ifndef DS_OPTIONAL_H
#define DS_OPTIONAL_H

#include "ds_def.h"

/** Declares a named optional value for the given type. */
#define ds_DECLARE_OPTIONAL_NAMED(name, T, deleter)\
\
typedef struct {\
    T data;\
    ds_bool valid;\
} name;\
\
ds_API static inline name name##_new(T data) {\
    return (name) {\
        data,\
        ds_true,\
    };\
}\
\
ds_API static inline name name##_none(void) {\
    return (name) {0};\
}\
\
ds_API static inline ds_bool name##_valid(const name *self) {\
    ds_assert(self != ds_NULL);\
    return self->valid;\
}\
\
ds_API static inline ds_bool name##_empty(const name *self) {\
    ds_assert(self != ds_NULL);\
    return !self->valid;\
}\
\
ds_API static inline T name##_take(name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->valid);\
    self->valid = ds_false;\
    return self->data;\
}\
\
ds_API static inline T name##_take_or(name *self, T data) {\
    ds_assert(self != ds_NULL);\
    ds_bool valid = self->valid;\
    self->valid = ds_false;\
    return valid ? self->data : data;\
}\
\
ds_API static inline T *name##_borrow(name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->valid);\
    return &self->data;\
}\
\
ds_API static inline const T *name##_borrow_const(const name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->valid);\
    return &self->data;\
}\
\
ds_API static inline void name##_reset(name *self, T data) {\
    ds_assert(self != ds_NULL);\
    if (self->valid) {\
        deleter(&self->data);\
    }\
    self->data = data;\
    self->valid = ds_true;\
}\
\
ds_API static inline void name##_clear(name *self) {\
    ds_assert(self != ds_NULL);\
    if (self->valid) {\
        deleter(&self->data);\
    }\
    self->valid = ds_false;\
}\
\
ds_API static inline name *name##_map(name *self, T(*transform)(T)) {\
    ds_assert(self != ds_NULL);\
    ds_assert(transform != ds_NULL);\
    if (self->valid) {\
        self->data = transform(self->data);\
    }\
    return self;\
}\
\
ds_API static inline name *name##_filter(name *self, ds_bool(*predicate)(T)) {\
    ds_assert(self != ds_NULL);\
    ds_assert(predicate != ds_NULL);\
    if (self->valid) {\
        if (!predicate(self->data)) {\
            deleter(&self->data);\
            self->valid = ds_false;\
        }\
    }\
    return self;\
}\
\
ds_API static inline name *name##_reduce(name *self, T(*accumulator)(T, T), T data) {\
    ds_assert(self != ds_NULL);\
    ds_assert(accumulator != ds_NULL);\
    if (self->valid) {\
        self->data = accumulator(self->data, data);\
    }\
    return self;\
}\
\
ds_API static inline const name *name##_foreach(const name *self, void(*action)(T)) {\
    ds_assert(self != ds_NULL);\
    ds_assert(action != ds_NULL);\
    if (self->valid) {\
        action(self->data);\
    }\
    return self;\
}\
\
ds_API static inline void name##_delete(name *self) {\
    ds_assert(self != ds_NULL);\
    if (self->valid) {\
        deleter(&self->data);\
    }\
    *self = (name) {0};\
}

/** Declares an optional value for the given type. */
#define ds_DECLARE_OPTIONAL(T, deleter)\
        ds_DECLARE_OPTIONAL_NAMED(optional_##T, T, deleter)

#endif // DS_OPTIONAL_H
