#ifndef DS_ARENA_H
#define DS_ARENA_H

#include <stddef.h>

// Bump allocator over a caller-owned buffer. ctx is a ds_arena *
typedef struct ds_arena {
    unsigned char *base;
    size_t cap;
    size_t cur_offset;
} ds_arena;

void *ds_arena_alloc(void *ctx, size_t size, size_t align);

// Resizes in place if ptr is the most recent allocation and already aligned
// for align; otherwise moves (shrinks that need no move return ptr)
void *ds_arena_realloc(void *ctx, void *ptr, size_t old_size, size_t new_size,
                       size_t align);

// Only reclaims space if ptr is the most recent allocation; otherwise a no-op
void ds_arena_free(void *ctx, void *ptr, size_t size);

// Releases every allocation at once
void ds_arena_reset(void *ctx);

#endif
