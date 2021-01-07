#ifndef __UTILS_H
#define __UTILS_H 

#include "common.h"
#include "result.h"
#include "slice.h"
#include "allocator.h"

#ifndef __wasm__
#include "string.h"
#endif

#define DEBUGLOG debug_log

void debug_log(const char *string, ...);

static inline i32 string_compare_len(const char *a, const char *b, usize len)
{
#if defined(__wasm__)
#error "Not implmenented"
    return 0;
#else
    return strncmp(a, b, len);
#endif
}

static inline i32 string_compare(const char *a, const char *b)
{
#if defined(__wasm__)
#error "Not implmenented"
    return 0;
#else
    return strcmp(a, b);
#endif
}

static inline usize string_len(const char *str)
{
#if defined(__wasm__)
#error "Not implmenented"
    return 0;
#else
    return strlen(str);
#endif
}

#ifndef __wasm__
Result(Slice(charptr)) read_file_lines(Allocator *a, const char *filename, int *num_lines);
#endif /* __wasm__ */

#endif /* __UTILS_H */
