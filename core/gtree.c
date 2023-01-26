/*
 * QuantumOS Copyright (c) 2022-2023
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <sys/memory.h>

#include <core/string.h>
#include <core/gtree.h>
#include <core/list.h>

__gtree_t *gtree_create(void)
{
    return (__gtree_t *) kcalloc(1, sizeof(__gtree_t));
}

__gtreenode_t *gtree_node_create(void *__value)
{
    __gtreenode_t *__node = kcalloc(1, sizeof(__gtreenode_t));

    __node->__children = list_create();
    __node->__val      = __value;

    return __node;
}

__gtreenode_t *gtree_insert(__gtree_t *__tree, __gtreenode_t *__subroot, void *__value)
{
    __gtreenode_t *__treenode = kcalloc(1, sizeof(__gtreenode_t));

    __treenode->__children = list_create();
    __treenode->__val      = __value;

    if (!__tree->__root)
    {
        __tree->__root = __treenode;

        return __treenode;
    }

    list_insert_front(__subroot->__children, __treenode);

    return __treenode;
}

__gtreenode_t *gtree_find_parent(__gtree_t *__tree, __gtreenode_t *__node, int *__cindex)
{
    if (__node == __tree->__root)
    {
        return NULL;
    }

    return gtree_find_parent_recur(__tree, __node, __tree->__root, __cindex);
}

__gtreenode_t *gtree_find_parent_recur(__gtree_t *__tree, __gtreenode_t *__node, __gtreenode_t *__subroot, int *__cindex)
{
    int i = 0;

    if ((i == list_contains(__subroot->__children, __node)) != -1)
    {
        *__cindex = i;

        return __subroot;
    }

    FOREACH(__child, __subroot->__children)
    {
        __gtreenode_t *__ret = gtree_find_parent_recur(__tree, __node, __child->__val, __cindex);

        if (__ret != NULL)
        {
            return __ret;
        }
    }

    return NULL;
}

void gtree_remove(__gtree_t *__tree, __gtreenode_t *__node)
{
    int __cindex = -1;

    __gtreenode_t *__parent = gtree_find_parent(__tree, __node, &__cindex);

    if (__parent != NULL)
    {
        __gtreenode_t *__target = list_remove_by_index(__parent->__children, __cindex);

        kfree(__target);
    }
}

void gtree_to_list_recur(__gtreenode_t *__subroot, __list_t *__list)
{
    if (__subroot == NULL)
    {
        return;
    }

    FOREACH(__child, __subroot->__children)
    {
        __gtreenode_t *__curr = (__gtreenode_t *) __child->__val;

        void *__val = __curr->__val;

        list_insert_back(__list, __val);

        gtree_to_list_recur(__child->__val, __list);
    }
}

void gtree_to_list(__gtree_t *__tree, __list_t *__list)
{
    gtree_to_list_recur(__tree->__root, __list);
}

void gtree_to_array_recur(__gtreenode_t *__subroot, void **__array, int *__size)
{
    if (__subroot == NULL)
    {
        return;
    }

    void *__val = (void *) __subroot->__val;

    __array[*__size] = __val;

    *__size = (*__size + 1);

    FOREACH(__child, __subroot->__children)
    {
        gtree_to_array_recur(__child->__val, __array, __size);
    }
}

void gtree_to_array(__gtree_t *__tree, void **__array, int *__size)
{
    gtree_to_array_recur(__tree->__root, __array, __size);
}
