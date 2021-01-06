#ifndef __COMMON_H
#define __COMMON_H

#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef unsigned char uchar;
typedef unsigned int uint;

typedef size_t usize;
typedef intptr_t iptr;
typedef uintptr_t uptr;
typedef void *voidptr;

typedef u8 boolean;

enum
{
    true  = 1,
    false = 0
};

typedef u8 *u8ptr;
typedef u16 *u16ptr;
typedef u32 *u32ptr;
typedef u64 *u64pytr;
typedef i8 *i8ptr;
typedef i16 *i16ptr;
typedef i32 *i32ptr;
typedef i64 *i64pytr;
typedef void *voidptr;
typedef char *charptr;
typedef unsigned char *ucharptr;

#define TOKENPASTE(x, y)   x##y
#define TOKENCOMBINE(x, y) TOKENPASTE(x, y)
#define STRINGIFY(x)       #x
#define TOSTRING(x)        STRINGIFY(x)

#if defined(__wasm__)
#define export __attribute__((used, visibility("default")))
#define export_named(name) __attribute__((used, visibility("default"), export_name(#name)))
#else
#define export
#define export_name(name)
#endif /* defined(__wasm__) */

#define WASM_JS(...)

#ifdef __wasm__
extern void common_wasm_print_message(const char *);
// clang-format off
WASM_JS(
        function common_wasm_print_message(ptr) { console.log(decodeStringAt(ptr)); }
)
// clang-format on

static inline void common_wasm_panic_message(const char *msg)
{
    common_wasm_print_message(msg);
    __builtin_trap();
}

#endif /* __wasm__ */

#if defined(_MSC_VER)
#define panic() __debugbreak()
#elif defined(__wasm__)
#define panic() common_wasm_panic_message("PANIC: " TOSTRING(__FILE__) ":" TOSTRING(__LINE__))
#elif defined(__clang__) || defined(__GNUC__)
#define panic() __builtin_trap()
#else
#define panic() ((*((int *)0)) = 0)
#endif /* defined(_MSC_VER) */

#if defined(_WIN32) && !defined(_WIN64)
#define CSTDCALL __stdcall
#else
#define CSTDCALL
#endif

#define DECL_FUNC_POINTER(qualifier, ret, typename, varname, ...) \
    typedef ret(CSTDCALL typename)(__VA_ARGS__);                  \
    qualifier typename *varname;

#if 1
#define assert(exp) \
    if (!(exp)) {   \
        panic();    \
    }
#else
#define assert(exp)
#endif

#define forn(name, n)               for (usize name = 0; name < n; name++)
#define for_range(name, start, end) for (usize name = start; name < end; name++)

#define local static

#define UNUSED_VARIABLE(x) (void)(x)

#define c_max(a, b)      (((a) > (b)) ? (a) : (b))
#define c_min(a, b)      (((a) > (b)) ? (b) : (a))
#define c_abs(x)         (((x) > 0) ? (x) : (-(x)))
#define c_isnum(x)       ((x) >= '0' && (x) <= '9')
#define c_clamp(x, a, b) (t_max(t_min((x), b), a))

#define KB(x) (usize)(1024 * (x))
#define MB(x) (1024 * KB(x))
#define GB(x) (1024 * MB(x))

static inline u32 c_powi(u32 x, u32 y)
{
    u32 result = 1;

    for (;;) {
        if (y == 0)
            break;
        result *= x;
        y--;
    }

    return result;
}

static inline usize c_string_len(const char *s)
{
    usize len = 0;

    for (;;) {
        if (*s++ == '\0')
            break;
        len++;
    }

    return len;
}

#ifdef __wasm__
export static void *memset(void *ptr, int value, size_t num)
{
    unsigned char v  = (unsigned char)value;
    unsigned char *p = (unsigned char *)ptr;

    while (num > 0) {
        *p = v;
        num--;
    }

    return ptr;
}

export static void *memcpy(void *dst, const void *src, size_t num)
{
    unsigned char *d = (unsigned char *)dst;
    unsigned char *s = (unsigned char *)src;

    while (num > 0) {
        *d = *s;
        num--;
    }

    return dst;
}
#endif /* __wasm__ */

#endif /* __COMMON_H */
