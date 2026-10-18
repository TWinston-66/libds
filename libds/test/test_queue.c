#include <ds/queue.h>

#include <criterion/criterion.h>

static ds_queue queue;

static void setup(void) {
    ds_queue_init(&queue);
}

TestSuite(queue, .init = setup);

/// BASE ///

Test(queue, starts_empty) {
    cr_assert(ds_queue_empty(&queue));
}

Test(queue, is_empty) {
    ds_queue_node a;

    ds_queue_enqueue(&queue, &a);
    cr_assert_eq(ds_queue_empty(&queue), false);

    ds_queue_dequeue(&queue);

    cr_assert_eq(ds_queue_empty(&queue), true);
    cr_assert_eq(queue.list.head.next, &queue.list.head);
    cr_assert_eq(queue.list.head.prev, &queue.list.head);
}

/// CORE ///

// Enqueue
Test(queue, enqueue) {
    ds_queue_node a, b;

    ds_queue_enqueue(&queue, &a);
    ds_queue_enqueue(&queue, &b);

    // head <-> a <-> b <-> head
    cr_assert_eq(queue.list.head.next, &a.lnode);
    cr_assert_eq(a.lnode.next, &b.lnode);
    cr_assert_eq(b.lnode.next, &queue.list.head);

    cr_assert_eq(queue.list.head.prev, &b.lnode);
    cr_assert_eq(b.lnode.prev, &a.lnode);
    cr_assert_eq(a.lnode.prev, &queue.list.head);
}

// Dequeue
Test(queue, dequeue) {
    ds_queue_node a, b;

    ds_queue_enqueue(&queue, &a);
    ds_queue_enqueue(&queue, &b);

    cr_assert_eq(ds_queue_dequeue(&queue), &a);
    cr_assert_eq(a.lnode.next, NULL);
    cr_assert_eq(a.lnode.prev, NULL);

    // head <-> b <-> head
    cr_assert_eq(queue.list.head.next, &b.lnode);
    cr_assert_eq(b.lnode.next, &queue.list.head);

    cr_assert_eq(queue.list.head.prev, &b.lnode);
    cr_assert_eq(b.lnode.prev, &queue.list.head);
}

Test(queue, dequeue_empty) {
    cr_assert_eq(ds_queue_dequeue(&queue), NULL);

    cr_assert_eq(queue.list.head.next, &queue.list.head);
    cr_assert_eq(queue.list.head.prev, &queue.list.head);
}

// Peek
Test(queue, peek) {
    ds_queue_node a, b;

    ds_queue_enqueue(&queue, &a);
    ds_queue_enqueue(&queue, &b);

    cr_assert_eq(ds_queue_peek(&queue), &a);
    cr_assert_eq(ds_queue_peek(&queue), &a);

    // head <-> a <-> b <-> head
    cr_assert_eq(queue.list.head.next, &a.lnode);
    cr_assert_eq(a.lnode.next, &b.lnode);
    cr_assert_eq(b.lnode.next, &queue.list.head);

    cr_assert_eq(queue.list.head.prev, &b.lnode);
    cr_assert_eq(b.lnode.prev, &a.lnode);
    cr_assert_eq(a.lnode.prev, &queue.list.head);

    ds_queue_dequeue(&queue);
    cr_assert_eq(ds_queue_peek(&queue), &b);
}

Test(queue, peek_empty) {
    cr_assert_eq(ds_queue_peek(&queue), NULL);

    cr_assert_eq(queue.list.head.next, &queue.list.head);
    cr_assert_eq(queue.list.head.prev, &queue.list.head);
}

// Drain
Test(queue, drain) {
    ds_queue_node a, b, c;

    ds_queue_enqueue(&queue, &a);
    ds_queue_enqueue(&queue, &b);
    ds_queue_enqueue(&queue, &c);

    cr_assert_eq(ds_queue_dequeue(&queue), &a);
    cr_assert_eq(ds_queue_dequeue(&queue), &b);
    cr_assert_eq(ds_queue_dequeue(&queue), &c);

    cr_assert_eq(ds_queue_empty(&queue), true);
    cr_assert_eq(queue.list.head.next, &queue.list.head);
    cr_assert_eq(queue.list.head.prev, &queue.list.head);
}

// Reinsert
Test(queue, reinsert) {
    ds_queue_node a, b;

    ds_queue_enqueue(&queue, &a);
    ds_queue_enqueue(&queue, &b);

    ds_queue_dequeue(&queue);
    ds_queue_enqueue(&queue, &a);

    // head <-> b <-> a <-> head
    cr_assert_eq(queue.list.head.next, &b.lnode);
    cr_assert_eq(b.lnode.next, &a.lnode);
    cr_assert_eq(a.lnode.next, &queue.list.head);

    cr_assert_eq(queue.list.head.prev, &a.lnode);
    cr_assert_eq(a.lnode.prev, &b.lnode);
    cr_assert_eq(b.lnode.prev, &queue.list.head);
}

// Many
Test(queue, many) {
    enum { N = 100 };
    ds_queue_node nodes[N];

    for (int i = 0; i < N; i++) {
        ds_queue_enqueue(&queue, &nodes[i]);
    }

    // Forward
    ds_dlist_node *cur = queue.list.head.next;
    for (int i = 0; i < N; i++) {
        cr_assert_eq(cur, &nodes[i].lnode);
        cur = cur->next;
    }
    cr_assert_eq(cur, &queue.list.head);

    // Backward
    cur = queue.list.head.prev;
    for (int i = N - 1; i >= 0; i--) {
        cr_assert_eq(cur, &nodes[i].lnode);
        cur = cur->prev;
    }
    cr_assert_eq(cur, &queue.list.head);

    // Drain
    for (int i = 0; i < N; i++) {
        cr_assert_eq(ds_queue_dequeue(&queue), &nodes[i]);
    }
    cr_assert(ds_queue_empty(&queue));
}
