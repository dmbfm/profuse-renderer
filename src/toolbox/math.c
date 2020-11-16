#include "toolbox.h"
#include "toolbox_common.h"

int t_powi(int n, int x)
{
    int r = 1;
    while (x) {
        r *= n;
        x--;
    };

    return r;
}

boolean tgm_is_equalf(f32 x1, f32 x2)
{
    f32 d = x1 - x2;

    return ((d > -T_EPS) && (d < T_EPS));
}

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
    return t_sqrtf(v.x * v.x + v.y * v.y);
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
    return t_sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
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


Vec3 vec3_spherical(float polar_angle, float azimuthal_angle, float radius)
{
    Vec3 r;

    r.x = radius * t_sinf(polar_angle) * t_cosf(azimuthal_angle);
    r.z = radius * t_sinf(polar_angle) * t_sinf(azimuthal_angle);
    r.y = radius * t_cosf(polar_angle);

    return r;
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
    return t_sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
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

    return r;
}

Mat4 mat4_look_at(Vec3 from, Vec3 to, Vec3 up)
{
    Vec3 k = vec3_normalized(vec3_sub(from, to));
    Vec3 i = vec3_normalized(vec3_cross(up, k));
    Vec3 j = vec3_cross(k, i);
    //i = vec3_cross(j, k);

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

    float c = 1.0f / t_tanf(fov / 2);

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

    float c = t_cosf(angle);
    float s = t_sinf(angle);

    result.m11 = c;
    result.m13 = s; 
    result.m31 = -s;
    result.m33 = c;

    return result;
}

#ifdef TOOLBOX_RUN_TESTS
void toolbox_math_test()
{
    t_printf("Running t_math.h tests...\n");

    t_test(tgm_is_equalf, {
        t_assert(tgm_is_equalf(1.0f, 1.0f));
        t_assert(!tgm_is_equalf(1.0f, 1.0001f));
        t_assert(!tgm_is_equalf(1.0f, -1.0001f));
        t_assert(tgm_is_equalf(232323, 232323));
    });

    t_test(vec2_len, {
        Vec2 v = { 1, 0 };
        t_assert(vec2_len(v) == 1.0f);
        Vec2 v2 = { 1, 1 };
        t_assert(tgm_is_equalf(vec2_len(v2), 1.41421356237));
    });

    t_test(vec2_add, {
        Vec2 v1 = { 1.0f, 2.0f };
        Vec2 v2 = { 3.0f, 4.0f };
        Vec2 v3 = vec2_add(v1, v2);
        t_assert(tgm_is_equalf(v3.x, 4.0f));
        t_assert(tgm_is_equalf(v3.y, 6.0f));
    });

    t_test(vec2_sub, {
        Vec2 v1 = { 1.0f, 2.0f };
        Vec2 v2 = { 3.0f, 4.0f };
        Vec2 v3 = vec2_sub(v1, v2);
        t_assert(tgm_is_equalf(v3.x, -2.0f));
        t_assert(tgm_is_equalf(v3.y, -2.0f));
    });

    t_test(vec2_dot, {
        Vec2 v1 = { 1.0f, 0.f };
        Vec2 v2 = { 0, 0 };
        Vec2 v3 = { 0.5f, 0.5f };
        Vec2 v4 = { 0.75f, 0.25f };

        t_assert(tgm_is_equalf(vec2_dot(v1, v2), 0));
        t_assert(tgm_is_equalf(vec2_dot(v3, v4), 0.5f));
    });

    t_test(vec3_add, {
        Vec3 v1 = { 1.0f, 2.0f, 3.0f };
        Vec3 v2 = { 3.0f, 4.0f, 5.0f };
        Vec3 v3 = vec3_add(v1, v2);
        t_assert(vec3_is_equal(v3, (Vec3){ 4.0f, 6.0f, 8.0f }));
    });

    t_test(vec3_add, {
        Vec3 v1 = { 1.0f, 2.0f, 3.0f };
        Vec3 v2 = { 3.0f, 4.0f, 5.0f };
        Vec3 v3 = vec3_sub(v1, v2);
        t_assert(vec3_is_equal(v3, (Vec3){ -2, -2, -2 }));
    });

    t_test(vec3_mul, {
        Vec3 v1 = { 1.0f, 2.0f, 3.0f };
        Vec3 v2 = { 3.0f, 4.0f, 5.0f };
        Vec3 v3 = vec3_mul(v1, v2);
        t_assert(vec3_is_equal(v3, (Vec3){ 3, 8, 15 }));
    });

    t_test(vec3_smul, {
        Vec3 v2 = { 3.0f, 4.0f, 5.0f };
        Vec3 v3 = vec3_smul(2, v2);
        t_assert(vec3_is_equal(v3, (Vec3){ 6, 8, 10 }));
    });

    t_test(vec3_dot, {
        t_assert(tgm_is_equalf(vec3_dot(vec3_i(), vec3_i()), 1));
        t_assert(tgm_is_equalf(vec3_dot(vec3_i(), vec3_j()), 0));
        t_assert(tgm_is_equalf(vec3_dot(vec3_i(), vec3_k()), 0));
        t_assert(tgm_is_equalf(vec3_dot(vec3_j(), vec3_j()), 1));
        t_assert(tgm_is_equalf(vec3_dot(vec3_j(), vec3_k()), 0));
        t_assert(tgm_is_equalf(vec3_dot(vec3_k(), vec3_k()), 1));
    });

    t_test(vec3_len, { t_assert(tgm_is_equalf(vec3_len(vec3(1.0f, 2.0f, 3.0f)), 3.741657386f)); });

    t_test(vec3_cross, {
        t_assert(vec3_is_equal(vec3_cross(vec3_i(), vec3_j()), vec3_k()));
        t_assert(vec3_is_equal(vec3_cross(vec3_k(), vec3_i()), vec3_j()));
        t_assert(vec3_is_equal(vec3_cross(vec3_j(), vec3_k()), vec3_i()));
    });

    t_test(vec3_normalized, { t_assert(tgm_is_equalf(vec3_len(vec3_normalized(vec3(234.34, 32.23, -234.3))), 1)); });

    t_test(vec4_add, {
        Vec4 v1 = { 1.0f, 2.0f, 3.0f, 7.0f };
        Vec4 v2 = { 3.0f, 4.0f, 5.0f, 9.0f };
        Vec4 v3 = vec4_add(v1, v2);
        t_assert(vec4_is_equal(v3, (Vec4){ 4.0f, 6.0f, 8.0f, 16 }));
    });

    t_test(vec4_add, {
        Vec4 v1 = { 1.0f, 2.0f, 3.0f, 7.0f };
        Vec4 v2 = { 3.0f, 4.0f, 5.0f, 9.0f };
        Vec4 v3 = vec4_sub(v1, v2);
        t_assert(vec4_is_equal(v3, (Vec4){ -2, -2, -2, -2 }));
    });

    t_test(vec4_mul, {
        Vec4 v1 = { 1.0f, 2.0f, 3.0f, 7.0f };
        Vec4 v2 = { 3.0f, 4.0f, 5.0f, 9.0f };
        Vec4 v3 = vec4_mul(v1, v2);
        t_assert(vec4_is_equal(v3, (Vec4){ 3, 8, 15, 63 }));
    });

    t_test(vec4_smul, {
        Vec4 v2 = { 3.0f, 4.0f, 5.0f, 6.0f };
        Vec4 v3 = vec4_smul(2, v2);
        t_assert(vec4_is_equal(v3, (Vec4){ 6, 8, 10, 12 }));
    });

    t_test(vec4_dot, {
        t_assert(tgm_is_equalf(vec4_dot(vec4_i(), vec4_i()), 1));
        t_assert(tgm_is_equalf(vec4_dot(vec4_i(), vec4_j()), 0));
        t_assert(tgm_is_equalf(vec4_dot(vec4_i(), vec4_k()), 0));
        t_assert(tgm_is_equalf(vec4_dot(vec4_j(), vec4_j()), 1));
        t_assert(tgm_is_equalf(vec4_dot(vec4_j(), vec4_k()), 0));
        t_assert(tgm_is_equalf(vec4_dot(vec4_k(), vec4_k()), 1));
    });

    t_test(vec4_len, { t_assert(tgm_is_equalf(vec4_len(vec4(1.0f, 2.0f, 3.0f, 5.0f)), 6.2449979984)); });

    t_test(vec4_normalized,
           { t_assert(tgm_is_equalf(vec4_len(vec4_normalized(vec4(234.34, 32.23, -234.3, 3.34))), 1)); });

    // TODO: Add better tests?j
    t_test(mat4_mul, {
        Mat4 m1     = { 0, 2, 0, 0, 0, 0, 0, 4, 0, 3, 0, 0, 0, 0, 1, 0 };
        Mat4 m2     = { 1, 4, 0, 1, 2, 0, 5, 0, 3, 5, 0, 2, 5, 0, 4, 0 };
        Mat4 result = { 0, 2, 1, 16, 0, 19, 0, 0, 0, 6, 2, 20, 0, 22, 0, 0 };
        t_assert(mat4_is_equal(mat4_mul(mat4_id(), mat4_id()), mat4_id()));
        t_assert(mat4_is_equal(mat4_mul(mat4_id(), mat4_diag(2.5f)), mat4_diag(2.5f)));
        t_assert(mat4_is_equal(mat4_mul(m1, m2), result));
    });

    t_printf("All t_math.h tests passed!\n");
}

#else
void toolbox_math_test() {}
#endif
