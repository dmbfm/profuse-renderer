#ifndef __SLICE_H
#define __SLICE_H

#include "common.h"
#include "maybe.h"

/**
 *
 * This file defines array slices (or fat arrays?), which are just arrays wich length information and
 * (optional )bounds checking.
 *
 * To disable bounds-checking, just define `SLICE_FAST` somewhere.
 *
 */

#define Slice(type) Slice__##type

#define make_slice_type(type)  \
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

make_slice_type(i8);
make_slice_type(i16);
make_slice_type(i32);
make_slice_type(i64);
make_slice_type(u8);
make_slice_type(u16);
make_slice_type(u32);
make_slice_type(u64);
make_slice_type(char);
make_slice_type(usize);
make_slice_type(f32);
make_slice_type(f64);
make_slice_type(Maybe(u32));

#endif /* __SLICE_H */
