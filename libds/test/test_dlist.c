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

// Insert
Test(dlist, insert_before) {
    ds_dlist_node a, b;

    ds_dlist_push_back(&list, &a);

    ds_dlist_insert_before(&a, &b);

    // head <-> b <-> a <-> head
    cr_assert_eq(list.head.next, &b);
    cr_assert_eq(b.next, &a);
    cr_assert_eq(a.next, &list.head);

    cr_assert_eq(list.head.prev, &a);
    cr_assert_eq(a.prev, &b);
    cr_assert_eq(b.prev, &list.head);
}

Test(dlist, insert_after) {
    ds_dlist_node a, b;

    ds_dlist_push_back(&list, &a);

    ds_dlist_insert_after(&a, &b);

    // head <-> a <-> b <-> head
    cr_assert_eq(list.head.next, &a);
    cr_assert_eq(b.next, &list.head);
    cr_assert_eq(a.next, &b);

    cr_assert_eq(list.head.prev, &b);
    cr_assert_eq(a.prev, &list.head);
    cr_assert_eq(b.prev, &a);
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

    ds_dlist_pop_front(&list);

    // head <-> b <-> head
    cr_assert_eq(list.head.next, &b);
    cr_assert_eq(b.next, &list.head);

    cr_assert_eq(list.head.prev, &b);
    cr_assert_eq(b.prev, &list.head);
}

Test(dlist, pop_empty) {
    ds_dlist_pop_front(&list);

    cr_assert_eq(list.head.next, &list.head);
    cr_assert_eq(list.head.prev, &list.head);
}

// remove
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
