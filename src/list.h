#ifndef __LIST_H
#define __LIST_H

#include "allocator.h"
#include "common.h"
#include "result.h"
#include "slice.h"

// NOTE: Will using the header like we do in here cause alignment issues?

typedef struct ListHdr
{
    Allocator *a;
    usize len;
    usize cap;
} ListHdr;

// Create a new list
#define list_new(type, a) (type *)__list_init(a, 0, sizeof(type))

// Create a list with given capacity
#define list_new_cap(type, a, cap) (type *)__list_init(a, cap, sizeof(type))

// Returns a pointer to the list header
#define __list_hdr(l) ((ListHdr *)((uptr)(l) - sizeof(ListHdr)))

// Returns the lists length
#define list_len(l) __list_hdr(l)->len

// Returns the lists capacity
#define list_cap(l) __list_hdr(l)->cap

// Returns the lists allocator
#define __list_allocator(l) __list_hdr(l)->a

// Does the list need to grow so we can add 'n' elements?
#define __list_needs_grow(l, n) (list_len(l) + (n) >= list_cap(l))

// Grow if it needs to, else do nothing
#define __list_maybe_grow(l, n) (__list_needs_grow(l, n) ? __list_grow(l, n) : 0)

// Actually grow the list
#define __list_grow(l, n) (*((uptr *)&(l)) = __list_growf((uptr)(l), (n), sizeof(*(l))))

// Push an element to the list
#define list_push(l, v) (__list_maybe_grow(l, 1), (l)[list_len(l)++] = (v))

// Free the list
#define list_free(l) (__list_allocator(l)->free(__list_allocator(l), (uptr)l - sizeof(ListHdr)), (l) = 0)

#define list_fill_len(l, v)               \
    for (int i = 0; i < list_len(l); i++) \
    (l)[i] = (v)

#define list_fill_cap(l, v)               \
    for (int i = 0; i < list_cap(l); i++) \
    list_push(l, v)

#define list_fill_n(l, n, v)    \
    for (int i = 0; i < n; i++) \
    (n < list_len(l) ? (l)[i] = (v) : list_push(l, v))

#define list_for(name,l) for(usize name = 0; name < list_len(l); name++) 

#define list_as_slice(type, l) slice_from_array(type, (l), list_len(l))

static inline uptr __list_growf(uptr list, usize increment, usize element_size)
{
    if (!list)
        panic();

    uptr p;
    Result(uptr) rp;

    usize new_cap  = c_max(2 * list_cap(list), list_len(list) + increment);
    usize new_size = new_cap * element_size + 2 * sizeof(ListHdr);
    Allocator *a   = __list_allocator(list);

    rp = a->realloc(a, (uptr)__list_hdr(list), new_size);

    p = result_unwrap(rp);

    ListHdr *hdr = (ListHdr *)p;

    hdr->cap = new_cap;

    return p + sizeof(ListHdr);
}

static inline uptr __list_init(Allocator *a, usize cap, usize element_size)
{
    if (cap == 0)
        cap = 10;

    usize size = cap * element_size + sizeof(ListHdr);

    Result(uptr) rp = a->alloc(a, size);

    ListHdr *hdr = (ListHdr *)result_unwrap(rp);

    hdr->a   = a;
    hdr->len = 0;
    hdr->cap = cap;

    return (uptr)hdr + sizeof(ListHdr);
}

#endif /* __LIST_H */
