#include "common.h"
#include <stdarg.h>
#include <stddef.h>

#ifdef __wasm__
// Sketchy snprintf implementation
typedef struct t__SnprintfHead {
    int      size;
    uint32_t nchars;
    char *   s;
} t__SnprintfHead;

static void t__snprintf_add_char(t__SnprintfHead *head, char c) {
    if ((head->size > 0) && head->s) {
        head->size--;
        *head->s++ = c;
    }

    head->nchars++;
}

static void t__snprintf_add_string(t__SnprintfHead *head, char *s) {
    while (*s) {
        t__snprintf_add_char(head, *s++);
    }
}

static void t__snprintf_add_uint(t__SnprintfHead *head, unsigned int value) {
    if (value == 0) {
        t__snprintf_add_char(head, '0');
        return;
    }

    int ndigits = 0;
    int x       = value;
    while (x) {
        x /= 10;
        ndigits++;
    }

    int acc = 0;

    do {
        int digit = (value - acc) / c_powi(10, ndigits - 1);
        acc += digit * c_powi(10, ndigits - 1);
        ndigits--;
        char c = (char)(digit + '0');
        t__snprintf_add_char(head, c);
    } while (ndigits);
}

static void t__snprintf_add_int(t__SnprintfHead *head, int value) {
    if (value >= 0) {
        t__snprintf_add_uint(head, (unsigned int)value);
    } else {
        t__snprintf_add_char(head, '-');
        t__snprintf_add_uint(head, (unsigned int)(-value));
    }
}

static void t__snprintf_add_float(t__SnprintfHead *head, float value, int digits) {
    int32_t intpart = (int32_t)value;

    t__snprintf_add_int(head, intpart);

    if (digits == 0) return;

    t__snprintf_add_char(head, '.');

    int32_t fracpart = (int32_t)((value - intpart) * c_powi(10, digits));

    if (fracpart < 0) {
        fracpart = -fracpart;
    }

    t__snprintf_add_int(head, fracpart);
}

typedef struct t__SnprintfFlags {
    int minus;
    int plus;
    int space;
    int hashtag;
    int zero;
} t__SnprintfFlags;

typedef enum {
    TG__SNPRINTF_WIDTH_ARG,
    TG__SNPRINTF_WIDTH_NUMERIC,
    TG__SNPRINTF_WIDTH_NONE,
} t__SnprintfWidthType;

typedef struct t__SnprintfWidth {
    t__SnprintfWidthType type;
    int                  value;
} t__SnprintfWidth;

int t__snprintf_parse_flag(t__SnprintfFlags *flags, char value) {
    switch (value) {
        case '-':
        {
            flags->minus = 1;
            return 1;
        } break;
        case '+':
        {
            flags->plus = 1;
            return 1;
        } break;
        case ' ':
        {
            flags->space = 1;
            return 1;
        } break;
        case '#':
        {
            flags->hashtag = 1;
            return 1;
        } break;
        case '0':
        {
            flags->zero = 1;
            return 1;
        } break;
        default: return -1;
    }
}

static void t__snprintf_parse_flags(t__SnprintfFlags *flags, char **head) {
    *flags = (t__SnprintfFlags){0};

    char *s = (char *)*head;

    while (t__snprintf_parse_flag(flags, *s) == 1)
        s++;

    *head = s;
}

static int t__snprintf_parse_uint(char **head, int *result) {
    if (!c_isnum(**head)) return 0;

    int r = 0;

    while (c_isnum(**head)) {
        int curr = **head - '0';
        r        = r * 10 + curr;
        (*head)++;
    }

    *result = r;

    return 1;
}

static void t__snprintf_parse_width(t__SnprintfWidth *width, char **head) {
    *width = (t__SnprintfWidth){0};

    if (**head == '*') {
        width->type = TG__SNPRINTF_WIDTH_ARG;
        (*head)++;
        return;
    }

    if (!c_isnum(**head)) {
        width->type = TG__SNPRINTF_WIDTH_NONE;
        return;
    }

    int result;
    if (t__snprintf_parse_uint(head, &result)) {
        width->type  = TG__SNPRINTF_WIDTH_NUMERIC;
        width->value = result;
    } else {
        width->type = TG__SNPRINTF_WIDTH_NONE;
    }
}

static void t__snprintf_parse_precision(int *precision, char **head) {
    if (**head != '.') {
        *precision = 6;
        return;
    }

    (*head)++;
    if (!t__snprintf_parse_uint(head, precision)) {
        // Assume 0 precision when only '.' is given
        *precision = 0;
        return;
    }
}

int formatv(char *s, size_t n, const char *fmt, va_list args) {
    // va_list args;
    // va_start(args, fmt);
    char c;

    t__SnprintfHead head = {((int)n) - 1, 0, s};

    while ((c = *fmt++) != 0) {
        switch (c) {
            case '%':
            {
                t__SnprintfFlags flags;
                t__SnprintfWidth width;
                int              precision;

                t__snprintf_parse_flags(&flags, (char **)&fmt);
                t__snprintf_parse_width(&width, (char **)&fmt);

                if (width.type == TG__SNPRINTF_WIDTH_ARG) {
                    width.value = (int)va_arg(args, int);
                }

                t__snprintf_parse_precision(&precision, (char **)&fmt);

                switch (*fmt++) {
                    case 's':
                    {
                        char *str = va_arg(args, char *);
                        t__snprintf_add_string(&head, str);
                    } break;
                    case '%':
                    {
                        t__snprintf_add_char(&head, '%');
                    } break;
                    case 'i':
                    case 'd':
                    {
                        int value = va_arg(args, int);

                        if (flags.plus && value >= 0) {
                            t__snprintf_add_char(&head, '+');
                        }

                        t__snprintf_add_int(&head, value);
                    } break;
                    case 'u':
                    {
                        unsigned int value = va_arg(args, unsigned int);
                        t__snprintf_add_uint(&head, value);
                    } break;
                    case 'f':
                    {
                        double value = va_arg(args, double);
                        t__snprintf_add_float(&head, (float)value, precision);
                    }
                    default: break;
                }

            } break;
            default: t__snprintf_add_char(&head, c); break;
        }
    }

    if (head.s) *head.s++ = 0;

    return head.nchars;
}
#else
    #include <stdio.h>
int formatv(char *s, usize n, const char *fmt, va_list arg) {
    return vsnprintf(s, n, fmt, arg);
}
#endif

int format(char *s, usize n, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    int result = formatv(s, n, fmt, args);

    va_end(args);

    return result;
}
