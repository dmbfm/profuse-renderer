#ifndef __HEAP_H
#define __HEAP_H

#include "allocator.h"
#include "common.h"

// Todo: Have a 'heap_allocator' global depending on the
// platform, insted of 'heap_wasm_free_list_allocator' etc.

extern Allocator heap_allocator;

#ifdef __wasm__
extern Allocator heap_wasm_free_list_allocator;
void             heap_wasm_free_list_print_blocks(
                void (*printfn)(const char *));
void heap_wasm_free_list_print_region_info(
    void (*printfn)(const char *),
    uptr region);
void wasm_free_list_print_all(
    void (*printfn)(const char *));
#endif /* __wasm__ */

#endif /* __HEAP_H */
