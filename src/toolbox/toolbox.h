#if !defined(TOOLBOX_H)
#define TOOLBOX_H 

/**
 *
 * Configuration preprocessor defines:
 *
 * TOOLBOX_NO_STD_LIB: Don't use the C std lib. Disables file/io functions.
 * TOOLBOX_DEBUG: Enable debug/assert
 *
 */

#include "toolbox_common.h"

#ifdef __wasm__
#define TOOLBOX_NO_STD_LIB
#endif

#ifndef TOOLBOX_NO_STD_LIB
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#endif

#if defined(TOOLBOX_DEBUG)
#endif

// Some general-purpose helpful macros
#define t_max(x, y) (((x) >= (y)) ? (x) : (y))
#define t_min(x, y) (((x) >= (y)) ? (y) : (x))
#define t_abs(x)    (((x) > 0) ? (x) : (-(x)))
#define t_isnum(x) ((x) >= '0' && (x) <= '9')

#define forn(name, N) for (int name = 0; name < (N); name++)

int t_powi(int n, int x);

#ifndef TOOLBOX_NO_STD_LIB 
uint t_file_len(FILE *f);
char **t_read_file_lines(char *filename, int *out_num_lines);
#endif

#ifdef TOOLBOX_NO_STD_LIB
int t__snprintf(char *s, size_t n, const char *fmt, ...);
void *t__memcpy(void *dst, void *src, size_t num)
{
    u8 *d = (u8 *) dst;
    u8 *s = (u8 *) src;

    while(num)
    {
        *d++ = *s++;
        num--;
    }

    return dst;
}

#define t_snprintf t__snprintf
#define t_memcpy t__memcpy
#endif

#ifndef t_snprintf
#define t_snprintf snprintf
#endif

#ifndef t_printf
#define t_printf printf
#endif

#ifndef t_malloc
#define t_malloc malloc
#endif

#ifndef t_realloc
#define t_realloc realloc
#endif

#ifndef t_free
#define t_free free
#endif

#ifndef t_fopen
#define t_fopen fopen
#endif

#ifndef t_fseek
#define t_fseek fskeek
#endif

#ifndef t_memcpy
#define t_memcpy memcpy
#endif

// Stretchy buffer
#define t_arr_push(a, v) (t__arr_push(a, v))
#define t_arr_len(a)     ((a) ? (t__arr_len(a)) : 0)
#define t_arr_last(a)    ((a)[t_arr_len(a) - 1])
#define t_arr_free(a)                 \
    {                                  \
        if (a)                         \
            t_free(t__arr_header((a))); \
    }
#define t_arr_pop(a) t__arr_pop(a)

#define t__arr_header(a) (((int *)(a)) - 2)
#define t__arr_cap(a)    (t__arr_header(a)[0])
#define t__arr_len(a)    (t__arr_header(a)[1])
#define t__arr_pop(a)    ((a) ? (a)[t__arr_len(a)--] : 0)

#define t__arr_push(a, v)           (t__arr_grow_if_needed(a, 1), (a)[t__arr_len(a)++] = (v))
#define t__arr_needs_grow(a, n)     (((a) == 0) || (t__arr_len(a) + n >= t__arr_cap(a)))
#define t__arr_grow_if_needed(a, n) (t__arr_needs_grow(a, n) ? t__arr_growa(a, n) : 0)
#define t__arr_growa(a, n)          ((a) = t__arr_grow((a), n, sizeof(*(a))))

void *t__arr_grow(void *arr, int n, int element_size)
{
    int current_cap_double = arr ? 2 * t__arr_cap(arr) : 0;
    int min_needed         = t_arr_len(arr) + n;
    int increment          = t_max(current_cap_double, min_needed);

    int *new_buffer = (int *)t_realloc(arr ? t__arr_header(arr) : 0, 2 * sizeof(int) + (increment * element_size));

    if (!new_buffer) {
        return (void *)0;
    }

    if (!arr) {
        new_buffer[1] = 0;
    }

    new_buffer[0] = increment;

    return (void *)(new_buffer + 2);
}

#endif
