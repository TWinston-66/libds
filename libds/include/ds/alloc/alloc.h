#ifndef DS_ALLOC_H
#define DS_ALLOC_H

#include <stddef.h>

// - align must be a power of two; anything else returns NULL
// - size 0 is treated as 1: it returns a unique pointer that must still be passed to free
// - Callers must pass back the exact size (or old_size) the block currently has; allocators may rely on it
typedef struct ds_alloc {
    // Returns NULL on failure
    void *(*alloc)(void *ctx, size_t size, size_t align);

    // ptr == NULL behaves like alloc
    // On success returns the (possibly moved) block with min(old_size, new_size) bytes preserved
    // On failure returns NULL and ptr is left valid and unchanged
    void *(*realloc)(void *ctx, void *ptr, size_t old_size, size_t new_size,
                     size_t align);

    // ptr may be NULL
    void (*free)(void *ctx, void *ptr, size_t size);

    void *ctx;
} ds_alloc;

#endif
