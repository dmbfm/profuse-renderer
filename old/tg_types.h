#if !defined(__TG_TYPES_H)
#define __TG_TYPES_H

#define global static

#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef intptr_t iptr;
typedef uintptr_t uptr;

typedef float f32;
typedef double f64;

typedef u8 boolean;

#define true ((boolean)1);
#define false ((boolean)0);

#endif
