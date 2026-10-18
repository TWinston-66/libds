#ifndef DS_DLIST_H
#define DS_DLIST_H

#include <stdbool.h>

typedef struct ds_dlist_node {
    struct ds_dlist_node *next, *prev;
} ds_dlist_node;

typedef struct ds_dlist {
    ds_dlist_node head; // dummy anchor
} ds_dlist;

/// BASE ///

void ds_dlist_init(ds_dlist *list);
bool ds_dlist_empty(const ds_dlist *list);

// Insert
void ds_dlist_insert_after(ds_dlist_node *pos, ds_dlist_node *node);
void ds_dlist_insert_before(ds_dlist_node *pos, ds_dlist_node *node);

/// CORE ///

// Push
void ds_dlist_push_back(ds_dlist *list, ds_dlist_node *node);
void ds_dlist_push_front(ds_dlist *list, ds_dlist_node *node);

// Pop
ds_dlist_node *ds_dlist_pop_back(ds_dlist *list);
ds_dlist_node *ds_dlist_pop_front(ds_dlist *list);

// Remove
void ds_dlist_remove(ds_dlist_node *node);

#endif
