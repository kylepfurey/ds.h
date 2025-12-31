// .h
// Generic Sorted Binary Tree Set Data Structure
// by Kyle Furey

/**
 * ds_set.h
 *
 * ds_DECLARE_SET_NAMED(
 *      name,               - The name of the data structure and function prefix.
 *
 *      T,                  - The type to generate this data structure with.
 *
 *      x_y_comparer,       - Inline comparison code used to compare values <x> and <y>.
 *                            You can use ds_DEFAULT_COMPARE for trivial types.
 *
 *      x_y_equals,         - Inline comparison code used to equate values <x> and <y>.
 *                            You can use ds_DEFAULT_EQUALS for trivial types.
 *
 *      deleter,            - The name of the function used to deallocate T.
 *                            ds_void_deleter may be used for trivial types.
 * )
 *
 * This is a sorted binary tree that stores a collection of unique elements.
 * Elements are inserted relative to their neighbors to ensure order is kept.
 * Sets are great for storing and checking whether an element exists.
 * Sets can grow large and imbalanced depending on insertion order.
 *
 * Sets are great for storing unique values. New elements replace "identical" elements on insert.
 * Mathematical operations like subset and union allow easy set comparison and combination.
 *
 * * Returns a new set.
 * * This data structure must be deleted with set_delete().
 *
 *   set          set_new             ( void )
 *
 * * Returns a new set shallow copied from <set>.
 * * This data structure must be deleted with set_delete().
 *
 *   set          set_copy            ( const set* set )
 *
 * * Returns the number of elements in the set.
 *
 *   size_t       set_count           ( const set* self )
 *
 * * Returns whether the set is empty.
 *
 *   bool         set_empty           ( const set* self )
 *
 * * Returns a pointer to the least value in the set.
 * * The set must not be empty.
 *
 *   const T*     set_least           ( const set* self )
 *
 * * Returns a pointer to the greatest value in the set.
 * * The set must not be empty.
 *
 *   const T*     set_greatest        ( const set* self )
 *
 * * Returns a pointer to a value that matches <data> in the set.
 * * Returns NULL if no value matches.
 *
 *   const T*     set_find            ( const set* self, T data )
 *
 * * Returns whether the set contains <data>.
 *
 *   bool         set_contains        ( const set* self, T data )
 *
 * * Inserts a new element into the set.
 * * Returns whether a value was overwritten.
 *
 *   bool         set_insert          ( set* self, T data )
 *
 * * Deletes an element in the set.
 * * Returns whether an element was deleted.
 *
 *   bool         set_erase           ( set* self, T data )
 *
 * * Returns whether <self> is a subset of <set>.
 * * <or_equal> determines whether set equality returns true.
 *
 *   bool         set_subset          ( const set* self, const set* set, bool or_equal )
 *
 * * Mutates <self> by inserting all elements in <set> into <self>.
 * * Returns <self>.
 *
 *   set*         set_union           ( set* self, const set* set )
 *
 * * Mutates <self> by removing elements from <self> not present in <set>.
 * * Returns <self>.
 *
 *   set*         set_intersect       ( set* self, const set* set )
 *
 * * Mutates <self> by removing elements from <self> present in <set>.
 * * Returns <self>.
 *
 *   set*         set_difference      ( set* self, const set* set )
 *
 * * Deletes all elements in the set.
 *
 *   void         set_clear           ( set* self )
 *
 * * Iterates the set in-order calling <action> on each element.
 *
 *   void         set_foreach         ( const set* self, void (*action)(T) )
 *
 * * Safely deletes a set.
 *
 *   void         set_delete          ( set* self )
 */

#ifndef DS_SET_H
#define DS_SET_H

#include "ds_def.h"

/** Declares a named sorted binary tree set of the given type. */
#define ds_DECLARE_SET_NAMED(name, T, x_y_comparer, x_y_equals, deleter)\
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
ds_API static inline name name##_new() {\
    return (name) {\
        0,\
        NULL,\
    };\
}\
\
ds_API static inline ds_size name##_count(const name *self) {\
    ds_assert(self != NULL);\
    return self->count;\
}\
\
ds_API static inline ds_bool name##_empty(const name *self) {\
    ds_assert(self != NULL);\
    return self->count == 0;\
}\
\
ds_API static inline const T *name##_least(const name *self) {\
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
ds_API static inline const T *name##_greatest(const name *self) {\
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
ds_API static inline const T *name##_find(const name *self, T data) {\
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
ds_API static inline ds_bool name##_contains(const name *self, T data) {\
    ds_assert(self != NULL);\
    return name##_find(self, data) != NULL;\
}\
\
ds_API static inline ds_bool name##_insert(name *self, T data) {\
    ds_assert(self != NULL);\
    if (self->root == NULL) {\
        ds_assert(self->count == 0);\
        ++self->count;\
        ds__##name##_node *node = (ds__##name##_node *) ds_calloc(1, sizeof(ds__##name##_node));\
        ds_assert(node != NULL);\
        node->data = data;\
        self->root = node;\
        return ds_false;\
    }\
    ds_assert(self->count > 0);\
    ds__##name##_node *current = self->root;\
    while (ds_true) {\
        T x = data;\
        T y = current->data;\
        if ((x_y_equals)) {\
            deleter(&current->data);\
            current->data = data;\
            return ds_true;\
        }\
        if ((x_y_comparer)) {\
            if (current->right == NULL) {\
                ++self->count;\
                ds__##name##_node *node = (ds__##name##_node *) ds_calloc(1, sizeof(ds__##name##_node));\
                ds_assert(node != NULL);\
                current->right = node;\
                node->data = data;\
                return ds_false;\
            }\
            current = current->right;\
        } else {\
            if (current->left == NULL) {\
                ++self->count;\
                ds__##name##_node *node = (ds__##name##_node *) ds_calloc(1, sizeof(ds__##name##_node));\
                ds_assert(node != NULL);\
                current->left = node;\
                node->data = data;\
                return ds_false;\
            }\
            current = current->left;\
        }\
    }\
}\
\
ds_API static inline void ds__##name##_copy(name *self, const ds__##name##_node *node) {\
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
ds_API static inline name name##_copy(const name *set) {\
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
ds_API static inline ds_bool name##_erase(name *self, T data) {\
    ds_assert(self != NULL);\
    if (self->root == NULL) {\
        ds_assert(self->count == 0);\
        return ds_false;\
    }\
    ds_assert(self->count > 0);\
    ds__##name##_node *parent = NULL;\
    ds__##name##_node *current = self->root;\
    while (ds_true) {\
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
            return ds_true;\
        }\
        parent = current;\
        if ((x_y_comparer)) {\
            if (current->right == NULL) {\
                return ds_false;\
            }\
            current = current->right;\
        } else {\
            if (current->left == NULL) {\
                return ds_false;\
            }\
            current = current->left;\
        }\
    }\
}\
\
ds_API static inline ds_bool ds__##name##_subset(const name *set, const ds__##name##_node *node) {\
    ds_assert(set != NULL && node != NULL);\
    if (node->left != NULL) {\
        if (!ds__##name##_subset(set, node->left)) {\
            return ds_false;\
        }\
    }\
    if (!name##_contains(set, node->data)) {\
        return ds_false;\
    }\
    if (node->right != NULL) {\
        if (!ds__##name##_subset(set, node->right)) {\
            return ds_false;\
        }\
    }\
    return ds_true;\
}\
\
ds_API static inline ds_bool name##_subset(const name *self, const name *set, ds_bool or_equal) {\
    ds_assert(self != NULL);\
    ds_assert(set != NULL);\
    if (self->root == NULL) {\
        return ds_true;\
    }\
    return ds__##name##_subset(set, self->root) && (or_equal || (self->count != set->count));\
}\
\
ds_API static inline void ds__##name##_union(name *self, const ds__##name##_node *node) {\
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
ds_API static inline name *name##_union(name *self, const name *set) {\
    ds_assert(self != NULL);\
    ds_assert(set != NULL);\
    if (set->root != NULL) {\
        ds__##name##_union(self, set->root);\
    }\
    return self;\
}\
\
ds_API static inline void ds__##name##_intersect(name *self, ds__##name##_node *node, const name *set) {\
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
ds_API static inline name *name##_intersect(name *self, const name *set) {\
    ds_assert(self != NULL);\
    ds_assert(set != NULL);\
    if (self->root != NULL) {\
        ds__##name##_intersect(self, self->root, set);\
    }\
    return self;\
}\
\
ds_API static inline void ds__##name##_difference(name *self, const ds__##name##_node *node) {\
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
ds_API static inline name *name##_difference(name *self, const name *set) {\
    ds_assert(self != NULL);\
    ds_assert(set != NULL);\
    if (set->root != NULL) {\
        ds__##name##_difference(self, set->root);\
    }\
    return self;\
}\
\
ds_API static inline void ds__##name##_clear(ds__##name##_node *node) {\
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
ds_API static inline void name##_clear(name *self) {\
    ds_assert(self != NULL);\
    ds_assert((self->root == NULL) == (self->count == 0));\
    if (self->root != NULL) {\
        ds__##name##_clear(self->root);\
    }\
    self->count = 0;\
    self->root = NULL;\
}\
\
ds_API static inline void ds__##name##_foreach(const ds__##name##_node *node, void(*action)(T)) {\
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
ds_API static inline void name##_foreach(const name *self, void(*action)(T)) {\
    ds_assert(self != NULL);\
    ds_assert(action != NULL);\
    ds_assert((self->root == NULL) == (self->count == 0));\
    if (self->root != NULL) {\
        ds__##name##_foreach(self->root, action);\
    }\
}\
\
ds_API static inline void name##_delete(name *self) {\
    ds_assert(self != NULL);\
    name##_clear(self);\
    *self = (name) {0};\
}

/** Declares a sorted binary tree set of the given type. */
#define ds_DECLARE_SET(T, x_y_comparer, x_y_equals, deleter)\
        ds_DECLARE_SET_NAMED(T##_set, T, x_y_comparer, x_y_equals, deleter)

#endif // DS_SET_H
