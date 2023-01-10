#ifndef MEMORY_H
#define MEMORY_H

typedef enum
{
    FALSE,
    TRUE
} BOOL;

typedef struct __kmem_memory_block
{
    struct
    {
        unsigned int __size;
        unsigned char __is_free;
    } __metainfo;

    struct __kmem_memory_block *__next;

    void *__address;
} __attribute__((packed)) MEMORY_BLOCK;

static void *__kmem_global_start = (void *) 0;
static void *__kmem_global_end   = (void *) 0;

static unsigned long __kmem_global_total_size = 0;
static unsigned long __kmem_global_used_size  = 0;

static MEMORY_BLOCK *__kmem_global_head = (void *) 0;

BOOL kmem_is_free(MEMORY_BLOCK *__block);

MEMORY_BLOCK *kmem_get_worst_fit(int __size);
MEMORY_BLOCK *kmem_new_block(int __size);

void kmem_initialize(void *__start, void *__end);

void *kbrk(int __size);

void *kmalloc(int __size);
void *kcalloc(int __n, int __size);
void *krealloc(void *__address, int __nsize);

void *kmemcpy(void *__dest, const void *__src, unsigned int __n);
void *kmemset(void *__dest, char __c, unsigned int __n);

void kfree(void *__address);

#endif