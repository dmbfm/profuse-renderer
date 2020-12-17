#include <stdio.h>
//#include <stdlib.h>
#include "src/common.h"
#include <assert.h>
#include <malloc.h>
#include <string.h>

uptr raw_mem;

const usize mem_size = KB(64);

enum
{
    FREE     = 0x22,
    RESERVED = 0x32
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
} BlockFooter;

typedef struct Memory
{
    u8 flag;
    uptr start;
    uptr end;
    usize capacity;
    BlockHeader head;
} Memory;

static Memory memory;

void init_memory()
{
    memory.start    = raw_mem;
    memory.capacity = mem_size;
    memory.end      = memory.start + memory.capacity;

    memory.head = (BlockHeader){
        .flag = RESERVED, .size = 0, .next = (BlockHeader *)memory.start, .prev = (BlockHeader *)memory.start
    };

    usize bsize = memory.capacity - sizeof(BlockHeader) - sizeof(BlockFooter);

    BlockHeader header = { .flag = FREE, .size = bsize, .next = &memory.head, .prev = &memory.head };

    BlockFooter footer = { .flag = FREE, .size = bsize };

    memcpy((void *)memory.start, &header, sizeof(BlockHeader));
    memcpy((void *)(memory.end - sizeof(BlockFooter)), &footer, sizeof(BlockFooter));
}

void free_list_remove(BlockHeader *h)
{
    h->next->prev = h->prev;
    h->prev->next = h->next;
}

void free_list_prepend(BlockHeader *h)
{
    h->next = memory.head.next;
    h->prev = &memory.head;
    h->next->prev = h;
    h->prev->next = h;
}

void release(uptr region)
{
    //printf("RELEASE %llu", region - memory.start);
    BlockHeader *h          = (BlockHeader *)(region - sizeof(BlockHeader));
    BlockFooter *prevfooter = (BlockFooter *)((uptr)h - sizeof(BlockFooter));
    BlockFooter *footer = (BlockFooter *)(region + h->size);
    BlockHeader *rightheader = (BlockHeader *)((uptr)footer + sizeof(BlockFooter));

    assert(region >= memory.start && region <= memory.end);
    assert(h->flag == RESERVED);
    assert(footer->flag == RESERVED);
    assert(h->size == footer->size);

    // Add block to the free list
    h->next = memory.head.next;
    h->prev = &memory.head;
    h->next->prev = h;
    h->prev->next = h;
    h->flag = FREE;
    footer->flag = FREE;

    // Merge left
    if (((uptr)h != memory.start) && (prevfooter->flag == FREE)) {
        BlockHeader *leftheader = (BlockHeader *) ((uptr) prevfooter - (prevfooter->size) - sizeof(BlockHeader));
        assert(leftheader->flag == FREE);
        leftheader->size = leftheader->size + h->size + sizeof(BlockHeader) + sizeof(BlockFooter);
        footer->size = leftheader->size;
        footer->flag = FREE;

        assert(((uptr)leftheader + sizeof(BlockHeader) + leftheader->size) == (uptr)footer);

        free_list_remove(h);
        
        h = leftheader;
    }

    // Merge right
    if ((uptr) rightheader < memory.end && rightheader->flag == FREE) {
        free_list_remove(rightheader);

        h->size = h->size + rightheader->size + sizeof(BlockHeader) + sizeof(BlockFooter);

        BlockFooter *newfooter = (BlockFooter *) ((uptr)h + sizeof(BlockHeader) + h->size); 
        newfooter->flag = FREE;
        newfooter->size = h->size;
    }
}

void print_memory()
{
    BlockHeader *h = &memory.head;

    printf("---------- FREE BLOCKS ------------\n");
    while((h = h->next) != &memory.head)
    {
        printf("BLOCK: size = %lu [%lu, %lu]\n", h->size, (uptr)h - memory.start, (uptr)h+sizeof(BlockHeader)+h->size+sizeof(BlockFooter) - memory.start);
    }
}

uptr alloc(usize amount)
{
    BlockHeader *current = &memory.head;

    while ((current = current->next) != &memory.head) {
        if (current->flag == FREE) {
            if (current->size >= amount) {
                uptr region = (uptr)current + sizeof(BlockHeader);

                current->flag = RESERVED;
                // current->next = 0;
                // current->prev = 0;

                BlockFooter *footer = (BlockFooter *)((uptr)current + sizeof(BlockHeader) + current->size);
                footer->flag        = RESERVED;

                usize remaining = current->size - amount;

                const usize K = 8;

                if (remaining > (sizeof(BlockHeader) + sizeof(BlockFooter) + K)) {
                    current->size = amount;
                    footer->size  = amount;

                    // Move current footer down
                    footer = memcpy((void *)((uptr)region + amount), (void *)footer, sizeof(BlockFooter));

                    // Create new block header
                    uptr newheaderloc     = (uptr)region + amount + sizeof(BlockFooter);
                    usize newblocksize    = remaining - (sizeof(BlockHeader) + sizeof(BlockFooter));
                    BlockHeader newheader = (BlockHeader){ .flag = FREE, .size = newblocksize, .next = 0, .prev = 0 };
                    BlockHeader *newblockheader =
                        memcpy((void *)((uptr)footer + sizeof(BlockFooter)), (void *)&newheader, sizeof(BlockHeader));

                    // Create new block footer
                    uptr newfooterloc           = (uptr)newheaderloc + newblocksize;
                    BlockFooter newfooter       = (BlockFooter){ .flag = FREE, .size = newblocksize };
                    BlockFooter *newblockfooter = memcpy((void *)newfooterloc, (void *)&newfooter, sizeof(BlockFooter));

                    // Replace current block with new block in the linked list
                    newblockheader->next = current->next;
                    newblockheader->prev = current->prev;
                    current->next->prev  = newblockheader;
                    current->prev->next  = newblockheader;
                } else {
                    // possibly allocate more
                    current->size = amount + remaining;
                    footer->size  = amount + remaining;

                    // Remove current block from the linked list
                    current->next->prev = current->prev;
                    current->prev->next = current->next;
                }

                current->next = 0;
                current->prev = 0;

                return region;
            }
        }
    }

    // assert(0);

    return 0;
}

usize counter = 0;
int main()
{
    raw_mem = (uptr)malloc(mem_size);

    assert(raw_mem);

    init_memory();
    printf("INITIAL\n");
    print_memory();

    int *p = 0;
    printf("ALLOCATIONS:\n");
    while ((p = (int *)alloc(100 * sizeof(int)))) {
        print_memory();
        for (int i = 0; i < 100; i++) {
            p[i] = counter;
        }
        counter++;
    }

    uptr c = memory.start;
    for (int i = 0; i < counter; i++) {
        c += sizeof(BlockHeader);
        int *ints = (int *)c;
        for (int k = 0; k < 100; k++) {
            // printf("%d\n", ints[k]);
            assert(ints[k] == i);
        }
        c += 100 * sizeof(int) + sizeof(BlockFooter);
    }

    usize M             = (100 * sizeof(int)) + sizeof(BlockHeader) + sizeof(BlockFooter);
    usize expectedcount = mem_size / M;

    assert(counter == expectedcount);

    printf("Allocated %llu arrays of 100 ints\n", (unsigned long long)counter);


    printf("RELEASES\n");
    c = memory.start;
    for (int i = 0; i < (counter); i++) {
        c += sizeof(BlockHeader);
        int *ints = (int *)c;
        if ((i % 2) == 0) {
            release((uptr)ints);
            print_memory();
        }
        c += 100 * sizeof(int) + sizeof(BlockFooter);
    }

    c = memory.start;
    for (int i = 0; i < (counter); i++) {
        c += sizeof(BlockHeader);
        int *ints = (int *)c;
        if ((i % 2) != 0) {
            release((uptr)ints);
            print_memory();
        }
        c += 100 * sizeof(int) + sizeof(BlockFooter);
    }
}
