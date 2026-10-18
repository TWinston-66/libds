#ifndef DS_UTIL_H
#define DS_UTIL_H

#define ds_container_of(ptr, type, member)                                     \
    ((type *)((char *)(ptr) - offsetof(type, member)))

#endif
