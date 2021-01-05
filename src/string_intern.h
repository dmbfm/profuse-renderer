#ifndef __STRING_INTERN_H
#define __STRING_INTERN_H 

#include "common.h"
#include "allocator.h"

typedef struct StringInternEntry
{
    usize len;
    char *str;
} StringInternEntry;

typedef struct StringInternTable
{
    StringInternEntry *entries;
    Allocator *a;
    boolean initialized;
} StrigInternTable;


void string_intern_init(Allocator *a, StrigInternTable *table);
void string_intern_close(StrigInternTable *table);
const char *string_intern_range(StrigInternTable *table, char *start, char *end);
const char *string_intern(StrigInternTable *table, char *str);

#endif /* __STRING_INTERN_H */
