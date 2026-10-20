#include <ds/alloc/arena.h>
#include <ds/util.h>

#include <stddef.h>
#include <string.h>


static inline size_t reserved(size_t size) {
    return size ? size : 1;
}

void *ds_arena_alloc(void *ctx, size_t size, size_t align) {
    ds_arena *arena = ctx;
    if (!DS_IS_POW2(align))
        return NULL;
    size = reserved(size);
    uintptr_t current_adr = (uintptr_t)(arena->base + arena->cur_offset);
    uintptr_t aligned_adr = ds_align_up(current_adr, align);
    size_t aligned_offset = (size_t)(aligned_adr - (uintptr_t)arena->base);

    if (aligned_offset > arena->cap || size > arena->cap - aligned_offset) {
        return NULL;
    }

    arena->cur_offset = aligned_offset + size;

    return arena->base + aligned_offset;
}

static inline int is_last(ds_arena *a, unsigned char *p, size_t size) {
    return p + reserved(size) == a->base + a->cur_offset;
}

void *ds_arena_realloc(void *ctx, void *ptr, size_t old_size, size_t new_size,
                       size_t align) {
    ds_arena *arena = ctx;
    unsigned char *p = ptr;

    if (!DS_IS_POW2(align))
        return NULL;

    if (!p)
        return ds_arena_alloc(ctx, new_size, align);

    int aligned = (uintptr_t)p == ds_align_up((uintptr_t)p, align);

    // grow or shrink in place
    if (aligned && is_last(arena, p, old_size)) {
        size_t start = (size_t)(p - arena->base);
        if (reserved(new_size) > arena->cap - start)
            return NULL;
        arena->cur_offset = start + reserved(new_size);
        return p;
    }

    if (aligned && new_size <= old_size)
        return p;

    // misaligned for the new align, or not last and growing
    void *np = ds_arena_alloc(ctx, new_size, align);
    if (np)
        memcpy(np, p, old_size < new_size ? old_size : new_size);
    return np;
}

void ds_arena_free(void *ctx, void *ptr, size_t size) {
    ds_arena *arena = ctx;
    unsigned char *p = ptr;

    if (p && is_last(arena, p, size)) {
        arena->cur_offset = (size_t)(p - arena->base);
    }
}

void ds_arena_reset(void *ctx) {
    ds_arena *arena = ctx;
    arena->cur_offset = 0;
}
