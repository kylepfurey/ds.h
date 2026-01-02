// .h
// Generic Mutable String Data Structure
// by Kyle Furey

/**
 * ds_string.h
 *
 * ds_DECLARE_STRING_NAMED(
 *      name,               - The name of the data structure and function prefix.
 *
 *      T,                  - The character type to generate this data structure with.
* )
 *
 * The "string" struct is automatically generated with the char type.
 *
 * Strings are widely used in C but reallocation can be painful and unsafe.
 * This type is a wrapper for a vector meant for string operations.
 * It is mutable, meaning memory is owned and the buffer can be modified.
 *
 * This is a safe choice for string builders, storing user input, or just general use.
 * The vector always ensures enough memory is available, the string is null-terminated,
 * and that the size is cached for fast calculations.
 *
 * * Returns a new string with <str> copied into its buffer.
 * * This data structure must be deleted with string_delete().
 *
 *   string           string_new              ( const char* str )
 *
 * * Returns a new string copied from <string>.
 * * The new string owns its own memory and must be deleted with string_delete().
 *
 *   string           string_copy             ( const string* str )
 *
 * * Returns the number of characters in the string.
 *
 *   size_t           string_length           ( const string* self )
 *
 * * Returns the current maximum number of characters that can be contained in the string.
 *
 *   size_t           string_capacity         ( const string* self )
 *
 * * Returns whether the string is empty.
 *
 *   bool             string_empty            ( const string* self )
 *
 * * Returns the character in the string at <index>.
 * * <index> must be a valid index.
 *
 *   char             string_get              ( const string* self, size_t index )
 *
 * * Sets the character in the string at <index>.
 * * If ds_VECTOR_TRUNC_ASSERT is true, this will assert if data is 0.
 * * <index> must be a valid index.
 * * Returns a pointer to the string's buffer.
 *
 *   const char*      string_set              ( string* self, size_t index, char data )
 *
 * * Returns a pointer to the string's buffer.
 *
 *   const char*      string_cstr             ( const string* self )
 *
 * * Mutates <self> to be the substring at <index> to <index> + <length>.
 * * <index> must be a valid index or the end.
 * * Returns a pointer to the string's buffer.
 *
 *   const char*      string_substr           ( string* self, size_t index, size_t length )
 *
 * * Returns the difference in values between the first non-identical bytes.
 * * Returns 0 if both strings are identical.
 *
 *   int              string_compare          ( const string* self, const char* str )
 *
 * * Resizes the vector to contain <length> number of characters.
 * * If ds_VECTOR_TRUNC_ASSERT is true, this will assert if characters are deleted.
 *
 *   void             string_resize           ( string* self, size_t length )
 *
 * * Inserts a copy of <str> into <self> at <index>.
 * * <index> must be a valid index.
 * * Returns a pointer to the string's buffer.
 *
 *   const char*      string_insert           ( string* self, size_t index, const char* str )
 *
 * * Deletes the characters at <index> to <index> + <length>.
 * * <index> must be a valid index.
 * * Returns a pointer to the string's buffer.
 *
 *   const char*      string_erase            ( string* self, size_t index, size_t length )
 *
 * * Copies <str> to the end of the string.
 * * Returns a pointer to the string's buffer.
 *
 *   const char*      string_append           ( string* self, const char* str )
 *
 * * Copies <str> to the beginning of the string.
 * * Returns a pointer to the string's buffer.
 *
 *   const char*      string_prepend          ( string* self, const char* str )
 *
 * * Returns the index of the first sequence of characters matching <str>.
 * * Returns ds_NOT_FOUND if no string was found.
 *
 *   size_t           string_find             ( const string* self, const char* str )
 *
 * * Returns the index of the last sequence of characters matching <str>.
 * * Returns ds_NOT_FOUND if no string was found.
 *
 *   size_t           string_find_last        ( const string* self, const char* str )
 *
 * * Returns whether the string contains a copy of <str>.
 *
 *   bool             string_contains         ( const string* self, const char* str )
 *
 * * Replaces the first sequence of characters matching <find> with a copy of <replace>.
 * * Returns a pointer to the string's buffer.
 *
 *   const char*      string_replace_first    ( string* self, const char* find, const char* replace )
 *
 * * Replaces the last sequence of characters matching <find> with a copy of <replace>.
 * * Returns a pointer to the string's buffer.
 *
 *   const char*      string_replace_last     ( string* self, const char* find, const char* replace )
 *
 * * Replaces all sequences of characters matching <find> with a copy of <replace>.
 * * Returns a pointer to the string's buffer.
 *
 *   const char*      string_replace_all      ( string* self, const char* find, const char* replace )
 *
 * * Reverses the string.
 * * Returns a pointer to the string's buffer.
 *
 *   const char*      string_reverse          ( string* self )
 *
 * * Converts the string to lowercase.
 * * Returns a pointer to the string's buffer.
 *
 *   const char*      string_lower            ( string* self )
 *
 * * Converts the string to uppercase.
 * * Returns a pointer to the string's buffer.
 *
 *   const char*      string_upper            ( string* self )
 *
 * * Trims whitespace at the beginning and end of the string.
 * * If <resize> is true, the string is resized exactly to its minimum length.
 * * Returns a pointer to the string's buffer.
 *
 *   const char*      string_trim             ( string* self, bool resize )
 *
 * * Deletes all characters in a string.
 *
 *   void             string_clear            ( string* self )
 *
 * * Iterates and mutates the string.
 * * This replaces each character after calling <transform>.
 * * If ds_VECTOR_TRUNC_ASSERT is true, this will assert if <transform> returns 0.
 * * Returns a pointer to the string's buffer.
 *
 *   const char*      string_map              ( string* self, char(*transform)(char) )
 *
 * * Iterates and mutates the string.
 * * This deletes characters that return false in <predicate>.
 * * Returns the new size of the string.
 *
 *   size_t           string_filter           ( string* self, bool(*predicate)(char) )
 *
 * * Iterates the string and computes a character.
 * * Each character is "applied" to <start> using <accumulator>:
 * * foreach char => start = accumulator(start, char)
 * * Returns the accumulated character.
 *
 *   char             string_reduce           ( string* self, char start, char(*accumulator)(char, char) )
 *
 * * Iterates the vector calling <action> on each character.
 *
 *   void             string_foreach          ( const string* self, void(*action)(char) )
 *
 * * Safely deletes a string.
 *
 *   void             string_delete           ( string* self )
 */

#ifndef DS_STRING_H
#define DS_STRING_H

#include "ds_vector.h"

/** Declares a named mutable string of the given type. */
#define ds_DECLARE_STRING_NAMED(name, T)\
\
ds_API static inline ds_size ds__##name##_strlen(const T *str) {\
    ds_assert(str != ds_NULL);\
    ds_size length = 0;\
    while (*str != 0) {\
        ++length;\
        ++str;\
    }\
    return length;\
}\
\
ds_API static inline ds_int ds__##name##_strcmp(const T *left, const T *right) {\
    ds_assert(left != ds_NULL);\
    ds_assert(right != ds_NULL);\
    while (*left != 0 && *right != 0 && (*left == *right)) {\
        left++;\
        right++;\
    }\
    return (ds_int) ((ds_uint) *left - (ds_uint) *right);\
}\
\
ds_API static inline ds_int ds__##name##_strncmp(const T *left, const T *right, ds_size length) {\
    ds_assert(left != ds_NULL);\
    ds_assert(right != ds_NULL);\
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
ds_DECLARE_VECTOR_NAMED(ds__##name##_vector, T, ds_void_deleter)\
\
typedef struct {\
    ds__##name##_vector buffer;\
} name;\
\
ds_API static inline name name##_new(const T *str) {\
    ds_assert(str != ds_NULL);\
    ds_size length = ds__##name##_strlen(str);\
    name self = (name) {\
        ds__##name##_vector_new(length + 1),\
    };\
    ds_memcpy(self.buffer.array, str, sizeof(T) * (length + 1));\
    self.buffer.count = length;\
    return self;\
}\
\
ds_API static inline name name##_copy(const name *str) {\
    ds_assert(str != ds_NULL);\
    return (name) {\
        ds__##name##_vector_copy(&str->buffer),\
    };\
}\
\
ds_API static inline ds_size name##_length(const name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    return self->buffer.count;\
}\
\
ds_API static inline ds_size name##_capacity(const name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    return self->buffer.capacity;\
}\
\
ds_API static inline ds_bool name##_empty(const name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    return self->buffer.count == 0;\
}\
\
ds_API static inline T name##_get(const name *self, ds_size index) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != ds_NULL);\
    ds_assert(index < self->buffer.count);\
    return self->buffer.array[index];\
}\
\
ds_API static inline const T *name##_set(name *self, ds_size index, T data) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != ds_NULL);\
    ds_assert(index < self->buffer.count);\
    if (ds_VECTOR_TRUNC_ASSERT) {\
        ds_assert(data != 0);\
    }\
    self->buffer.array[index] = data;\
    if (data == 0) {\
        self->buffer.count = index;\
    }\
    return self->buffer.array;\
}\
\
ds_API static inline const T *name##_cstr(const name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != ds_NULL);\
    return self->buffer.array;\
}\
\
ds_API static inline const T *name##_substr(name *self, ds_size index, ds_size length) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(index <= self->buffer.count);\
    ds_assert(self->buffer.array != ds_NULL);\
    if (length == 0) {\
        self->buffer.count = 0;\
        self->buffer.array[0] = 0;\
        return self->buffer.array;\
    }\
    if (index + length > self->buffer.count) {\
        length = self->buffer.count - index;\
    }\
    if (index > 0) {\
        ds_memmove(self->buffer.array, self->buffer.array + index, sizeof(T) * length);\
    }\
    self->buffer.count = length;\
    self->buffer.array[length] = 0;\
    ds_assert(self->buffer.array[self->buffer.count] == 0);\
    return self->buffer.array;\
}\
\
ds_API static inline ds_int name##_compare(const name *self, const T *str) {\
    ds_assert(self != ds_NULL);\
    ds_assert(str != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != ds_NULL);\
    return ds__##name##_strcmp(self->buffer.array, str);\
}\
\
ds_API static inline void name##_resize(name *self, ds_size length) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != ds_NULL);\
    if (ds_VECTOR_TRUNC_ASSERT) {\
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
ds_API static inline const T *name##_insert(name *self, ds_size index, const T *str) {\
    ds_assert(self != ds_NULL);\
    ds_assert(str != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(index <= self->buffer.count);\
    ds_assert(self->buffer.array != ds_NULL);\
    ds_size length = ds__##name##_strlen(str);\
    if (self->buffer.count + length + 1 > self->buffer.capacity) {\
        ds__##name##_vector_resize(&self->buffer, self->buffer.count + length + 1);\
    }\
    ds_memmove(\
        self->buffer.array + index + length,\
        self->buffer.array + index,\
        sizeof(T) * (self->buffer.count - index + 1)\
    );\
    ds_memmove(self->buffer.array + index, str, sizeof(T) * length);\
    self->buffer.count += length;\
    ds_assert(self->buffer.array[self->buffer.count] == 0);\
    return self->buffer.array;\
}\
\
ds_API static inline const T *name##_erase(name *self, ds_size index, ds_size length) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(index < self->buffer.count);\
    ds_assert(self->buffer.array != ds_NULL);\
    if (length == 0) {\
        return self->buffer.array;\
    }\
    if (index + length > self->buffer.count) {\
        length = self->buffer.count - index;\
    }\
    ds_memmove(\
        self->buffer.array + index,\
        self->buffer.array + index + length,\
        sizeof(T) * (self->buffer.count - index - length + 1)\
    );\
    self->buffer.count -= length;\
    ds_assert(self->buffer.array[self->buffer.count] == 0);\
    return self->buffer.array;\
}\
\
ds_API static inline const T *name##_append(name *self, const T *str) {\
    ds_assert(self != ds_NULL);\
    ds_assert(str != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    return name##_insert(self, self->buffer.count, str);\
}\
\
ds_API static inline const T *name##_prepend(name *self, const T *str) {\
    ds_assert(self != ds_NULL);\
    ds_assert(str != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    return name##_insert(self, 0, str);\
}\
\
ds_API static inline ds_size name##_find(const name *self, const T *str) {\
    ds_assert(self != ds_NULL);\
    ds_assert(str != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != ds_NULL);\
    ds_size length = ds__##name##_strlen(str);\
    if (length == 0 || length > self->buffer.count) {\
        return ds_NOT_FOUND;\
    }\
    for (ds_size i = 0; i + length <= self->buffer.count; ++i) {\
        if (ds__##name##_strncmp(self->buffer.array + i, str, length) == 0) {\
            return i;\
        }\
    }\
    return ds_NOT_FOUND;\
}\
\
ds_API static inline ds_size name##_find_last(const name *self, const T *str) {\
    ds_assert(self != ds_NULL);\
    ds_assert(str != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != ds_NULL);\
    ds_size length = ds__##name##_strlen(str);\
    if (length == 0 || length > self->buffer.count) {\
        return ds_NOT_FOUND;\
    }\
    for (ds_diff i = self->buffer.count - length + 1; i-- > 0;) {\
        if (ds__##name##_strncmp(self->buffer.array + i, str, length) == 0) {\
            return i;\
        }\
    }\
    return ds_NOT_FOUND;\
}\
\
ds_API static inline ds_bool name##_contains(const name *self, const T *str) {\
    ds_assert(self != ds_NULL);\
    ds_assert(str != ds_NULL);\
    return name##_find(self, str) != ds_NOT_FOUND;\
}\
\
ds_API static inline const T *name##_replace_first(name *self, const T *find, const T *replace) {\
    ds_assert(self != ds_NULL);\
    ds_assert(find != ds_NULL);\
    ds_assert(replace != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != ds_NULL);\
    ds_size index = name##_find(self, find);\
    if (index == ds_NOT_FOUND) {\
        return self->buffer.array;\
    }\
    name##_erase(self, index, ds__##name##_strlen(find));\
    name##_insert(self, index, replace);\
    ds_assert(self->buffer.array[self->buffer.count] == 0);\
    return self->buffer.array;\
}\
\
ds_API static inline const T *name##_replace_last(name *self, const T *find, const T *replace) {\
    ds_assert(self != ds_NULL);\
    ds_assert(find != ds_NULL);\
    ds_assert(replace != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != ds_NULL);\
    ds_size index = name##_find_last(self, find);\
    if (index == ds_NOT_FOUND) {\
        return self->buffer.array;\
    }\
    name##_erase(self, index, ds__##name##_strlen(find));\
    name##_insert(self, index, replace);\
    ds_assert(self->buffer.array[self->buffer.count] == 0);\
    return self->buffer.array;\
}\
\
ds_API static inline const T *name##_replace_all(name *self, const T *find, const T *replace) {\
    ds_assert(self != ds_NULL);\
    ds_assert(find != ds_NULL);\
    ds_assert(replace != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != ds_NULL);\
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
        if (index == ds_NOT_FOUND) {\
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
ds_API static inline const T *name##_reverse(name *self) {\
    ds_assert(self != ds_NULL);\
    return ds__##name##_vector_reverse(&self->buffer);\
}\
\
ds_API static inline const T *name##_lower(name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != ds_NULL);\
    for (ds_size i = 0; i < self->buffer.count; ++i) {\
        self->buffer.array[i] = (T) ds_tolower((ds_int) self->buffer.array[i]);\
    }\
    return self->buffer.array;\
}\
\
ds_API static inline const T *name##_upper(name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != ds_NULL);\
    for (ds_size i = 0; i < self->buffer.count; ++i) {\
        self->buffer.array[i] = (T) ds_toupper((ds_int) self->buffer.array[i]);\
    }\
    return self->buffer.array;\
}\
\
ds_API static inline const T *name##_trim(name *self, ds_bool resize) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != ds_NULL);\
    ds_size start = 0;\
    ds_size end = self->buffer.count;\
    while (start < end && ds_isspace((ds_int) self->buffer.array[start])) {\
        ++start;\
    }\
    while (end > start && ds_isspace((ds_int) self->buffer.array[end - 1])) {\
        --end;\
    }\
    ds_size length = end - start;\
    if (start > 0 && length > 0) {\
        ds_memmove(self->buffer.array, self->buffer.array + start, sizeof(T) * (length));\
    }\
    self->buffer.count = length;\
    self->buffer.array[length] = 0;\
    if (resize) {\
        ds__##name##_vector_resize(&self->buffer, length + 1);\
    }\
    return self->buffer.array;\
}\
\
ds_API static inline void name##_clear(name *self) {\
    ds_assert(self != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != ds_NULL);\
    self->buffer.count = 0;\
    self->buffer.array[0] = 0;\
}\
\
ds_API static inline const T *name##_map(name *self, T(*transform)(T)) {\
    ds_assert(self != ds_NULL);\
    ds_assert(transform != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != ds_NULL);\
    for (ds_size i = 0; i < self->buffer.count; ++i) {\
        self->buffer.array[i] = transform(self->buffer.array[i]);\
        if (ds_VECTOR_TRUNC_ASSERT) {\
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
ds_API static inline ds_size name##_filter(name *self, ds_bool(*predicate)(T)) {\
    ds_assert(self != ds_NULL);\
    ds_assert(predicate != ds_NULL);\
    ds_assert(self->buffer.count < self->buffer.capacity);\
    ds_assert(self->buffer.array != ds_NULL);\
    ds_size count = ds__##name##_vector_filter(&self->buffer, predicate);\
    self->buffer.array[count] = 0;\
    return count;\
}\
\
ds_API static inline T name##_reduce(const name *self, T start, T(*accumulator)(T, T)) {\
    ds_assert(self != ds_NULL);\
    ds_assert(accumulator != ds_NULL);\
    return ds__##name##_vector_reduce(&self->buffer, start, accumulator);\
}\
\
ds_API static inline void name##_foreach(const name *self, void(*action)(T)) {\
    ds_assert(self != ds_NULL);\
    ds_assert(action != ds_NULL);\
    ds__##name##_vector_foreach(&self->buffer, action);\
}\
\
ds_API static inline void name##_delete(name *self) {\
    ds_assert(self != ds_NULL);\
    ds__##name##_vector_delete(&self->buffer);\
    *self = (name) {0};\
}

/** Declares a mutable string of the given type. */
#define ds_DECLARE_STRING(T)\
        ds_DECLARE_STRING_NAMED(T##_string, T)

/** Declares the default mutable char string type. */
ds_DECLARE_STRING_NAMED(string, char)

#endif // DS_STRING_H
