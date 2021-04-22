#include "allocator.h"
#include "common.h"
#include "error.h"
#include "format.h"
#include "heap.h"
#include "math.h"
#include "mem_common.h"
#include "result.h"
#include "utils.h"

/* TODO:
 * - Sanity tests?
 * - Record allocations for debugging, like in stb.h's
 * - Check alignment on free list allocations
 * malloc wrapper
 */

extern u8 __heap_base;

#define HEAP_WASM_HEAP_BASE_UPTR ((uptr)&__heap_base)

extern u32 wasm_get_memory_size();
extern u32 wasm_memory_grow(u32);

enum { HEAP_WASM_PAGE_SIZE = KB(64), HEAP_WASM_MAX_ALIGN = 8 };

typedef struct HeapWasmMemory {
    uptr    start;
    uptr    end;
    usize   capacity;
    usize   num_pages;
    usize   bytes_per_page;
    boolean initialized;
} HeapWasmMemory;

void heap_wasm_memory_init(HeapWasmMemory *m) {
    m->start          = HEAP_WASM_HEAP_BASE_UPTR;
    m->capacity       = (wasm_get_memory_size() - m->start);
    m->end            = (m->start + m->capacity);
    m->initialized    = true;
    m->bytes_per_page = HEAP_WASM_PAGE_SIZE;
    m->num_pages      = m->capacity / m->bytes_per_page;
}

boolean heap_wasm_pointer_check_bounds(HeapWasmMemory *m, uptr ptr) {
    return ptr < m->end;
}

Result(uptr) heap_wasm_memory_grow(HeapWasmMemory *m, usize amount) {
    usize pagesneeded = math_div_ceil_u32(amount, m->bytes_per_page);

    if (pagesneeded == 0) return result_ok(uptr, m->end);

    usize prevnumpages    = wasm_memory_grow(pagesneeded);
    usize newcap          = (wasm_get_memory_size() - m->start);
    usize currentnumpages = newcap / m->bytes_per_page;

    // If the number of pages did not change, we failed to
    // grow the web assembly memory.
    if (prevnumpages == currentnumpages) return result_error(uptr, ERR_OUT_OF_MEMORY);

    uptr old_end = m->end;

    m->capacity  = newcap;
    m->end       = m->start + m->capacity;
    m->num_pages = currentnumpages;

    return result_ok(uptr, old_end);
}

// Result(uptr) heap_wasm_memory_bump(HeapWasmMemory *m, usize amount) {
//    if (!m->initialized) heap_wasm_memory_init(m);
//
//    usize available = m->capacity - m->offset;
//    if (amount > available) {
//        if (!heap_wasm_memory_grow(m, amount - available)) return result_error(uptr, ERR_OUT_OF_MEMORY);
//    }
//
//    uptr ptr = m->start + m->offset;
//    m->offset += amount;
//
//    return result_ok(uptr, ptr);
//}
//
// Result(uptr) heap_wasm_memory_bump_align(HeapWasmMemory *m, usize amount, usize alignment) {
//    if (!m->initialized) heap_wasm_memory_init(m);
//
//    uptr current          = m->start + m->offset;
//    Result(uptr) raligned = mem_align_pointer_forward(current, alignment);
//
//    result_raise(uptr, raligned);
//
//    uptr aligned = raligned.value;
//
//    usize delta = aligned - current;
//
//    Result(uptr) rptr = heap_wasm_memory_bump(m, amount + delta);
//
//    result_raise(uptr, rptr);
//
//    return result_ok(uptr, aligned);
//}

typedef enum { BLOCK_FREE = 0x22, BLOCK_RESERVED = 0x32 } BlockState;

static const char *block_state_string(BlockState s) {
    switch (s) {
        case BLOCK_FREE: return "Free";
        case BLOCK_RESERVED: return "Reserved";
        default: return "Invalid Flag";
    }
}

typedef struct BlockHeader {
    u8                  flag;
    usize               size;
    struct BlockHeader *next;
    struct BlockHeader *prev;
} BlockHeader;

typedef struct BlockFooter {
    u8    flag;
    usize size;
} BlockFooter;

typedef struct HeapWasmFreeListAllocatorState {
    BlockHeader    head;
    HeapWasmMemory heap;
    boolean        initialized;
    usize          default_alignment;
    usize          min_block_size;
    BlockHeader *  first;
} HeapWasmFreeListAllocatorState;

extern void *memcpy(void *destination, const void *source, size_t num);

local ErrorCode wasm_free_list_allocator_init(HeapWasmFreeListAllocatorState *allocator, usize default_alignment) {
    if (!allocator->heap.initialized) heap_wasm_memory_init(&allocator->heap);

    // Save the default allocator alignment. If 0 is given,
    // use the default alignment defined above
    allocator->default_alignment = default_alignment == 0 ? HEAP_WASM_MAX_ALIGN : default_alignment;

    // The min block size used in allocations
    allocator->min_block_size = 128;

    // Init allocator head node
    allocator->head.flag = BLOCK_RESERVED;
    allocator->head.size = 0;
    allocator->head.next = 0;
    allocator->head.prev = 0;

    // Align first block so that the header fits before the
    // user pointer
    Result(uptr) r_first =
        mem_align_pointer_forward(allocator->heap.start + sizeof(BlockHeader), allocator->default_alignment);

    // Failed to align?
    result_raise_error_code(r_first);

    // This is the first user pointer
    uptr first = r_first.value;

    // If we can't fit the footer with something remaining,
    // we're out of memory
    if ((allocator->heap.end - sizeof(BlockFooter)) <= first) {
        return ERR_OUT_OF_MEMORY;
    }

    // Calculate user-available capacity of the block
    usize block_size = (allocator->heap.end - sizeof(BlockFooter)) - first;

    assert(block_size);

    // Define the block's header and footer
    BlockHeader header = {.flag = BLOCK_FREE, .size = block_size, .next = &allocator->head, .prev = &allocator->head};
    BlockFooter footer = {.flag = BLOCK_FREE, .size = block_size};

    // Calculate where the header and footer should be
    // stored at
    BlockHeader *header_ptr = (BlockHeader *)(first - sizeof(BlockHeader));
    BlockFooter *footer_ptr = (BlockFooter *)(allocator->heap.end - sizeof(BlockFooter));

    // Copy the header and footer data
    *header_ptr = header;
    *footer_ptr = footer;

    // Link head to the new/first block
    allocator->head.next = header_ptr;
    allocator->head.prev = header_ptr;

    // Save the first block pointer
    allocator->first = header_ptr;

    // Initialization was successfull :)
    allocator->initialized = true;

    return ERR_OK;
}

local void wasm_free_list_allocator_remove_block(BlockHeader *h) {
    h->next->prev = h->prev;
    h->prev->next = h->next;
}

local void wasm_free_list_allocator_prepend_block(HeapWasmFreeListAllocatorState *allocator, BlockHeader *h) {
    h->next       = allocator->head.next;
    h->prev       = &allocator->head;
    h->next->prev = h;
    h->prev->next = h;
}

local void wasm_free_list_print(HeapWasmFreeListAllocatorState *allocator, void (*printfn)(const char *)) {
    if (!allocator->initialized) wasm_free_list_allocator_init(allocator, HEAP_WASM_MAX_ALIGN);

    BlockHeader *current = &allocator->head;
    usize        i       = 1;
    char         b[256];
    printfn("--------- WASM FREE LIST START ------------");
    format(b, 256, "Heap: %d [%d, %d]\n", allocator->heap.capacity, allocator->heap.start, allocator->heap.end);
    printfn(b);
    while ((current = current->next) != &allocator->head) {
        format(b,
               256,
               "Block(%d): %d [%d, %d] \n",
               i,
               current->size,
               (uptr)current,
               (uptr)current + sizeof(BlockHeader) + sizeof(BlockFooter) + current->size);
        printfn(b);
        i++;
    }
    printfn("--------- WASM FREE LIST END ------------");
}

typedef struct HeapWasmFreeListRegionInfo {
    BlockHeader *header;
    BlockFooter *footer;
    uptr         header_start;
    uptr         footer_end;
    usize        total;
    usize        allocated;
} HeapWasmFreeListRegionInfo;

HeapWasmFreeListRegionInfo wasm_free_list_region_info(uptr region) {
    HeapWasmFreeListRegionInfo info = {0};

    info.header       = (BlockHeader *)(region - sizeof(BlockHeader));
    info.footer       = (BlockFooter *)(region + info.header->size);
    info.header_start = (uptr)info.header;
    info.footer_end   = (uptr)info.footer + sizeof(BlockFooter);
    info.total        = info.footer_end - info.header_start;
    info.allocated    = info.header->size;

    return info;
}

void heap_wasm_free_list_print_region_info(void (*printfn)(const char *), uptr region) {
    HeapWasmFreeListRegionInfo info = wasm_free_list_region_info(region);

    char buf[256];
    printfn("Block info:");
    format(buf, 256, "\tTotal size: %d", info.total);
    printfn(buf);
    format(buf, 256, "\tAllocated size: %d", info.allocated);
    printfn(buf);
    format(buf, 256, "\tFlag: %s", block_state_string((BlockState)info.header->flag));
    printfn(buf);
    format(buf, 256, "\tRange: [%d, %d]", info.header_start, info.footer_end);
    printfn(buf);
    format(buf, 256, "\tRegion: %d", region);
    printfn(buf);
    format(buf,
           256,
           "\tsizeof(BlockHeader): %d, "
           "sizeof(BlockFooter): %d",
           sizeof(BlockHeader),
           sizeof(BlockFooter));
    printfn(buf);
}

static Result(uptr) wasm_free_list_allocator_alloc(HeapWasmFreeListAllocatorState *allocator, usize amount) {
    // If the allocator was not initialized, initialize it
    // with the default alignment
    if (!allocator->initialized) wasm_free_list_allocator_init(allocator, HEAP_WASM_MAX_ALIGN);

    assert(allocator->initialized);

    // Total amount needed to create a new aligned block =
    // AlignForward(amount + H + F)
    usize amount_needed_for_new_block =
        result_unwrap(mem_align_pointer_forward((uptr)(amount + sizeof(BlockHeader) + sizeof(BlockFooter)),
                                                allocator->default_alignment));

    // Traverse the free block list
    BlockHeader *current = &allocator->head;
    static int   counter = 0;
    counter++;
    while ((current = current->next) != &allocator->head) {

        // If the current block has enough capacity...
        if (current->size >= amount) {

            // Get the pointer to the allocated region
            uptr user_pointer = (uptr)current + sizeof(BlockHeader);

            // Double-check alignment
            assert(mem_is_aligned(user_pointer, allocator->default_alignment));

            // Check if we have enough memory for a new
            // block of at least "min_block_size" free
            // allocable memory
            if (current->size >= (amount_needed_for_new_block + allocator->min_block_size)) {

                // The effective amount that will be
                // allocated, more ore equal than "amount"
                usize allocated_amount = amount_needed_for_new_block - sizeof(BlockFooter) - sizeof(BlockHeader);
                usize initial_size     = current->size;

                // The size of the next block
                usize next_block_size = initial_size - sizeof(BlockFooter) - sizeof(BlockHeader) - allocated_amount;

                // Make sure it is larger than the min block
                // size
                assert(next_block_size >= allocator->min_block_size);

                // Set the allocated amount on the header
                current->size = allocated_amount;
                current->flag = BLOCK_RESERVED;

                wasm_free_list_allocator_remove_block(current);

                // Set the footer of the current block
                uptr current_footer                   = (uptr)current + sizeof(BlockHeader) + current->size;
                ((BlockFooter *)current_footer)->flag = BLOCK_RESERVED;
                ((BlockFooter *)current_footer)->size = current->size;

                // Set the header of the created block
                uptr next_header                   = (uptr)current_footer + sizeof(BlockFooter);
                ((BlockHeader *)next_header)->flag = BLOCK_FREE;
                ((BlockHeader *)next_header)->size = next_block_size;

                assert(next_header == user_pointer + current->size + sizeof(BlockFooter));

                // Set the footer of the created block
                uptr next_footer = next_header + sizeof(BlockHeader) + next_block_size;    //(uptr)current +
                                                                                           // initial_size;
                ((BlockFooter *)next_footer)->flag = BLOCK_FREE;
                ((BlockFooter *)next_footer)->size = next_block_size;

                // Add the created block to the free list
                wasm_free_list_allocator_prepend_block(allocator, (BlockHeader *)next_header);

            } else {
                // Allocate the whole block

                // Mark header as reserved
                current->flag = BLOCK_RESERVED;

                // Mark footer as reserved
                BlockFooter *footer = (BlockFooter *)current + sizeof(BlockHeader) + current->size;
                footer->flag        = BLOCK_RESERVED;
                footer->size        = current->size;

                // Remove block from free list
                wasm_free_list_allocator_remove_block(current);
            }

            // If everything worked, return the allocated
            // region's pointer
            return result_ok(uptr, user_pointer);
        }
    }

    Result(uptr) r_old_end = heap_wasm_memory_grow(&allocator->heap, allocator->heap.capacity * 2);

    if (result_is_error(r_old_end)) {
        return result_error(uptr, ERR_OUT_OF_MEMORY);
    }

    uptr bump = r_old_end.value;

    BlockHeader *bumpHeader = (BlockHeader *)bump;
    BlockFooter *bumpFooter = (BlockFooter *)(allocator->heap.end - sizeof(BlockFooter));

    bumpHeader->flag = BLOCK_FREE;
    bumpHeader->size = (void *)bumpFooter - (void *)bumpHeader - sizeof(BlockHeader);
    bumpFooter->flag = BLOCK_FREE;
    bumpFooter->size = bumpHeader->size;

    wasm_free_list_allocator_prepend_block(allocator, bumpHeader);

    return wasm_free_list_allocator_alloc(allocator, amount);
}

static void wasm_free_list_allocator_free(HeapWasmFreeListAllocatorState *allocator, uptr region) {
    HeapWasmMemory *memory      = &allocator->heap;
    BlockHeader *   header      = (BlockHeader *)((uptr)region - sizeof(BlockHeader));
    BlockFooter *   left_footer = (BlockFooter *)((uptr)header - sizeof(BlockFooter));

    BlockFooter *footer       = (BlockFooter *)((uptr)region + header->size);
    BlockHeader *right_header = (BlockHeader *)((uptr)footer + sizeof(BlockFooter));

    assert(region >= memory->start);
    assert(header->flag == BLOCK_RESERVED);
    assert(footer->flag == BLOCK_RESERVED);
    assert(header->size == footer->size);

    wasm_free_list_allocator_prepend_block(allocator, header);
    header->flag = BLOCK_FREE;
    footer->flag = BLOCK_FREE;

    // Merge left
    if (((uptr)left_footer > (memory->start + sizeof(BlockHeader))) && (left_footer->flag == BLOCK_FREE)) {

        BlockHeader *left_header = (BlockHeader *)((uptr)left_footer - left_footer->size - sizeof(BlockHeader));

        assert(left_header->flag == BLOCK_FREE);

        left_header->size = (uptr)footer - (uptr)left_header - sizeof(BlockHeader);
        footer->size      = left_header->size;
        footer->flag      = BLOCK_FREE;

        wasm_free_list_allocator_remove_block(header);
        header = left_header;
    }

    // Merge right
    if (((uptr)right_header < memory->end) && right_header->flag == BLOCK_FREE) {

        wasm_free_list_allocator_remove_block(right_header);

        BlockFooter *right_footer = (BlockFooter *)((uptr)right_header + right_header->size + sizeof(BlockHeader));

        header->size = ((uptr)right_footer - (uptr)header) - sizeof(BlockHeader);

        right_footer->size = header->size;
        right_footer->flag = BLOCK_FREE;
    }
}

static HeapWasmFreeListAllocatorState wasm_free_list_allocator_state;

static Result(uptr) wasm_free_list_alloc(Allocator *allocator, usize amount) {
    return wasm_free_list_allocator_alloc(&wasm_free_list_allocator_state, amount);
}

static void wasm_free_list_free(Allocator *allocator, uptr region) {
    wasm_free_list_allocator_free(&wasm_free_list_allocator_state, region);
}

static Result(uptr) wasm_free_list_realloc(Allocator *allocator, uptr region, usize amount) {
    if (amount == 0) {
        return result_error(uptr, ERR_REALLOC_ZERO);
    }

    Result(uptr) r = wasm_free_list_alloc(allocator, amount);

    result_raise(uptr, r);

    uptr new_region = r.value;

    // TODO: this is dumb and temporary.... we should copy the smallest of amount or old region size
    memcpy((void *)new_region, (void *)region, amount);

    return result_ok(uptr, new_region);
}

Allocator heap_wasm_free_list_allocator = {.alloc   = wasm_free_list_alloc,
                                           .free    = wasm_free_list_free,
                                           .realloc = wasm_free_list_realloc};

void heap_wasm_free_list_print_blocks(void (*printfn)(const char *)) {
    wasm_free_list_print(&wasm_free_list_allocator_state, printfn);
}

void wasm_free_list_print_all(void (*printfn)(const char *)) {
    if (!wasm_free_list_allocator_state.initialized)
        wasm_free_list_allocator_init(&wasm_free_list_allocator_state, HEAP_WASM_MAX_ALIGN);

    BlockHeader *header = wasm_free_list_allocator_state.first;

    printfn("=========== Wasm Allocator Blocks ===========");

    while ((uptr)header < wasm_free_list_allocator_state.heap.end) {
        heap_wasm_free_list_print_region_info(printfn, (uptr)header + sizeof(BlockHeader));
        header = (BlockHeader *)((uptr)header + header->size + sizeof(BlockFooter) + sizeof(BlockHeader));
    }

    printfn("=============================================");
}

#ifdef __RUN_TESTS
//#if 1

#include "test.h"
#undef __RUN_TESTS
#include "math.c"

u8 __heap_base;

typedef struct HeapWasmMockMemory {
    uptr  base;
    usize size;
    usize max_pages;
    ;
    usize num_pages;
} HeapWasmMockMemory;

local HeapWasmMockMemory __mock_memory;

u32 wasm_get_memory_size() {
    return __mock_memory.size;
};
u32 wasm_memory_grow(u32 pages) {
    if ((__mock_memory.num_pages + pages) > __mock_memory.max_pages) return __mock_memory.num_pages;

    usize np = __mock_memory.num_pages;
    __mock_memory.num_pages += pages;
    __mock_memory.size += pages * HEAP_WASM_PAGE_SIZE;

    return np;
};

void __mock_memory_init() {
    __mock_memory.base = (uptr)malloc(10 * HEAP_WASM_PAGE_SIZE);
    expect(__mock_memory.base);
    expect(mem_is_aligned(__mock_memory.base, ALLOCATOR_MAX_ALIGNMENT));
    __mock_memory.size      = HEAP_WASM_PAGE_SIZE;
    __mock_memory.max_pages = 10;
    __mock_memory.num_pages = 1;
}

void __mock_memory_deinit() {
    free((void *)__mock_memory.base);
}
void heap_wasm_memory_init_custom_base(HeapWasmMemory *m, uptr base) {
    m->start          = base;
    m->capacity       = wasm_get_memory_size();
    m->end            = (m->start + m->capacity);
    m->offset         = 0;
    m->initialized    = true;
    m->bytes_per_page = HEAP_WASM_PAGE_SIZE;
    m->num_pages      = m->capacity / m->bytes_per_page;
}

test(heap_wasm_memory_bump) {
    __mock_memory_init();

    HeapWasmMemory  mm = {0};
    HeapWasmMemory *m  = &mm;
    heap_wasm_memory_init_custom_base(m, __mock_memory.base);
    Result(uptr) rx = heap_wasm_memory_bump(m, 10);

    expect(result_is_ok(rx));
    expect(rx.value == m->start);

    rx = heap_wasm_memory_bump(m, 10);
    expect(result_is_ok(rx));
    expect(rx.value == m->start + 10);
    expect(m->offset == 20);
    expect(m->capacity == HEAP_WASM_PAGE_SIZE);
    expect(m->num_pages == 1);

    rx = heap_wasm_memory_bump(m, HEAP_WASM_PAGE_SIZE);
    expect(result_is_ok(rx));
    expect(m->offset == (20 + HEAP_WASM_PAGE_SIZE));
    expect(m->capacity == (2 * HEAP_WASM_PAGE_SIZE));
    expect(m->num_pages == 2);

    __mock_memory_deinit();
}

test(heap_wasm_memory_bump_align) {
    __mock_memory_init();

    HeapWasmMemory m = {0};
    heap_wasm_memory_init_custom_base(&m, __mock_memory.base);

    Result(uptr) rx = heap_wasm_memory_bump_align(&m, 10, ALLOCATOR_MAX_ALIGNMENT);
    expect(result_is_ok(rx));
    expect(mem_is_aligned(rx.value, ALLOCATOR_MAX_ALIGNMENT));

    __mock_memory_deinit();
}

test(wasm_free_list_allocator) {
    __mock_memory_init();

    HeapWasmFreeListAllocatorState a = {0};
    heap_wasm_memory_init_custom_base(&a.heap, __mock_memory.base);
    wasm_free_list_allocator_init(&a, 8);

    Result(uptr) rx = wasm_free_list_allocator_alloc(&a, 100);

    expect(result_is_ok(rx));
    expect(rx.value ==
           result_unwrap(mem_align_pointer_forward(a.heap.start + sizeof(BlockHeader), HEAP_WASM_MAX_ALIGN)));

    __mock_memory_deinit();
}

suite() {
    run_test(heap_wasm_memory_bump);
    run_test(heap_wasm_memory_bump_align);
    run_test(wasm_free_list_allocator);
}

#endif /* __RUN_TESTS */
