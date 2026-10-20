#ifndef DS_UTIL_H
#define DS_UTIL_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#define ds_container_of(ptr, type, member)                                     \
    ((type *)((char *)(ptr) - offsetof(type, member)))

#define DS_IS_POW2(x) ((x) != 0 && ((x) & ((x) - 1)) == 0)

static inline uintptr_t ds_align_up(uintptr_t x, size_t align) {
    assert(DS_IS_POW2(align));
    return (x + (align - 1)) & ~(uintptr_t)(align - 1);
}

#endif
