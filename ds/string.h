// .h
// Generic Mutable String Data Structure
// by Kyle Furey

#ifndef DS_STRING_H
#define DS_STRING_H

#include "vector.h"

/** An index indicating something was not found. */
#define NOT_FOUND ((size_t) -1)

/** Declares a named mutable string of the given type. */
#define DECLARE_STRING_NAMED(name, T)\
\
static inline size_t __##name##_strlen(const T *str) {\
    assert(str != NULL);\
    size_t length = 0;\
    while (*str != 0) {\
        ++length;\
        ++str;\
    }\
    return length;\
}\
\
static inline int __##name##_strcmp(const T *left, const T *right) {\
    assert(left != NULL);\
    assert(right != NULL);\
    while (*left != 0 && *right != 0 && (*left == *right)) {\
        left++;\
        right++;\
    }\
    return (int) ((unsigned int) *left - (unsigned int) *right);\
}\
\
DECLARE_VECTOR_NAMED(__##name##_vector, T, void_deleter)\
\
typedef struct {\
    __##name##_vector buffer;\
} name;\
\
static inline name name##_new(const T *str) {\
    assert(str != NULL);\
    size_t length = __##name##_strlen(str);\
    name self = (name) {\
        __##name##_vector_new(length + 1),\
    };\
    memcpy(self.buffer.array, str, sizeof(T) * length + 1);\
    self.buffer.count = length;\
    return self;\
}\
\
static inline name name##_copy(const name *str) {\
    assert(str != NULL);\
    return (name) {\
        __##name##_vector_copy(&str->buffer),\
    };\
}\
\
static inline size_t name##_length(const name *self) {\
    assert(self != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    return self->buffer.count;\
}\
\
static inline size_t name##_capacity(const name *self) {\
    assert(self != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    return self->buffer.capacity;\
}\
\
static inline bool name##_empty(const name *self) {\
    assert(self != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    return self->buffer.count == 0;\
}\
\
static inline T name##_get(const name *self, size_t index) {\
    assert(self != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    assert(self->buffer.array != NULL);\
    assert(index < self->buffer.count);\
    return self->buffer.array[index];\
}\
\
static inline const T *name##_cstr(const name *self) {\
    assert(self != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    assert(self->buffer.array != NULL);\
    return self->buffer.array;\
}\
\
static inline const T *name##_substr(name *self, size_t start, size_t length) {\
    assert(self != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    assert(start <= self->buffer.count);\
    assert(start + length <= self->buffer.count);\
    assert(self->buffer.array != NULL);\
    if (length == 0) {\
        self->buffer.count = 0;\
        self->buffer.array[0] = '\0';\
        return self->buffer.array;\
    }\
    if (start > 0) {\
        memmove(self->buffer.array, self->buffer.array + start, length);\
    }\
    self->buffer.count = length;\
    self->buffer.array[length] = '\0';\
    assert(self->buffer.array[self->buffer.count] == '\0');\
    return self->buffer.array;\
}\
\
static inline int name##_compare(const name *self, const T *str) {\
    assert(self != NULL);\
    assert(str != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    assert(self->buffer.array != NULL);\
    return __##name##_strcmp(self->buffer.array, str);\
}\
\
static inline void name##_resize(name *self, size_t length) {\
    assert(self != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    assert(self->buffer.array != NULL);\
    if (length + 1 > self->buffer.capacity) {\
        __##name##_vector_resize(&self->buffer, length + 1);\
    } else if (length < self->buffer.count) {\
        self->buffer.count = length;\
        self->buffer.array[length] = '\0';\
    }\
    assert(self->buffer.array[self->buffer.count] == '\0');\
}\
\
static inline const T *name##_insert(name *self, size_t index, const T *str) {\
    assert(self != NULL);\
    assert(str != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    assert(index <= self->buffer.count);\
    assert(self->buffer.array != NULL);\
    size_t length = __##name##_strlen(str);\
    if (self->buffer.count + length + 1 > self->buffer.capacity) {\
        __##name##_vector_resize(&self->buffer, self->buffer.count + length + 1);\
    }\
    memmove(\
        self->buffer.array + index + length,\
        self->buffer.array + index,\
        self->buffer.count - index + 1\
    );\
    memmove(self->buffer.array + index, str, length);\
    self->buffer.count += length;\
    assert(self->buffer.array[self->buffer.count] == '\0');\
    return self->buffer.array;\
}\
\
static inline const T *name##_erase(name *self, size_t index, size_t length) {\
    assert(self != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    assert(index < self->buffer.count);\
    assert(self->buffer.array != NULL);\
    if (length == 0) {\
        return self->buffer.array;\
    }\
    if (index + length > self->buffer.count) {\
        length = self->buffer.count - index;\
    }\
    memmove(\
        self->buffer.array + index,\
        self->buffer.array + index + length,\
        self->buffer.count - index - length + 1\
    );\
    self->buffer.count -= length;\
    assert(self->buffer.array[self->buffer.count] == '\0');\
    return self->buffer.array;\
}\
\
static inline const T *name##_append(name *self, const T *str) {\
    assert(self != NULL);\
    assert(str != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    return name##_insert(self, self->buffer.count, str);\
}\
\
static inline const T *name##_prepend(name *self, const T *str) {\
    assert(self != NULL);\
    assert(str != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    return name##_insert(self, 0, str);\
}\
\
static inline size_t name##_find(const name *self, const T *str) {\
    assert(self != NULL);\
    assert(str != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    assert(self->buffer.array != NULL);\
    size_t length = __##name##_strlen(str);\
    if (length == 0 || length > self->buffer.count) {\
        return NOT_FOUND;\
    }\
    for (size_t i = 0; i + length <= self->buffer.count; ++i) {\
        if (__##name##_strcmp(self->buffer.array + i, str) == 0) {\
            return i;\
        }\
    }\
    return NOT_FOUND;\
}\
\
static inline size_t name##_find_last(const name *self, const T *str) {\
    assert(self != NULL);\
    assert(str != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    assert(self->buffer.array != NULL);\
    size_t length = __##name##_strlen(str);\
    if (length == 0 || length > self->buffer.count) {\
        return NOT_FOUND;\
    }\
    for (ptrdiff_t i = self->buffer.count - length + 1; i-- > 0;) {\
        if (__##name##_strcmp(self->buffer.array + i, str) == 0) {\
            return i;\
        }\
    }\
    return NOT_FOUND;\
}\
\
static inline bool name##_contains(const name *self, const T *str) {\
    assert(self != NULL);\
    assert(str != NULL);\
    return name##_find(self, str) != NOT_FOUND;\
}\
\
static inline const T *name##_replace_first(name *self, const T *find, const T *replace) {\
    assert(self != NULL);\
    assert(find != NULL);\
    assert(replace != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    assert(self->buffer.array != NULL);\
    size_t index = name##_find(self, find);\
    if (index == NOT_FOUND) {\
        return self->buffer.array;\
    }\
    name##_erase(self, index, __##name##_strlen(find));\
    name##_insert(self, index, replace);\
    assert(self->buffer.array[self->buffer.count] == '\0');\
    return self->buffer.array;\
}\
\
static inline const T *name##_replace_last(name *self, const T *find, const T *replace) {\
    assert(self != NULL);\
    assert(find != NULL);\
    assert(replace != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    assert(self->buffer.array != NULL);\
    size_t index = name##_find_last(self, find);\
    if (index == NOT_FOUND) {\
        return self->buffer.array;\
    }\
    name##_erase(self, index, __##name##_strlen(find));\
    name##_insert(self, index, replace);\
    assert(self->buffer.array[self->buffer.count] == '\0');\
    return self->buffer.array;\
}\
\
static inline const T *name##_replace_all(name *self, const T *find, const T *replace) {\
    assert(self != NULL);\
    assert(find != NULL);\
    assert(replace != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    assert(self->buffer.array != NULL);\
    size_t find_len = __##name##_strlen(find);\
    size_t replace_len = __##name##_strlen(replace);\
    if (find_len == 0) {\
        return self->buffer.array;\
    }\
    size_t start = 0;\
    while (start <= self->buffer.count - find_len) {\
        self->buffer.array += start;\
        self->buffer.count -= start;\
        size_t index = name##_find(self, find);\
        self->buffer.array -= start;\
        self->buffer.count += start;\
        if (index == NOT_FOUND) {\
            break;\
        }\
        index += start;\
        name##_erase(self, index, find_len);\
        name##_insert(self, index, replace);\
        start = index + replace_len;\
    }\
    assert(self->buffer.array[self->buffer.count] == '\0');\
    return self->buffer.array;\
}\
\
static inline const T *name##_reverse(name *self) {\
    assert(self != NULL);\
    return __##name##_vector_reverse(&self->buffer);\
}\
\
static inline const T *name##_upper(name *self) {\
    assert(self != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    assert(self->buffer.array != NULL);\
    for (size_t i = 0; i < self->buffer.count; ++i) {\
        self->buffer.array[i] = (T) toupper((int) self->buffer.array[i]);\
    }\
    return self->buffer.array;\
}\
\
static inline const T *name##_lower(name *self) {\
    assert(self != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    assert(self->buffer.array != NULL);\
    for (size_t i = 0; i < self->buffer.count; ++i) {\
        self->buffer.array[i] = (T) tolower((int) self->buffer.array[i]);\
    }\
    return self->buffer.array;\
}\
\
static inline const T *name##_trim(name *self) {\
    assert(self != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    assert(self->buffer.array != NULL);\
    size_t start = 0;\
    size_t end = self->buffer.count;\
    while (start < end && isspace((int) self->buffer.array[start])) {\
        ++start;\
    }\
    while (end > start && isspace((int) self->buffer.array[end - 1])) {\
        --end;\
    }\
    size_t length = end - start;\
    if (start > 0 && length > 0) {\
        memmove(self->buffer.array, self->buffer.array + start, length);\
    }\
    self->buffer.count = length;\
    self->buffer.array[length] = '\0';\
    __##name##_vector_resize(&self->buffer, length + 1);\
    return self->buffer.array;\
}\
\
static inline void name##_clear(name *self) {\
    assert(self != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    assert(self->buffer.array != NULL);\
    self->buffer.count = 0;\
    self->buffer.array[0] = '\0';\
}\
\
static inline const T *name##_map(name *self, T(*transform)(T)) {\
    assert(self != NULL);\
    assert(transform != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    assert(self->buffer.array != NULL);\
    for (size_t i = 0; i < self->buffer.count; ++i) {\
        self->buffer.array[i] = transform(self->buffer.array[i]);\
        if (self->buffer.array[i] == '\0') {\
            self->buffer.count = i;\
            break;\
        }\
    }\
    return self->buffer.array;\
}\
\
static inline size_t name##_filter(name *self, bool(*predicate)(T)) {\
    assert(self != NULL);\
    assert(predicate != NULL);\
    assert(self->buffer.count < self->buffer.capacity);\
    assert(self->buffer.array != NULL);\
    size_t count = __##name##_vector_filter(&self->buffer, predicate);\
    self->buffer.array[count] = '\0';\
    return count;\
}\
\
static inline T name##_reduce(name *self, T start, T(*accumulator)(T, T)) {\
    assert(self != NULL);\
    assert(accumulator != NULL);\
    return __##name##_vector_reduce(&self->buffer, start, accumulator);\
}\
\
static inline void name##_foreach(const name *self, void(*action)(T)) {\
    assert(self != NULL);\
    assert(action != NULL);\
    __##name##_vector_foreach(&self->buffer, action);\
}\
\
static inline void name##_free(name *self) {\
    assert(self != NULL);\
    __##name##_vector_free(&self->buffer);\
}

/** Declares a mutable string of the given type. */
#define DECLARE_STRING(T) DECLARE_STRING_NAMED(T##_string, T)

/** Declares the default char string type. */
DECLARE_STRING_NAMED(string, char)

#endif // DS_STRING_H
