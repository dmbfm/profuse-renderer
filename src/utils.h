#ifndef __UTILS_H
#define __UTILS_H

#include "allocator.h"
#include "common.h"
#include "result.h"
#include "slice.h"

#ifndef __wasm__
    #include "string.h"
#endif

#define DEBUGLOG debug_log

void debug_log(const char *string, ...);

static inline i32 string_compare_len(const char *a,
                                     const char *b,
                                     usize       len) {
#if defined(__wasm__)
    for (usize i = 0; i < len; i++) {
        char c1 = a[i];
        char c2 = b[i];

        if (c1 != c2) {
            if (c1 < c2) {
                return -1;
            } else {
                return 1;
            }
        }

        if (c1 == 0 && c2 == 0) {
            break;
        }
    }

    return 0;
#else
    return strncmp(a, b, len);
#endif
}

static inline i32 string_compare(const char *a,
                                 const char *b) {
#if defined(__wasm__)
    int i = 0;
    for (;;) {
        char c1 = a[i];
        char c2 = b[i];

        if (c1 != c2) {
            if (c1 < c2) {
                return -1;
            } else {
                return 1;
            }
        }

        if (c1 == 0 && c2 == 0) {
            break;
        }

        i++;
    }

    return 0;
#else
    return strcmp(a, b);
#endif
}

static inline usize string_len(const char *str) {
#if defined(__wasm__)
    usize len = 0;
    for (;;) {
        char c = str[len];

        if (c == 0) {
            break;
        }

        len++;
    }

    return len;
#else
    return strlen(str);
#endif
}

#ifndef __wasm__
Result(Slice(charptr))
    read_file_lines(Allocator *a, const char *filename);
#endif /* __wasm__ */

#endif /* __UTILS_H */
