#include "toolbox.h"
#ifndef TOOLBOX_NO_STD_LIB

unsigned int t_file_len(FILE *f)
{
    unsigned int curr = ftell(f);
    fseek(f, 0, SEEK_END);
    unsigned int len = (unsigned int)ftell(f);
    fseek(f, curr, SEEK_SET);
    return len;
}

char **t_read_file_lines(char *filename, int *num_lines)
{
    FILE *file;

    file = fopen(filename, "rb");

    if (!file) {
        printf("Failed to open file %s\n", filename);
        return 0;
    }

    unsigned int len = t_file_len(file);

    char *buffer = (char *)t_malloc(len + 1);
    fread(buffer, 1, len, file);
    buffer[len] = 0;

    fclose(file);

    char *s = buffer;
    int lc  = 1;
    while (*s) {
        if (*s == '\n' || *s == '\r') {
            int crlf = (s[0] + s[1]) == ('\n' + '\r');
            if (crlf)
                s++;

            // Only count non-empty lines
            if (s[1]) {
                lc++;
            }
        }
        s++;
    }

    char **list = (char **)t_malloc(sizeof(*list) * (lc + 1) + len + 1);
    list[lc]    = 0;
    t_memcpy(&list[lc + 1], buffer, len + 1);
    t_free(buffer);
    buffer = (char *)&list[lc + 1];
    if (num_lines)
        *num_lines = lc;

    s       = buffer;
    list[0] = s;
    lc      = 1;
    while (*s) {
        if (*s == '\n' || *s == '\r') {
            int crlf = (s[0] + s[1]) == ('\n' + '\r');
            if (crlf)
                s++;
            *s = 0;
            // Only count non-empty lines
            if (s[1]) {
                list[lc] = s + 1;
                lc++;
            }
        }
        s++;
    }

    return list;
}

#endif
