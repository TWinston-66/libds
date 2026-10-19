#include <stddef.h>
#include <stdlib.h>

static void *libc_malloc(void *ctx, size_t size) {
    (void)ctx;
    return malloc(size);
}

static void *libc_realloc(void *ctx, void *ptr, size_t old, size_t new) {
    (void)ctx;
    (void)old;
    return realloc(ptr, new);
}
static void libc_free(void *ctx, void *ptr, size_t sz) {
    (void)ctx;
    (void)sz;
    free(ptr);
}
