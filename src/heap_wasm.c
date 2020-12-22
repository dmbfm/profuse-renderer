#include "allocator.h"
#include "common.h"
#include "error.h"
#include "heap.h"
#include "math.h"
#include "mem_common.h"
#include "result.h"

extern u8 __heap_base;

#define HEAP_WASM_HEAP_BASE_UPTR ((uptr)&__heap_base)

extern u32 wasm_get_memory_size();
extern u32 wasm_memory_grow(u32);

enum
{
    HEAP_WASM_PAGE_SIZE = KB(64)
};

typedef struct HeapWasmMemory
{
    uptr start;
    uptr end;
    usize offset;
    usize capacity;
    usize num_pages;
    usize bytes_per_page;
    boolean initialized;
} HeapWasmMemory;

HeapWasmMemory heap_wasm_memory;

void heap_wasm_memory_init(HeapWasmMemory *m)
{
    m->start          = HEAP_WASM_HEAP_BASE_UPTR;
    m->capacity       = wasm_get_memory_size();
    m->end            = (m->start + m->capacity);
    m->offset         = 0;
    m->initialized    = true;
    m->bytes_per_page = HEAP_WASM_PAGE_SIZE;
    m->num_pages      = m->capacity / m->bytes_per_page;
}

boolean heap_wasm_memory_grow(HeapWasmMemory *m, usize amount)
{
    usize pagesneeded = math_div_ceil_u32(amount, m->bytes_per_page);

    if (pagesneeded == 0)
        return true;

    usize prevnumpages    = wasm_memory_grow(pagesneeded);
    usize newcap          = wasm_get_memory_size();
    usize currentnumpages = newcap / m->bytes_per_page;

    // If the number of pages did not change, we failed to grow the web assembly memory.
    if (prevnumpages == currentnumpages)
        return false;

    m->capacity  = newcap;
    m->end       = m->start + m->capacity;
    m->num_pages = currentnumpages;

    return true;
}

Result(uptr) heap_wasm_memory_bump(HeapWasmMemory *m, usize amount)
{
    if (!m->initialized)
        heap_wasm_memory_init(m);

    usize available = m->capacity - m->offset;
    if (amount > available) {
        if (!heap_wasm_memory_grow(m, amount - available))
            return result_error(uptr, ERR_OUT_OF_MEMORY);
    }

    uptr ptr = m->start + m->offset;
    m->offset += amount;

    return result_ok(uptr, ptr);
}

Result(uptr) heap_wasm_memory_bump_align(HeapWasmMemory *m, usize amount, usize alignment)
{
    if (!m->initialized)
        heap_wasm_memory_init(m);

    uptr current          = m->start + m->offset;
    Result(uptr) raligned = mem_align_pointer_forward(current, alignment);

    result_raise(raligned);

    uptr aligned = raligned.value;

    usize delta = aligned - current;

    Result(uptr) rptr = heap_wasm_memory_bump(m, amount + delta);

    result_raise(rptr);

    return result_ok(uptr, aligned);
}

enum
{
    BLOCK_FREE     = 0x22,
    BLOCK_RESERVED = 0x32
};

typedef struct BlockHeader
{
    u8 flag;
    usize size;
    struct BlockHeader *next;
    struct BlockHeader *prev;
} BlockHeader;

typedef struct BlockFooter
{
    u8 flag;
    usize size;
    usize __fill1;
    usize __fill2;
} BlockFooter;

typedef struct HeapWasmFreeListAllocatorState
{
    BlockHeader head;
    HeapWasmMemory wasm_memory;
    uptr start;
    boolean initialized;
} HeapWasmFreeListAllocatorState;


extern void * memcpy ( void * destination, const void * source, size_t num );

static void wasm_free_list_allocator_init(HeapWasmFreeListAllocatorState *allocator)
{
    heap_wasm_memory_init(&allocator->wasm_memory);

    // Move forward to next max-alinged position...
    Result(uptr) rstart = mem_align_pointer_forward(allocator->wasm_memory.start, ALLOCATOR_MAX_ALIGNMENT);

    if (result_is_error(rstart))
        panic();

    allocator->start = rstart.value;
    allocator->head  = (BlockHeader){ .flag = BLOCK_RESERVED,
                                     .size = allocator->wasm_memory.capacity,
                                     .next = (BlockHeader *)allocator->start,
                                     .prev = (BlockHeader *)allocator->start };

    HeapWasmMemory *m    = &allocator->wasm_memory;
    Result(uptr) rfooter_loc = mem_aling_pointer_back(m->end - sizeof(BlockFooter), ALLOCATOR_MAX_ALIGNMENT);

    if (result_is_error(rfooter_loc))
        panic();

    uptr footer_loc = rfooter_loc.value;

    usize block_capacity = (footer_loc - allocator->start) - sizeof(BlockHeader) - sizeof(BlockFooter);

    BlockHeader header = {
        .flag = BLOCK_FREE, .size = block_capacity, .next = &allocator->head, .prev = &allocator->head
    };

    BlockFooter footer = { .flag = BLOCK_FREE, .size = block_capacity };

    ASSERT(mem_is_aligned(allocator->start, ALLOCATOR_MAX_ALIGNMENT));
    ASSERT(mem_is_aligned(footer_loc, ALLOCATOR_MAX_ALIGNMENT));
    ASSERT(mem_is_aligned((uptr) block_capacity, ALLOCATOR_MAX_ALIGNMENT));
    ASSERT(footer_loc > allocator->start)

    memcpy((void*)allocator->start, &header, sizeof(BlockHeader));
    memcpy((void *)footer_loc, &footer, sizeof(BlockFooter));
}

void wasm_free_list_allocator_remove_block(BlockHeader *h)
{
    h->next->prev = h->prev;
    h->prev->next = h->next;
}

void wasm_free_list_allocator_prepend_block(HeapWasmFreeListAllocatorState *allocator, BlockHeader *h)
{
    h->next = allocator->head.next;
    h->prev = &allocator->head;
    h->next->prev = h;
    h->prev->next = h;
}

void wasm_free_list_allocator_alloc(HeapWasmFreeListAllocatorState *allocator, usize amount)
{
    if (!allocator->initialized) wasm_free_list_allocator_init(allocator);

    // Go through each free block in the list...
    BlockHeader *current = &allocator->head;
    while((current = current->next) != &allocator->head) {
        // Found a free block
        if (current->flag == BLOCK_FREE) {

        }
    }
}

//#ifdef __RUN_TESTS
#if 1

#include "test.h"
#undef __RUN_TESTS
#include "math.c"

u8 __heap_base;

typedef struct HeapWasmMockMemory
{
    uptr base;
    usize size;
    usize max_pages;
    ;
    usize num_pages;
} HeapWasmMockMemory;

static HeapWasmMockMemory __mock_memory;

u32 wasm_get_memory_size()
{
    return __mock_memory.size;
};
u32 wasm_memory_grow(u32 pages)
{
    if ((__mock_memory.num_pages + pages) > __mock_memory.max_pages)
        return __mock_memory.num_pages;

    usize np = __mock_memory.num_pages;
    __mock_memory.num_pages += pages;
    __mock_memory.size += pages * HEAP_WASM_PAGE_SIZE;

    return np;
};

void __mock_memory_init()
{
    __mock_memory.base = (uptr)malloc(10 * HEAP_WASM_PAGE_SIZE);
    assert(__mock_memory.base);
    assert(mem_is_aligned(__mock_memory.base, ALLOCATOR_MAX_ALIGNMENT));
    __mock_memory.size      = HEAP_WASM_PAGE_SIZE;
    __mock_memory.max_pages = 10;
    __mock_memory.num_pages = 1;
}

void __mock_memory_deinit()
{
    free((void *)__mock_memory.base);
}
void heap_wasm_memory_init_custom_base(HeapWasmMemory *m, uptr base)
{
    m->start          = base;
    m->capacity       = wasm_get_memory_size();
    m->end            = (m->start + m->capacity);
    m->offset         = 0;
    m->initialized    = true;
    m->bytes_per_page = HEAP_WASM_PAGE_SIZE;
    m->num_pages      = m->capacity / m->bytes_per_page;
}

test(heap_wasm_memory_bump)
{
    __mock_memory_init();

    HeapWasmMemory mm = { 0 };
    HeapWasmMemory *m = &mm;
    heap_wasm_memory_init_custom_base(m, __mock_memory.base);
    Result(uptr) rx = heap_wasm_memory_bump(m, 10);

    assert(result_is_ok(rx));
    assert(rx.value == m->start);

    rx = heap_wasm_memory_bump(m, 10);
    assert(result_is_ok(rx));
    assert(rx.value == m->start + 10);
    assert(m->offset == 20);
    assert(m->capacity == HEAP_WASM_PAGE_SIZE);
    assert(m->num_pages == 1);

    rx = heap_wasm_memory_bump(m, HEAP_WASM_PAGE_SIZE);
    assert(result_is_ok(rx));
    assert(m->offset == (20 + HEAP_WASM_PAGE_SIZE));
    assert(m->capacity == (2 * HEAP_WASM_PAGE_SIZE));
    assert(m->num_pages == 2);

    __mock_memory_deinit();
}

test(heap_wasm_memory_bump_align)
{
    __mock_memory_init();

    HeapWasmMemory m = { 0 };
    heap_wasm_memory_init_custom_base(&m, __mock_memory.base);

    Result(uptr) rx = heap_wasm_memory_bump_align(&m, 10, ALLOCATOR_MAX_ALIGNMENT);
    assert(result_is_ok(rx));
    assert(mem_is_aligned(rx.value, ALLOCATOR_MAX_ALIGNMENT));

    __mock_memory_deinit();
}

test(wasm_free_list_allocator)
{
    __mock_memory_init();

    HeapWasmFreeListAllocatorState a = { 0 };
    wasm_free_list_allocator_init(&a);

    __mock_memory_deinit();
}

suite()
{
    run_test(heap_wasm_memory_bump);
    run_test(heap_wasm_memory_bump_align);
    run_test(wasm_free_list_allocator);
}

#endif /* __RUN_TESTS */
