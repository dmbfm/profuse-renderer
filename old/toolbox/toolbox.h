#if !defined(TOOLBOX_H)
#define TOOLBOX_H

/**
 *
 * Configuration preprocessor defines:
 *
 * TOOLBOX_NO_STD_LIB: Don't use the C std lib. Disables file/io functions.
 * TOOLBOX_DEBUG: Enable debug/assert
 *
 */

#include "toolbox_common.h"

#ifdef __wasm__
#define TOOLBOX_NO_STD_LIB
#endif

#ifndef TOOLBOX_NO_STD_LIB
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#endif

// Some general-purpose helpful macros
#define t_max(x, y) (((x) >= (y)) ? (x) : (y))
#define t_min(x, y) (((x) >= (y)) ? (y) : (x))
#define t_abs(x)    (((x) > 0) ? (x) : (-(x)))
#define t_isnum(x)  ((x) >= '0' && (x) <= '9')
#define t_clamp(x, a, b) (t_max(t_min((x), b), a))

#define forn(name, N) for (int name = 0; name < (N); name++)

#ifndef TOOLBOX_NO_STD_LIB
uint t_file_len(FILE *f);
char **t_read_file_lines(char *filename, int *out_num_lines);
#endif

#ifdef TOOLBOX_NO_STD_LIB
int t__snprintf(char *s, size_t n, const char *fmt, ...);
void *t__memcpy(void *dst, void *src, size_t num)
{
    u8 *d = (u8 *)dst;
    u8 *s = (u8 *)src;

    while (num) {
        *d++ = *s++;
        num--;
    }

    return dst;
}

size_t t__strlen(const char *s)
{
    size_t len = 0;
    while(*s++) len++;

    return len;
}

void *t__memchr (void * ptr, int value, size_t num)
{
    u8 uv = (u8) value;
    u8 *p = (u8 *) ptr;

    while(num)
    {
        if (*p == uv) return (void *)p; 
        p++;
        num--;
    }

    return 0;
}


char *strncpy(char *restrict dest, const char *restrict src, size_t count )
{
    return (char *) t__memcpy((void *) dest, (void *) src, count);
}


int strncmp( const char *lhs, const char *rhs, size_t count ) 
{
    char l, r;

    while(count)
    {
        l = *lhs++;
        r = *rhs++;

        if (l != r)
        {
            return (r - l);
        }

        count--;
    }

    return 0;
} 

#endif


#ifdef __wasm__
#define t_snprintf t__snprintf
#define t_memcpy   t__memcpy
#define t_strlen t__strlen
#define t_memchr t__memchr
#define t_strncpy t__strncpy
#endif

#ifndef t_strncpy
#define t_strncpy strncpy
#endif

#ifndef t_memchr
#define t_memchr memchr
#endif

#ifndef t_strlen
#define t_strlen strlen
#endif

#ifndef t_snprintf
#define t_snprintf snprintf
#endif

#ifndef t_printf
#define t_printf printf
#endif

#ifndef t_malloc
#define t_malloc malloc
#endif

#ifndef t_realloc
#define t_realloc realloc
#endif

#ifndef t_free
#define t_free free
#endif

#ifndef t_fopen
#define t_fopen fopen
#endif

#ifndef t_fseek
#define t_fseek fskeek
#endif

#ifndef t_memcpy
#define t_memcpy memcpy
#endif

#ifndef t_sqrtf
#define t_sqrtf sqrtf
#endif

#ifndef t_tanf
#define t_tanf tanf
#endif

#ifndef t_cosf
#define t_cosf cosf
#endif

#ifndef t_sinf
#define t_sinf sinf
#endif

#ifndef t_memset
#define t_memset memset
#endif

#define t_test(name, ...)                \
    t_printf("  Testing %s... ", #name); \
    __VA_ARGS__                          \
    t_printf("Passed!\n");

// Stretchy buffer
#define t_arr_push(a, v) (t__arr_push(a, v))
#define t_arr_len(a)     ((a) ? (t__arr_len(a)) : 0)
#define t_arr_last(a)    ((a)[t_arr_len(a) - 1])
#define t_arr_free(a)                   \
    {                                   \
        if (a)                          \
            t_free(t__arr_header((a))); \
    }
#define t_arr_pop(a) t__arr_pop(a)

#define t__arr_header(a) (((int *)(a)) - 2)
#define t__arr_cap(a)    (t__arr_header(a)[0])
#define t__arr_len(a)    (t__arr_header(a)[1])
#define t__arr_pop(a)    ((a) ? (a)[t__arr_len(a)--] : 0)

#define t__arr_push(a, v)           (t__arr_grow_if_needed(a, 1), (a)[t__arr_len(a)++] = (v))
#define t__arr_needs_grow(a, n)     (((a) == 0) || (t__arr_len(a) + n >= t__arr_cap(a)))
#define t__arr_grow_if_needed(a, n) (t__arr_needs_grow(a, n) ? t__arr_growa(a, n) : 0)
#define t__arr_growa(a, n)          ((a) = t__arr_grow((a), n, sizeof(*(a))))

void *t__arr_grow(void *arr, int n, int element_size)
{
    int current_cap_double = arr ? 2 * t__arr_cap(arr) : 0;
    int min_needed         = t_arr_len(arr) + n;
    int increment          = t_max(current_cap_double, min_needed);

    int *new_buffer = (int *)t_realloc(arr ? t__arr_header(arr) : 0, 2 * sizeof(int) + (increment * element_size));

    if (!new_buffer) {
        return (void *)0;
    }

    if (!arr) {
        new_buffer[1] = 0;
    }

    new_buffer[0] = increment;

    return (void *)(new_buffer + 2);
}

#define T_EPS        0.0000001
#define T_PI         3.14159265358979323846
#define T_DEG2RAD(x) ((x) * (T_PI / 180.0))
#define T_RAD2DEG(x) ((x) * (180.0 / T_PI))

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
typedef Vec4 Quaternion;

typedef union Mat4
{
    struct
    {
        float m11, m21, m31, m41, m12, m22, m32, m42, m13, m23, m33, m43, m14, m24, m34, m44;
    };

    f32 values[16];
} Mat4;

int t_powi(int n, int x);
boolean tgm_is_equalf(f32 x1, f32 x2);

Vec2 vec2(f32 x, f32 y);
Vec2 vec2_fill(f32 x);
Vec2 vec2_zero();
Vec2 vec2_ones();
Vec2 vec2_add(Vec2 a, Vec2 b);
Vec2 vec2_sub(Vec2 a, Vec2 b);
Vec2 vec2_mul(Vec2 a, Vec2 b);
Vec2 vec2_smul(f32 a, Vec2 v);
f32 vec2_dot(Vec2 a, Vec2 b);
f32 vec2_len(Vec2 v);
Vec2 vec2_normalized(Vec2 v);
Vec3 vec3(f32 x, f32 y, f32 z);
Vec3 vec3_zero();
Vec3 vec3_ones();
Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_mul(Vec3 a, Vec3 b);
Vec3 vec3_smul(float x, Vec3 a);

f32 vec3_dot(Vec3 a, Vec3 b);
f32 vec3_len(Vec3 v);
Vec3 vec3_normalized(Vec3 v);
Vec3 vec3_cross(Vec3 a, Vec3 b);
Vec3 vec3_i();
Vec3 vec3_j();
Vec3 vec3_k();
Vec3 vec3_spherical(float polar_angle, float azimuthal_angle, float radius);
boolean vec3_is_equal(Vec3 a, Vec3 b);

Vec4 vec4(f32 x, f32 y, f32 z, f32 w);
Vec4 vec4_zero();
Vec4 vec4_ones();
Vec4 vec4_add(Vec4 a, Vec4 b);
Vec4 vec4_sub(Vec4 a, Vec4 b);
Vec4 vec4_mul(Vec4 a, Vec4 b);
Vec4 vec4_smul(float x, Vec4 a);
f32 vec4_dot(Vec4 a, Vec4 b);
f32 vec4_len(Vec4 v);
Vec4 vec4_normalized(Vec4 v);
Vec4 vec4_i();
Vec4 vec4_j();
Vec4 vec4_k();
Vec4 vec4_w();
Vec4 vec4_transform(Vec4 v, Mat4 m);
boolean vec4_is_equal(Vec4 a, Vec4 b);

#define quaternion_add  vec4_add
#define quaternion_sub  vec4_sub
#define quaternion_smul vec4_smul

// Quaternion quaternion_mul(Quaternion a, Quaternion b);
// Quaternion quaternion_conjugate(Quaternion a, Quaternion b);
// Quaternion quaternion_norm(Quaternion a);
// Quaternion quaternion_inv(Quaternion a);
// Quaternion quaternion_normalize(Quaternion a);
// Quaternion quaternion_from_axis_angle(Vec3 axis, float angle);

Mat4 mat4_id();
Mat4 mat4_diag(f32 v);
boolean mat4_is_equal(Mat4 a, Mat4 b);
Mat4 mat4_mul(Mat4 a, Mat4 b);
Mat4 mat4_translation(Vec3 t);
Mat4 mat4_look_at(Vec3 from, Vec3 to, Vec3 up);
Mat4 mat4_perspective(f32 fov, f32 aspect, f32 n, f32 f);
Mat4 mat4_ortho(f32 width, f32 height, f32 n, f32 f);
Mat4 mat4_rotY(float angle);

#endif
