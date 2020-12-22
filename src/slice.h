#ifndef __SLICE_H
#define __SLICE_H

#include "common.h"
#include "maybe.h"
#include "result.h"

/**
 *
 * This file defines array slices (or fat arrays?), which are just arrays wich length information and
 * (optional )bounds checking.
 *
 * To disable bounds-checking, just define `SLICE_FAST` somewhere.
 *
 */

#define Slice(type) TOKENPASTE(Slice_, type)

#define slice_make_type(type)  \
    typedef struct Slice(type) \
    {                          \
        usize len;             \
        type *data;            \
    }                          \
    Slice(type);

#define slice_from_array(type, arr, n)                ((Slice(type)){ .len = (n), .data = &(arr)[0] })
#define slice_from_array_range(type, arr, start, end) slice_from_array(type, &arr[start], (end - start))
#define slice_len(s)                                  (s.len)
#define slice_data(s)                                 (s.data)

#ifdef SLICE_FAST
#define slice_get(s, i)    (s.data[(i)])
#define slice_set(s, i, v) (s.data[(i)] = (v))
#else
#define slice_get(s, i)    (((i) < slice_len(s)) ? (s.data[(i)]) : (panic(), s.data[0]))
#define slice_set(s, i, v) (((i) < slice_len(s)) ? (s.data[(i)] = (v)) : (panic(), s.data[0]))
#endif

#define slice_for(name, s) for(usize name = 0; name < slice_len(s); name ++)

slice_make_type(i8);
slice_make_type(i16);
slice_make_type(i32);
slice_make_type(i64);
slice_make_type(u8);
slice_make_type(u16);
slice_make_type(u32);
slice_make_type(u64);
slice_make_type(char);
slice_make_type(usize);
slice_make_type(f32);
slice_make_type(f64);
slice_make_type(Maybe(u32));

result_make_type(Slice(u8));

#endif /* __SLICE_H */
