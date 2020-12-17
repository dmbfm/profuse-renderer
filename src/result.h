#ifndef __RESULT_H
#define __RESULT_H

#include "common.h"
#include "error.h"

#define Result(type)           Result_##type
#define result_is_ok(r)        (r.is_error != true)
#define result_is_error(r)     (r.is_error == true)
#define result_ok(type, v)     ((Result(type)){ .is_error = false, .value = v, .error_code = ERR_OK })
#define result_error(type, ec) ((Result(type)){ .is_error = true, .error_code = ec })
#define result_ifok(r)         if (result_is_ok(r))
#define result_unwrap(r)       (result_is_error(r) ? panic() : 0, r.value)

#define result_set_ok(r, v) \
    r.is_error   = false;   \
    r.value      = v;       \
    r.error_code = ERR_OK
#define result_set_error(r, ec) \
    r.is_error   = true;        \
    r.error_code = ec

#define result_make_type(type) \
    typedef struct             \
    {                          \
        boolean is_error;      \
        type value;            \
        u32 error_code;        \
    } Result(type)

result_make_type(u8);
result_make_type(u16);
result_make_type(u32);
result_make_type(i8);
result_make_type(i16);
result_make_type(i32);
result_make_type(uptr);
result_make_type(char);
result_make_type(charptr);
result_make_type(usize);

#endif /* __RESULT_H */
