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

/// CORE ///

// Push
Test(stack, push) {
    ds_stack_node a, b;

    ds_stack_push(&stack, &a);
    ds_stack_push(&stack, &b);

    // head <-> b <-> a <-> head
    cr_assert_eq(stack.list.head.next, &b);
    cr_assert_eq(b.lnode.next, &a);
    cr_assert_eq(a.lnode.next, &stack.list.head);

    cr_assert_eq(stack.list.head.prev, &a);
    cr_assert_eq(a.lnode.prev, &b);
    cr_assert_eq(b.lnode.prev, &stack.list.head);
}

// Pop
Test(stack, pop) {
    ds_stack_node a, b;

    ds_stack_push(&stack, &a);
    ds_stack_push(&stack, &b);

    cr_assert_eq(ds_stack_pop(&stack), &b);

    // head <-> a <-> head
    cr_assert_eq(stack.list.head.next, &a);
    cr_assert_eq(a.lnode.next, &stack.list.head);

    cr_assert_eq(stack.list.head.prev, &a);
    cr_assert_eq(a.lnode.prev, &stack.list.head);
}
