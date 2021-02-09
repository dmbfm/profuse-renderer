#include "string_intern.h"
#include "common.h"
#include "list.h"
#include "utils.h"

void string_intern_init(Allocator *       a,
                        StrigInternTable *table) {
    table->entries     = list_new(StringInternEntry, a);
    table->initialized = true;
    table->a           = a;
}

void string_intern_close(StrigInternTable *table) {
    forn(i, list_len(table->entries)) {
        StringInternEntry e = table->entries[i];
        table->a->free(table->a, (uptr)e.str);
    }

    list_free(table->entries);
}

const char *string_intern_range(StrigInternTable *table,
                                char *            start,
                                char *            end) {
    assert(table->initialized);
    assert(start);
    assert(end);
    assert(end >= start);

    usize len = end - start;

    // Go trough each entry in the table
    forn(i, list_len(table->entries)) {
        StringInternEntry e = table->entries[i];

        // If the strings are equal, return the canonical
        // one
        if (e.len == len &&
            string_compare_len(start, e.str, e.len) == 0) {
            return e.str;
        }
    }

    // Alocate space for new string
    Result(uptr) ra = table->a->alloc(table->a, len + 1);
    char *str       = (char *)result_unwrap(ra);

    // Copy new string to allocated space
    memcpy(str, start, len);
    str[len] = 0;

    // Create new entry
    StringInternEntry e = {.str = str, .len = len};

    // Add to current entries
    list_push(table->entries, e);

    // Return the added/canonical string
    return str;
}

const char *string_intern(StrigInternTable *table,
                          char *            str) {
    return string_intern_range(table,
                               str,
                               str + string_len(str));
}

#if defined(__RUN_TESTS)

    #include "test.h"

    #undef __RUN_TESTS
    #include "heap.c"

test(string_intern) {
    StrigInternTable t = {0};
    string_intern_init(&heap_allocator, &t);

    const char *name     = string_intern(&t, "Name");
    const char *sym_int  = string_intern(&t, "int");
    const char *sym_char = string_intern(&t, "char");
    const char *op_add   = string_intern(&t, "+");

    expect(name == string_intern(&t, "Name"));
    expect(sym_int == string_intern(&t, "int"));
    expect(sym_char == string_intern(&t, "char"));
    expect(op_add == string_intern(&t, "+"));

    string_intern_close(&t);
}

suite() {
    run_test(string_intern);
}

#endif /* defined(__RUN_TESTS) */
