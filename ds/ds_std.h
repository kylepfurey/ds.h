// .h
// Data Structure Standard Library Includes
// by Kyle Furey

/**
 * ds_std.h
 *
 * The standard library is included for some data structure operations.
 * Specifically, some functions like memcpy() and memmove() are used for efficiency.
 *
 * Not all the includes below are required, but some are included for redundancy.
 * Each standard library include is as follows:
 *
 * stdio.h      - fprintf() and stderr in ds_assert()
 * stdlib.h     - malloc(), calloc(), realloc(), free() in ds_def.h
 * stddef.h     - NULL, SIZE_MAX, and size_t
 * stdbool.h    - true, false, and bool
 * stdint.h     - uint8_t and ptrdiff_t in ds_def.h
 * string.h     - memset(), memcpy(), and memmove()
 * ctype.h      - tolower() and toupper() in ds_string.h
 * math.h       - Mathematical functions
 * assert.h     - assert()
 */

#ifndef DS_STD_H
#define DS_STD_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#endif // DS_STD_H
