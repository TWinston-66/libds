#include <ds/alloc/default.h>
#include <ds/util.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define DS_MALLOC_ALIGN _Alignof(max_align_t)

void *ds_libc_malloc(void *ctx, size_t size, size_t align) {
    (void)ctx;
    if (!DS_IS_POW2(align))
        return NULL;
    // size 0 still yields a unique freeable block
    if (size == 0)
        size = 1;
    if (align <= DS_MALLOC_ALIGN)
        return malloc(size);
    if (size > SIZE_MAX - (align - 1))
        return NULL;
    return aligned_alloc(align, (size_t)ds_align_up(size, align));
}

void *ds_libc_realloc(void *ctx, void *ptr, size_t old_size, size_t new_size,
                      size_t align) {
    if (!DS_IS_POW2(align))
        return NULL;
    if (new_size == 0) {
        new_size = 1;
    }
    if (align <= DS_MALLOC_ALIGN)
        return realloc(ptr, new_size);

    void *np = ds_libc_malloc(ctx, new_size, align);
    if (!np)
        return NULL;
    if (ptr) {
        memcpy(np, ptr, old_size < new_size ? old_size : new_size);
        free(ptr);
    }
    return np;
}

void ds_libc_free(void *ctx, void *ptr, size_t size) {
    (void)ctx;
    (void)size;
    free(ptr);
}
