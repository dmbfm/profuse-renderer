#ifndef __HEAP_H
#define __HEAP_H 

#include "common.h"
#include "allocator.h"

extern Allocator heap_allocator;

#ifdef __wasm__
extern Allocator heap_wasm_free_list_allocator; 
#endif /* __wasm__ */

#endif /* __HEAP_H */
