#include "utils.h"
#include "allocator.h"
#include "common.h"
#include "format.h"
#include <stdarg.h>

#ifndef __wasm__
    #include <stdio.h>
    #include <stdlib.h>
#endif /* __wasm__ */

static void debug_log_output_string(const char *string) {
#if defined(__wasm__)
    common_wasm_print_message(string);
#elif defined(_WIN32)
    #include "win32_api.h"
    OutputDebugString(string);
    OutputDebugString("\n");
#else
    printf("%d\n", string);
#endif /* defined(__wasm__) */
}

void debug_log(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    size_t len = formatv(0, 0, fmt, args);
    len++;

#ifdef _MSC_VER
    char *buf = (char *)malloc(len);
#else
    char buf[len];
#endif

    formatv(buf, len, fmt, args);
    debug_log_output_string(buf);

#ifdef _MSC_VER
    free(buf);
#endif
    va_end(args);
}

#ifndef __wasm__

static usize file_byte_size(FILE *f) {
    u32 curr = ftell(f);
    fseek(f, 0, SEEK_END);
    usize size = ftell(f);
    fseek(f, curr, SEEK_SET);

    return size;
}

Result(Slice(charptr))
    read_file_lines(Allocator *a, const char *filename) {
    FILE *f;

    // Try opening the file
    f = fopen(filename, "rb");
    if (!f) {
        return result_error(Slice(charptr), ERR_OPEN_FILE);
    }

    // Calculate the file size
    usize len = file_byte_size(f);

    // Allocate buffer to read the file into
    Result(uptr) rbuffer = a->alloc(a, len + 1);
    result_raise(Slice(charptr), rbuffer);
    char *buffer = (char *)rbuffer.value;

    // Read the file
    fread(buffer, 1, len, f);
    buffer[len] = 0;

    // Close the file
    fclose(f);

    // Scan the file to count the number of lines
    char *c  = buffer;
    u32   lc = 0;
    while (*c != 0) {
        if (*c == '\n') {
            lc++;
        }

        c++;
    }

    // Allocate space for (LC + 1) array of char pointers +
    // all the text (len + 1)
    Result(uptr) r_list =
        a->alloc(a, lc * sizeof(charptr) + len + 1);
    result_raise(Slice(charptr), r_list);
    charptr *list = (charptr *)r_list.value;

    // Copy all the text after the list of char pointers has
    // ended
    memcpy(&list[lc], buffer, len + 1);

    // Free the text buffer
    a->free(a, (uptr)buffer);

    // Create a slice for the list
    Slice(charptr) slice =
        slice_from_array(charptr, list, lc);

    // Go trough all the text again...
    buffer = (char *)&list[lc];
    slice_set(slice, 0, buffer);
    c     = buffer;
    u32 n = 0;
    while (*c != 0) {
        if (*c == '\r' && *(c + 1) == '\n') {
            *c = 0;
            c++;
        }

        if (*c == '\n') {
            // When encountering a new line, we set it to 0,
            // so it becomes the end of a string
            *c = 0;

            // And then we set the start of the next string
            // in the list to be the address of the
            // character after the new line (except for the
            // last one!)
            n++;
            if (n < lc) slice_set(slice, n, (c + 1));
        }

        c++;
    }

    return result_ok(Slice(charptr), slice);
}

#endif /* __wasm__ */

#if defined(__RUN_TESTS)

    #include "test.h"

    #undef __RUN_TESTS
    #include "format.c"
    #include "heap.c"

test(read_file_lines) {
    Result(Slice(charptr)) r_lines =
        read_file_lines(&heap_allocator,
                        "test_data\\read_file_lines.txt");

    expect(result_is_ok(r_lines));

    Slice(charptr) lines = r_lines.value;

    expect(slice_len(lines) == 3);
    expect(string_compare(slice_get(lines, 0),
                          "This is line 1") == 0);
    expect(string_compare(slice_get(lines, 1),
                          "This is line 2") == 0);
    expect(string_compare(slice_get(lines, 2),
                          "And this is line 3!") == 0);
}

suite() {
    run_test(read_file_lines);
}

#endif
