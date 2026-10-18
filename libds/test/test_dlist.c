#include <ds/dlist.h>

#include <criterion/criterion.h>

static ds_dlist list;

static void setup(void) {
    ds_dlist_init(&list);
}

TestSuite(dlist, .init = setup);

/// BASE ///

Test(dlist, starts_empty) {
    cr_assert(ds_dlist_empty(&list));
}

Test(dlist, is_empty) {
    ds_dlist_node a;

    ds_dlist_push_back(&list, &a);
    cr_assert_eq(ds_dlist_empty(&list), false);

    ds_dlist_pop_front(&list);

    cr_assert_eq(ds_dlist_empty(&list), true);
    cr_assert_eq(list.head.next, &list.head);
    cr_assert_eq(list.head.prev, &list.head);
}

// Insert
Test(dlist, insert_before) {
    ds_dlist_node a, b, c;

    ds_dlist_push_back(&list, &a);

    ds_dlist_insert_before(&a, &b);

    // head <-> b <-> a <-> head
    cr_assert_eq(list.head.next, &b);
    cr_assert_eq(b.next, &a);
    cr_assert_eq(a.next, &list.head);

    cr_assert_eq(list.head.prev, &a);
    cr_assert_eq(a.prev, &b);
    cr_assert_eq(b.prev, &list.head);

    ds_dlist_insert_before(&a, &c);

    // head <-> b <-> c <-> a <-> head
    cr_assert_eq(list.head.next, &b);
    cr_assert_eq(b.next, &c);
    cr_assert_eq(c.next, &a);
    cr_assert_eq(a.next, &list.head);

    cr_assert_eq(list.head.prev, &a);
    cr_assert_eq(a.prev, &c);
    cr_assert_eq(c.prev, &b);
    cr_assert_eq(b.prev, &list.head);
}

Test(dlist, insert_after) {
    ds_dlist_node a, b, c;

    ds_dlist_push_back(&list, &a);

    ds_dlist_insert_after(&a, &b);

    // head <-> a <-> b <-> head
    cr_assert_eq(list.head.next, &a);
    cr_assert_eq(a.next, &b);
    cr_assert_eq(b.next, &list.head);

    cr_assert_eq(list.head.prev, &b);
    cr_assert_eq(b.prev, &a);
    cr_assert_eq(a.prev, &list.head);

    ds_dlist_insert_after(&a, &c);

    // head <-> a <-> c <-> b <-> head
    cr_assert_eq(list.head.next, &a);
    cr_assert_eq(a.next, &c);
    cr_assert_eq(c.next, &b);
    cr_assert_eq(b.next, &list.head);

    cr_assert_eq(list.head.prev, &b);
    cr_assert_eq(b.prev, &c);
    cr_assert_eq(c.prev, &a);
    cr_assert_eq(a.prev, &list.head);
}

/// CORE ///

// Push
Test(dlist, push_back) {
    ds_dlist_node a, b;

    ds_dlist_push_back(&list, &a);
    ds_dlist_push_back(&list, &b);

    // head <-> a <-> b <-> head
    cr_assert_eq(list.head.next, &a);
    cr_assert_eq(b.next, &list.head);
    cr_assert_eq(a.next, &b);

    cr_assert_eq(list.head.prev, &b);
    cr_assert_eq(a.prev, &list.head);
    cr_assert_eq(b.prev, &a);
}

Test(dlist, push_front) {
    ds_dlist_node a, b;

    ds_dlist_push_back(&list, &a);
    ds_dlist_push_front(&list, &b);

    // head <-> b <-> a <-> head
    cr_assert_eq(list.head.next, &b);
    cr_assert_eq(b.next, &a);
    cr_assert_eq(a.next, &list.head);

    cr_assert_eq(list.head.prev, &a);
    cr_assert_eq(a.prev, &b);
    cr_assert_eq(b.prev, &list.head);
}

// Pop
Test(dlist, pop_back) {
    ds_dlist_node a, b;

    ds_dlist_push_back(&list, &a);
    ds_dlist_push_back(&list, &b);

    cr_assert_eq(ds_dlist_pop_back(&list), &b);
    cr_assert_eq(b.next, NULL);
    cr_assert_eq(b.prev, NULL);

    // head <-> a <-> head
    cr_assert_eq(list.head.next, &a);
    cr_assert_eq(a.next, &list.head);

    cr_assert_eq(list.head.prev, &a);
    cr_assert_eq(a.prev, &list.head);
}

Test(dlist, pop_front) {
    ds_dlist_node a, b;

    ds_dlist_push_back(&list, &a);
    ds_dlist_push_back(&list, &b);

    cr_assert_eq(ds_dlist_pop_front(&list), &a);
    cr_assert_eq(a.next, NULL);
    cr_assert_eq(a.prev, NULL);

    // head <-> b <-> head
    cr_assert_eq(list.head.next, &b);
    cr_assert_eq(b.next, &list.head);

    cr_assert_eq(list.head.prev, &b);
    cr_assert_eq(b.prev, &list.head);
}

Test(dlist, pop_empty) {
    cr_assert_eq(ds_dlist_pop_front(&list), NULL);
    cr_assert_eq(ds_dlist_pop_back(&list), NULL);

    cr_assert_eq(list.head.next, &list.head);
    cr_assert_eq(list.head.prev, &list.head);
}

// Remove
Test(dlist, remove) {
    ds_dlist_node a, b, c;

    ds_dlist_push_back(&list, &a);
    ds_dlist_push_back(&list, &b);
    ds_dlist_push_back(&list, &c);

    ds_dlist_remove(&b);

    // head <-> a <-> c <-> head
    cr_assert_eq(list.head.next, &a);
    cr_assert_eq(a.next, &c);
    cr_assert_eq(c.next, &list.head);

    cr_assert_eq(list.head.prev, &c);
    cr_assert_eq(c.prev, &a);
    cr_assert_eq(a.prev, &list.head);

    // NULL <-> b <-> NULL
    cr_assert_eq(b.next, NULL);
    cr_assert_eq(b.prev, NULL);
}

Test(dlist, remove_first) {
    ds_dlist_node a, b, c;

    ds_dlist_push_back(&list, &a);
    ds_dlist_push_back(&list, &b);
    ds_dlist_push_back(&list, &c);

    ds_dlist_remove(&a);

    // head <-> b <-> c <-> head
    cr_assert_eq(list.head.next, &b);
    cr_assert_eq(b.next, &c);
    cr_assert_eq(c.next, &list.head);

    cr_assert_eq(list.head.prev, &c);
    cr_assert_eq(c.prev, &b);
    cr_assert_eq(b.prev, &list.head);

    // NULL <-> a <-> NULL
    cr_assert_eq(a.next, NULL);
    cr_assert_eq(a.prev, NULL);
}

Test(dlist, remove_last) {
    ds_dlist_node a, b, c;

    ds_dlist_push_back(&list, &a);
    ds_dlist_push_back(&list, &b);
    ds_dlist_push_back(&list, &c);

    ds_dlist_remove(&c);

    // head <-> a <-> b <-> head
    cr_assert_eq(list.head.next, &a);
    cr_assert_eq(a.next, &b);
    cr_assert_eq(b.next, &list.head);

    cr_assert_eq(list.head.prev, &b);
    cr_assert_eq(b.prev, &a);
    cr_assert_eq(a.prev, &list.head);

    // NULL <-> c <-> NULL
    cr_assert_eq(c.next, NULL);
    cr_assert_eq(c.prev, NULL);
}

Test(dlist, remove_only) {
    ds_dlist_node a;

    ds_dlist_push_back(&list, &a);

    ds_dlist_remove(&a);

    // head <-> head
    cr_assert_eq(ds_dlist_empty(&list), true);
    cr_assert_eq(list.head.next, &list.head);
    cr_assert_eq(list.head.prev, &list.head);

    // NULL <-> a <-> NULL
    cr_assert_eq(a.next, NULL);
    cr_assert_eq(a.prev, NULL);
}

// Reinsert
Test(dlist, reinsert) {
    ds_dlist_node a, b;

    ds_dlist_push_back(&list, &a);
    ds_dlist_push_back(&list, &b);

    ds_dlist_pop_front(&list);
    ds_dlist_push_back(&list, &a);

    // head <-> b <-> a <-> head
    cr_assert_eq(list.head.next, &b);
    cr_assert_eq(b.next, &a);
    cr_assert_eq(a.next, &list.head);

    cr_assert_eq(list.head.prev, &a);
    cr_assert_eq(a.prev, &b);
    cr_assert_eq(b.prev, &list.head);
}

// Many
Test(dlist, many) {
    enum { N = 100 };
    ds_dlist_node nodes[N];

    for (int i = 0; i < N; i++) {
        ds_dlist_push_back(&list, &nodes[i]);
    }

    // Forward
    ds_dlist_node *cur = list.head.next;
    for (int i = 0; i < N; i++) {
        cr_assert_eq(cur, &nodes[i]);
        cur = cur->next;
    }
    cr_assert_eq(cur, &list.head);

    // Backward
    cur = list.head.prev;
    for (int i = N - 1; i >= 0; i--) {
        cr_assert_eq(cur, &nodes[i]);
        cur = cur->prev;
    }
    cr_assert_eq(cur, &list.head);

    // Drain from both ends
    for (int i = 0; i < N / 2; i++) {
        cr_assert_eq(ds_dlist_pop_front(&list), &nodes[i]);
        cr_assert_eq(ds_dlist_pop_back(&list), &nodes[N - 1 - i]);
    }
    cr_assert(ds_dlist_empty(&list));
}
