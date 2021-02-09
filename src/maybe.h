#ifndef __MAYBE_H
#define __MAYBE_H

#include "common.h"

#define Maybe(type)           TOKENPASTE(Maybe_, type)
#define maybe_some(type, v)   ((Maybe(type)){.is_something = true, .value = v})
#define maybe_nothing(type)   ((Maybe(type)){0})
#define maybe_is_something(m) (m.is_something != false)
#define maybe_is_nothing(m)   (m.is_something == false)
#define maybe_unwrap(m)       (maybe_is_nothing(m) ? panic() : 0, m.value)
#define maybe_ifsome(m)       if (maybe_is_something(m))
#define maybe_set_some(m, v)  (m.is_something = true, m.value = v)
#define maybe_set_nothing(m)  m.is_something = false;
#define maybe_map(type, m, f) (maybe_is_something(m) ? maybe_some(type, f(m.value)) : maybe_nothing(type))
#define maybe_lift(type, m1, m2, f) \
    (maybe_is_something(m1) && maybe_is_something(m2) ? maybe_some(type, f(m1.value, m2.value)) : maybe_nothing(type))

#define maybe_make_type(type) \
    typedef struct {          \
        u8   is_something;    \
        type value;           \
    } Maybe(type)

maybe_make_type(u8);
maybe_make_type(u32);
maybe_make_type(i32);
maybe_make_type(f32);
maybe_make_type(char);
maybe_make_type(charptr);

#endif /* __MAYBE_H */
