#include <ds/stack.h>
#include <ds/util.h>

#include <stddef.h>

/// BASE ///

void ds_stack_init(ds_stack *stack) {
    ds_dlist_init(&stack->list);
}

bool ds_stack_empty(const ds_stack *stack) {
    return ds_dlist_empty(&stack->list);
}

/// CORE ///

// Push
void ds_stack_push(ds_stack *stack, ds_stack_node *node) {
    ds_dlist_push_front(&stack->list, &node->lnode);
}

// Pop
ds_stack_node *ds_stack_pop(ds_stack *stack) {
    ds_dlist_node *tmp = ds_dlist_pop_front(&stack->list);
    if (!tmp) {
        return NULL;
    }
    return ds_container_of(tmp, ds_stack_node, lnode);
}
