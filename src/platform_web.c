#include "mem_common.h"
#include "platform.h"
#include "result.h"
#include "format.h"
#include "math.h"

#include "platform_web_js_imports.c"

extern u8 __heap_base;
extern void *memcpy(void *dst, const void *src, size_t num);

static const usize WasmMemoryPageSize = KB(64);

typedef struct PlatformWebMemory
{
    uptr start;
    uptr end;
    usize current_offset;
    usize capacity;
    usize available;
    usize num_pages;
    usize bytes_per_page;
} PlatformWebMemory;

typedef struct PlatformWeb
{
    PlatformWebMemory memory;
} PlatformWeb;

static PlatformWeb platform_web;

static void memory_init(PlatformWebMemory *memory)
{
    // Get the heap address
    memory->start = (uptr) &__heap_base;

    memory->capacity = wasm_get_memory_size();
    memory->bytes_per_page= WasmMemoryPageSize; 
    memory->num_pages = memory->capacity / memory->bytes_per_page;
    memory->available = memory->capacity;
    memory->current_offset = 0;
    memory->end = memory->start + memory->capacity;
}

static Result(usize) memory_bump(PlatformWebMemory *memory, usize amount)
{
    uptr brk = memory->start + memory->current_offset;

    Result(uptr) rnext = mem_align_pointer_forward(brk + amount, mem_max_alignment);
    if (result_is_error(rnext)) return result_error(usize, rnext.error_code);
    uptr next = rnext.value;

    if (next > memory->end) {
        usize need = next - memory->end;
        usize needpages = math_div_ceil_u32(need, memory->bytes_per_page);
        usize oldnumpages = (usize) wasm_memory_grow(needpages);
        usize current_size = wasm_get_memory_size();
        usize current_pages = current_size / memory->bytes_per_page;

        if (oldnumpages == current_pages) {
            return result_error(usize, ERR_OUT_OF_MEMORY);
        }

        usize bytesgrown = needpages * memory->bytes_per_page;

        memory->num_pages += needpages;
        memory->capacity += bytesgrown;
        memory->end += bytesgrown;
        memory->available += bytesgrown;
    }

    usize final_amount = next - memory->start - memory->current_offset;
    memory->current_offset += final_amount;
    memory->available -= final_amount;

    return result_ok(usize, final_amount);
}

static void memory_clear(PlatformWebMemory *memory)
{
    memory->available = memory->capacity;
    memory->current_offset = 0;
}

void platform_print_fmt(const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    char buf[512];
    formatv(buf, 512, fmt, arg);
    wasm_print_line(buf);
}

void platform_web_print_memory()
{
    PlatformWebMemory *m = &platform_web.memory;

    platform_print_fmt("Wasm Memory:\n");
    platform_print_fmt("start = %u", m->start);
    platform_print_fmt("end = %u", m->end);
    platform_print_fmt("capacity = %u", m->capacity);
    platform_print_fmt("available = %u", m->available);
    platform_print_fmt("pages = %u", m->num_pages);
    platform_print_fmt("offset = %u", m->current_offset);
    platform_print_fmt("--------------", m->current_offset);
}

export int main(int argc, char **argv)
{
    platform_print_fmt("Hello!");

    memory_init(&platform_web.memory);

    platform_print_fmt("sin 0.234 = %f", math_sinf(0.234));

    platform_web_print_memory();
    memory_bump(&platform_web.memory, 128);
    platform_web_print_memory();
    memory_bump(&platform_web.memory, 1);
    platform_web_print_memory();
    memory_bump(&platform_web.memory, 600 * WasmMemoryPageSize);
    platform_web_print_memory();
    memory_clear(&platform_web.memory);
    platform_web_print_memory();
}

