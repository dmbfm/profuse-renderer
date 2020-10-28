#if !defined(__TG_H__)
#define __TG_H__

// Includes
#ifndef __wasm__
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#endif
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#if defined(_WIN32)
#include <Windows.h>
#endif

#if defined(TG_DEBUG)
#ifndef tg_assert
#define tg_assert assert //tg_debug_assert(exp)
#endif
#else
#undef tg_assert
#define tg_assert(exp)
#endif

// Some general-purpose helpful macros
#define tg_max(x, y) (((x) >= (y)) ? (x) : (y))
#define tg_min(x, y) (((x) >= (y)) ? (y) : (x))
#define tg_abs(x)    (((x) > 0) ? (x) : (-(x)))
#define tg_isnum(x) ((x) >= '0' && (x) <= '9')

#define forn(name, N) for (int name = 0; name < (N); name++)

int tg_powi(int n, int x)
{
    int r = 1;
    while (x) {
        r *= n;
        x--;
    };

    return r;
}

// Sketchy snprintf implementation
typedef struct tg__SnprintfHead
{
    uint32_t size;
    uint32_t nchars;
    char *s;
} tg__SnprintfHead;

void tg__snprintf_add_char(tg__SnprintfHead *head, char c)
{
    if (head->size) {
        head->size--;
        *head->s++ = c;
    }

    head->nchars++;
}

void tg__snprintf_add_string(tg__SnprintfHead *head, char *s)
{
    while (*s) {
        tg__snprintf_add_char(head, *s++);
    }
}

void tg__snprintf_add_uint(tg__SnprintfHead *head, unsigned int value)
{
    int ndigits = 0;
    int x = value;
    while (x) {
        x /= 10;
        ndigits++;
    }

    int acc = 0;

    do {
        int digit = (value - acc) / tg_powi(10, ndigits - 1);
        acc += digit * tg_powi(10, ndigits - 1);
        ndigits--;
        char c = (char)(digit + '0');
        tg__snprintf_add_char(head, c);
    } while (ndigits);
}

void tg__snprintf_add_int(tg__SnprintfHead *head, int value)
{
    if (value >= 0) {
        tg__snprintf_add_uint(head, (unsigned int)value);
    } else {
        tg__snprintf_add_char(head, '-');
        tg__snprintf_add_uint(head, (unsigned int)(-value));
    }
}

void tg__snprintf_add_float(tg__SnprintfHead *head, float value, int digits)
{
    int32_t intpart = (int32_t)value;

    tg__snprintf_add_int(head, intpart);

    if (digits == 0)
        return;

    tg__snprintf_add_char(head, '.');

    int32_t fracpart = (int32_t)((value - intpart) * tg_powi(10, digits));

    if (fracpart < 0) {
        fracpart = -fracpart;
    }

    tg__snprintf_add_int(head, fracpart);
}

typedef struct tg__SnprintfFlags
{
    int minus;
    int plus;
    int space;
    int hashtag;
    int zero;
} tg__SnprintfFlags;

typedef enum
{
    TG__SNPRINTF_WIDTH_ARG,
    TG__SNPRINTF_WIDTH_NUMERIC,
    TG__SNPRINTF_WIDTH_NONE,
} tg__SnprintfWidthType;

typedef struct tg__SnprintfWidth
{
    tg__SnprintfWidthType type;
    int value;
} tg__SnprintfWidth;

int tg__snprintf_parse_flag(tg__SnprintfFlags *flags, char value)
{
    switch (value) {
    case '-': {
        flags->minus = 1;
        return 1;
    } break;
    case '+': {
        flags->plus = 1;
        return 1;
    } break;
    case ' ': {
        flags->space = 1;
        return 1;
    } break;
    case '#': {
        flags->hashtag = 1;
        return 1;
    } break;
    case '0': {
        flags->zero = 1;
        return 1;
    } break;
    default:
        return -1;
    }
}

void tg__snprintf_parse_flags(tg__SnprintfFlags *flags, char **head)
{
    *flags = (tg__SnprintfFlags){ 0 };

    char *s = (char *)*head;

    while (tg__snprintf_parse_flag(flags, *s) == 1)
        s++;

    *head = s;
}

int tg__snprintf_parse_uint(char **head, int *result)
{
    if (!tg_isnum(**head))
        return 0;

    int r = 0;

    while (tg_isnum(**head)) {
        int curr = **head - '0';
        r        = r * 10 + curr;
        (*head)++;
    }

    *result = r;

    return 1;
}

void tg__snprintf_parse_width(tg__SnprintfWidth *width, char **head)
{
    *width = (tg__SnprintfWidth){ 0 };

    if (**head == '*') {
        width->type = TG__SNPRINTF_WIDTH_ARG;
        (*head)++;
        return;
    }

    if (!tg_isnum(**head)) {
        width->type = TG__SNPRINTF_WIDTH_NONE;
        return;
    }

    int result;
    if (tg__snprintf_parse_uint(head, &result)) {
        width->type  = TG__SNPRINTF_WIDTH_NUMERIC;
        width->value = result;
    } else {
        width->type = TG__SNPRINTF_WIDTH_NONE;
    }
}

void tg__snprintf_parse_precision(int *precision, char **head)
{
    if (**head != '.') {
        *precision = 6;
        return;
    }

    (*head)++;
    if (!tg__snprintf_parse_uint(head, precision)) {
        // Assume 0 precision when only '.' is given
        *precision = 0;
        return;
    }
}

int tg__snprintf(char *s, size_t n, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char c;

    tg__SnprintfHead head = { (uint32_t) (n - 1), 0, s };

    while ((c = *fmt++)) {
        switch (c) {
        case '%': {
            tg__SnprintfFlags flags;
            tg__SnprintfWidth width;
            int precision;

            tg__snprintf_parse_flags(&flags, (char **)&fmt);
            tg__snprintf_parse_width(&width, (char **)&fmt);

            if (width.type == TG__SNPRINTF_WIDTH_ARG) {
                width.value = (int)va_arg(args, int);
            }

            tg__snprintf_parse_precision(&precision, (char **)&fmt);

            switch (*fmt++) {
            case 's': {
                char *str = va_arg(args, char *);
                tg__snprintf_add_string(&head, str);
            } break;
            case '%': {
                tg__snprintf_add_char(&head, '%');
            } break;
            case 'i':
            case 'd': {
                int value = va_arg(args, int);

                if (flags.plus && value >= 0) {
                    tg__snprintf_add_char(&head, '+');
                }

                tg__snprintf_add_int(&head, value);
            } break;
            case 'u': {
                unsigned int value = va_arg(args, unsigned int);
                tg__snprintf_add_uint(&head, value);
            } break;
            case 'f': {
                double value = va_arg(args, double);
                tg__snprintf_add_float(&head, (float) value, precision);
            }
            default:
                break;
            }

        } break;
        default:
            tg__snprintf_add_char(&head, c);
            break;
        }
    }

    *head.s++ = 0;

    return head.nchars;
}

// Wrap stdlib methods

// TODO: this is just weird... the best thing todo would be to move snprintf implementation to 
// it's own file (maybe a tg_lib.h with stdlib sketchy replacements)
#ifndef tg_snprintf
#define tg_snprintf tg__snprintf
#endif

#ifndef tg_printf
#define tg_printf printf
#endif

#ifndef tg_malloc
#define tg_malloc malloc
#endif

#ifndef tg_realloc
#define tg_realloc realloc
#endif

#ifndef tg_free
#define tg_free free
#endif

// Stretchy buffer
#define tg_arr_push(a, v) (tg__arr_push(a, v))
#define tg_arr_len(a)     ((a) ? (tg__arr_len(a)) : 0)
#define tg_arr_last(a)    ((a)[tg_arr_len(a) - 1])
#define tg_arr_free(a)                 \
    {                                  \
        if (a)                         \
            tg_free(tg__arr_header((a))); \
    }
#define tg_arr_pop(a) tg__arr_pop(a)

#define tg__arr_header(a) (((int *)(a)) - 2)
#define tg__arr_cap(a)    (tg__arr_header(a)[0])
#define tg__arr_len(a)    (tg__arr_header(a)[1])
#define tg__arr_pop(a)    ((a) ? (a)[tg__arr_len(a)--] : 0)

#define tg__arr_push(a, v)           (tg__arr_grow_if_needed(a, 1), (a)[tg__arr_len(a)++] = (v))
#define tg__arr_needs_grow(a, n)     (((a) == 0) || (tg__arr_len(a) + n >= tg__arr_cap(a)))
#define tg__arr_grow_if_needed(a, n) (tg__arr_needs_grow(a, n) ? tg__arr_growa(a, n) : 0)
#define tg__arr_growa(a, n)          ((a) = tg__arr_grow((a), n, sizeof(*(a))))

void *tg__arr_grow(void *arr, int n, int element_size)
{
    int current_cap_double = arr ? 2 * tg__arr_cap(arr) : 0;
    int min_needed         = tg_arr_len(arr) + n;
    int increment          = tg_max(current_cap_double, min_needed);

    int *new_buffer = (int *)tg_realloc(arr ? tg__arr_header(arr) : 0, 2 * sizeof(int) + (increment * element_size));

    if (!new_buffer) {
        return (void *)0;
    }

    if (!arr) {
        new_buffer[1] = 0;
    }

    new_buffer[0] = increment;

    return (void *)(new_buffer + 2);
}

// File IO
#ifndef __wasm__
unsigned int tg_file_len(FILE *f)
{
    unsigned int curr = ftell(f);
    fseek(f, 0, SEEK_END);
    unsigned int len = (unsigned int)ftell(f);
    fseek(f, curr, SEEK_SET);
    return len;
}

char **tg_read_file_lines(char *filename, int *num_lines)
{
    FILE *file;

    file = fopen(filename, "rb");

    if (!file) {
        printf("Failed to open file %s\n", filename);
        return 0;
    }

    unsigned int len = tg_file_len(file);

    char *buffer = (char *)tg_malloc(len + 1);
    fread(buffer, 1, len, file);
    buffer[len] = 0;

    fclose(file);

    char *s = buffer;
    int lc  = 1;
    while (*s) {
        if (*s == '\n' || *s == '\r') {
            int crlf = (s[0] + s[1]) == ('\n' + '\r');
            if (crlf)
                s++;

            // Only count non-empty lines
            if (s[1]) {
                lc++;
            }
        }
        s++;
    }

    char **list = (char **)tg_malloc(sizeof(*list) * (lc + 1) + len + 1);
    list[lc]    = 0;
    memcpy(&list[lc + 1], buffer, len + 1);
    tg_free(buffer);
    buffer = (char *)&list[lc + 1];
    if (num_lines)
        *num_lines = lc;

    s       = buffer;
    list[0] = s;
    lc      = 1;
    while (*s) {
        if (*s == '\n' || *s == '\r') {
            int crlf = (s[0] + s[1]) == ('\n' + '\r');
            if (crlf)
                s++;
            *s = 0;
            // Only count non-empty lines
            if (s[1]) {
                list[lc] = s + 1;
                lc++;
            }
        }
        s++;
    }

    return list;
}
#endif

// Tests
#ifdef __TG_RUN_TESTS
void tg_arr_test() {}

void tg_h_test()
{
    tg_arr_test();

    char buf[256];
    tg_snprintf(buf, 256, "\nstring:(%s), float: (%++34.3f), int: (%+++23d), uint: (%u)\n", "something", -128.123,
                23423, 9845984);
    tg_printf("%s", buf);
}

#else
void tg_h_test() {}
#endif

#endif
