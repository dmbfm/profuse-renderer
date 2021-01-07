#include "math.h"
#include "result.h"

#ifndef __wasm__
#include <math.h>
#endif

// TODO: Mat3
// TODO: Projection/lookat matrix tests

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

    return u.f + 1.0f;
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

u32 math_div_ceil_u32(u32 a, u32 b)
{
    return ((a - !!a) / b) + !!a;
}

// TODO: This is not a good equality tests for floats:
//       - https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
//       - https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html
boolean math_is_equalf(f32 a, f32 b)
{
    const f32 e = 1.0e-5f;

    return (c_abs(a - b) <= e);
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

extern f32 math_wasm_tanf(f32);
WASM_JS(
        function math_wasm_tanf(x) { return Math.tan(x) }
       )


extern f32 math_wasm_sqrtf(f32);
WASM_JS(
    function math_wasm_sqrtf(x) { return Math.sqrt(x) }
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

f32 math_tanf(f32 x)
{
#ifdef __wasm__
    return math_wasm_cosf(x);
#else
    return tanf(x);
#endif /* __wasm__ */
}

f32 math_sqrtf(f32 x)
{
#ifdef __wasm__
    return math_wasm_sqrtf(x);
#else
    return sqrtf(x);
#endif
}

Vec2 vec2(f32 x, f32 y)
{
    return (Vec2){ .x = x, .y = y };
}

Vec2 vec2_fill(f32 x)
{
    return (Vec2){ .x = x, .y = x };
}

Vec2 vec2_zero()
{
    return (Vec2){ .x = 0, .y = 0 };
}

Vec2 vec2_ones()
{
    return (Vec2){ .x = 1, .y = 1 };
}

Vec2 vec2_add(Vec2 a, Vec2 b)
{
    return (Vec2){ .x = a.x + b.x, .y = a.y + b.y };
}

Vec2 vec2_sub(Vec2 a, Vec2 b)
{
    return (Vec2){ .x = a.x - b.x, .y = a.y - b.y };
}

Vec2 vec2_mul(Vec2 a, Vec2 b)
{
    return (Vec2){ .x = a.x * b.x, .y = a.y * b.y };
}

Vec2 vec2_smul(f32 a, Vec2 v)
{
    return (Vec2){ .x = a * v.x, .y = a * v.y };
}

f32 vec2_dot(Vec2 a, Vec2 b)
{
    return a.x * b.x + a.y * b.y;
}

f32 vec2_len(Vec2 v)
{
    return math_sqrtf(v.x * v.x + v.y * v.y);
}

Vec2 vec2_normalized(Vec2 v)
{
    return vec2_smul(1.0f / vec2_len(v), v);
}

boolean vec2_is_equal(Vec2 a, Vec2 b)
{
    return math_is_equalf(a.x, b.x) && math_is_equalf(a.y, b.y);
}

Vec3 vec3(f32 x, f32 y, f32 z)
{
    return (Vec3){ .x = x, .y = y, .z = z };
}

Vec3 vec3_fill(f32 value)
{
    return vec3(value, value, value);
}

Vec3 vec3_zero()
{
    return (Vec3){ 0 };
}

Vec3 vec3_ones()
{
    return vec3(1, 1, 1);
}

Vec3 vec3_add(Vec3 a, Vec3 b)
{
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3 vec3_sub(Vec3 a, Vec3 b)
{
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3 vec3_mul(Vec3 a, Vec3 b)
{
    return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

Vec3 vec3_smul(float x, Vec3 a)
{
    return vec3(x * a.x, x * a.y, x * a.z);
}

f32 vec3_dot(Vec3 a, Vec3 b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

f32 vec3_len(Vec3 v)
{
    return math_sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vec3 vec3_normalized(Vec3 v)
{
    return vec3_smul(1.0f / vec3_len(v), v);
}

Vec3 vec3_cross(Vec3 a, Vec3 b)
{
    return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

Vec3 vec3_i()
{
    return vec3(1, 0, 0);
}

Vec3 vec3_j()
{
    return vec3(0, 1, 0);
}

Vec3 vec3_k()
{
    return vec3(0, 0, 1);
}

boolean vec3_is_equal(Vec3 a, Vec3 b)
{
    return math_is_equalf(a.x, b.x) && math_is_equalf(a.y, b.y) && math_is_equalf(a.z, b.z);
}

Vec3 vec3_spherical(float polar_angle, float azimuthal_angle, float radius)
{
    Vec3 r;

    r.x = radius * math_sinf(polar_angle) * math_cosf(azimuthal_angle);
    r.z = radius * math_sinf(polar_angle) * math_sinf(azimuthal_angle);
    r.y = radius * math_cosf(polar_angle);

    return r;
}

Vec4 vec4(f32 x, f32 y, f32 z, f32 w)
{
    return (Vec4){ .x = x, .y = y, .z = z, .w = w };
}

Vec4 vec4_zero()
{
    return (Vec4){ 0 };
}

Vec4 vec4_ones()
{
    return vec4(1, 1, 1, 1);
}

Vec4 vec4_add(Vec4 a, Vec4 b)
{
    return vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

Vec4 vec4_sub(Vec4 a, Vec4 b)
{
    return vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

Vec4 vec4_mul(Vec4 a, Vec4 b)
{
    return vec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

Vec4 vec4_smul(float x, Vec4 a)
{
    return vec4(x * a.x, x * a.y, x * a.z, x * a.w);
}

f32 vec4_dot(Vec4 a, Vec4 b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

f32 vec4_len(Vec4 v)
{
    return math_sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

Vec4 vec4_normalized(Vec4 v)
{
    return vec4_smul(1.0f / vec4_len(v), v);
}

Vec4 vec4_i()
{
    return vec4(1, 0, 0, 0);
}

Vec4 vec4_j()
{
    return vec4(0, 1, 0, 0);
}

Vec4 vec4_k()
{
    return vec4(0, 0, 1, 0);
}

Vec4 vec4_w()
{
    return vec4(0, 0, 0, 1);
}

boolean vec4_is_equal(Vec4 a, Vec4 b)
{
    return math_is_equalf(a.x, b.x) && math_is_equalf(a.y, b.y) && math_is_equalf(a.z, b.z) && math_is_equalf(a.w, b.w);
}

Mat4 mat4_id()
{
    Mat4 r = { 0 };

    r.m11 = 1;
    r.m22 = 1;
    r.m33 = 1;
    r.m44 = 1;

    return r;
}

Mat4 mat4_diag(f32 v)
{
    Mat4 r = { 0 };

    r.m11 = v;
    r.m22 = v;
    r.m33 = v;
    r.m44 = v;

    return r;
}

boolean mat4_is_equal(Mat4 a, Mat4 b)
{
    boolean result = true;

    for (int i = 0; i < 16; i++) {
        result &= (a.values[i] == b.values[i]);
    }

    return result;
}

Mat4 mat4_mul(Mat4 a, Mat4 b)
{
    Mat4 r;

    r.m11 = a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31 + a.m14 * b.m41;
    r.m12 = a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32 + a.m14 * b.m42;
    r.m13 = a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33 + a.m14 * b.m43;
    r.m14 = a.m11 * b.m14 + a.m12 * b.m24 + a.m13 * b.m34 + a.m14 * b.m44;

    r.m21 = a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31 + a.m24 * b.m41;
    r.m22 = a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32 + a.m24 * b.m42;
    r.m23 = a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33 + a.m24 * b.m43;
    r.m24 = a.m21 * b.m14 + a.m22 * b.m24 + a.m23 * b.m34 + a.m24 * b.m44;

    r.m31 = a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31 + a.m34 * b.m41;
    r.m32 = a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32 + a.m34 * b.m42;
    r.m33 = a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33 + a.m34 * b.m43;
    r.m34 = a.m31 * b.m14 + a.m32 * b.m24 + a.m33 * b.m34 + a.m34 * b.m44;

    r.m41 = a.m41 * b.m11 + a.m42 * b.m21 + a.m43 * b.m31 + a.m44 * b.m41;
    r.m42 = a.m41 * b.m12 + a.m42 * b.m22 + a.m43 * b.m32 + a.m44 * b.m42;
    r.m43 = a.m41 * b.m13 + a.m42 * b.m23 + a.m43 * b.m33 + a.m44 * b.m43;
    r.m44 = a.m41 * b.m14 + a.m42 * b.m24 + a.m43 * b.m34 + a.m44 * b.m44;

    return r;
}

Vec4 vec4_transform(Vec4 v, Mat4 m)
{
    return (Vec4){
        .x = v.x * m.m11 + v.y * m.m12 * v.z * m.m13 + v.w * m.m14,
        .y = v.x * m.m21 + v.y * m.m22 * v.z * m.m23 + v.w * m.m24,
        .z = v.x * m.m31 + v.y * m.m32 * v.z * m.m33 + v.w * m.m34,
        .w = v.x * m.m41 + v.y * m.m42 * v.z * m.m43 + v.w * m.m44,
    };
}

Mat4 mat4_translation(Vec3 t)
{
    Mat4 r = mat4_id();

    r.m14 = t.x;
    r.m24 = t.y;
    r.m34 = t.z;

    return r;
}

Mat4 mat4_look_at(Vec3 from, Vec3 to, Vec3 up)
{
    Vec3 k = vec3_normalized(vec3_sub(from, to));
    Vec3 i = vec3_normalized(vec3_cross(up, k));
    Vec3 j = vec3_cross(k, i);
    // i = vec3_cross(j, k);

    Mat4 result = { 0 };

    result.m11 = i.x;
    result.m12 = i.y;
    result.m13 = i.z;
    result.m14 = -vec3_dot(i, from);

    result.m21 = j.x;
    result.m22 = j.y;
    result.m23 = j.z;
    result.m24 = -vec3_dot(j, from);

    result.m31 = k.x;
    result.m32 = k.y;
    result.m33 = k.z;
    result.m34 = -vec3_dot(k, from);

    result.m44 = 1;

    return result;
}

Mat4 mat4_perspective(f32 fov, f32 aspect, f32 n, f32 f)
{
    Mat4 result = { 0 };

    float c = 1.0f / math_tanf(fov / 2);

    result.m11 = c / aspect;

    result.m22 = c;

    result.m33 = -(f + n) / (f - n);
    result.m34 = -2.0f * f * n / (f - n);
    result.m43 = -1;

    return result;
}

Mat4 mat4_ortho(f32 width, f32 height, f32 n, f32 f)
{
    Mat4 result = { 0 };

    result.m11 = 2 / width;
    result.m22 = 2 / height;
    result.m33 = 2 / (f - n);
    result.m34 = -(f + n) / (f - n);
    result.m44 = 1;

    return result;
}

Mat4 mat4_rotY(float angle)
{
    Mat4 result = mat4_id();

    float c = math_cosf(angle);
    float s = math_sinf(angle);

    result.m11 = c;
    result.m13 = s;
    result.m31 = -s;
    result.m33 = c;

    return result;
}

//#ifdef __RUN_TESTS
#if 1
#include "test.h"
#include <math.h>

test(math_floor_f32)
{
    float f = -23.343;
    expect(floorf(f) == math_floor_f32(f));

    f = 0.000234;
    expect(floorf(f) == math_floor_f32(f));

    f = 234234292.3432;
    expect(floorf(f) == math_floor_f32(f));

    f = -1;
    expect(floorf(f) == math_floor_f32(f));

    f = -34;
    expect(floorf(f) == math_floor_f32(f));

    f = 2;
    expect(floorf(f) == math_floor_f32(f));
}

test(math_floor_f32_to_i32)
{
    float f       = -23.343;
    Result(i32) r = math_floor_f32_to_i32(f);

    expect(result_is_ok(r));
    expect(r.value == -24);

    f = 345.345;
    r = math_floor_f32_to_i32(f);

    expect(result_is_ok(r));
    expect(r.value == 345);

    f = 1000000000000000.0f;
    r = math_floor_f32_to_i32(f);

    expect(result_is_error(r));
}

test(math_ceil_f32)
{
    float f = 23.34;
    expect(math_ceil_f32(f) == ceilf(f));
    f = 23;
    expect(math_ceil_f32(f) == ceilf(f));
    f = -23;
    expect(math_ceil_f32(f) == ceilf(f));
    f = 0.1234234;
    expect(math_ceil_f32(f) == ceilf(f));
    f = -0.1234234;
    expect(math_ceil_f32(f) == ceilf(f));
    f = 234234234.1234234;
    expect(math_ceil_f32(f) == ceilf(f));
}

test(math_round_f32)
{
    float f = 453.87349857;
    expect(roundf(f) == math_round_f32(f));

    f = 0.87349857;
    expect(roundf(f) == math_round_f32(f));
    f = 0.27349857;
    expect(roundf(f) == math_round_f32(f));
    f = 23423.87349857;
    expect(roundf(f) == math_round_f32(f));
    f = -234.87349857;
    expect(roundf(f) == math_round_f32(f));
    f = -343.349857;
    expect(roundf(f) == math_round_f32(f));
    f = 0.0000349857;
    expect(roundf(f) == math_round_f32(f));
}

test(math_div_ceil_u32)
{
    expect(math_div_ceil_u32(0, 10) == 0);
    expect(math_div_ceil_u32(5, 10) == 1);
    expect(math_div_ceil_u32(10, 10) == 1);
    expect(math_div_ceil_u32(11, 10) == 2);
}

test(vec2_is_equal)
{
    Vec2 v1 = vec2(1.0f, 2.0f);

    expect(vec2_is_equal(v1, v1));
}

test(vec2_add)
{
    Vec2 v1 = vec2(1.0f, 2.0f);
    Vec2 v2 = vec2(3.0f, 4.0f);
    Vec2 v3 = vec2_add(v1, v2);

    expect(math_is_equalf(v3.x, 4.0f));
    expect(math_is_equalf(v3.y, 6.0f));
}

test(vec2_sub)
{
    Vec2 v1 = vec2(1.0f, 2.0f);
    Vec2 v2 = vec2(3.0f, 4.0f);
    Vec2 v3 = vec2_sub(v1, v2);

    expect(vec2_is_equal(v3, vec2(-2.0f, -2.0f)));
}

test(vec2_mul)
{
    Vec2 v1 = vec2(1.0f, 2.0f);
    Vec2 v2 = vec2(3.0f, 4.0f);
    Vec2 v3 = vec2_mul(v1, v2);

    expect(vec2_is_equal(v3, vec2(3.0f, 8.0f)));
}

test(vec2_smul)
{
    Vec2 v2 = vec2(3.0f, 4.0f);
    Vec2 v3 = vec2_smul(2.0f, v2);

    expect(vec2_is_equal(v3, vec2(6.0f, 8.0f)));
}

test(vec2_dot)
{
    Vec2 v1 = vec2(1.0f, 2.0f);
    Vec2 v2 = vec2(3.0f, 4.0f);

    expect(math_is_equalf(vec2_dot(v1, v2), 11.0f));
}

test(vec2_len)
{
    Vec2 v = vec2(3.4f, 72.34f);

    expect(math_is_equalf(vec2_len(v), 72.41985639));
}

test(vec2_fill)
{
    Vec2 v = vec2_fill(2.0f);

    expect(vec2_is_equal(v, vec2(2.0, 2.0)));
}

test(vec3_is_equal)
{
    Vec3 v1 = vec3(1.0f, 2.0f, 3.0f);

    expect(vec3_is_equal(v1, v1));
}

test(vec3_add)
{
    Vec3 v1 = vec3(1.0f, 2.0f, 6.0f);
    Vec3 v2 = vec3(3.0f, 4.0f, 8.0f);
    Vec3 v3 = vec3_add(v1, v2);

    expect(vec3_is_equal(v3, vec3(4.0f, 6.0f, 14.0f)));
}

test(vec3_sub)
{
    Vec3 v1 = vec3(1.0f, 2.0f, 6.0f);
    Vec3 v2 = vec3(3.0f, 4.0f, 8.0f);
    Vec3 v3 = vec3_sub(v1, v2);

    expect(vec3_is_equal(v3, vec3(-2.0f, -2.0f, -2.0f)));
}

test(vec3_mul)
{
    Vec3 v1 = vec3(1.0f, 2.0f, 6.0f);
    Vec3 v2 = vec3(3.0f, 4.0f, 8.0f);
    Vec3 v3 = vec3_mul(v1, v2);

    expect(vec3_is_equal(v3, vec3(3.0f, 8.0f, 48.0f)));
}

test(vec3_smul)
{
    Vec3 v2 = vec3(3.0f, 4.0f, 8.0f);
    Vec3 v3 = vec3_smul(2.0f, v2);

    expect(vec3_is_equal(v3, vec3(6.0f, 8.0f, 16.0f)));
}

test(vec3_dot)
{
    Vec3 v1 = vec3(1.0f, 2.0f, 6.0f);
    Vec3 v2 = vec3(3.0f, 4.0f, 8.0f);

    expect(math_is_equalf(vec3_dot(v1, v2), 59.0f));
}

test(vec3_len)
{
    Vec3 v = vec3(3.4f, 72.34f, 1.0f);

    expect(math_is_equalf(vec3_len(v), 72.42676024));
}

test(vec3_cross)
{
    expect(vec3_is_equal(vec3_cross(vec3_j(), vec3_k()), vec3_i()));
    expect(vec3_is_equal(vec3_cross(vec3_i(), vec3_j()), vec3_k()));
    expect(vec3_is_equal(vec3_cross(vec3_k(), vec3_i()), vec3_j()));
}

test(vec4_is_equal)
{
    Vec4 v1 = vec4(1.0f, 2.0f, 3.0f, 10.f);

    expect(vec4_is_equal(v1, v1));
}

test(vec4_add)
{
    Vec4 v1 = vec4(1.0f, 2.0f, 6.0f, 10.0f);
    Vec4 v2 = vec4(3.0f, 4.0f, 8.0f, 12.0f);
    Vec4 v3 = vec4_add(v1, v2);

    expect(vec4_is_equal(v3, vec4(4.0f, 6.0f, 14.0f, 22.0f)));
}

test(vec4_sub)
{
    Vec4 v1 = vec4(1.0f, 2.0f, 6.0f, 10.0f);
    Vec4 v2 = vec4(3.0f, 4.0f, 8.0f, 12.0f);
    Vec4 v3 = vec4_sub(v1, v2);

    expect(vec4_is_equal(v3, vec4(-2.0f, -2.0f, -2.0f, -2.0f)));
}

test(vec4_mul)
{
    Vec4 v1 = vec4(1.0f, 2.0f, 6.0f, 10.0f);
    Vec4 v2 = vec4(3.0f, 4.0f, 8.0f, 12.0f);
    Vec4 v3 = vec4_mul(v1, v2);

    expect(vec4_is_equal(v3, vec4(3.0f, 8.0f, 48.0f, 120.0f)));
}

test(vec4_smul)
{
    Vec4 v2 = vec4(3.0f, 4.0f, 8.0f, 10.0f);
    Vec4 v3 = vec4_smul(2.0f, v2);

    expect(vec4_is_equal(v3, vec4(6.0f, 8.0f, 16.0f, 20.0f)));
}

test(vec4_dot)
{
    Vec4 v1 = vec4(1.0f, 2.0f, 6.0f, 10.0f);
    Vec4 v2 = vec4(3.0f, 4.0f, 8.0f, 12.0f);

    expect(math_is_equalf(vec4_dot(v1, v2), 179.0f));
}

test(vec4_len)
{
    Vec4 v = vec4(3.4f, 72.34f, 1.0f, 1.0f);

    expect(math_is_equalf(vec4_len(v), 72.433663444f));
}

test(mat4_mul)
{
    Mat4 m1     = { 0, 2, 0, 0, 0, 0, 0, 4, 0, 3, 0, 0, 0, 0, 1, 0 };
    Mat4 m2     = { 1, 4, 0, 1, 2, 0, 5, 0, 3, 5, 0, 2, 5, 0, 4, 0 };
    Mat4 result = { 0, 2, 1, 16, 0, 19, 0, 0, 0, 6, 2, 20, 0, 22, 0, 0 };

    expect(mat4_is_equal(mat4_mul(mat4_id(), mat4_id()), mat4_id()));
    expect(mat4_is_equal(mat4_mul(mat4_id(), mat4_diag(2.5f)), mat4_diag(2.5f)));
    expect(mat4_is_equal(mat4_mul(m1, m2), result));
}

test(mat4_translation)
{
    f32 t = 10.0f;
    Mat4 result = {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  t, t, t, 1};

    expect(mat4_is_equal(result, mat4_translation(vec3(t, t, t))));
}

suite()
{
    run_test(math_floor_f32);
    run_test(math_floor_f32_to_i32);
    run_test(math_ceil_f32);
    run_test(math_round_f32);
    run_test(math_div_ceil_u32);

    run_test(vec2_is_equal);
    run_test(vec2_add);
    run_test(vec2_sub);
    run_test(vec2_mul);
    run_test(vec2_smul);
    run_test(vec2_dot);
    run_test(vec2_len);
    run_test(vec2_fill);

    run_test(vec3_is_equal);
    run_test(vec3_add);
    run_test(vec3_sub);
    run_test(vec3_mul);
    run_test(vec3_smul);
    run_test(vec3_dot);
    run_test(vec3_len);
    run_test(vec3_cross);

    run_test(vec4_is_equal);
    run_test(vec4_add);
    run_test(vec4_sub);
    run_test(vec4_mul);
    run_test(vec4_smul);
    run_test(vec4_dot);
    run_test(vec4_len);

    run_test(mat4_mul);
    run_test(mat4_translation);
}

#endif /* __RUN_TESTS */
