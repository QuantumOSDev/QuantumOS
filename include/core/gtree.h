#ifndef GTREE_H
#define GTREE_H

#include <core/list.h>

typedef struct __gtreenode
{
    __list_t *__children;

    void *__val;
} __gtreenode_t;

typedef struct __gtree
{
    __gtreenode_t *__root;
} __gtree_t;

__gtree_t *gtree_create(void);

__gtreenode_t *gtree_node_create(void *__value);
__gtreenode_t *gtree_insert(__gtree_t *__tree, __gtreenode_t *__subroot, void *__value);
__gtreenode_t *gtree_find_parent(__gtree_t *__tree, __gtreenode_t *__node, int *__cindex);
__gtreenode_t *gtree_find_parent_recur(__gtree_t *__tree, __gtreenode_t *__node, __gtreenode_t *__subroot, int *__cindex);

void gtree_remove(__gtree_t *__tree, __gtreenode_t *__node);

void gtree_to_list_recur(__gtreenode_t *__subroot, __list_t *__list);
void gtree_to_list(__gtree_t *__tree, __list_t *__list);
void gtree_to_array_recur(__gtreenode_t *__subroot, void **__array, int *__size);
void gtree_to_array(__gtree_t *__tree, void **__array, int *__size);

#endif
