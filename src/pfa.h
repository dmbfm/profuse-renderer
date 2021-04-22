#ifndef __PFA_H
#define __PFA_H 

#include "common.h"
#include "result.h"
#include "utils.h"
#include "list.h"

typedef struct PfaHeader {
    u8 magic[8];
    u8 crc;
    u32 offset;
    u32 file_length;
    u32 major_version;
    u32 minor_version;
} PfaHeader;

typedef struct PfaIndexEntry {
    u32 chunk_type;
    u32 offset;
} PfaIndexEntry;

typedef struct PfaIndex {
    PfaIndexEntry *entries;
    int count;
} PfaIndex;

typedef struct Pfa {
    PfaHeader header;
    PfaIndex index;
} Pfa;

typedef struct Reader {
    void *data;
    usize byte_length;
    usize cursor;
    usize saved_cursor;
} Reader;

ErrorCode reader_read_u8(Reader *r, u8 *out) {
    if (r->cursor >= r->byte_length) {
        *out = 0;
        return ERR_EOF;
    }

    *out = *(u8 *)(r->data + r->cursor++);
    return ERR_OK;
}

ErrorCode reader_read_u32(Reader *r, u32 *out) {
    if ((r->cursor + 4) >= r->byte_length) {
        *out = 0;
        return ERR_EOF;
    }

    *out = *(u32 *)(r->data + r->cursor);
    r->cursor += 4;

    return ERR_OK;
}

ErrorCode reader_read_bytes(Reader *r, u8 *out, usize n, int *bytes_copied) {
    int count = 0;
    for (;;) {

        if (count >= n) break;

        u8 b;
        if (reader_read_u8(r, &b) != ERR_OK) {
            break;
        } else {
            out[count++] = b;
        }
    }

    if (bytes_copied) 
        *bytes_copied = count;

    if (count < n) {
        return ERR_EOF;
    }

    return ERR_OK;
}

void reader_save_cursor(Reader *r) {
    r->saved_cursor = r->cursor;
}

void reader_restore_cursor(Reader *r) {
    r->cursor = r->saved_cursor;
}

void pfa_load(Allocator *a, Pfa *pfa, void *data, usize byte_length) {
    Reader r = { .data = data, .byte_length = byte_length, .cursor = 0 };

    reader_read_bytes(&r, pfa->header.magic, 8, 0);
    reader_read_u8(&r, &pfa->header.crc);
    reader_read_u32(&r, &pfa->header.offset);
    reader_read_u32(&r, &pfa->header.file_length);
    reader_read_u32(&r, &pfa->header.major_version);
    reader_read_u32(&r, &pfa->header.minor_version);

    DEBUGLOG("MAGIC = %s", (char *)pfa->header.magic);
    DEBUGLOG("CRC = %d", pfa->header.crc);
    DEBUGLOG("OFFSET = %d", pfa->header.offset);
    DEBUGLOG("FILE LENGTH = %d", pfa->header.file_length);
    DEBUGLOG("MAJOR VERSION = %d", pfa->header.major_version);
    DEBUGLOG("MINOR VERSION = %d", pfa->header.minor_version);
    
    //pfa->index.entries = list_new_cap(PfaIndexEntry, a, 10);
    pfa->index.count = 0;

    reader_save_cursor(&r);
    for (;;) {
        u32 entryType;
        u32 entryOffset;
        reader_read_u32(&r, &entryType);
        reader_read_u32(&r, &entryOffset);

        //DEBUGLOG("%d - %d", entryType, entryOffset);
        if (entryType == 0) {
            break;
        }

        //PfaIndexEntry e = { .chunk_type = entryType, .offset = entryOffset  };
        //DEBUGLOG("%d", pfa->index.count);
        //list_push(pfa->index.entries, e);
        pfa->index.count++;
    }
    
    reader_restore_cursor(&r);

    Result(uptr) r_entries = allocator_alloc(a, sizeof(PfaIndexEntry) * pfa->index.count);
    pfa->index.entries = (PfaIndexEntry *) result_unwrap(r_entries);

    pfa->index.count = 0;
    for (;;) {
        u32 entryType;
        u32 entryOffset;
        reader_read_u32(&r, &entryType);
        reader_read_u32(&r, &entryOffset);

        if (entryType == 0) {
            break;
        }

        PfaIndexEntry e = { .chunk_type = entryType, .offset = entryOffset  };
        //DEBUGLOG("%d", pfa->index.count);
        //list_push(pfa->index.entries, e);
        pfa->index.entries[pfa->index.count++] = e;
    }
    
    DEBUGLOG("Index count: %d", pfa->index.count);
}

#endif /* __PFA_H */
