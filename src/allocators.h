#ifndef __ALLOCATORS_H
#define __ALLOCATORS_H

#include "common.h"
#include "result.h"

typedef struct
{
    Result(uptr) (*alloc)(usize);
    void (*free)(uptr);
} AllocatorsChildAllocator;

typedef struct
{
    uptr base;
    usize current_offset;
    usize capacity;
    boolean initialized;
    AllocatorsChildAllocator child_allocator;
} AllocatorsLinear;

ErrorCode allocators_linear_init(AllocatorsLinear *allocator, AllocatorsChildAllocator child_allocator,
                                 usize initial_capacity);
void allocators_linear_deinit(AllocatorsLinear *allocator);
Result(uptr) allocators_linear_alloc(AllocatorsLinear *allocator, usize amount);

#endif /* __ALLOCATORS_H */
