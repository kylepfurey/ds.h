// .h
// Generic Sorted Set Data Structure
// by Kyle Furey

/**
 *
 */

#ifndef DS_SET_H
#define DS_SET_H

#include "ds_def.h"

/** Declares a named sorted set of the given type. */
#define DECLARE_SET_NAMED(name, T, x_y_comparer, x_y_equals, deleter)\
\
typedef struct ds__##name##_node {\
    T data;\
    struct ds__##name##_node *left;\
    struct ds__##name##_node *right;\
} ds__##name##_node;\
\
typedef struct {\
    ds_size count;\
    ds__##name##_node *root;\
} name;\
\
static inline name name##_new() {\
    return (name) {\
        0,\
        NULL,\
    };\
}\
\
static inline ds_size name##_count(const name *self) {\
    ds_assert(self != NULL);\
    return self->count;\
}\
\
static inline bool name##_empty(const name *self) {\
    ds_assert(self != NULL);\
    return self->count == 0;\
}\
\
static inline const T *name##_least(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->count > 0);\
    ds_assert(self->root != NULL);\
    const ds__##name##_node *current = self->root;\
    while (current->left != NULL) {\
        current = current->left;\
    }\
    return &current->data;\
}\
\
static inline const T *name##_greatest(const name *self) {\
    ds_assert(self != NULL);\
    ds_assert(self->count > 0);\
    ds_assert(self->root != NULL);\
    const ds__##name##_node *current = self->root;\
    while (current->right != NULL) {\
        current = current->right;\
    }\
    return &current->data;\
}\
\
static inline const T *name##_find(const name *self, T data) {\
    ds_assert(self != NULL);\
    const ds__##name##_node *current = self->root;\
    ds_assert((current == NULL) == (self->count == 0));\
    while (current != NULL) {\
        T x = data;\
        T y = current->data;\
        if ((x_y_equals)) {\
            return &current->data;\
        }\
        if ((x_y_comparer)) {\
            current = current->right;\
        } else {\
            current = current->left;\
        }\
    }\
    return NULL;\
}\
\
static inline bool name##_contains(const name *self, T data) {\
    ds_assert(self != NULL);\
    return name##_find(self, data) != NULL;\
}\
\
static inline bool name##_insert(name *self, T data) {\
    ds_assert(self != NULL);\
    if (self->root == NULL) {\
        ds_assert(self->count == 0);\
        ++self->count;\
        ds__##name##_node *node = (ds__##name##_node *) ds_calloc(1, sizeof(ds__##name##_node));\
        ds_assert(node != NULL);\
        node->data = data;\
        self->root = node;\
        return false;\
    }\
    ds_assert(self->count > 0);\
    ds__##name##_node *current = self->root;\
    while (true) {\
        T x = data;\
        T y = current->data;\
        if ((x_y_equals)) {\
            deleter(&current->data);\
            current->data = data;\
            return true;\
        }\
        if ((x_y_comparer)) {\
            if (current->right == NULL) {\
                ++self->count;\
                ds__##name##_node *node = (ds__##name##_node *) ds_calloc(1, sizeof(ds__##name##_node));\
                ds_assert(node != NULL);\
                current->right = node;\
                node->data = data;\
                return false;\
            }\
            current = current->right;\
        } else {\
            if (current->left == NULL) {\
                ++self->count;\
                ds__##name##_node *node = (ds__##name##_node *) ds_calloc(1, sizeof(ds__##name##_node));\
                ds_assert(node != NULL);\
                current->left = node;\
                node->data = data;\
                return false;\
            }\
            current = current->left;\
        }\
    }\
}\
\
static inline void ds__##name##_copy(name *self, const ds__##name##_node *node) {\
    ds_assert(self != NULL && node != NULL);\
    if (node->left != NULL) {\
        ds__##name##_copy(self, node->left);\
    }\
    if (node->right != NULL) {\
        ds__##name##_copy(self, node->right);\
    }\
    name##_insert(self, node->data);\
}\
\
static inline name name##_copy(const name *set) {\
    ds_assert(set != NULL);\
    ds_assert((set->root == NULL) == (set->count == 0));\
    name self = (name) {\
        0,\
        NULL,\
    };\
    if (set->root != NULL) {\
        ds__##name##_copy(&self, set->root);\
    }\
    ds_assert(self.count == set->count);\
    return self;\
}\
\
static inline bool name##_erase(name *self, T data) {\
    ds_assert(self != NULL);\
    if (self->root == NULL) {\
        ds_assert(self->count == 0);\
        return false;\
    }\
    ds_assert(self->count > 0);\
    ds__##name##_node *parent = NULL;\
    ds__##name##_node *current = self->root;\
    while (true) {\
        T x = data;\
        T y = current->data;\
        if ((x_y_equals)) {\
            --self->count;\
            ds__##name##_node *replace_parent = current;\
            ds__##name##_node *replace = current->left;\
            if (replace != NULL) {\
                while (replace->right != NULL) {\
                    replace_parent = replace;\
                    replace = replace->right;\
                }\
                if (replace_parent == current) {\
                    replace_parent->left = replace->left;\
                } else {\
                    replace_parent->right = replace->left;\
                }\
                if (current->left != replace) {\
                    replace->left = current->left;\
                }\
                replace->right = current->right;\
            } else {\
                replace = current->right;\
            }\
            if (parent == NULL) {\
                self->root = replace;\
            } else {\
                if (parent->left == current) {\
                    parent->left = replace;\
                } else {\
                    parent->right = replace;\
                }\
            }\
            deleter(&current->data);\
            ds_free(current);\
            return true;\
        }\
        parent = current;\
        if ((x_y_comparer)) {\
            if (current->right == NULL) {\
                return false;\
            }\
            current = current->right;\
        } else {\
            if (current->left == NULL) {\
                return false;\
            }\
            current = current->left;\
        }\
    }\
}\
\
static inline bool ds__##name##_subset(const name *set, const ds__##name##_node *node) {\
    ds_assert(set != NULL && node != NULL);\
    if (node->left != NULL) {\
        if (!ds__##name##_subset(set, node->left)) {\
            return false;\
        }\
    }\
    if (!name##_contains(set, node->data)) {\
        return false;\
    }\
    if (node->right != NULL) {\
        if (!ds__##name##_subset(set, node->right)) {\
            return false;\
        }\
    }\
    return true;\
}\
\
static inline bool name##_subset(const name *self, const name *set, bool or_equal) {\
    ds_assert(self != NULL);\
    ds_assert(set != NULL);\
    if (self->root == NULL) {\
        return true;\
    }\
    return ds__##name##_subset(set, self->root) && (or_equal || (self->count != set->count));\
}\
\
static inline void ds__##name##_union(name *self, const ds__##name##_node *node) {\
    ds_assert(self != NULL && node != NULL);\
    if (node->left != NULL) {\
        ds__##name##_union(self, node->left);\
    }\
    name##_insert(self, node->data);\
    if (node->right != NULL) {\
        ds__##name##_union(self, node->right);\
    }\
}\
\
static inline name *name##_union(name *self, const name *set) {\
    ds_assert(self != NULL);\
    ds_assert(set != NULL);\
    if (set->root != NULL) {\
        ds__##name##_union(self, set->root);\
    }\
    return self;\
}\
\
static inline void ds__##name##_intersect(name *self, ds__##name##_node *node, const name *set) {\
    ds_assert(self != NULL && node != NULL && set != NULL);\
    if (node->left != NULL) {\
        ds__##name##_intersect(self, node->left, set);\
    }\
    if (node->right != NULL) {\
        ds__##name##_intersect(self, node->right, set);\
    }\
    if (!name##_contains(set, node->data)) {\
        name##_erase(self, node->data);\
    }\
}\
\
static inline name *name##_intersect(name *self, const name *set) {\
    ds_assert(self != NULL);\
    ds_assert(set != NULL);\
    if (self->root != NULL) {\
        ds__##name##_intersect(self, self->root, set);\
    }\
    return self;\
}\
\
static inline void ds__##name##_difference(name *self, const ds__##name##_node *node) {\
    ds_assert(self != NULL && node != NULL);\
    if (node->left != NULL) {\
        ds__##name##_difference(self, node->left);\
    }\
    name##_erase(self, node->data);\
    if (node->right != NULL) {\
        ds__##name##_difference(self, node->right);\
    }\
}\
\
static inline name *name##_difference(name *self, const name *set) {\
    ds_assert(self != NULL);\
    ds_assert(set != NULL);\
    if (set->root != NULL) {\
        ds__##name##_difference(self, set->root);\
    }\
    return self;\
}\
\
static inline void ds__##name##_clear(ds__##name##_node *node) {\
    ds_assert(node != NULL);\
    if (node->left != NULL) {\
        ds__##name##_clear(node->left);\
    }\
    ds__##name##_node *right = node->right;\
    deleter(&node->data);\
    ds_free(node);\
    if (right != NULL) {\
        ds__##name##_clear(right);\
    }\
}\
\
static inline void name##_clear(name *self) {\
    ds_assert(self != NULL);\
    ds_assert((self->root == NULL) == (self->count == 0));\
    if (self->root != NULL) {\
        ds__##name##_clear(self->root);\
    }\
    self->count = 0;\
    self->root = NULL;\
}\
\
static inline void ds__##name##_foreach(const ds__##name##_node *node, void(*action)(T)) {\
    ds_assert(node != NULL && action != NULL);\
    if (node->left != NULL) {\
        ds__##name##_foreach(node->left, action);\
    }\
    action(node->data);\
    if (node->right != NULL) {\
        ds__##name##_foreach(node->right, action);\
    }\
}\
\
static inline void name##_foreach(const name *self, void(*action)(T)) {\
    ds_assert(self != NULL);\
    ds_assert(action != NULL);\
    ds_assert((self->root == NULL) == (self->count == 0));\
    if (self->root != NULL) {\
        ds__##name##_foreach(self->root, action);\
    }\
}\
\
static inline void name##_delete(name *self) {\
    ds_assert(self != NULL);\
    name##_clear(self);\
    *self = (name) {0};\
}

/** Declares a sorted set of the given type. */
#define DECLARE_SET(T, x_y_comparer, x_y_equals, deleter) DECLARE_SET_NAMED(T##_set, T, x_y_comparer, x_y_equals, deleter)

#endif // DS_SET_H
