#ifndef DS_STACK_H
#define DS_STACK_H

#include <ds/dlist.h>

typedef struct ds_stack_node {
    ds_dlist_node lnode;
} ds_stack_node;

typedef struct ds_stack {
    ds_dlist list;
} ds_stack;

/// BASE ///

void ds_stack_init(ds_stack *stack);
bool ds_stack_empty(ds_stack *stack);

/// CORE ///

// Push
void ds_stack_push(ds_stack *stack, ds_stack_node *node);

// Pop
ds_stack_node *ds_stack_pop(ds_stack *stack);

#endif
