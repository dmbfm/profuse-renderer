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

#if defined(_MSC_VER)
#define panic() __debugbreak
#elif defined(__clang__) || defined(__GNUC__)
#define panic() __builtin_trap()
#else 
#define panic() ((*((int *)0)) = 0)
#endif

#if 1
#define ASSERT(exp) if (!(exp)) { panic(); } 
#else
#define ASSERT(exp)
#endif

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) > (b)) ? (b) : (a))

#endif /* __COMMON_H */
