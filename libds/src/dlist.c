#include <ds/dlist.h>

#include <stddef.h>

///// BASE /////

void ds_dlist_init(ds_dlist *list) {
    list->head.next = &list->head;
    list->head.prev = &list->head;
}

bool ds_dlist_empty(const ds_dlist *list) {
    return list->head.next == &list->head;
}

// Insert before/after
void ds_dlist_insert_before(ds_dlist_node *pos, ds_dlist_node *node) {
    ds_dlist_node *tmp = pos->prev;
    pos->prev = node;
    node->next = pos;

    node->prev = tmp;
    tmp->next = node;
}

void ds_dlist_insert_after(ds_dlist_node *pos, ds_dlist_node *node) {
    ds_dlist_node *tmp = pos->next;
    pos->next = node;
    node->next = tmp;

    node->prev = pos;
    tmp->prev = node;
}

///// CORE /////

// push
void ds_dlist_push_back(ds_dlist *list, ds_dlist_node *node) {
    ds_dlist_insert_before(&list->head, node);
}

void ds_dlist_push_front(ds_dlist *list, ds_dlist_node *node) {
    ds_dlist_insert_after(&list->head, node);
}

// pop
ds_dlist_node *ds_dlist_pop_back(ds_dlist *list) {
    if (ds_dlist_empty(list))
        return NULL;
    ds_dlist_node *node = list->head.prev;
    ds_dlist_remove(node);
    return node;
}

ds_dlist_node *ds_dlist_pop_front(ds_dlist *list) {
    if (ds_dlist_empty(list))
        return NULL;
    ds_dlist_node *node = list->head.next;
    ds_dlist_remove(node);
    return node;
}

// remove
void ds_dlist_remove(ds_dlist_node *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;

    node->next = NULL;
    node->prev = NULL;
}
