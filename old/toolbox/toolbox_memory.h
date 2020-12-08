#ifndef TOOLBOX_MEMORY_H
#define TOOLBOX_MEMORY_H

#include "toolbox_common.h"

// Note: GingerBill uses 2*(sizeof(void *))... is it necessary?
// https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/
#define TOOLBOX_MEMORY_DEFAULT_ALIGNMENT (sizeof(void *))
#define TOOLBOX_MEMORY_IS_POT(x)         (((x) & ((x)-1)) == 0)

extern void *memset(void *, int, size_t);

typedef struct ToolboxMemoryArena
{
    void *start;
    size_t size;
    uptr current_offset;
    uptr last_offset;
} ToolboxMemoryArena;

typedef struct ToolboxMemoryArenaTemp
{
    ToolboxMemoryArena *arena;
    uptr current_offset;
    uptr last_offset;
} ToolboxMemoryArenaTemp;

void *toolbox_memory_arena_alloc(ToolboxMemoryArena *arena, size_t amount);
void *toolbox_memory_arena_alloc_aligned(ToolboxMemoryArena *arena, size_t amount, size_t align);
void toolbox_memory_arena_init(ToolboxMemoryArena *arena, void *start, size_t byte_size);
void toolbox_memory_arena_free(ToolboxMemoryArena *arena); 
void toolbox_memory_arena_free_all(ToolboxMemoryArena *arena);
ToolboxMemoryArenaTemp toolbox_memory_arena_temp_start(ToolboxMemoryArena *arena);
void toolbox_memory_arena_temp_end(ToolboxMemoryArenaTemp t);

#ifdef TOOLBOX_MEMORY_IMPL
void toolbox_memory_arena_init(ToolboxMemoryArena *arena, void *start, size_t byte_size)
{
    arena->start          = start;
    arena->size           = byte_size;
    arena->current_offset = 0;
    arena->last_offset    = 0;
}

void *toolbox_memory_arena_alloc_aligned(ToolboxMemoryArena *arena, size_t amount, size_t align)
{
    t_assert(TOOLBOX_MEMORY_IS_POT(align));

    uptr mod = arena->current_offset % align;

    if (mod != 0) {
        arena->current_offset += align - mod;
    }

    if (arena->current_offset + amount <= arena->size) {
        void *ptr          = arena->start + arena->current_offset;
        arena->last_offset = arena->current_offset;
        arena->current_offset += amount;
        memset(ptr, 0, amount);
        return ptr;
    }

    return 0;
}

void *toolbox_memory_arena_alloc(ToolboxMemoryArena *arena, size_t amount)
{
    return toolbox_memory_arena_alloc_aligned(arena, amount, TOOLBOX_MEMORY_DEFAULT_ALIGNMENT);
}

void toolbox_memory_arena_free(ToolboxMemoryArena *arena) {}

void toolbox_memory_arena_free_all(ToolboxMemoryArena *arena)
{
    arena->current_offset = 0;
    arena->last_offset    = 0;
}

ToolboxMemoryArenaTemp toolbox_memory_arena_temp_start(ToolboxMemoryArena *arena)
{
    return (ToolboxMemoryArenaTemp){ .arena          = arena,
                                     .current_offset = arena->current_offset,
                                     .last_offset    = arena->last_offset };
}

void toolbox_memory_arena_temp_end(ToolboxMemoryArenaTemp t) {
    t.arena->current_offset = t.current_offset;
    t.arena->last_offset = t.last_offset;
}

#endif
#endif
