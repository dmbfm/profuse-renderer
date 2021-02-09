#ifndef __ALLOCATOR_H
#define __ALLOCATOR_H

#include "common.h"
#include "result.h"
#include "slice.h"
#include <stddef.h>

typedef struct Allocator {
    Result(uptr) (*alloc)(struct Allocator *allocator, usize amount);
    Result(uptr) (*realloc)(struct Allocator *allocator, uptr region, usize amount);
    void (*free)(struct Allocator *allocator, uptr region);
    uptr allocator_state;
} Allocator;

Result(uptr) allocacor_alloc(Allocator *allocator, usize amount);

#endif /* __ALLOCATOR_H */
