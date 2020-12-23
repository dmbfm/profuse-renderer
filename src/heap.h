#ifndef __HEAP_H
#define __HEAP_H 

#include "common.h"
#include "allocator.h"

extern Allocator heap_allocator;

#ifdef __wasm__
extern Allocator heap_wasm_free_list_allocator; 
void heap_wasm_free_list_print_blocks(void (*printfn)(const char *));
#endif /* __wasm__ */

#endif /* __HEAP_H */
