#ifndef __MATH_H

#include "common.h"
#include "result.h"

#define MATH_I32_MAX 0x7FFFFFFF
#define MATH_U32_MAX 0xFFFFFFFF
#define MATH_I64_MAX 0x7FFFFFFFFFFFFFFF
#define MATH_U64_MAX 0xFFFFFFFFFFFFFFFF

f32 math_floor(f32 x);
Result(i32) math_floor_f32_to_i32(f32 x);
i32 math_floor_f32_to_i32_unsafe(f32 x);

f32 math_ceil_f32(f32 x);
Result(i32) math_ceil_f32_to_i32(f32 x);
i32 math_ceil_to_i32_unsafe(f32 x);

f32 math_round_f32(f32 x);
Result(i32) math_round_f32_to_i32(f32 x);
i32 math_round_f32_to_i32_unsafe(f32 x);

i32 math_div_ceil_u32(u32 a, u32 b);

f32 math_sinf(f32 x);
f32 math_cosf(f32 x);

#define __MATH_H
#endif /* __MATH_H */
