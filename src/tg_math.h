#if !defined(__TG_MATH_H)
#define __TG_MATH_H

#include "tg.h"
#include "tg_types.h"

#define __TGM_USE_INTRINSICS

#ifdef __wasm__
#undef __TGM_USE_INTRINSICS
#endif

#ifdef __TGM_USE_INTRINSICS
#include <xmmintrin.h>
f32 tgm_sqrtf(float x)
{
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(x)));
}
#else
#ifdef __wasm__
extern float tg__wasm_js_sqrtf(f32 x);

TG_WASM_JS(function tg__wasm_js_sqrtf(x) { return Math.sqrt(x); })

#define tgm_sqrtf tg__wasm_js_sqrtf
#else
#include <math.h>
#define tgm_sqrtf sqrtf
#endif
#endif

#define TGM_EPS 0.0000001

boolean tgm_is_equalf(f32 x1, f32 x2)
{
    f32 d = x1 - x2;

    return ((d > -TGM_EPS) && (d < TGM_EPS));
}

typedef union Vec2
{
    struct
    {
        f32 x;
        f32 y;
    };

    struct
    {
        f32 u;
        f32 v;
    };

    f32 values[2];
} Vec2;

typedef union Vec3
{
    struct
    {
        f32 x;
        f32 y;
        f32 z;
    };

    struct
    {
        f32 r;
        f32 g;
        f32 b;
    };

    f32 values[3];
} Vec3;

typedef union Vec4
{
    struct
    {
        f32 x;
        f32 y;
        f32 z;
        f32 w;
    };

    struct
    {
        f32 r;
        f32 g;
        f32 b;
        f32 a;
    };

    f32 values[4];
} Vec4;

typedef Vec2 Point2;
typedef Vec2 TexCoord;
typedef Vec3 Point3;
typedef Vec3 Color3f;
typedef Vec3 Color4f;

typedef union Mat4
{
    struct
    {
        float m11, m21, m31, m41, m12, m22, m32, m42, m13, m23, m33, m43, m14, m24, m34, m44; 
    };

    f32 values[16];
} Mat4;

Vec2 vec2(f32 x, f32 y)
{
    return (Vec2){ x, y };
}

Vec2 vec2_fill(f32 x)
{
    return (Vec2){ x, x };
}

Vec2 vec2_zero()
{
    return (Vec2){ 0, 0 };
}

Vec2 vec2_ones()
{
    return (Vec2){ 1, 1 };
}

Vec2 vec2_add(Vec2 a, Vec2 b)
{
    return (Vec2){ a.x + b.x, a.y + b.y };
}

Vec2 vec2_sub(Vec2 a, Vec2 b)
{
    return (Vec2){ a.x - b.x, a.y - b.y };
}

Vec2 vec2_mul(Vec2 a, Vec2 b)
{
    return (Vec2){ a.x * b.x, a.y * b.y };
}

Vec2 vec2_smul(f32 a, Vec2 v)
{
    return (Vec2){ a * v.x, a * v.y };
}

f32 vec2_dot(Vec2 a, Vec2 b)
{
    return a.x * b.x + a.y * b.y;
}

f32 vec2_len(Vec2 v)
{
    return tgm_sqrtf(v.x * v.x + v.y * v.y);
}

Vec2 vec2_normalized(Vec2 v)
{
    return vec2_smul(1.0f / vec2_len(v), v);
}

Vec3 vec3(f32 x, f32 y, f32 z)
{
    return (Vec3){ x, y, z };
}

Vec3 vec3_zero()
{
    return (Vec3){ 0 };
}

Vec3 vec3_ones()
{
    return (Vec3){ 1, 1, 1 };
}

Vec3 vec3_add(Vec3 a, Vec3 b)
{
    return (Vec3){ a.x + b.x, a.y + b.y, a.z + b.z };
}

Vec3 vec3_sub(Vec3 a, Vec3 b)
{
    return (Vec3){ a.x - b.x, a.y - b.y, a.z - b.z };
}

Vec3 vec3_mul(Vec3 a, Vec3 b)
{
    return (Vec3){ a.x * b.x, a.y * b.y, a.z * b.z };
}

Vec3 vec3_smul(float x, Vec3 a)
{
    return (Vec3){ x * a.x, x * a.y, x * a.z };
}

f32 vec3_dot(Vec3 a, Vec3 b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

f32 vec3_len(Vec3 v)
{
    return tgm_sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vec3 vec3_normalized(Vec3 v)
{
    return vec3_smul(1.0f / vec3_len(v), v);
}

Vec3 vec3_cross(Vec3 a, Vec3 b)
{
    return (Vec3){ a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}

Vec3 vec3_i()
{
    return (Vec3){ 1, 0, 0 };
}

Vec3 vec3_j()
{
    return (Vec3){ 0, 1, 0 };
}

Vec3 vec3_k()
{
    return (Vec3){ 0, 0, 1 };
}

boolean vec3_is_equal(Vec3 a, Vec3 b)
{
    return tgm_is_equalf(a.x, b.x) && tgm_is_equalf(a.y, b.y) && tgm_is_equalf(a.z, b.z);
}

Vec4 vec4(f32 x, f32 y, f32 z, f32 w)
{
    return (Vec4){ x, y, z, w };
}

Vec4 vec4_zero()
{
    return (Vec4){ 0 };
}

Vec4 vec4_ones()
{
    return (Vec4){ 1, 1, 1, 1 };
}

Vec4 vec4_add(Vec4 a, Vec4 b)
{
    return (Vec4){ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

Vec4 vec4_sub(Vec4 a, Vec4 b)
{
    return (Vec4){ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

Vec4 vec4_mul(Vec4 a, Vec4 b)
{
    return (Vec4){ a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
}

Vec4 vec4_smul(float x, Vec4 a)
{
    return (Vec4){ x * a.x, x * a.y, x * a.z, x * a.w };
}

f32 vec4_dot(Vec4 a, Vec4 b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

f32 vec4_len(Vec4 v)
{
    return tgm_sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

Vec4 vec4_normalized(Vec4 v)
{
    return vec4_smul(1.0f / vec4_len(v), v);
}

Vec4 vec4_i()
{
    return (Vec4){ 1, 0, 0 };
}

Vec4 vec4_j()
{
    return (Vec4){ 0, 1, 0 };
}

Vec4 vec4_k()
{
    return (Vec4){ 0, 0, 1 };
}

Vec4 vec4_w()
{
    return (Vec4){ 0, 0, 0, 1 };
}

boolean vec4_is_equal(Vec4 a, Vec4 b)
{
    return tgm_is_equalf(a.x, b.x) && tgm_is_equalf(a.y, b.y) && tgm_is_equalf(a.z, b.z) && tgm_is_equalf(a.w, b.w);
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

    for (int i = 0; i < 16; i++)
    {
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
    return (Vec4) {
        v.x * m.m11 + v.y * m.m12 * v.z * m.m13 + v.w * m.m14,
        v.x * m.m21 + v.y * m.m22 * v.z * m.m23 + v.w * m.m24,
        v.x * m.m31 + v.y * m.m32 * v.z * m.m33 + v.w * m.m34,
        v.x * m.m41 + v.y * m.m42 * v.z * m.m43 + v.w * m.m44,
    };
}

Mat4 mat4_translation(Vec3 t)
{
    Mat4 r = mat4_id();

    r.m14 = t.x; 
    r.m24 = t.y; 
    r.m34 = t.z; 

    return  r;
}

#define __TG_RUN_TESTS
#ifdef __TG_RUN_TESTS

#ifndef tg_test
#define tg_test(name, ...)                \
    tg_printf("  Testing %s... ", #name); \
    __VA_ARGS__                           \
    tg_printf("Passed!\n");
#endif

void tg_math_test()
{
    tg_printf("Running tg_math.h tests...\n");

    tg_test(tgm_is_equalf, {
        tg_assert(tgm_is_equalf(1.0f, 1.0f));
        tg_assert(!tgm_is_equalf(1.0f, 1.0001f));
        tg_assert(!tgm_is_equalf(1.0f, -1.0001f));
        tg_assert(tgm_is_equalf(232323, 232323));
    });

    tg_test(vec2_len, {
        Vec2 v = { 1, 0 };
        tg_assert(vec2_len(v) == 1.0f);
        Vec2 v2 = { 1, 1 };
        tg_assert(tgm_is_equalf(vec2_len(v2), 1.41421356237));
    });

    tg_test(vec2_add, {
        Vec2 v1 = { 1.0f, 2.0f };
        Vec2 v2 = { 3.0f, 4.0f };
        Vec2 v3 = vec2_add(v1, v2);
        tg_assert(tgm_is_equalf(v3.x, 4.0f));
        tg_assert(tgm_is_equalf(v3.y, 6.0f));
    });

    tg_test(vec2_sub, {
        Vec2 v1 = { 1.0f, 2.0f };
        Vec2 v2 = { 3.0f, 4.0f };
        Vec2 v3 = vec2_sub(v1, v2);
        tg_assert(tgm_is_equalf(v3.x, -2.0f));
        tg_assert(tgm_is_equalf(v3.y, -2.0f));
    });

    tg_test(vec2_dot, {
        Vec2 v1 = { 1.0f, 0.f };
        Vec2 v2 = { 0, 0 };
        Vec2 v3 = { 0.5f, 0.5f };
        Vec2 v4 = { 0.75f, 0.25f };

        tg_assert(tgm_is_equalf(vec2_dot(v1, v2), 0));
        tg_assert(tgm_is_equalf(vec2_dot(v3, v4), 0.5f));
    });

    tg_test(vec3_add, {
        Vec3 v1 = { 1.0f, 2.0f, 3.0f };
        Vec3 v2 = { 3.0f, 4.0f, 5.0f };
        Vec3 v3 = vec3_add(v1, v2);
        tg_assert(vec3_is_equal(v3, (Vec3){ 4.0f, 6.0f, 8.0f }));
    });

    tg_test(vec3_add, {
        Vec3 v1 = { 1.0f, 2.0f, 3.0f };
        Vec3 v2 = { 3.0f, 4.0f, 5.0f };
        Vec3 v3 = vec3_sub(v1, v2);
        tg_assert(vec3_is_equal(v3, (Vec3){ -2, -2, -2 }));
    });

    tg_test(vec3_mul, {
        Vec3 v1 = { 1.0f, 2.0f, 3.0f };
        Vec3 v2 = { 3.0f, 4.0f, 5.0f };
        Vec3 v3 = vec3_mul(v1, v2);
        tg_assert(vec3_is_equal(v3, (Vec3){ 3, 8, 15 }));
    });

    tg_test(vec3_smul, {
        Vec3 v2 = { 3.0f, 4.0f, 5.0f };
        Vec3 v3 = vec3_smul(2, v2);
        tg_assert(vec3_is_equal(v3, (Vec3){ 6, 8, 10 }));
    });

    tg_test(vec3_dot, {
        tg_assert(tgm_is_equalf(vec3_dot(vec3_i(), vec3_i()), 1));
        tg_assert(tgm_is_equalf(vec3_dot(vec3_i(), vec3_j()), 0));
        tg_assert(tgm_is_equalf(vec3_dot(vec3_i(), vec3_k()), 0));
        tg_assert(tgm_is_equalf(vec3_dot(vec3_j(), vec3_j()), 1));
        tg_assert(tgm_is_equalf(vec3_dot(vec3_j(), vec3_k()), 0));
        tg_assert(tgm_is_equalf(vec3_dot(vec3_k(), vec3_k()), 1));
    });

    tg_test(vec3_len, { tg_assert(tgm_is_equalf(vec3_len(vec3(1.0f, 2.0f, 3.0f)), 3.741657386f)); });

    tg_test(vec3_cross, {
        tg_assert(vec3_is_equal(vec3_cross(vec3_i(), vec3_j()), vec3_k()));
        tg_assert(vec3_is_equal(vec3_cross(vec3_k(), vec3_i()), vec3_j()));
        tg_assert(vec3_is_equal(vec3_cross(vec3_j(), vec3_k()), vec3_i()));
    });

    tg_test(vec3_normalized, { tg_assert(tgm_is_equalf(vec3_len(vec3_normalized(vec3(234.34, 32.23, -234.3))), 1)); });

    tg_test(vec4_add, {
        Vec4 v1 = { 1.0f, 2.0f, 3.0f, 7.0f };
        Vec4 v2 = { 3.0f, 4.0f, 5.0f, 9.0f };
        Vec4 v3 = vec4_add(v1, v2);
        tg_assert(vec4_is_equal(v3, (Vec4){ 4.0f, 6.0f, 8.0f, 16 }));
    });

    tg_test(vec4_add, {
        Vec4 v1 = { 1.0f, 2.0f, 3.0f, 7.0f };
        Vec4 v2 = { 3.0f, 4.0f, 5.0f, 9.0f };
        Vec4 v3 = vec4_sub(v1, v2);
        tg_assert(vec4_is_equal(v3, (Vec4){ -2, -2, -2, -2 }));
    });

    tg_test(vec4_mul, {
        Vec4 v1 = { 1.0f, 2.0f, 3.0f, 7.0f };
        Vec4 v2 = { 3.0f, 4.0f, 5.0f, 9.0f };
        Vec4 v3 = vec4_mul(v1, v2);
        tg_assert(vec4_is_equal(v3, (Vec4){ 3, 8, 15, 63 }));
    });

    tg_test(vec4_smul, {
        Vec4 v2 = { 3.0f, 4.0f, 5.0f, 6.0f };
        Vec4 v3 = vec4_smul(2, v2);
        tg_assert(vec4_is_equal(v3, (Vec4){ 6, 8, 10, 12 }));
    });

    tg_test(vec4_dot, {
        tg_assert(tgm_is_equalf(vec4_dot(vec4_i(), vec4_i()), 1));
        tg_assert(tgm_is_equalf(vec4_dot(vec4_i(), vec4_j()), 0));
        tg_assert(tgm_is_equalf(vec4_dot(vec4_i(), vec4_k()), 0));
        tg_assert(tgm_is_equalf(vec4_dot(vec4_j(), vec4_j()), 1));
        tg_assert(tgm_is_equalf(vec4_dot(vec4_j(), vec4_k()), 0));
        tg_assert(tgm_is_equalf(vec4_dot(vec4_k(), vec4_k()), 1));
    });

    tg_test(vec4_len, { tg_assert(tgm_is_equalf(vec4_len(vec4(1.0f, 2.0f, 3.0f, 5.0f)), 6.2449979984)); });

    tg_test(vec4_normalized,
            { tg_assert(tgm_is_equalf(vec4_len(vec4_normalized(vec4(234.34, 32.23, -234.3, 3.34))), 1)); });

    // TODO: Add better tests?j
    tg_test(mat4_mul, {
        Mat4 m1 = { 0, 2, 0, 0, 0, 0, 0, 4, 0, 3, 0, 0, 0, 0, 1, 0 };
        Mat4 m2 = { 1, 4, 0, 1, 2, 0, 5, 0, 3, 5, 0, 2, 5, 0, 4, 0 };
        Mat4 result = { 0, 2, 1, 16, 0, 19, 0, 0, 0, 6, 2, 20, 0, 22, 0, 0 };
        tg_assert(mat4_is_equal(mat4_mul(mat4_id(), mat4_id()), mat4_id()));
        tg_assert(mat4_is_equal(mat4_mul(mat4_id(), mat4_diag(2.5f)), mat4_diag(2.5f)));
        tg_assert(mat4_is_equal(mat4_mul(m1, m2), result));
            });

    tg_printf("All tg_math.h tests passed!\n");
}

#else

void tg_math_test() {}

#endif

#endif
