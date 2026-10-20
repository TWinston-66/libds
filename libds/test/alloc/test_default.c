#include <ds/alloc/alloc.h>
#include <ds/alloc/default.h>

#include <criterion/criterion.h>

#include <stdint.h>
#include <string.h>

static ds_alloc alloc;

static void setup(void) {

    alloc = (ds_alloc){
        .alloc = ds_libc_malloc,
        .realloc = ds_libc_realloc,
        .free = ds_libc_free,
        .ctx = NULL,
    };
}

TestSuite(default, .init = setup);

/// CORE ///

// Alloc
Test(default, alloc) {
    unsigned char *p = alloc.alloc(alloc.ctx, 64, 1);

    cr_assert_neq(p, NULL);
    memset(p, 0xAB, 64);

    alloc.free(alloc.ctx, p, 64);
}

Test(default, alloc_aligned) {
    for (size_t align = 1; align <= 4096; align <<= 1) {
        void *p = alloc.alloc(alloc.ctx, 10, align);

        cr_assert_neq(p, NULL);
        cr_assert_eq((uintptr_t)p % align, 0);

        alloc.free(alloc.ctx, p, 10);
    }
}

Test(default, alloc_bad_align) {
    size_t aligns[] = {0, 3, 6, 24};

    for (size_t i = 0; i < sizeof(aligns) / sizeof(aligns[0]); i++) {
        cr_assert_eq(alloc.alloc(alloc.ctx, 8, aligns[i]), NULL);
    }
}

Test(default, alloc_zero_size) {
    void *a = alloc.alloc(alloc.ctx, 0, 1);
    void *b = alloc.alloc(alloc.ctx, 0, 64);

    cr_assert_neq(a, NULL);
    cr_assert_neq(b, NULL);
    cr_assert_neq(a, b);
    cr_assert_eq((uintptr_t)b % 64, 0);

    alloc.free(alloc.ctx, a, 0);
    alloc.free(alloc.ctx, b, 0);
}

Test(default, alloc_overflow) {
    cr_assert_eq(alloc.alloc(alloc.ctx, SIZE_MAX, 64), NULL);
}

// Realloc
Test(default, realloc_null_is_alloc) {
    void *p = alloc.realloc(alloc.ctx, NULL, 0, 16, 1);

    cr_assert_neq(p, NULL);

    alloc.free(alloc.ctx, p, 16);
}

Test(default, realloc_grow) {
    unsigned char *p = alloc.alloc(alloc.ctx, 4, 1);
    memcpy(p, "abcd", 4);

    p = alloc.realloc(alloc.ctx, p, 4, 4096, 1);

    cr_assert_neq(p, NULL);
    cr_assert_eq(memcmp(p, "abcd", 4), 0);

    alloc.free(alloc.ctx, p, 4096);
}

Test(default, realloc_shrink) {
    unsigned char *p = alloc.alloc(alloc.ctx, 8, 1);
    memcpy(p, "abcdefgh", 8);

    p = alloc.realloc(alloc.ctx, p, 8, 4, 1);

    cr_assert_neq(p, NULL);
    cr_assert_eq(memcmp(p, "abcd", 4), 0);

    alloc.free(alloc.ctx, p, 4);
}

Test(default, realloc_keeps_alignment) {
    for (size_t align = 64; align <= 4096; align <<= 1) {
        unsigned char *p = alloc.alloc(alloc.ctx, 4, align);
        memcpy(p, "abcd", 4);

        p = alloc.realloc(alloc.ctx, p, 4, 1000, align);

        cr_assert_neq(p, NULL);
        cr_assert_eq((uintptr_t)p % align, 0);
        cr_assert_eq(memcmp(p, "abcd", 4), 0);

        alloc.free(alloc.ctx, p, 1000);
    }
}

Test(default, realloc_to_zero) {
    void *p = alloc.alloc(alloc.ctx, 16, 1);
    void *q = alloc.alloc(alloc.ctx, 16, 64);

    p = alloc.realloc(alloc.ctx, p, 16, 0, 1);
    q = alloc.realloc(alloc.ctx, q, 16, 0, 64);

    cr_assert_neq(p, NULL);
    cr_assert_neq(q, NULL);
    cr_assert_eq((uintptr_t)q % 64, 0);

    alloc.free(alloc.ctx, p, 0);
    alloc.free(alloc.ctx, q, 0);
}

Test(default, realloc_bad_align) {
    unsigned char *p = alloc.alloc(alloc.ctx, 4, 1);
    memcpy(p, "abcd", 4);

    cr_assert_eq(alloc.realloc(alloc.ctx, p, 4, 16, 0), NULL);
    cr_assert_eq(alloc.realloc(alloc.ctx, p, 4, 16, 24), NULL);

    cr_assert_eq(memcmp(p, "abcd", 4), 0);

    alloc.free(alloc.ctx, p, 4);
}

// Free
Test(default, free_null) {
    alloc.free(alloc.ctx, NULL, 0);
}
