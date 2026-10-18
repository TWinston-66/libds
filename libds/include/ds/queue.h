#ifndef DS_QUEUE_H
#define DS_QUEUE_H

#include <ds/dlist.h>

typedef struct ds_queue_node {
    ds_dlist_node lnode;
} ds_queue_node;

typedef struct ds_queue {
    ds_dlist list;
} ds_queue;

/// BASE ///

void ds_queue_init(ds_queue *queue);
bool ds_queue_empty(const ds_queue *queue);

/// CORE ///

// Enqueue
void ds_queue_enqueue(ds_queue *queue, ds_queue_node *node);

// Dequeue
ds_queue_node *ds_queue_dequeue(ds_queue *queue);

// Peek
ds_queue_node *ds_queue_peek(ds_queue *queue);

#endif
