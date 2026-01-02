// .h
// ds.h Standard Library Includes
// by Kyle Furey

/**
 * ds_std.h
 *
 * The standard library is included for some data structure operations.
 * Specifically, some functions like memcpy() and memmove() are used for efficiency.
 * All standard library dependencies are defined in ds_def.h.
 *
 * Not all the includes below are required, but some are included for redundancy.
 * Each standard library include is as follows:
 *
 * stddef.h     - NULL, ptrdiff_t, and size_t
 * stdint.h     - uint8_t and SIZE_MAX
 * stdbool.h    - true, false, and bool
 * stdlib.h     - malloc(), calloc(), realloc(), and free(), abort() for ds_assert()
 * string.h     - memset(), memcpy(), memmove(), and strlen()
 * ctype.h      - tolower(), toupper(), and isspace()
 * stdio.h      - fprintf() and stderr for ds_assert()
 * assert.h     - assert()
 * math.h       - math functions
 *
 * ds_def.h can be modified to reduce or eliminate standard library dependency.
 */

#ifndef DS_STD_H
#define DS_STD_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#endif // DS_STD_H
