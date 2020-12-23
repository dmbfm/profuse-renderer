#ifndef __ALLOCATOR_H
#define __ALLOCATOR_H 

#include "common.h"
#include "result.h"
#include "slice.h"
#include <stddef.h>
#include <stdalign.h>

enum
{
    // From this: https://github.com/microsoft/mimalloc/blob/master/include/mimalloc-types.h. Seems reasonable 
    ALLOCATOR_MAX_ALIGNMENT = 8 //alignof(max_align_t),
};

typedef struct Allocator
{
    Result(uptr) (*alloc)(struct Allocator *allocator, usize amount);
    void (*free)(struct Allocator *allocator, uptr region);
    //Result(usize) (*resize)(struct Allocator *allocator,  uptr region, usize new_amount, usize alignment);
    uptr allocator_state;
} Allocator;

Result(uptr) allocacor_alloc(Allocator *allocator, usize amount);

#endif /* __ALLOCATOR_H */
