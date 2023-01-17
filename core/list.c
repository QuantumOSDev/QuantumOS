#include <sys/memory.h>

#include <core/string.h>

#include <core/list.h>

__list_t *list_create(void)
{
    __list_t *__list = kcalloc(1, sizeof(__list_t));

    return __list;
}

unsigned int list_size(__list_t *__list)
{
    if (!__list)
    {
        return 0;
    }

    return __list->__size;
}

__listnode_t *list_insert_front(__list_t *__list, void *__val)
{
    __listnode_t *__node = kcalloc(1, sizeof(__listnode_t));

    __list->__head->__prev = __node;

    __node->__next = __list->__head;
    __node->__val  = __val;

    if (!__list->__head)
    {
        __list->__tail = __node;
    }

    __list->__head = __node;
    __list->__size++;

    return __node;
}

void list_insert_back(__list_t *__list, void *__val)
{
    __listnode_t *__node = kcalloc(1, sizeof(__listnode_t));

    __node->__prev = __list->__tail;

    if (__list->__tail)
    {
        __list->__tail->__next = __node;

        __node->__val = __val;

        if (!__list->__head)
        {
            __list->__head = __node;
        }

        __list->__tail = __node;
        __list->__size++;
    }
}

void *list_remove_node(__list_t *__list, __listnode_t *__node)
{
    void *__val = __node->__val;

    if (__list->__head == __node)
    {
        return list_remove_front(__list);
    }
    else if (__list->__tail == __node)
    {
        return list_remove_back(__list);
    }
    else
    {
        __node->__next->__prev = __node->__prev;
        __node->__prev->__next = __node->__next;

        __list->__size--;

        kfree(__node);
    }

    return __val;
}

void *list_remove_front(__list_t *__list)
{
    if (!__list->__head)
    {
        return 0;
    }

    __listnode_t *__node = __list->__head;

    void *__val = __node->__val;

    __list->__head = __node->__next;

    if (__list->__head)
    {
        __list->__head->__prev = NULL;
    }

    kfree(__node);

    __list->__size--;

    return __val;
}

void *list_remove_back(__list_t *__list)
{
    if (!__list->__head)
    {
        return 0;
    }

    __listnode_t *__node = __list->__tail;

    void *__val = __node->__val;

    __list->__tail = __node->__prev;

    if (__list->__tail)
    {
        __list->__tail->__next = NULL;
    }

    kfree(__node);

    __list->__size--;

    return __val;
}

void list_push(__list_t *__list, void *__val)
{
    list_insert_back(__list, __val);
}

__listnode_t *list_pop(__list_t *__list)
{
    if (!__list->__head)
    {
        return NULL;
    }

    __listnode_t *__node = __list->__tail;

    __list->__tail = __node->__prev;

    if (__list->__tail)
    {
        __list->__tail->__next = NULL;
    }

    __list->__size--;

    return __node;
}

void list_enqueue(__list_t *__list, void *__val)
{
    list_insert_front(__list, __val);
}

__listnode_t *list_dequeue(__list_t *__list)
{
    return list_pop(__list);
}

void *list_peek_front(__list_t *__list)
{
    if (!__list->__head)
    {
        return NULL;
    }

    return __list->__head->__val;
}

void *list_peek_back(__list_t *__list)
{
    if (!__list->__tail)
    {
        return NULL;
    }

    return __list->__tail->__val;
}

void list_destroy(__list_t *__list)
{
    __listnode_t *__node = __list->__head;

    while (__node != NULL)
    {
        __listnode_t *__temp = __node;

        __node = __node->__next;

        kfree(__temp);
    }

    kfree(__list);
}

void listnode_destroy(__listnode_t *__node)
{
    kfree(__node);
}

int list_contains(__list_t *__list, void *__val)
{
    int i = 0;

    FOREACH(__listnode, __list)
    {
        if (__listnode->__val == __val)
        {
            return i;
        }

        i++;
    }

    return -1;
}

__listnode_t *list_get_node_by_index(__list_t *__list, int __index)
{
    if (__index < 0 || __index >= list_size(__list))
    {
        return NULL;
    }

    int c = 0;

    FOREACH(__listnode, __list)
    {
        if (__index == c)
        {
            return __listnode;
        }

        c++;
    }

    return NULL;
}

void *list_remove_by_index(__list_t *__list, int __index)
{
    __listnode_t *__node = list_get_node_by_index(__list, __index);

    return list_remove_node(__list, __node);
}
