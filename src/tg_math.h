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
f32 tgm_sqrtf(float x) { return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(x))); }
#else
#ifdef __wasm__
extern float tg_wasm_js_sqrtf(f32 x);
#define tgm_sqrtf tg_wasm_js_sqrtf
#else
#include <math.h>
#define tgm_sqrtf sqrtf
#endif
#endif

#define TGM_EPS 0.0000001

boolean tgm_is_equalf(f32 x1, f32 x2) {
  f32 d = x1 - x2;

  return ((d > -TGM_EPS) && (d < TGM_EPS));
}

typedef union Vec2 {
  struct {
    f32 x;
    f32 y;
  };

  struct {
    f32 u;
    f32 v;
  };

  f32 values[2];
} Vec2;

typedef union Vec3 {
  struct {
    f32 x;
    f32 y;
    f32 z;
  };

  struct {
    f32 r;
    f32 g;
    f32 b;
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

  struct {
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

Vec2 vec2(f32 x, f32 y) { return (Vec2){x, y}; }

Vec2 vec2_fill(f32 x) { return (Vec2){x, x}; }

Vec2 vec2_zeroes() { return (Vec2){0, 0}; }

Vec2 vec2_ones() { return (Vec2){1, 1}; }

Vec2 vec2_add(Vec2 a, Vec2 b) { return (Vec2){a.x + b.x, a.y + b.y}; }

Vec2 vec2_sub(Vec2 a, Vec2 b) { return (Vec2){a.x - b.x, a.y - b.y}; }

Vec2 vec2_mul(Vec2 a, Vec2 b) { return (Vec2){a.x * b.x, a.y * b.y}; }

Vec2 vec2_smul(f32 a, Vec2 v) { return (Vec2){a * v.x, a * v.y}; }

f32 vec2_dot(Vec2 a, Vec2 b) { return a.x * b.x + a.y * b.y; }

f32 vec2_len(Vec2 v) { return tgm_sqrtf(v.x * v.x + v.y * v.y); }

#define __TG_RUN_TESTS
#ifdef __TG_RUN_TESTS

#ifndef tg_test
#define tg_test(name, ...)                                                     \
  tg_printf("  Testing %s... ", #name);                                        \
  __VA_ARGS__                                                                  \
  tg_printf("Passed!\n");
#endif

void tg_math_test() {
  tg_printf("Running tg_math.h tests...\n");

  tg_test(tgm_is_equalf, {
    tg_assert(tgm_is_equalf(1.0f, 1.0f));
    tg_assert(!tgm_is_equalf(1.0f, 1.0001f));
    tg_assert(!tgm_is_equalf(1.0f, -1.0001f));
    tg_assert(tgm_is_equalf(232323, 232323));
  });

  tg_test(vec2_len, {
    Vec2 v = {1, 0};
    tg_assert(vec2_len(v) == 1.0f);
    Vec2 v2 = {1, 1};
    tg_assert(tgm_is_equalf(vec2_len(v2), 1.41421356237));
  });

  tg_test(vec2_add, {
    Vec2 v1 = {1.0f, 2.0f};
    Vec2 v2 = {3.0f, 4.0f};
    Vec2 v3 = vec2_add(v1, v2);
    tg_assert(tgm_is_equalf(v3.x, 4.0f));
    tg_assert(tgm_is_equalf(v3.y, 6.0f));
  });

  tg_test(vec2_sub, {
    Vec2 v1 = {1.0f, 2.0f};
    Vec2 v2 = {3.0f, 4.0f};
    Vec2 v3 = vec2_sub(v1, v2);
    tg_assert(tgm_is_equalf(v3.x, -2.0f));
    tg_assert(tgm_is_equalf(v3.y, -2.0f));
  });

  tg_printf("All tg_math.h tests passed!\n");
}

#else

void tg_math_test() {}

#endif

#endif
