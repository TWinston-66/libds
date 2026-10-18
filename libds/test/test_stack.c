#include <ds/stack.h>

#include <criterion/criterion.h>

static ds_stack stack;

static void setup(void) {
    ds_stack_init(&stack);
}

TestSuite(stack, .init = setup);

/// BASE ///

Test(stack, starts_empty) {
    cr_assert(ds_stack_empty(&stack));
}

Test(stack, is_empty) {
    ds_stack_node a;

    ds_stack_push(&stack, &a);
    cr_assert_eq(ds_stack_empty(&stack), false);

    ds_stack_pop(&stack);

    cr_assert_eq(ds_stack_empty(&stack), true);
    cr_assert_eq(stack.list.head.next, &stack.list.head);
    cr_assert_eq(stack.list.head.prev, &stack.list.head);
}

/// CORE ///

// Push
Test(stack, push) {
    ds_stack_node a, b;

    ds_stack_push(&stack, &a);
    ds_stack_push(&stack, &b);

    // head <-> b <-> a <-> head
    cr_assert_eq(stack.list.head.next, &b.lnode);
    cr_assert_eq(b.lnode.next, &a.lnode);
    cr_assert_eq(a.lnode.next, &stack.list.head);

    cr_assert_eq(stack.list.head.prev, &a.lnode);
    cr_assert_eq(a.lnode.prev, &b.lnode);
    cr_assert_eq(b.lnode.prev, &stack.list.head);
}

// Pop
Test(stack, pop) {
    ds_stack_node a, b;

    ds_stack_push(&stack, &a);
    ds_stack_push(&stack, &b);

    cr_assert_eq(ds_stack_pop(&stack), &b);
    cr_assert_eq(b.lnode.next, NULL);
    cr_assert_eq(b.lnode.prev, NULL);

    // head <-> a <-> head
    cr_assert_eq(stack.list.head.next, &a.lnode);
    cr_assert_eq(a.lnode.next, &stack.list.head);

    cr_assert_eq(stack.list.head.prev, &a.lnode);
    cr_assert_eq(a.lnode.prev, &stack.list.head);
}

Test(stack, pop_empty) {
    cr_assert_eq(ds_stack_pop(&stack), NULL);

    cr_assert_eq(stack.list.head.next, &stack.list.head);
    cr_assert_eq(stack.list.head.prev, &stack.list.head);
}

// Drain
Test(stack, drain) {
    ds_stack_node a, b, c;

    ds_stack_push(&stack, &a);
    ds_stack_push(&stack, &b);
    ds_stack_push(&stack, &c);

    cr_assert_eq(ds_stack_pop(&stack), &c);
    cr_assert_eq(ds_stack_pop(&stack), &b);
    cr_assert_eq(ds_stack_pop(&stack), &a);

    cr_assert_eq(ds_stack_empty(&stack), true);
    cr_assert_eq(stack.list.head.next, &stack.list.head);
    cr_assert_eq(stack.list.head.prev, &stack.list.head);
}

// Reinsert
Test(stack, reinsert) {
    ds_stack_node a, b;

    ds_stack_push(&stack, &a);
    ds_stack_push(&stack, &b);

    ds_stack_pop(&stack);
    ds_stack_pop(&stack);
    ds_stack_push(&stack, &b);
    ds_stack_push(&stack, &a);

    // head <-> a <-> b <-> head
    cr_assert_eq(stack.list.head.next, &a.lnode);
    cr_assert_eq(a.lnode.next, &b.lnode);
    cr_assert_eq(b.lnode.next, &stack.list.head);

    cr_assert_eq(stack.list.head.prev, &b.lnode);
    cr_assert_eq(b.lnode.prev, &a.lnode);
    cr_assert_eq(a.lnode.prev, &stack.list.head);
}

// Many
Test(stack, many) {
    enum { N = 100 };
    ds_stack_node nodes[N];

    for (int i = 0; i < N; i++) {
        ds_stack_push(&stack, &nodes[i]);
    }

    // Forward
    ds_dlist_node *cur = stack.list.head.next;
    for (int i = N - 1; i >= 0; i--) {
        cr_assert_eq(cur, &nodes[i].lnode);
        cur = cur->next;
    }
    cr_assert_eq(cur, &stack.list.head);

    // Backward
    cur = stack.list.head.prev;
    for (int i = 0; i < N; i++) {
        cr_assert_eq(cur, &nodes[i].lnode);
        cur = cur->prev;
    }
    cr_assert_eq(cur, &stack.list.head);

    // Drain
    for (int i = N - 1; i >= 0; i--) {
        cr_assert_eq(ds_stack_pop(&stack), &nodes[i]);
    }
    cr_assert(ds_stack_empty(&stack));
}
