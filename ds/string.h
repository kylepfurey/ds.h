// .h
// Dynamic name Data Structure
// by Kyle Furey

#ifndef DS_STRING_H
#define DS_STRING_H

#include "vector.h"

/** Declares a named dynamic name of the given type. */
#define DECLARE_STRING_NAMED(name, T)\
\
DECLARE_VECTOR_NAMED(__##name##_vector, T, void_deleter)\
\
typedef struct {\
    __##name##_vector vector;\
} name;\
\
static inline name name##_new(const T *str) {\
    assert(str != NULL);\
    size_t length = strlen(str);\
    name self = (name) { __##name##_vector_new(length + 1), };\
    memmove(self.vector.array, str, sizeof(T) * length + 1);\
    self.vector.count = length;\
    return self;\
}\
\
static inline name name##_copy(const name *str) {\
    assert(str != NULL);\
    assert(str->vector.count < str->vector.capacity);\
    name self = (name) { __##name##_vector_new(str->vector.count + 1), };\
    memmove(self.vector.array, str->vector.array, str->vector.count + 1);\
    self.vector.count = str->vector.count;\
    return self;\
}\
\
static inline size_t name##_length(const name *self) {\
    assert(self != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    return self->vector.count;\
}\
\
static inline size_t name##_capacity(const name *self) {\
    assert(self != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    return self->vector.capacity;\
}\
\
static inline bool name##_empty(const name *self) {\
    assert(self != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    return self->vector.count == 0;\
}\
\
static inline T name##_get(const name *self, size_t index) {\
    assert(self != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    assert(self->vector.array != NULL);\
    assert(index < self->vector.count);\
    return self->vector.array[index];\
}\
\
static inline const T *name##_cstr(const name *self) {\
    assert(self != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    assert(self->vector.array != NULL);\
    return self->vector.array;\
}\
\
static inline const T *name##_substr(name *self, size_t start, size_t length) {\
    assert(self != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    assert(start <= self->vector.count);\
    assert(start + length <= self->vector.count);\
    assert(self->vector.array != NULL);\
    if (length == 0) {\
        self->vector.count = 0;\
        self->vector.array[0] = '\0';\
        return self->vector.array;\
    }\
    if (start > 0) {\
        memmove(self->vector.array, self->vector.array + start, length);\
    }\
    self->vector.count = length;\
    self->vector.array[length] = '\0';\
    assert(self->vector.array[self->vector.count] == '\0');\
    return self->vector.array;\
}\
\
static inline int name##_compare(const name *self, const T *str) {\
    assert(self != NULL);\
    assert(str != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    assert(self->vector.array != NULL);\
    return strcmp(self->vector.array, str);\
}\
\
static inline void name##_resize(name *self, size_t length) {\
    assert(self != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    assert(self->vector.array != NULL);\
    if (length + 1 > self->vector.capacity) {\
        __##name##_vector_resize(&self->vector, length + 1);\
    } else if (length < self->vector.count) {\
        self->vector.count = length;\
        self->vector.array[length] = '\0';\
    }\
    assert(self->vector.array[self->vector.count] == '\0');\
}\
\
static inline const T *name##_insert(name *self, size_t index, const T *str) {\
    assert(self != NULL);\
    assert(str != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    assert(index <= self->vector.count);\
    assert(self->vector.array != NULL);\
    size_t length = strlen(str);\
    if (self->vector.count + length + 1 > self->vector.capacity) {\
        __##name##_vector_resize(&self->vector, self->vector.count + length + 1);\
    }\
    memmove(\
        self->vector.array + index + length,\
        self->vector.array + index,\
        self->vector.count - index + 1\
    );\
    memmove(self->vector.array + index, str, length);\
    self->vector.count += length;\
    assert(self->vector.array[self->vector.count] == '\0');\
    return self->vector.array;\
}\
\
static inline const T *name##_erase(name *self, size_t index, size_t length) {\
    assert(self != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    assert(index < self->vector.count);\
    assert(self->vector.array != NULL);\
    if (length == 0) {\
        return self->vector.array;\
    }\
    if (index + length > self->vector.count) {\
        length = self->vector.count - index;\
    }\
    memmove(\
        self->vector.array + index,\
        self->vector.array + index + length,\
        self->vector.count - index - length + 1\
    );\
    self->vector.count -= length;\
    assert(self->vector.array[self->vector.count] == '\0');\
    return self->vector.array;\
}\
\
static inline const T *name##_append(name *self, const T *str) {\
    assert(self != NULL);\
    assert(str != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    return name##_insert(self, self->vector.count, str);\
}\
\
static inline const T *name##_prepend(name *self, const T *str) {\
    assert(self != NULL);\
    assert(str != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    return name##_insert(self, 0, str);\
}\
\
static inline size_t name##_find(const name *self, const T *str) {\
    assert(self != NULL);\
    assert(str != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    assert(self->vector.array != NULL);\
    size_t length = strlen(str);\
    if (length == 0 || length > self->vector.count) {\
        return NOT_FOUND;\
    }\
    for (size_t i = 0; i + length <= self->vector.count; ++i) {\
        if (strncmp(self->vector.array + i, str, length) == 0) {\
            return i;\
        }\
    }\
    return NOT_FOUND;\
}\
\
static inline size_t name##_find_last(const name *self, const T *str) {\
    assert(self != NULL);\
    assert(str != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    assert(self->vector.array != NULL);\
    size_t length = strlen(str);\
    if (length == 0 || length > self->vector.count) {\
        return NOT_FOUND;\
    }\
    for (size_t i = self->vector.count - length + 1; i-- > 0;) {\
        if (strncmp(self->vector.array + i, str, length) == 0) {\
            return i;\
        }\
    }\
    return NOT_FOUND;\
}\
\
static inline T *name##_replace_first(name *self, const T *find, const T *replace) {\
    assert(self != NULL);\
    assert(find != NULL);\
    assert(replace != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    assert(self->vector.array != NULL);\
    size_t index = name##_find(self, find);\
    if (index == NOT_FOUND) {\
        return self->vector.array;\
    }\
    name##_erase(self, index, strlen(find));\
    name##_insert(self, index, replace);\
    assert(self->vector.array[self->vector.count] == '\0');\
    return self->vector.array;\
}\
\
static inline T *name##_replace_last(name *self, const T *find, const T *replace) {\
    assert(self != NULL);\
    assert(find != NULL);\
    assert(replace != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    assert(self->vector.array != NULL);\
    size_t index = name##_find_last(self, find);\
    if (index == NOT_FOUND) {\
        return self->vector.array;\
    }\
    name##_erase(self, index, strlen(find));\
    name##_insert(self, index, replace);\
    assert(self->vector.array[self->vector.count] == '\0');\
    return self->vector.array;\
}\
\
static inline T *name##_replace_all(name *self, const T *find, const T *replace) {\
    assert(self != NULL);\
    assert(find != NULL);\
    assert(replace != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    assert(self->vector.array != NULL);\
    size_t find_len = strlen(find);\
    size_t replace_len = strlen(replace);\
    if (find_len == 0) {\
        return self->vector.array;\
    }\
    size_t start = 0;\
    while (start <= self->vector.count - find_len) {\
        self->vector.array += start;\
        self->vector.count -= start;\
        size_t index = name##_find(self, find);\
        self->vector.array -= start;\
        self->vector.count += start;\
        if (index == NOT_FOUND) {\
            break;\
        }\
        index += start;\
        name##_erase(self, index, find_len);\
        name##_insert(self, index, replace);\
        start = index + replace_len;\
    }\
    assert(self->vector.array[self->vector.count] == '\0');\
    return self->vector.array;\
}\
\
static inline const T *name##_reverse(name *self) {\
    assert(self != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    assert(self->vector.array != NULL);\
    __##name##_vector_reverse(&self->vector);\
    return self->vector.array;\
}\
\
static inline T *name##_upper(name *self) {\
    assert(self != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    assert(self->vector.array != NULL);\
    for (size_t i = 0; i < self->vector.count; ++i) {\
        self->vector.array[i] = toupper(self->vector.array[i]);\
    }\
    return self->vector.array;\
}\
\
static inline T *name##_lower(name *self) {\
    assert(self != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    assert(self->vector.array != NULL);\
    for (size_t i = 0; i < self->vector.count; ++i) {\
        self->vector.array[i] = tolower(self->vector.array[i]);\
    }\
    return self->vector.array;\
}\
\
static inline T *name##_trim(name *self) {\
    assert(self != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    assert(self->vector.array != NULL);\
    size_t start = 0;\
    size_t end = self->vector.count;\
    while (start < end && isspace(self->vector.array[start])) {\
        ++start;\
    }\
    while (end > start && isspace(self->vector.array[end - 1])) {\
        --end;\
    }\
    size_t length = end - start;\
    if (start > 0 && length > 0) {\
        memmove(self->vector.array, self->vector.array + start, length);\
    }\
    self->vector.count = length;\
    self->vector.array[length] = '\0';\
    __##name##_vector_resize(&self->vector, length + 1);\
    return self->vector.array;\
}\
\
static inline void name##_clear(name *self) {\
    assert(self != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    assert(self->vector.array != NULL);\
    self->vector.count = 0;\
    self->vector.array[0] = '\0';\
}\
\
static inline bool name##_foreach(const name *self, bool (*action)(T)) {\
    assert(self != NULL);\
    assert(action != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    assert(self->vector.array != NULL);\
    for (size_t i = 0; i < self->vector.count; ++i) {\
        if (!action(self->vector.array[i])) {\
            return false;\
        }\
    }\
    return true;\
}\
\
static inline const T *name##_transform(name *self, T (*transform)(T)) {\
    assert(self != NULL);\
    assert(transform != NULL);\
    assert(self->vector.count < self->vector.capacity);\
    assert(self->vector.array != NULL);\
    for (size_t i = 0; i < self->vector.count; ++i) {\
        self->vector.array[i] = transform(self->vector.array[i]);\
    }\
    return self->vector.array;\
}\
\
static inline void name##_free(name *self) {\
    assert(self != NULL);\
    __##name##_vector_free(&self->vector);\
}

/** Declares a dynamic name of the given type. */
#define DECLARE_STRING(T) DECLARE_VECTOR_NAMED(T##_string, T)

/** Declares the default char string type. */
DECLARE_STRING_NAMED(string, char)

#endif // DS_STRING_H
