# ds.h

This is a generic header-only C99 core data structure library. Each header file in the ds folder provide macros for generating type-safe data structures. Each data structure's API is documented in a top-level comment and in this README file below.

## Table of Contents

0.  [Caveats](#caveats)
1.  [Block Allocator](#ds_arenah)
2.  [Dynamic Array](#ds_vectorh)
3.  [Mutable String](#ds_stringh)
4.  [Doubly Linked List](#ds_listh)
5.  [Double-Ended Priority Queue](#ds_queueh)
6.  [Sorted Binary Tree Set](#ds_seth)
7.  [Key-Value Hash Map](#ds_maph)
8.  [Unique Reference](#ds_uniqueh)
9.  [Shared Reference](#ds_sharedh)
10. [Weak Reference](#ds_weakh)
11. [Slab Allocator](#ds_slabh)
12. [Multicast Signal](#ds_signalh)

## Caveats

This library, while simple, is not without its faults. There are some key notes to go over before jumping in and using it.

1. This is a single-threaded library. There is no intention for this library to support concurrency. There are likely ways to extend this library to be thread-safe, but it was not built with multithreading as a priority.

2. Asserts are everywhere in this library to catch errors as soon as possible. This is to ensure invariants are maintained and that functions are used as expected by the library. If you are having trouble with assertions, you can expand + format the macro to find the exact spot where your code breaks. As with any assert, NDEBUG will make asserts a no-op. Read the documentation to ensure the API is being followed as intended, or just change it yourself.

3. This library implements the most common data structures in each language, but they are not guaranteed to be the most efficient. These data structures are for general use and can certainly be optimized, but each implementation is simple and aims to be as readable as possible despite the nature of macros. If you need an efficient data structure library, I would recommend using [EASTL](https://github.com/electronicarts/EASTL) in C++.

4. This library by default requires the C standard library, but all dependencies are wrapped in the [ds_def.h](ds/ds_def.h) header. It can easily be modified for customization or embedded programming by changing a few macros.

## [ds_arena.h](ds/ds_arena.h)

```c
ds_DECLARE_ARENA_NAMED(
     name,               - The name of the data structure and function prefix.
     alignment,          - The alignment of allocated arena memory.
)
```

The "arena" struct is automatically generated with the default word size for alignment.

This is a simple block allocator. It allocates a buffer and is divided per allocation.
This allocator is type-agnostic and operates with raw memory to create new objects.
Allocations are made at runtime using `arena_malloc()`.

This is a fantastic alternative to `malloc()` if you want deterministic heap memory.

Returns a new arena with at least `<size>` bytes of aligned memory.
This data structure must be deleted with `arena_delete()`.

```c
arena              arena_new            ( size_t size )
```

Allocates at least `<size>` number of the bytes.
Returns a new pointer or `NULL`.

```c
void*              arena_malloc         ( arena* self, size_t size )
```

Allocates a `<count>` * `<size>` array of zeroed memory.
Returns a new pointer or `NULL`.

```c
void*              arena_calloc         ( arena* self, size_t count, size_t size )
```

Reallocates `<ptr>` to have at least `<size>` bytes.
Returns a new pointer or `NULL`.

```c
void*              arena_realloc        ( arena* self, void* ptr, ds_size size )
```

Frees `<ptr>` from the arena so it may be reused again.
`<ptr>` can be `NULL`.

```c
void               arena_free           ( arena* self, void* ptr )
```

Safely deletes an arena.
If `ds_ARENA_LEAK_ASSERT` is true, this will assert if the arena leaked memory.

```c
void               arena_delete         ( arena* self )
```

## [ds_vector.h](ds/ds_vector.h)

```c
ds_DECLARE_VECTOR_NAMED(
     name,               - The name of the data structure and function prefix.
     T,                  - The type to generate this data structure with.
     deleter,            - The name of the function used to deallocate T.
                           ds_void_deleter may be used for trivial types.
)
```

Vectors are dynamic arrays. They own an expanding buffer to hold new elements.
Vectors also cache their size and capacity for runtime checks when accessing memory.

Vectors are excellent general-use data structures for storing multiple elements.
Indexing into vectors provide the fastest random access to their elements.
Inserting and removing into the back of a vector is also very fast.

Returns a new vector with a current capacity of `<capacity>` elements.
`<capacity>` must be greater than `0`.
This data structure must be deleted with `vector_delete()`.

```c
vector             vector_new           ( size_t capacity )
```

Returns a new vector shallow copied from `<vector>`.
This data structure must be deleted with `vector_delete()`.

```c
vector             vector_copy          ( const vector* vector )
```

Returns the number of elements in the vector.

```c
size_t             vector_count         ( const vector* self )
```

Returns the current maximum number of elements that can be contained in the vector.

```c
size_t             vector_capacity      ( const vector* self )
```

Returns whether the vector is empty.

```c
bool               vector_empty         ( const vector* self )
```

Returns a pointer to the element in the vector at `<index>`.
`<index>` must be a valid index.

```c
T*                 vector_get           ( vector* self, size_t index )
```

Returns a pointer to the element in the vector at `<index>`.
`<index>` must be a valid index.

```c
const T*           vector_get_const     ( const vector* self, size_t index )
```

Returns a pointer to the vector's array.

```c
T*                 vector_array         ( vector* self )
```

Returns a pointer to the vector's array.

```c
const T*           vector_array_const   ( const vector* self )
```

Resizes the vector to contain `<capacity>` number of elements.
If `ds_VECTOR_TRUNC_ASSERT` is true, this will assert if elements are deleted.

```c
void               vector_resize        ( vector* self, size_t capacity )
```

Inserts `<data>` at `<index>` into the vector.
`<index>` must be a valid index or the end of the vector.

```c
void               vector_insert        ( vector* self, size_t index, T data )
```

Erases the element at `<index>` from the vector.
`<index>` must be a valid index.

```c
void               vector_erase         ( vector* self, size_t index )
```

Inserts `<data>` at the end of the vector.

```c
void               vector_push          ( vector* self, T data )
```

Deletes the element at the end of the vector.
The vector must not be empty.

```c
void               vector_pop           ( vector* self )
```

Reverses and returns the vector.

```c
T*                 vector_reverse       ( vector* self )
```

Deletes all elements in a vector.

```c
void               vector_clear         ( vector* self )
```

Iterates and mutates the vector.
This replaces each element after calling `<transform>`.
Elements in the vector are not deleted, only updated.
Returns the vector's data.

```c
T*                 vector_map           ( vector* self, T(*transform)(T) )
```

Iterates and mutates the vector.
This deletes elements that return false in `<predicate>`.
Returns the new size of the vector.

```c
size_t             vector_filter        ( vector* self, bool(*predicate)(T) )
```

Iterates the vector and computes a value.
Each element is "applied" to `<start>` using `<accumulator>`:
`foreach elem => start = accumulator(start, elem)`
Returns the accumulated value.

```c
T                  vector_reduce        ( vector* self, T start, T(*accumulator)(T, T) )
```

Iterates the vector calling `<action>` on each element.

```c
void               vector_foreach       ( const vector* self, void(*action)(T) )
```

Safely deletes a vector.

```c
void               vector_delete        ( vector* self )
```

## [ds_string.h](ds/ds_string.h)

```c
ds_DECLARE_STRING_NAMED(
     name,               - The name of the data structure and function prefix.
     T,                  - The character type to generate this data structure with.
)
```

The "string" struct is automatically generated with the char type.

Strings are widely used in C but reallocation can be painful and unsafe.
This type is a wrapper for a vector meant for string operations.
It is mutable, meaning memory is owned and the buffer can be modified.

This is a safe choice for string builders, storing user input, or just general use.
The vector always ensures enough memory is available, the string is null-terminated,
and that the size is cached for fast calculations.

Returns a new string with `<str>` copied into its buffer.
This data structure must be deleted with `string_delete()`.

```c
string             string_new           ( const char* str )
```

Returns a new string shallow copied from `<string>`.
This data structure must be deleted with `string_delete()`.

```c
string             string_copy          ( const string* str )
```

Returns the number of characters in the string.

```c
size_t             string_length        ( const string* self )
```

Returns the current maximum number of characters that can be contained in the string.

```c
size_t             string_capacity      ( const string* self )
```

Returns whether the string is empty.

```c
bool               string_empty         ( const string* self )
```

Returns the character in the string at `<index>`.
`<index>` must be a valid index.

```c
char               string_get           ( const string* self, size_t index )
```

Sets the character in the string at `<index>`.
If `ds_VECTOR_TRUNC_ASSERT` is true, this will assert if data is `0`.
`<index>` must be a valid index.
Returns a pointer to the string's buffer.

```c
const char*        string_set           ( string* self, size_t index, char data )
```

Returns a pointer to the string's buffer.

```c
const char*        string_cstr          ( const string* self )
```

Mutates `<self>` to be the substring at `<index>` to `<index>` + `<length>`.
`<index>` must be a valid index or the end.
Returns a pointer to the string's buffer.

```c
const char*        string_substr        ( string* self, size_t index, size_t length )
```

Returns the difference in values between the first non-identical bytes.
Returns `0` if both strings are identical.

```c
int                string_compare       ( const string* self, const char* str )
```

Resizes the vector to contain `<length>` number of characters.
If `ds_VECTOR_TRUNC_ASSERT` is true, this will assert if characters are deleted.

```c
void               string_resize        ( string* self, size_t length )
```

Inserts a copy of `<str>` into `<self>` at `<index>`.
`<index>` must be a valid index.
Returns a pointer to the string's buffer.

```c
const char*        string_insert        ( string* self, size_t index, const char* str )
```

Deletes the characters at `<index>` to `<index>` + `<length>`.
`<index>` must be a valid index.
Returns a pointer to the string's buffer.

```c
const char*        string_erase         ( string* self, size_t index, size_t length )
```

Copies `<str>` to the end of the string.
Returns a pointer to the string's buffer.

```c
const char*        string_append        ( string* self, const char* str )
```

Copies `<str>` to the beginning of the string.
Returns a pointer to the string's buffer.

```c
const char*        string_prepend       ( string* self, const char* str )
```

Returns the index of the first sequence of characters matching `<str>`.
Returns `-1` if no string was found.

```c
size_t             string_find          ( const string* self, const char* str )
```

Returns the index of the last sequence of characters matching `<str>`.
Returns `-1` if no string was found.

```c
size_t             string_find_last     ( const string* self, const char* str )
```

Returns whether the string contains a copy of `<str>`.

```c
bool               string_contains      ( const string* self, const char* str )
```

Replaces the first sequence of characters matching `<find>` with a copy of `<replace>`.
Returns a pointer to the string's buffer.

```c
const char*        string_replace_first ( string* self, const char* find, const char* replace )
```

Replaces the last sequence of characters matching `<find>` with a copy of `<replace>`.
Returns a pointer to the string's buffer.

```c
const char*        string_replace_last  ( string* self, const char* find, const char* replace )
```

Replaces all sequences of characters matching `<find>` with a copy of `<replace>`.
Returns a pointer to the string's buffer.

```c
const char*        string_replace_all   ( string* self, const char* find, const char* replace )
```

Reverses the string.
Returns a pointer to the string's buffer.

```c
const char*        string_reverse       ( string* self )
```

Converts the string to lowercase.
Returns a pointer to the string's buffer.

```c
const char*        string_lower         ( string* self )
```

Converts the string to uppercase.
Returns a pointer to the string's buffer.

```c
const char*        string_upper         ( string* self )
```

Trims whitespace at the beginning and end of the string.
If `<resize>` is true, the string is resized exactly to its minimum length.
Returns a pointer to the string's buffer.

```c
const char*        string_trim          ( string* self, bool resize )
```

Deletes all characters in a string.

```c
void               string_clear         ( string* self )
```

Iterates and mutates the string.
This replaces each character after calling `<transform>`.
If `ds_VECTOR_TRUNC_ASSERT` is true, this will assert if `<transform>` returns `0`.
Returns a pointer to the string's buffer.

```c
const char*        string_map           ( string* self, char(*transform)(char) )
```

Iterates and mutates the string.
This deletes characters that return false in `<predicate>`.
Returns the new size of the string.

```c
size_t             string_filter        ( string* self, bool(*predicate)(char) )
```

Iterates the string and computes a character.
Each character is "applied" to `<start>` using `<accumulator>`:
`foreach char => start = accumulator(start, char)`
Returns the accumulated character.

```c
char               string_reduce        ( string* self, char start, char(*accumulator)(char, char) )
```

Iterates the vector calling `<action>` on each character.

```c
void               string_foreach       ( const string* self, void(*action)(char) )
```

Safely deletes a string.

```c
void               string_delete        ( string* self )
```

## [ds_list.h](ds/ds_list.h)

```c
ds_DECLARE_LIST_NAMED(
     name,           - The name of the data structure and function prefix.
     T,              - The type to generate this data structure with.
     deleter,        - The name of the function used to deallocate T.
                       ds_void_deleter may be used for trivial types.
)
```

This is a doubly linked list. Data is allocated non-contiguously with pointer links.
This is the only data structure to publicly expose its node type in this library.
This is intentional for O(1) ordered insertions and removals via a node pointer.

This is a great data structure for stacks, queues, and deques.
Insertion and deletion is fast with direct node references.
Iteration is slower than contiguous containers due to pointer chasing and cache misses.

This is the underlying list_node type. You must not modify its pointers.

```c
typedef struct {
     T                   data;
     list_node* const    previous;
     list_node* const    next;
} list_node;
```

Returns a new list.
This data structure must be deleted with `list_delete()`.

```c
list               list_new             ( void )
```

Returns a new list shallow copied from `<list>`.
This data structure must be deleted with `list_delete()`.

```c
list               list_copy            ( const list* list )
```

Returns the number of nodes in the list.

```c
size_t             list_count           ( const list* self )
```

Returns whether the list is empty.

```c
bool               list_empty           ( const list* self )
```

Returns the node at the front of the list.
The list must not be empty.

```c
list_node*         list_front           ( list* self )
```

Returns the node at the front of the list.
The list must not be empty.

```c
const list_node*   list_front_const     ( const list* self )
```

Returns the node at the end of the list.
The list must not be empty.

```c
list_node*         list_back            ( list* self )
```

Returns the node at the end of the list.
The list must not be empty.

```c
const list_node*   list_back_const      ( const list* self )
```

Traverses the linked list to find and return a node.
`<index>` must be valid.

```c
list_node*         list_get             ( list* self, size_t index )
```

Traverses the linked list to find and return a node.
`<index>` must be valid.

```c
const list_node*   list_get_const       ( const list* self, size_t index )
```

Prepends and returns a new node containing `<data>` before `<node>`.
`<node>` must be owned by list.

```c
list_node*         list_insert_before   ( list* self, list_node* node, T data )
```

Appends and returns a new node containing `<data>` after `<node>`.
`<node>` must be owned by list.

```c
list_node*         list_insert_after    ( list* self, list_node* node, T data )
```

Deletes a `<node>` from the list.
`<node>` must be owned by list.

```c
void               list_erase           ( list* self, list_node* node )
```

Prepends and returns a new node containing `<data>` to the front of the list.

```c
list_node*         list_push_front      ( list* self, T data )
```

Appends and returns a new node containing `<data>` at the end of the list.

```c
list_node*         list_push_back       ( list* self, T data )
```

Deletes the node at the front of the list.

```c
void               list_pop_front       ( list* self )
```

Deletes the node at the end of the list.

```c
void               list_pop_back        ( list* self )
```

Deletes all nodes in the list.

```c
void               list_clear           ( list* self )
```

Iterates the list calling `<action>` on each element.

```c
void               list_foreach         ( const list* self, void (*action)(T) )
```

Safely deletes a list.

```c
void               list_delete          ( list* self )
```

## [ds_queue.h](ds/ds_queue.h)

```c
ds_DECLARE_QUEUE_NAMED(
     name,               - The name of the data structure and function prefix.
     T,                  - The type to generate this data structure with.
     P,                  - The type used to sort T.
     x_y_comparer,       - Inline comparison code used to compare priorities <x> and <y>.
                           You can use ds_DEFAULT_COMPARE for trivial types.
     deleter,            - The name of the function used to deallocate T.
                           ds_void_deleter may be used for trivial types.
)
```

This is a double-ended priority queue. It's a linked list with automatic ordering.
Elements are sorted via a priority value. The first and last element are O(1) accessible.
If you don't need priority ordering, use a regular linked list or vector.

This is excellent for frontiers when doing small graph searches,
or collecting elements for processing in-order later.

Returns a new queue.
This data structure must be deleted with `queue_delete()`.

```c
queue              queue_new            ( void )
```

Returns a new queue shallow copied from `<queue>`.
This data structure must be deleted with `queue_delete()`.

```c
queue              queue_copy           ( const queue* queue )
```

Returns the number of elements in the queue.

```c
size_t             queue_count          ( const queue* self )
```

Returns whether the queue is empty.

```c
bool               queue_empty          ( const queue* self )
```

Returns a pointer to the element in the queue with the greatest priority.
The queue must not be empty.

```c
T*                 queue_first          ( queue* self )
```

Returns a pointer to the element in the queue with the greatest priority.
The queue must not be empty.

```c
const T*           queue_first_const    ( const queue* self )
```

Returns a pointer to the element in the queue with the least priority.
The queue must not be empty.

```c
T*                 queue_last           ( queue* self )
```

Returns a pointer to the element in the queue with the least priority.
The queue must not be empty.

```c
const T*           queue_last_const     ( const queue* self )
```

Adds a new element with `<priority>` into the queue.

```c
void               queue_push           ( queue* self, T data, P priority )
```

Removes the element in the queue with the greatest priority.
The queue must not be empty.

```c
void               queue_pop_first      ( queue* self )
```

Removes the element in the queue with the least priority.
The queue must not be empty.

```c
void               queue_pop_last       ( queue* self )
```

Deletes all elements in the queue.

```c
void               queue_clear          ( queue* self )
```

Iterates the queue calling `<action>` on each element.

```c
void               queue_foreach        ( const queue* self, void (*action)(T) )
```

Safely deletes a queue.

```c
void               queue_delete         ( queue* self )
```

## [ds_set.h](ds/ds_set.h)

```c
ds_DECLARE_SET_NAMED(
     name,               - The name of the data structure and function prefix.
     T,                  - The type to generate this data structure with.
     x_y_comparer,       - Inline comparison code used to compare values <x> and <y>.
                           You can use ds_DEFAULT_COMPARE for trivial types.
     x_y_equals,         - Inline comparison code used to equate values <x> and <y>.
                           You can use ds_DEFAULT_EQUALS for trivial types.
     deleter,            - The name of the function used to deallocate T.
                           ds_void_deleter may be used for trivial types.
)
```

This is a sorted binary tree that stores a collection of unique elements.
Elements are inserted relative to their neighbors to ensure order is kept.
Sets are great for storing and checking whether an element exists.
Sets can grow large and imbalanced depending on insertion order.

Sets are great for storing unique values. New elements replace "identical" elements on insert.
Mathematical operations like subset and union allow easy set comparison and combination.

Returns a new set.
This data structure must be deleted with `set_delete()`.

```c
set                set_new              ( void )
```

Returns a new set shallow copied from `<set>`.
This data structure must be deleted with `set_delete()`.

```c
set                set_copy             ( const set* set )
```

Returns the number of elements in the set.

```c
size_t             set_count            ( const set* self )
```

Returns whether the set is empty.

```c
bool               set_empty            ( const set* self )
```

Returns a pointer to the least value in the set.
The set must not be empty.

```c
const T*           set_least            ( const set* self )
```

Returns a pointer to the greatest value in the set.
The set must not be empty.

```c
const T*           set_greatest         ( const set* self )
```

Returns a pointer to a value that matches `<data>` in the set.
Returns `NULL` if no value matches.

```c
const T*           set_find             ( const set* self, T data )
```

Returns whether the set contains `<data>`.

```c
bool               set_contains         ( const set* self, T data )
```

Inserts a new element into the set.
Returns whether a value was overwritten.

```c
bool               set_insert           ( set* self, T data )
```

Deletes an element in the set.
Returns whether an element was deleted.

```c
bool               set_erase            ( set* self, T data )
```

Returns whether `<self>` is a subset of `<set>`.
`<or_equal>` determines whether set equality returns true.

```c
bool               set_subset           ( const set* self, const set* set, bool or_equal )
```

Mutates `<self>` by inserting all elements in `<set>` into `<self>`.
Returns `<self>`.

```c
set*               set_union            ( set* self, const set* set )
```

Mutates `<self>` by removing elements from `<self>` not present in `<set>`.
Returns `<self>`.

```c
set*               set_intersect        ( set* self, const set* set )
```

Mutates `<self>` by removing elements from `<self>` present in `<set>`.
Returns `<self>`.

```c
set*               set_difference       ( set* self, const set* set )
```

Deletes all elements in the set.

```c
void               set_clear            ( set* self )
```

Iterates the set in-order calling `<action>` on each element.

```c
void               set_foreach          ( const set* self, void (*action)(T) )
```

Safely deletes a set.

```c
void               set_delete           ( set* self )
```

## [ds_map.h](ds/ds_map.h)

```c
ds_DECLARE_MAP_NAMED(
     name,           - The name of the data structure and function prefix.
     K,              - The key type used to index V.
     V,              - The value type to generate this data structure with.
     key_hasher,     - Inline hashing code used to hash a key named <key>.
                       You can use ds_DEFAULT_HASH, ds_INT_HASH, ds_STRING_HASH, or a custom hasher.
     x_y_equals,     - Inline comparison code used to equate keys <x> and <y>.
                       You can use ds_DEFAULT_EQUALS for trivial types.
     value_deleter,  - The name of the function used to deallocate V.
                       ds_void_deleter may be used for trivial types.
)
```

This is a simple open addressing key-value hash map.
Values are stored in buckets in an underlying vector.
Values are indexed by hashing their key type into a number for near O(1) operations.
This number is used to probe for finding where an element may be quickly.

It's one of the fastest options for storing, finding, and removing key-value pairs.

Returns a new map with `<capacity>` number of buckets.
`<capacity>` must be greater than `0`.
This data structure must be deleted with `map_delete()`.

```c
map                map_new              ( size_t capacity )
```

Returns a new map shallow copied from `<map>`.
This data structure must be deleted with `map_delete()`.

```c
map                map_copy             ( const map* map )
```

Returns the number of elements in the map.

```c
size_t             map_count            ( const map* self )
```

Returns the number of the buckets in the map.

```c
size_t             map_capacity         ( const map* self )
```

Returns whether the map is empty.

```c
bool               map_empty            ( const map* self )
```

Returns a pointer to a value that matches `<key>` in the map.
Returns `NULL` if no key is found.

```c
V*                 map_find             ( map* self, K key )
```

Returns a pointer to a value that matches `<key>` in the map.
Returns `NULL` if no key is found.

```c
const V*           map_find_const       ( const map* self, K key )
```

Returns whether the map contains `<key>`.

```c
bool               map_contains         ( const map* self, K key )
```

Sets the number of buckets in the map.
This must not be less than the number of elements.

```c
void               map_resize           ( map* self, size_t capacity )
```

Inserts a new key-value pair into the map.
Returns whether a value was overwritten.

```c
bool               map_insert           ( map* self, K key, V value )
```

Deletes the value that matches `<key>`.
Returns whether `<key>` was found.

```c
bool               map_erase            ( map* self, K key )
```

Deletes all pairs in the map.

```c
void               map_clear            ( map* self )
```

Iterates the map calling `<action>` on each key and value.

```c
void               map_foreach          ( const map* self, void (*action)(K, V) )
```

Iterates the map calling `<action>` on each key.

```c
void               map_foreach_key      ( const map* self, void (*action)(K) )
```

Iterates the map calling `<action>` on each value.

```c
void               map_foreach_value    ( const map* self, void (*action)(V) )
```

Safely deletes a map.

```c
void               map_delete           ( map* self )
```

## [ds_unique.h](ds/ds_unique.h)

```c
ds_DECLARE_UNIQUE_NAMED(
     name,               - The name of the data structure and function prefix.
     T,                  - The type to generate this data structure with.
     deleter,            - The name of the function used to deallocate T.
                           ds_void_deleter may be used for trivial types.
)
```

This is a wrapper over heap memory that aims to explicitly state who owns it.
Unique references must always be freed at the end of the scope they are created.

This a cleaner way of indicating who owns memory over raw pointers.
The wrapper over the actual pointer leaves zero overhead with clear ownership.

Returns a new unique reference containing `<data>`.
This data structure must be deleted with `unique_delete()`.

```c
unique             unique_new           ( T data )
```

Returns a pointer to `<self>`'s data.

```c
T*                 unique_get           ( unique* self )
```

Returns a pointer to `<self>`'s data.

```c
const T*           unique_get_const     ( const unique* self )
```

Resets the value in `<self>` with `<data>`.

```c
void               unique_reset         ( unique* self, T data )
```

Safely deletes a unique reference.

```c
void               unique_delete        ( unique* self )
```

## [ds_shared.h](ds/ds_shared.h)

```c
ds_DECLARE_SHARED_NAMED(
     name,               - The name of the data structure and function prefix.
     T,                  - The type to generate this data structure with.
     deleter,            - The name of the function used to deallocate T.
                           ds_void_deleter may be used for trivial types.
)
```

This is a reference counted pointer that ensures memory is freed when shared_count is 0.
Shared references create slight indirection for ensuring shared ownership of memory.

When memory is accessed from multiple contexts in a program, a shared reference ensures
the memory is freed only after all owners are done with it.

Returns a new shared reference containing `<data>`.
This data structure must be deleted with `shared_delete()`.

```c
shared             shared_new           ( T data )
```

Returns a new shared reference with the same address as `<shared>`.
This data structure must be deleted with `shared_delete()`.

```c
shared             shared_copy          ( shared* shared )
```

Returns the number of shared references to `<self>`'s data.

```c
uint               shared_shared_count  ( const shared* self )
```

Returns the number of weak references to `<self>`'s data.

```c
uint               shared_weak_count    ( const shared* self )
```

Returns a pointer to `<self>`'s data.

```c
T*                 shared_get           ( shared* self )
```

Returns a pointer to `<self>`'s data.

```c
const T*           shared_get_const     ( const shared* self )
```

Resets the value in `<self>` with `<data>`.

```c
void               shared_reset         ( shared* self, T data )
```

Safely deletes a shared reference.

```c
void               shared_delete        ( shared* self )
```

## [ds_weak.h](ds/ds_weak.h)

```c
ds_DECLARE_WEAK_NAMED(
     name,               - The name of the data structure and function prefix.
     shared_name,        - The name of the shared reference data structure to extend.
)
```

Weak references are a counterpart to shared references that don't own the memory.
These references act as a view into the memory only when needed. They have their own weak_count.
Weak references are "weak" because the memory may already be deleted by shared references.

While shared references are good for distributing pointers across multiple contexts,
weak references are great for managing memory internally while allowing external
parties to have temporary access to it. This is great for breaking reference cycles.

Returns a new weak reference from `<shared>`.
This data structure must be deleted with `weak_delete()`.

```c
weak               weak_new             ( shared* shared )
```

Returns a new weak reference with the same address as `<weak>`.
This data structure must be deleted with `weak_delete()`.

```c
weak               weak_copy            ( weak* weak )
```

Returns the number of shared references to `<self>`'s data.

```c
uint               weak_shared_count    ( const weak* self )
```

Returns the number of weak references to `<self>`'s data.

```c
uint               weak_weak_count      ( const weak* self )
```

Returns whether the weak reference is still valid.

```c
bool               weak_valid           ( const weak* self )
```

Returns a new shared reference with the same address as `<self>`.
`<self>` must be valid.
This data structure must be deleted with `shared_delete()`.

```c
shared             weak_upgrade         ( weak* self )
```

Safely deletes a weak reference.

```c
void               weak_delete          ( weak* self )
```

## [ds_slab.h](ds/ds_slab.h)

```c
ds_DECLARE_SLAB_NAMED(
     name,               - The name of the data structure and function prefix.
     T,                  - The type to generate this data structure with.
     deleter,            - The name of the function used to deallocate T.
                           ds_void_deleter may be used for trivial types.
)
```

This is a slab allocator. It is a mapping of IDs to same-size objects in a buffer.
Slab allocation is useful for reusing memory when large objects are being reallocated.

Lightweight IDs ensure O(1) access to the memory and a way to quickly return it.
Borrowing has the potential to resize the buffer, so use IDs to refresh pointers often.

Returns a new slab with a current capacity of `<capacity>` objects.
`<capacity>` must be greater than `0`.
This data structure must be deleted with `slab_delete()`.

```c
slab               slab_new             ( size_t capacity )
```

Returns a new slab shallow copied from `<slab>`.
This data structure must be deleted with `slab_delete()`.

```c
slab               slab_copy            ( const slab* slab )
```

Returns the number of objects in the slab.

```c
size_t             slab_count           ( const slab* self )
```

Returns the current maximum number of objects that can be contained in the slab.

```c
size_t             slab_capacity        ( const slab* self )
```

Returns whether the slab is empty.

```c
bool               slab_empty           ( const slab* self )
```

Returns whether `<id>` points to a valid object.

```c
bool               slab_valid           ( const slab* self, slab_id id )
```

Returns a pointer to an object with `<id>`.
`<id>` must be a valid ID.

```c
T*                 slab_get             ( slab* self, slab_id id )
```

Returns a pointer to an object with `<id>`.
`<id>` must be a valid ID.

```c
const T*           slab_get_const       ( const slab* self, slab_id id )
```

Allocates a new object in the slab with `<data>`.
This may resize the buckets and invalidate pointers, so store the ID.
Returns the objects new ID.

```c
slab_id            slab_borrow          ( slab* self, T data )
```

Frees the memory for the object with `<id>`.
`<id>` must be a valid ID.

```c
void               slab_return          ( slab* self, slab_id id )
```

Deletes all objects in a slab.

```c
void               slab_clear           ( slab* self )
```

Iterates the slab calling `<action>` on each object.

```c
void               slab_foreach         ( const slab* self, void(*action)(T) )
```

Safely deletes a slab.

```c
void               slab_delete          ( slab* self )
```

## [ds_signal.h](ds/ds_signal.h)

```c
ds_DECLARE_SIGNAL_NAMED(
     name,               - The name of the data structure and function prefix.
     T,                  - The type to generate this data structure with.
                           A pointer to T is always the first argument of the function signature.
     R,                  - The return type of the function signature.
     A...,               - Optionally any argument types of the function signature.
)
```

Signals are collections of bindings of objects to functions.
These functions share the same signature and the signal can call each function at once.
The signal will pass the same arguments to each function so each object is updated.

This is excellent for the observer pattern. Binding provides a handle for unbinding.
Objects can opaquely bind to an event and be notified when the event is triggered.
Objects must unbind themselves on destruction to avoid invalid memory access on invoke.

signal_func is an alias for a pointer to the function signature.

```c
typedef R (*)(T*, A...) signal_func;
```

Returns a new signal with a current capacity of `<capacity>` bindings.
`<capacity>` must be greater than `0`.
This data structure must be deleted with `signal_delete()`.

```c
signal             signal_new           ( size_t capacity )
```

Returns a new signal shallow copied from `<signal>`.
This data structure must be deleted with `signal_delete()`.

```c
signal             signal_copy          ( const signal* signal )
```

Returns the current number of bindings in the signal.

```c
size_t             signal_count         ( const signal* self )
```

Returns whether the signal has no bindings.

```c
bool               signal_empty         ( const signal* self )
```

Returns whether `<handle>` is bound to the signal.

```c
bool               signal_bound         ( const signal* self, signal_handle handle )
```

Binds `<target>` with `<func>` into the signal.
Returns a handle to the binding for unbinding.

```c
signal_handle      signal_bind          ( signal* self, T* target, signal_func func )
```

Unbinds `<handle>` from the signal.
`<handle>` must be bound to signal.

```c
void               signal_unbind        ( signal* self, signal_handle handle )
```

This is a macro, not a function.
Invokes `<self>` with the given arguments.
`<args>` must match the function signature and are passed to each binding.
The signal can be mutated while being invoked.

```c
MACRO              signal_invoke        ( signal* self, args... )
```

Deletes all bindings in a signal.

```c
void               signal_clear         ( signal* self )
```

Safely deletes a signal.

```c
void               signal_delete        ( signal* self )
```
