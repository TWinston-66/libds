#include <ds/alloc/alloc.h>
#include <ds/alloc/arena.h>

#include <criterion/criterion.h>

#include <stdint.h>
#include <string.h>

static _Alignas(64) unsigned char buf[1 << 16];
static ds_alloc alloc;
static ds_arena arena;

static void setup(void) {

    arena = (ds_arena){buf, sizeof(buf), 0};

    alloc = (ds_alloc){.alloc = ds_arena_alloc,
                       .realloc = ds_arena_realloc,
                       .free = ds_arena_free,
                       .ctx = &arena};
}

TestSuite(arena, .init = setup);

/// BASE ///

Test(arena, starts_empty) {
    cr_assert_eq(arena.cur_offset, 0);
}

Test(arena, first_alloc_is_base) {
    cr_assert_eq(alloc.alloc(alloc.ctx, 8, 1), buf);
    cr_assert_eq(arena.cur_offset, 8);
}

/// CORE ///

// Alloc
Test(arena, alloc_sequential) {
    unsigned char *a = alloc.alloc(alloc.ctx, 8, 1);
    unsigned char *b = alloc.alloc(alloc.ctx, 8, 1);

    cr_assert_eq(a, buf);
    cr_assert_eq(b, buf + 8);
    cr_assert_eq(arena.cur_offset, 16);
}

Test(arena, alloc_aligned) {
    for (size_t align = 1; align <= 64; align <<= 1) {
        // knock the offset off alignment first
        alloc.alloc(alloc.ctx, 1, 1);

        void *p = alloc.alloc(alloc.ctx, 1, align);
        cr_assert_neq(p, NULL);
        cr_assert_eq((uintptr_t)p % align, 0);
    }
}

Test(arena, alloc_bad_align) {
    size_t aligns[] = {0, 3, 6, 24};

    for (size_t i = 0; i < sizeof(aligns) / sizeof(aligns[0]); i++) {
        cr_assert_eq(alloc.alloc(alloc.ctx, 8, aligns[i]), NULL);
    }
    cr_assert_eq(arena.cur_offset, 0);
}

Test(arena, alloc_zero_size) {
    void *a = alloc.alloc(alloc.ctx, 0, 1);
    void *b = alloc.alloc(alloc.ctx, 0, 1);

    cr_assert_neq(a, NULL);
    cr_assert_neq(b, NULL);
    cr_assert_neq(a, b);
    cr_assert_eq(arena.cur_offset, 2);
}

Test(arena, alloc_exact_fit) {
    arena.cap = 16;

    cr_assert_eq(alloc.alloc(alloc.ctx, 16, 1), buf);
    cr_assert_eq(arena.cur_offset, 16);
    cr_assert_eq(alloc.alloc(alloc.ctx, 1, 1), NULL);
}

Test(arena, alloc_out_of_space) {
    arena.cap = 16;

    cr_assert_eq(alloc.alloc(alloc.ctx, 17, 1), NULL);
    cr_assert_eq(arena.cur_offset, 0);

    alloc.alloc(alloc.ctx, 9, 1);
    cr_assert_eq(alloc.alloc(alloc.ctx, 4, 8), NULL);
    cr_assert_eq(arena.cur_offset, 9);
}

// Realloc
Test(arena, realloc_null_is_alloc) {
    cr_assert_eq(alloc.realloc(alloc.ctx, NULL, 0, 8, 1), buf);
    cr_assert_eq(arena.cur_offset, 8);
}

Test(arena, realloc_grow_last_in_place) {
    unsigned char *p = alloc.alloc(alloc.ctx, 8, 1);
    memset(p, 0xAB, 8);

    cr_assert_eq(alloc.realloc(alloc.ctx, p, 8, 32, 1), p);
    cr_assert_eq(arena.cur_offset, 32);
    for (int i = 0; i < 8; i++) {
        cr_assert_eq(p[i], 0xAB);
    }
}

Test(arena, realloc_shrink_last_in_place) {
    unsigned char *p = alloc.alloc(alloc.ctx, 32, 1);

    cr_assert_eq(alloc.realloc(alloc.ctx, p, 32, 8, 1), p);
    cr_assert_eq(arena.cur_offset, 8);
}

Test(arena, realloc_grow_not_last_moves) {
    unsigned char *p = alloc.alloc(alloc.ctx, 4, 1);
    alloc.alloc(alloc.ctx, 4, 1);
    memcpy(p, "abcd", 4);

    unsigned char *np = alloc.realloc(alloc.ctx, p, 4, 16, 1);

    cr_assert_neq(np, NULL);
    cr_assert_neq(np, p);
    cr_assert_eq(memcmp(np, "abcd", 4), 0);
    cr_assert_eq(arena.cur_offset, 24);
}

Test(arena, realloc_shrink_not_last_stays) {
    unsigned char *p = alloc.alloc(alloc.ctx, 16, 1);
    alloc.alloc(alloc.ctx, 4, 1);

    cr_assert_eq(alloc.realloc(alloc.ctx, p, 16, 4, 1), p);
    cr_assert_eq(arena.cur_offset, 20);
}

Test(arena, realloc_misaligned_last_moves) {
    alloc.alloc(alloc.ctx, 1, 1);
    unsigned char *p = alloc.alloc(alloc.ctx, 3, 1); 
    memcpy(p, "xyz", 3);

    unsigned char *np = alloc.realloc(alloc.ctx, p, 3, 16, 16);

    cr_assert_neq(np, NULL);
    cr_assert_neq(np, p);
    cr_assert_eq((uintptr_t)np % 16, 0);
    cr_assert_eq(memcmp(np, "xyz", 3), 0);
}

Test(arena, realloc_misaligned_shrink_moves) {
    alloc.alloc(alloc.ctx, 1, 1);
    unsigned char *p = alloc.alloc(alloc.ctx, 8, 1);
    alloc.alloc(alloc.ctx, 1, 1);
    memcpy(p, "abcdefgh", 8);

    unsigned char *np = alloc.realloc(alloc.ctx, p, 8, 4, 8);

    cr_assert_neq(np, NULL);
    cr_assert_neq(np, p);
    cr_assert_eq((uintptr_t)np % 8, 0);
    cr_assert_eq(memcmp(np, "abcd", 4), 0);
}

Test(arena, realloc_to_zero) {
    unsigned char *p = alloc.alloc(alloc.ctx, 8, 1);

    cr_assert_eq(alloc.realloc(alloc.ctx, p, 8, 0, 1), p);

    cr_assert_neq(alloc.alloc(alloc.ctx, 4, 1), p);
}

Test(arena, realloc_fail_keeps_block) {
    arena.cap = 16;

    unsigned char *p = alloc.alloc(alloc.ctx, 8, 1);
    memcpy(p, "abcdefgh", 8);

    cr_assert_eq(alloc.realloc(alloc.ctx, p, 8, 32, 1), NULL);
    cr_assert_eq(arena.cur_offset, 8);
    cr_assert_eq(memcmp(p, "abcdefgh", 8), 0);


    alloc.alloc(alloc.ctx, 4, 1);

    cr_assert_eq(alloc.realloc(alloc.ctx, p, 8, 12, 1), NULL);
    cr_assert_eq(arena.cur_offset, 12);
    cr_assert_eq(memcmp(p, "abcdefgh", 8), 0);
}

Test(arena, realloc_bad_align) {
    unsigned char *p = alloc.alloc(alloc.ctx, 8, 1);

    cr_assert_eq(alloc.realloc(alloc.ctx, p, 8, 16, 0), NULL);
    cr_assert_eq(alloc.realloc(alloc.ctx, p, 8, 16, 3), NULL);
    cr_assert_eq(arena.cur_offset, 8);
}

// Free
Test(arena, free_last_reclaims) {
    alloc.alloc(alloc.ctx, 8, 1);
    void *p = alloc.alloc(alloc.ctx, 8, 1);

    alloc.free(alloc.ctx, p, 8);

    cr_assert_eq(arena.cur_offset, 8);
    cr_assert_eq(alloc.alloc(alloc.ctx, 8, 1), p);
}

Test(arena, free_not_last_noop) {
    void *p = alloc.alloc(alloc.ctx, 8, 1);
    alloc.alloc(alloc.ctx, 8, 1);

    alloc.free(alloc.ctx, p, 8);

    cr_assert_eq(arena.cur_offset, 16);
}

Test(arena, free_reverse_order) {
    void *a = alloc.alloc(alloc.ctx, 8, 1);
    void *b = alloc.alloc(alloc.ctx, 8, 1);
    void *c = alloc.alloc(alloc.ctx, 8, 1);

    alloc.free(alloc.ctx, c, 8);
    alloc.free(alloc.ctx, b, 8);
    alloc.free(alloc.ctx, a, 8);

    cr_assert_eq(arena.cur_offset, 0);
}

Test(arena, free_zero_size_last) {
    alloc.alloc(alloc.ctx, 8, 1);
    void *p = alloc.alloc(alloc.ctx, 0, 1);

    alloc.free(alloc.ctx, p, 0);

    cr_assert_eq(arena.cur_offset, 8);
}

Test(arena, free_null) {
    alloc.alloc(alloc.ctx, 8, 1);

    alloc.free(alloc.ctx, NULL, 0);

    cr_assert_eq(arena.cur_offset, 8);
}

// Reset
Test(arena, reset) {
    alloc.alloc(alloc.ctx, 8, 1);
    alloc.alloc(alloc.ctx, 8, 1);

    ds_arena_reset(&arena);

    cr_assert_eq(arena.cur_offset, 0);
    cr_assert_eq(alloc.alloc(alloc.ctx, 8, 1), buf);
}

// Many
Test(arena, many) {
    enum { N = 100 };
    unsigned char *ptrs[N];

    for (int i = 0; i < N; i++) {
        ptrs[i] = alloc.alloc(alloc.ctx, sizeof(int), _Alignof(int));
        cr_assert_neq(ptrs[i], NULL);
        cr_assert_eq((uintptr_t)ptrs[i] % _Alignof(int), 0);
        memcpy(ptrs[i], &i, sizeof(int));
    }

    for (int i = 0; i < N; i++) {
        int v;
        memcpy(&v, ptrs[i], sizeof(int));
        cr_assert_eq(v, i);
    }

    // Drain
    for (int i = N - 1; i >= 0; i--) {
        alloc.free(alloc.ctx, ptrs[i], sizeof(int));
    }
    cr_assert_eq(arena.cur_offset, 0);
}
