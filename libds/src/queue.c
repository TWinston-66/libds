#include <ds/queue.h>

#include <ds/util.h>

#include <stddef.h>

/// BASE ///

void ds_queue_init(ds_queue *queue) {
    ds_dlist_init(&queue->list);
}

bool ds_queue_empty(const ds_queue *queue) {
    return ds_dlist_empty(&queue->list);
}

/// CORE ///

// Enqueue
void ds_queue_enqueue(ds_queue *queue, ds_queue_node *node) {
    ds_dlist_push_back(&queue->list, &node->lnode);
}

// Dequeue
ds_queue_node *ds_queue_dequeue(ds_queue *queue) {
    ds_dlist_node *tmp = ds_dlist_pop_front(&queue->list);
    if (!tmp) {
        return NULL;
    }
    return ds_container_of(tmp, ds_queue_node, lnode);
}

// Peek
ds_queue_node *ds_queue_peek(ds_queue *queue) {
    if (ds_queue_empty(queue)) {
        return NULL;
    }
    ds_dlist_node *node = queue->list.head.next;
    return ds_container_of(node, ds_queue_node, lnode);
}
