#ifndef __ALLOCATOR_H
#define __ALLOCATOR_H 

#include "common.h"
#include "result.h"
#include "slice.h"
#include <stddef.h>
#include <stdalign.h>

enum
{
    ALLOCATOR_MAX_ALIGNMENT = alignof(max_align_t),
};

typedef struct Allocator
{
    Result(uptr) (*alloc)(struct Allocator *allocator, usize amount, usize alignment);
    Result(usize) (*resize)(struct Allocator *allocator,  uptr region, usize new_amount, usize alignment);
    uptr allocator_state;
} Allocator;

Result(uptr) allocacor_alloc(Allocator *allocator, usize amount);

#endif /* __ALLOCATOR_H */
