// .h
// Generic Mutable String Data Structure
// by Kyle Furey

/**
 * ds_string.h
 *
 * Strings are widely used in C but reallocation can be painful and unsafe.
 * This type is a wrapper for a vector meant for string operations.
 * It is mutable, meaning memory is owned and the buffer can be modified.
 *
 * This is a safe choice for string builders, storing user input, or just general use.
 * The vector always ensures enough memory is available, the string is null-terminated,
 * and that the size is cached for fast calculations.
 *
 * string           string_new              ( const char* str )
 *
 * string           string_copy             ( const string* str )
 *
 * size_t           string_length           ( const string* self )
 *
 * size_t           string_capacity         ( const string* self )
 *
 * bool             string_empty            ( const string* self )
 *
 * char             string_get              ( const string* self, size_t index )
 *
 * const char*      string_set              ( string* self, size_t index, char data )
 *
 * const char*      string_cstr             ( const string* self )
 *
 * const char*      string_substr           ( string* self, size_t start, size_t length )
 *
 * int              string_compare          ( const string* self, const char* str )
 *
 * void             string_resize           ( string* self, size_t length )
 *
 * const char*      string_insert           ( string* self, size_t index, const char* str )
 *
 * const char*      string_erase            ( string* self, size_t index, size_t length )
 *
 * const char*      string_append           ( string* self, const char* str )
 *
 * const char*      string_prepend          ( string* self, const char* str )
 *
 * size_t           string_find             ( const string* self, const char* str )
 *
 * size_t           string_find_last        ( const string* self, const char* str )
 *
 * bool             string_contains         ( const string* self, const char* str )
 *
 * const char*      string_replace_first    ( string* self, const char* find, const char* replace )
 *
 * const char*      string_replace_last     ( string* self, const char* find, const char* replace )
 *
 * const char*      string_replace_all      ( string* self, const char* find, const char* replace )
 *
 * const char*      string_reverse          ( string* self )
 *
 * const char*      string_upper            ( string* self )
 *
 * const char*      string_lower            ( string* self )
 *
 * const char*      string_trim             ( string* self, bool resize )
 *
 * void             string_clear            ( string* self )
 *
 * const char*      string_map              ( string* self, char(*transform)(char) )
 *
 * size_t           string_filter           ( string* self, bool(*predicate)(char) )
 *
 * char             string_reduce           ( string* self, char start, char(*accumulator)(char, char) )
 *
 * void             string_foreach          ( const string* self, void(*action)(char) )
 *
 * void             string_delete           ( string* self )
 */

#ifndef DS_STRING_H
#define DS_STRING_H

#include "ds_vector.h"

/** Declares a named mutable string of the given type. */
#define DECLARE_STRING_NAMED(name, T)\
\
DS_API static inline ds_size ds__##name##_strlen(const T *str) {\
    ds_assert(str != NULL);\
    ds_size length = 0;\
    while (*str != 0) {\
        ++length;\
        ++str;\
    }\
    return length;\
}\
\
DS_API static inline ds_int ds__##name##_strcmp(const T *left, const T *right) {\
    ds_assert(left != NULL);\
    ds_assert(right != NULL);\
    while (*left != 0 && *right != 0 && (*left == *right)) {\
        left++;\
        right++;\
    }\
    return (ds_int) ((ds_uint) *left - (ds_uint) *right);\
}\
\
DS_API static inline ds_int ds__##name##_strncmp(const T *left, const T *right, ds_size length) {\
    ds_assert(left != NULL);\
    ds_assert(right != NULL);\
    for (ds_size i = 0; i < length; ++i) {\
        if (left[i] != right[i]) {\
            return (ds_int) ((ds_uint) left[i] - (ds_uint) right[i]);\
        }\
        if (left[i] == 0) {\
            break;\
        }\
    }\
    return 0;\
}\
\
DECLARE_VECTOR_NAMED(ds__##name##_vector, T, void_deleter)\
\
typedef struct {\
    ds__##name##_vector buffer;\
} name;\
\
DS_API static inline name name##_new(const T *str) {\
    ds_assert(str != NULL);\
    ds_size length = ds__##name##_strlen(str);\
    name self = (name) {\
        ds__##name##_vector_new(length + 1),\
    };\
    memcpy(self.buffer.array, str, sizeof(T) * (length + 1));\
    self.buffer.count = length;\
    return self;\
}\
\
DS_API static inline name name##_copy(const name *str) {\
    ds_assert(str != NULL);\
    return (name) {\
        ds__##name##_vector_copy(&str->buffer),\
    };\
}\
\
DS_API static inline ds_size name##_length(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    return self->buffer.count;\
}\
\
DS_API static inline ds_size name##_capacity(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    return self->buffer.capacity;\
}\
\
DS_API static inline bool name##_empty(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    return self->buffer.count == 0;\
}\
\
DS_API static inline T name##_get(const name *self, ds_size index) {\
    ds_assert(self != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != NULL);\
    ds_assert(index < self->buffer.count);\
    return self->buffer.array[index];\
}\
\
DS_API static inline const T *name##_set(name *self, ds_size index, T data) {\
    ds_assert(self != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != NULL);\
    ds_assert(index < self->buffer.count);\
    if (VECTOR_TRUNC_ASSERT) {\
        ds_assert(data != 0);\
    }\
    self->buffer.array[index] = data;\
    if (data == 0) {\
        self->buffer.count = index;\
    }\
    return self->buffer.array;\
}\
\
DS_API static inline const T *name##_cstr(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != NULL);\
    return self->buffer.array;\
}\
\
DS_API static inline const T *name##_substr(name *self, ds_size start, ds_size length) {\
    ds_assert(self != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(start <= self->buffer.count);\
    ds_assert(start + length <= self->buffer.count);\
    ds_assert(self->buffer.array != NULL);\
    if (length == 0) {\
        self->buffer.count = 0;\
        self->buffer.array[0] = 0;\
        return self->buffer.array;\
    }\
    if (start > 0) {\
        memmove(self->buffer.array, self->buffer.array + start, sizeof(T) * length);\
    }\
    self->buffer.count = length;\
    self->buffer.array[length] = 0;\
    ds_assert(self->buffer.array[self->buffer.count] == 0);\
    return self->buffer.array;\
}\
\
DS_API static inline ds_int name##_compare(const name *self, const T *str) {\
    ds_assert(self != NULL);\
    ds_assert(str != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != NULL);\
    return ds__##name##_strcmp(self->buffer.array, str);\
}\
\
DS_API static inline void name##_resize(name *self, ds_size length) {\
    ds_assert(self != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != NULL);\
    if (VECTOR_TRUNC_ASSERT) {\
        ds_assert(length >= self->buffer.count);\
    }\
    if (length + 1 > self->buffer.capacity) {\
        ds__##name##_vector_resize(&self->buffer, length + 1);\
    } else if (length < self->buffer.count) {\
        ds__##name##_vector_resize(&self->buffer, length + 1);\
        self->buffer.count = length;\
        self->buffer.array[length] = 0;\
    }\
    ds_assert(self->buffer.array[self->buffer.count] == 0);\
}\
\
DS_API static inline const T *name##_insert(name *self, ds_size index, const T *str) {\
    ds_assert(self != NULL);\
    ds_assert(str != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(index <= self->buffer.count);\
    ds_assert(self->buffer.array != NULL);\
    ds_size length = ds__##name##_strlen(str);\
    if (self->buffer.count + length + 1 > self->buffer.capacity) {\
        ds__##name##_vector_resize(&self->buffer, self->buffer.count + length + 1);\
    }\
    memmove(\
        self->buffer.array + index + length,\
        self->buffer.array + index,\
        sizeof(T) * (self->buffer.count - index + 1)\
    );\
    memmove(self->buffer.array + index, str, sizeof(T) * length);\
    self->buffer.count += length;\
    ds_assert(self->buffer.array[self->buffer.count] == 0);\
    return self->buffer.array;\
}\
\
DS_API static inline const T *name##_erase(name *self, ds_size index, ds_size length) {\
    ds_assert(self != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(index < self->buffer.count);\
    ds_assert(self->buffer.array != NULL);\
    if (length == 0) {\
        return self->buffer.array;\
    }\
    if (index + length > self->buffer.count) {\
        length = self->buffer.count - index;\
    }\
    memmove(\
        self->buffer.array + index,\
        self->buffer.array + index + length,\
        sizeof(T) * (self->buffer.count - index - length + 1)\
    );\
    self->buffer.count -= length;\
    ds_assert(self->buffer.array[self->buffer.count] == 0);\
    return self->buffer.array;\
}\
\
DS_API static inline const T *name##_append(name *self, const T *str) {\
    ds_assert(self != NULL);\
    ds_assert(str != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    return name##_insert(self, self->buffer.count, str);\
}\
\
DS_API static inline const T *name##_prepend(name *self, const T *str) {\
    ds_assert(self != NULL);\
    ds_assert(str != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    return name##_insert(self, 0, str);\
}\
\
DS_API static inline ds_size name##_find(const name *self, const T *str) {\
    ds_assert(self != NULL);\
    ds_assert(str != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != NULL);\
    ds_size length = ds__##name##_strlen(str);\
    if (length == 0 || length > self->buffer.count) {\
        return NOT_FOUND;\
    }\
    for (ds_size i = 0; i + length <= self->buffer.count; ++i) {\
        if (ds__##name##_strncmp(self->buffer.array + i, str, length) == 0) {\
            return i;\
        }\
    }\
    return NOT_FOUND;\
}\
\
DS_API static inline ds_size name##_find_last(const name *self, const T *str) {\
    ds_assert(self != NULL);\
    ds_assert(str != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != NULL);\
    ds_size length = ds__##name##_strlen(str);\
    if (length == 0 || length > self->buffer.count) {\
        return NOT_FOUND;\
    }\
    for (ds_diff i = self->buffer.count - length + 1; i-- > 0;) {\
        if (ds__##name##_strncmp(self->buffer.array + i, str, length) == 0) {\
            return i;\
        }\
    }\
    return NOT_FOUND;\
}\
\
DS_API static inline bool name##_contains(const name *self, const T *str) {\
    ds_assert(self != NULL);\
    ds_assert(str != NULL);\
    return name##_find(self, str) != NOT_FOUND;\
}\
\
DS_API static inline const T *name##_replace_first(name *self, const T *find, const T *replace) {\
    ds_assert(self != NULL);\
    ds_assert(find != NULL);\
    ds_assert(replace != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != NULL);\
    ds_size index = name##_find(self, find);\
    if (index == NOT_FOUND) {\
        return self->buffer.array;\
    }\
    name##_erase(self, index, ds__##name##_strlen(find));\
    name##_insert(self, index, replace);\
    ds_assert(self->buffer.array[self->buffer.count] == 0);\
    return self->buffer.array;\
}\
\
DS_API static inline const T *name##_replace_last(name *self, const T *find, const T *replace) {\
    ds_assert(self != NULL);\
    ds_assert(find != NULL);\
    ds_assert(replace != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != NULL);\
    ds_size index = name##_find_last(self, find);\
    if (index == NOT_FOUND) {\
        return self->buffer.array;\
    }\
    name##_erase(self, index, ds__##name##_strlen(find));\
    name##_insert(self, index, replace);\
    ds_assert(self->buffer.array[self->buffer.count] == 0);\
    return self->buffer.array;\
}\
\
DS_API static inline const T *name##_replace_all(name *self, const T *find, const T *replace) {\
    ds_assert(self != NULL);\
    ds_assert(find != NULL);\
    ds_assert(replace != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != NULL);\
    ds_size find_len = ds__##name##_strlen(find);\
    ds_size replace_len = ds__##name##_strlen(replace);\
    if (find_len == 0) {\
        return self->buffer.array;\
    }\
    ds_size start = 0;\
    while (start <= self->buffer.count - find_len) {\
        ds_assert(start <= self->buffer.count);\
        self->buffer.array += start;\
        self->buffer.count -= start;\
        ds_size index = name##_find(self, find);\
        self->buffer.count += start;\
        self->buffer.array -= start;\
        if (index == NOT_FOUND) {\
            break;\
        }\
        index += start;\
        name##_erase(self, index, find_len);\
        name##_insert(self, index, replace);\
        start = index + replace_len;\
    }\
    ds_assert(self->buffer.array[self->buffer.count] == 0);\
    return self->buffer.array;\
}\
\
DS_API static inline const T *name##_reverse(name *self) {\
    ds_assert(self != NULL);\
    return ds__##name##_vector_reverse(&self->buffer);\
}\
\
DS_API static inline const T *name##_upper(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != NULL);\
    for (ds_size i = 0; i < self->buffer.count; ++i) {\
        self->buffer.array[i] = (T) toupper((ds_int) self->buffer.array[i]);\
    }\
    return self->buffer.array;\
}\
\
DS_API static inline const T *name##_lower(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != NULL);\
    for (ds_size i = 0; i < self->buffer.count; ++i) {\
        self->buffer.array[i] = (T) tolower((ds_int) self->buffer.array[i]);\
    }\
    return self->buffer.array;\
}\
\
DS_API static inline const T *name##_trim(name *self, bool resize) {\
    ds_assert(self != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != NULL);\
    ds_size start = 0;\
    ds_size end = self->buffer.count;\
    while (start < end && isspace((ds_int) self->buffer.array[start])) {\
        ++start;\
    }\
    while (end > start && isspace((ds_int) self->buffer.array[end - 1])) {\
        --end;\
    }\
    ds_size length = end - start;\
    if (start > 0 && length > 0) {\
        memmove(self->buffer.array, self->buffer.array + start, sizeof(T) * (length));\
    }\
    self->buffer.count = length;\
    self->buffer.array[length] = 0;\
    if (resize) {\
        ds__##name##_vector_resize(&self->buffer, length + 1);\
    }\
    return self->buffer.array;\
}\
\
DS_API static inline void name##_clear(name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != NULL);\
    self->buffer.count = 0;\
    self->buffer.array[0] = 0;\
}\
\
DS_API static inline const T *name##_map(name *self, T(*transform)(T)) {\
    ds_assert(self != NULL);\
    ds_assert(transform != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != NULL);\
    for (ds_size i = 0; i < self->buffer.count; ++i) {\
        self->buffer.array[i] = transform(self->buffer.array[i]);\
        if (VECTOR_TRUNC_ASSERT) {\
            ds_assert(self->buffer.array[i] != 0);\
        }\
        if (self->buffer.array[i] == 0) {\
            self->buffer.count = i;\
            break;\
        }\
    }\
    return self->buffer.array;\
}\
\
DS_API static inline ds_size name##_filter(name *self, bool(*predicate)(T)) {\
    ds_assert(self != NULL);\
    ds_assert(predicate != NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != NULL);\
    ds_size count = ds__##name##_vector_filter(&self->buffer, predicate);\
    self->buffer.array[count] = 0;\
    return count;\
}\
\
DS_API static inline T name##_reduce(name *self, T start, T(*accumulator)(T, T)) {\
    ds_assert(self != NULL);\
    ds_assert(accumulator != NULL);\
    return ds__##name##_vector_reduce(&self->buffer, start, accumulator);\
}\
\
DS_API static inline void name##_foreach(const name *self, void(*action)(T)) {\
    ds_assert(self != NULL);\
    ds_assert(action != NULL);\
    ds__##name##_vector_foreach(&self->buffer, action);\
}\
\
DS_API static inline void name##_delete(name *self) {\
    ds_assert(self != NULL);\
    ds__##name##_vector_delete(&self->buffer);\
    *self = (name) {0};\
}

/** Declares a mutable string of the given type. */
#define DECLARE_STRING(T) DECLARE_STRING_NAMED(T##_string, T)

/** Declares the default mutable string type. */
DECLARE_STRING_NAMED(string, char)

#endif // DS_STRING_H
