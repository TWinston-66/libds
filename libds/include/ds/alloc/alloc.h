#ifndef DS_ALLOC_H
#define DS_ALLOC_H

#include <stddef.h>

typedef struct ds_alloc {
    void *(*alloc)(void *ctx, size_t size, size_t align);
    void *(*realloc)(void *ctx, void *ptr, size_t old_size, size_t new_size,
                     size_t align);
    void (*pop)(void *ctx, void *ptr, size_t size);
    void (*reset)(void *ctx);
    void *ctx;
} ds_alloc;

#endif
