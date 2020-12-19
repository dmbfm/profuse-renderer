#include "math.h"
#include "result.h"

#ifndef __wasm__
#include <math.h>
#endif

f32 math_floor_f32(f32 x)
{
    union
    {
        float f;
        uint32_t i;
    } u = { x };

    int e = ((u.i >> 23) & 0xff) - 127;
    int m = u.i & 0x7FFFFF;

    if (e >= 23)
        return x;

    if (e >= 0) {
        m = 0x007FFFFF >> e;
        if ((u.i & m) == 0)
            return x;
        if (u.i >> 31)
            u.i += m;
        u.i &= ~m;
    } else {
        if ((u.i >> 31) == 0)
            u.i = 0;
        else if (u.i << 1)
            u.f = -1.0;
    }

    return u.f;
}

Result(i32) math_floor_f32_to_i32(f32 x)
{
    float f = math_floor_f32(x);

    if ((f < (float)MATH_I32_MAX) && (f > (-(float)MATH_I32_MAX))) {
        return result_ok(i32, (i32)f);
    }

    return result_error(i32, ERR_MATH_OVERFLOW);
}

i32 math_floor_f32_to_i32_unsafe(f32 x)
{
    return (i32)math_floor_f32(x);
}

f32 math_ceil_f32(f32 x)
{
    union
    {
        float f;
        uint32_t i;
    } u = { x };

    int e = ((u.i >> 23) & 0xff) - 127;
    int m = u.i & 0x7FFFFF;

    if (e >= 23)
        return x;

    if (e >= 0) {
        m = 0x007FFFFF >> e;
        if ((u.i & m) == 0)
            return x;
        if (u.i >> 31)
            u.i += m;
        u.i &= ~m;
    } else {
        if ((u.i >> 31) == 0)
            u.i = 0;
        else if (u.i << 1)
            u.f = -1.0;
    }

    return u.f + 1.0;
}

Result(i32) math_ceil_f32_to_i32(f32 x)
{
    float f = math_ceil_f32(x);

    if ((f < (float)MATH_I32_MAX) && (f < (-(float)MATH_I32_MAX))) {
        return result_error(i32, ERR_MATH_OVERFLOW);
    }

    return result_ok(i32, (i32)f);
}

i32 math_ceil_to_i32_unsafe(f32 x)
{
    return (i32)math_ceil_f32(x);
}

f32 math_round_f32(f32 x)
{
    return math_floor_f32(x + 0.5f);
}

Result(i32) math_round_f32_to_i32(f32 x)
{
    return math_floor_f32_to_i32(x + 0.5f);
}

i32 math_round_f32_to_i32_unsafe(f32 x)
{
    return math_floor_f32_to_i32_unsafe(x + 0.5f);
}

i32 math_div_ceil_u32(u32 a, u32 b)
{
    return ((a - !!a)/b) + !!a;
}

#ifdef __wasm__
// clang-format off
extern f32 math_wasm_sinf(f32);
WASM_JS(
        function math_wasm_sinf(x) { return Math.sin(x) }
       )

extern f32 math_wasm_cosf(f32);
WASM_JS(
        function math_wasm_cosf(x) { return Math.cos(x) }
       )

// clang-format on
#endif /* __wasm__ */

f32 math_sinf(f32 x)
{
#ifdef __wasm__
    return math_wasm_sinf(x);
#else
    return sinf(x);
#endif /* __wasm__ */
}

f32 math_cosf(f32 x)
{
#ifdef __wasm__
    return math_wasm_cosf(x);
#else
    return cosf(x);
#endif /* __wasm__ */
}

#ifdef __RUN_TESTS
//#if 1
#include "test.h"
#include <math.h>

test(math_floor_f32)
{
    float f = -23.343;
    assert(floorf(f) == math_floor_f32(f));

    f = 0.000234;
    assert(floorf(f) == math_floor_f32(f));

    f = 234234292.3432;
    assert(floorf(f) == math_floor_f32(f));

    f = -1;
    assert(floorf(f) == math_floor_f32(f));

    f = -34;
    assert(floorf(f) == math_floor_f32(f));

    f = 2;
    assert(floorf(f) == math_floor_f32(f));
}

test(math_floor_f32_to_i32)
{
    float f       = -23.343;
    Result(i32) r = math_floor_f32_to_i32(f);

    assert(result_is_ok(r));
    assert(r.value == -24);

    f = 345.345;
    r = math_floor_f32_to_i32(f);

    assert(result_is_ok(r));
    assert(r.value == 345);

    f = 1000000000000000.0f;
    r = math_floor_f32_to_i32(f);

    assert(result_is_error(r));
}

test(math_ceil_f32)
{
    float f = 23.34;
    assert(math_ceil_f32(f) == ceilf(f));
    f = 23;
    assert(math_ceil_f32(f) == ceilf(f));
    f = -23;
    assert(math_ceil_f32(f) == ceilf(f));
    f = 0.1234234;
    assert(math_ceil_f32(f) == ceilf(f));
    f = -0.1234234;
    assert(math_ceil_f32(f) == ceilf(f));
    f = 234234234.1234234;
    assert(math_ceil_f32(f) == ceilf(f));
}

test(math_round_f32)
{
    float f = 453.87349857;
    assert(roundf(f) == math_round_f32(f));

    f = 0.87349857;
    assert(roundf(f) == math_round_f32(f));
    f = 0.27349857;
    assert(roundf(f) == math_round_f32(f));
    f = 23423.87349857;
    assert(roundf(f) == math_round_f32(f));
    f = -234.87349857;
    assert(roundf(f) == math_round_f32(f));
    f = -343.349857;
    assert(roundf(f) == math_round_f32(f));
    f = 0.0000349857;
    assert(roundf(f) == math_round_f32(f));
}

test(math_div_ceil_u32)
{
    assert(math_div_ceil_u32(0, 10) == 0);
    assert(math_div_ceil_u32(5, 10) == 1);
    assert(math_div_ceil_u32(10, 10) == 1);
    assert(math_div_ceil_u32(11, 10) == 2);
}

suite()
{
    run_test(math_floor_f32);
    run_test(math_floor_f32_to_i32);
    run_test(math_ceil_f32);
    run_test(math_round_f32);
    run_test(math_div_ceil_u32);
}

#endif /* __RUN_TESTS */
