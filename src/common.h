#ifndef __COMMON_H
#define __COMMON_H

#include <stdint.h>
#include <stddef.h>

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

typedef u8 boolean;

enum
{
    true = 1,
    false = 0
};

typedef u8* u8ptr;
typedef u16* u16ptr;
typedef u32* u32ptr;
typedef u64* u64pytr;
typedef i8* i8ptr;
typedef i16* i16ptr;
typedef i32* i32ptr;
typedef i64* i64pytr;
typedef void* voidptr;
typedef char* charptr;
typedef unsigned char* ucharptr;

#if defined(__wasm__)
#define export __attribute__((used, visibility("default")))
#define export_named(name) __attribute__((used, visibility("default"), export_name(#name)))
#else
#define export
#define export_name(name)
#endif /* defined(__wasm__) */

#define WASM_JS(...)

#if defined(_MSC_VER)
#define panic() __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
#define panic() __builtin_trap()
#else 
#define panic() ((*((int *)0)) = 0)
#endif /* defined(_MSC_VER) */

#if 1
#define assert(exp) if (!(exp)) { panic(); } 
#else
#define assert(exp)
#endif

#define TOKENPASTE(x, y) x ## y

#define forn(name,n) for(usize name = 0; name < n; name++)
#define for_range(name,start,end) for(usize name = start; name < end; name++)

#define local static

#define c_max(a,b) (((a) > (b)) ? (a) : (b))
#define c_min(a,b) (((a) > (b)) ? (b) : (a))
#define c_abs(x)    (((x) > 0) ? (x) : (-(x)))
#define c_isnum(x)  ((x) >= '0' && (x) <= '9')
#define c_clamp(x, a, b) (t_max(t_min((x), b), a))

#define KB(x) (usize)(1024 * (x))
#define MB(x) (1024 * KB(x))
#define GB(x) (1024 * MB(x))

static inline u32 c_powi(u32 x, u32 y)
{
    u32 result = 1;

    while(true) {
        if (y == 0) break;
        result *= x;
        y--;
    }

    return result;
}

static inline usize c_string_len(const char *s)
{
    usize len = 0;

    while(true) {
        if (*s++ == '\0') break;
        len++;
    }

    return len;
}

#endif /* __COMMON_H */
