#ifndef DS_DEFAULT_H
#define DS_DEFAULT_H

#include <stddef.h>

void *ds_libc_malloc(void *ctx, size_t size, size_t align);

void *ds_libc_realloc(void *ctx, void *ptr, size_t old_size, size_t new_size,
                      size_t align);

void ds_libc_free(void *ctx, void *ptr, size_t size);

#endif
