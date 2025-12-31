// .h
// Generic Data Structure Library
// by Kyle Furey

/**
 * ds.h
 *
 * This is a C99 header-only data structure library.
 * Each of the following data structure headers provide ds_DECLARE and ds_DECLARE_NAMED macros.
 * These macros are used to generate type-safe data structures.
 *
 * This file simply includes all of them and some C standard library headers.
 * The library includes are as follows:
 *
 * ds_std.h         - C Standard Library Includes
 * ds_def.h         - ds.h Type, Macro, and Function Declarations
 * ds_arena.h       - Block Allocator
 * ds_vector.h      - Dynamic Array
 * ds_string.h      - Mutable String
 * ds_list.h        - Doubly Linked List
 * ds_queue.h       - Double Ended Priority Queue
 * ds_set.h         - Sorted Binary Tree Set
 * ds_map.h         - Key-Value Hash Map
 * ds_unique.h      - Unique Reference
 * ds_shared.h      - Shared Reference
 * ds_weak.h        - Weak Reference
 * ds_slab.h        - Slab Allocator
 * ds_signal.h      - Multicast Event
 */

#ifndef DS_H
#define DS_H

#include "ds/ds_std.h"
#include "ds/ds_def.h"
#include "ds/ds_arena.h"
#include "ds/ds_vector.h"
#include "ds/ds_string.h"
#include "ds/ds_list.h"
#include "ds/ds_queue.h"
#include "ds/ds_set.h"
#include "ds/ds_map.h"
#include "ds/ds_unique.h"
#include "ds/ds_shared.h"
#include "ds/ds_weak.h"
#include "ds/ds_slab.h"
#include "ds/ds_signal.h"

#endif // DS_H
