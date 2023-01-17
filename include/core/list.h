#ifndef LIST_H
#define LIST_H

#include <sys/memory.h>

typedef struct __listnode
{
    struct __listnode *__prev;
    struct __listnode *__next;

    void *__val;
} __listnode_t;

typedef struct __list
{
    __listnode_t *__head;
    __listnode_t *__tail;

    unsigned int __size;
} __list_t;

#define FOREACH(__T, __LIST) for (__listnode_t *__T = __LIST->__head; __T != NULL; __T = __T->__next)

__list_t *list_create(void);

unsigned int list_size(__list_t *__list);

__listnode_t *list_insert_front(__list_t *__list, void *__val);

void list_insert_back(__list_t *__list, void *__val);

void *list_remove_node(__list_t *__list, __listnode_t *__node);
void *list_remove_front(__list_t *__list);
void *list_remove_back(__list_t *__list);

void list_push(__list_t *__list, void *__val);

__listnode_t *list_pop(__list_t *__list);

void list_enqueue(__list_t *__list, void *__val);

__listnode_t *list_dequeue(__list_t *__list);

void *list_peek_front(__list_t *__list);
void *list_peek_back(__list_t *__list);

void list_destroy(__list_t *__list);
void listnode_destroy(__listnode_t *__node);

int list_contains(__list_t *__list, void *__val);

__listnode_t *list_get_node_by_index(__list_t *__list, int __index);

void *list_remove_by_index(__list_t *__list, int __index);

#endif
