#ifndef __MATH_H

    #include "common.h"
    #include "result.h"

    #define MATH_I32_MAX 0x7FFFFFFF
    #define MATH_U32_MAX 0xFFFFFFFF
    #define MATH_I64_MAX 0x7FFFFFFFFFFFFFFF
    #define MATH_U64_MAX 0xFFFFFFFFFFFFFFFF

f32 math_floor(f32 x);
f32 math_ceil_f32(f32 x);
f32 math_round_f32(f32 x);
i32 math_floor_f32_to_i32_unsafe(f32 x);
i32 math_ceil_to_i32_unsafe(f32 x);
i32 math_round_f32_to_i32_unsafe(f32 x);
u32 math_div_ceil_u32(u32 a, u32 b);
Result(i32) math_floor_f32_to_i32(f32 x);
Result(i32) math_ceil_f32_to_i32(f32 x);
Result(i32) math_round_f32_to_i32(f32 x);
boolean math_is_equalf(f32 a, f32 b);

f32 math_sqrtf(f32 x);
f32 math_sinf(f32 x);
f32 math_cosf(f32 x);
f32 math_tanf(f32 x);

typedef union Vec2 {
    struct {
        f32 x;
        f32 y;
    };

    f32 values[2];
} Vec2;

typedef union Vec3 {
    struct {
        f32 x;
        f32 y;
        f32 z;
    };

    f32 values[3];
} Vec3;

typedef union Vec4 {
    struct {
        f32 x;
        f32 y;
        f32 z;
        f32 w;
    };

    f32 values[4];
} Vec4;

typedef union Mat4 {
    struct {
        float m11, m21, m31, m41, m12, m22, m32, m42, m13, m23, m33, m43, m14, m24, m34, m44;
    };

    f32 values[16];
} Mat4;

Vec2 vec2(f32 x, f32 y);
Vec2 vec2_fill(f32 x);
Vec2 vec2_zero();
Vec2 vec2_ones();
Vec2 vec2_add(Vec2 a, Vec2 b);
Vec2 vec2_sub(Vec2 a, Vec2 b);
Vec2 vec2_mul(Vec2 a, Vec2 b);
Vec2 vec2_smul(f32 a, Vec2 v);
f32  vec2_dot(Vec2 a, Vec2 b);
f32  vec2_len(Vec2 v);
Vec2 vec2_normalized(Vec2 v);
Vec3 vec3(f32 x, f32 y, f32 z);
Vec3 vec3_zero();
Vec3 vec3_ones();
Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_mul(Vec3 a, Vec3 b);
Vec3 vec3_smul(float x, Vec3 a);

f32     vec3_dot(Vec3 a, Vec3 b);
f32     vec3_len(Vec3 v);
Vec3    vec3_normalized(Vec3 v);
Vec3    vec3_cross(Vec3 a, Vec3 b);
Vec3    vec3_i();
Vec3    vec3_j();
Vec3    vec3_k();
Vec3    vec3_spherical(float polar_angle, float azimuthal_angle, float radius);
boolean vec3_is_equal(Vec3 a, Vec3 b);

Vec4    vec4(f32 x, f32 y, f32 z, f32 w);
Vec4    vec4_zero();
Vec4    vec4_ones();
Vec4    vec4_add(Vec4 a, Vec4 b);
Vec4    vec4_sub(Vec4 a, Vec4 b);
Vec4    vec4_mul(Vec4 a, Vec4 b);
Vec4    vec4_smul(float x, Vec4 a);
f32     vec4_dot(Vec4 a, Vec4 b);
f32     vec4_len(Vec4 v);
Vec4    vec4_normalized(Vec4 v);
Vec4    vec4_i();
Vec4    vec4_j();
Vec4    vec4_k();
Vec4    vec4_w();
Vec4    vec4_transform(Vec4 v, Mat4 m);
boolean vec4_is_equal(Vec4 a, Vec4 b);

Mat4    mat4_id();
Mat4    mat4_diag(f32 v);
boolean mat4_is_equal(Mat4 a, Mat4 b);
Mat4    mat4_mul(Mat4 a, Mat4 b);
Mat4    mat4_translation(Vec3 t);
Mat4    mat4_look_at(Vec3 from, Vec3 to, Vec3 up);
Mat4    mat4_perspective(f32 fov, f32 aspect, f32 n, f32 f);
Mat4    mat4_ortho(f32 width, f32 height, f32 n, f32 f);
Mat4    mat4_rotY(float angle);

    #define __MATH_H
#endif /* __MATH_H */
