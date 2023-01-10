#include <sys/memory.h>

void kmem_initialize(void *__start, void *__end)
{
    if (__start > __end)
    {
        return;
    }

    __kmem_global_start = __start;
    __kmem_global_end = __end;

    __kmem_global_total_size = (__start - __end);
    __kmem_global_used_size = 0;
}

void *kbrk(int __size)
{
    void *__address = (void *)0;

    if (__size <= 0)
    {
        return (void *)0;
    }

    if ((int)(__kmem_global_total_size - __kmem_global_used_size) <= __size)
    {
        return (void *)0;
    }

    __address = (__kmem_global_start + __kmem_global_used_size + __size + sizeof(void *));

    __kmem_global_used_size = (__kmem_global_used_size + __size + sizeof(void *));

    return __address;
}

BOOL kmem_is_free(MEMORY_BLOCK *__block)
{
    if (!__block)
    {
        return FALSE;
    }

    return (__block->__metainfo.__is_free == TRUE);
}

MEMORY_BLOCK *kmem_get_worst_fit(int __size)
{
    /* Loop through the linked list of memory blocks */

    MEMORY_BLOCK *__block = __kmem_global_head;

    while (__block != (void *)0)
    {
        if (kmem_is_free(__block))
        {
            if ((int)__block->__metainfo.__size >= __size)
            {
                return __block;
            }
        }

        __block = __block->__next;
    }

    /* No blocks were found */

    return (void *)0;
}

MEMORY_BLOCK *kmem_new_block(int __size)
{
    /* Loop through the linked list of memory blocks */

    MEMORY_BLOCK *__block = __kmem_global_head;

    while (__block->__next != (void *)0)
    {
        __block = __block->__next;
    }

    /* Allocate a new block */

    MEMORY_BLOCK *__new_block = (MEMORY_BLOCK *)kbrk(sizeof(MEMORY_BLOCK));

    /* Set the data of the block */

    __new_block->__metainfo.__is_free = FALSE;
    __new_block->__metainfo.__size = __size;

    __new_block->__address = kbrk(__new_block->__metainfo.__size);
    __new_block->__next = (void *)0;

    /* Link the new block to the global linked list of blocks */

    __block->__next = __new_block;

    return __new_block;
}

void *kmalloc(int __size)
{
    if (__size <= 0)
    {
        /* Why do you even bother size is 0... */

        return (void *)0;
    }

    /* If the linked list is uninitialized aka (head == NULL) we set the head to the first block.
       Else just allocate a new block with worst-fit algorithm and push it onto the list.
       If worst-fit algorithm returns NULL we manually allocate a new block and push it onto the list.
    */

    if (__kmem_global_head == (void *)0)
    {
        __kmem_global_head->__metainfo.__is_free = FALSE;
        __kmem_global_head->__metainfo.__size = __size;

        __kmem_global_head->__address = kbrk(__kmem_global_head->__metainfo.__size);
        __kmem_global_head->__next = (void *)0;

        return __kmem_global_head->__address;
    }
    else
    {
        MEMORY_BLOCK *__worst_fit = kmem_get_worst_fit(__size);

        if (__worst_fit == (void *)0)
        {
            MEMORY_BLOCK *__block = kmem_new_block(__size);

            __block->__metainfo.__is_free = FALSE;
            __block->__metainfo.__size = __size;

            __block->__address = kbrk(__size);

            return __block->__address;
        }
        else
        {
            __worst_fit->__metainfo.__is_free = FALSE;

            return __worst_fit->__address;
        }
    }

    /* Failed to mallocate given size */

    return (void *)0;
}

void *kcalloc(int __n, int __size)
{
    /* Basicly the same as malloc but we set all the bytes in the given memory map from (start to end) in the given size to 0 */

    if (__n <= 0 || __size <= 0)
    {
        /* Why do you even bother... */

        return (void *)0;
    }

    /* Get the address */

    void *__pointer = kmalloc(__size * __n);

    /* Zero out the memory */

    kmemset(__pointer, 0, (__size * __n));

    /* Return the pointer */

    return __pointer;
}

void *krealloc(void *__address, int __nsize)
{
    /* We change the size of the block in memory */

    MEMORY_BLOCK *__block = __kmem_global_head;

    while (__block != (void *)0)
    {
        if (__block->__address == __address)
        {
            MEMORY_BLOCK *__resized_block = kmem_new_block(__nsize);

            if ((int)__block->__metainfo.__size > __nsize)
            {
                kmemcpy(__resized_block->__address, __address, __nsize);
            }
            else
            {
                kmemcpy(__resized_block->__address, __address, __block->__metainfo.__size);
            }

            __block->__metainfo.__is_free = TRUE;

            return __resized_block->__address;
        }

        __block = __block->__next;
    }

    /* Failed to resize memory block */

    return (void *)0;
}

void *kmemcpy(void *__dest, const void *__src, unsigned int __n)
{
    char *__pointer = __dest;
    char *__result  = __dest;

    const char *__pointer_semi = __src;

    while (__n--)
    {
        *__pointer++ = *__pointer_semi++;
    }

    return __result;
}

void *kmemset(void *__dest, char __c, unsigned int __n)
{
    char *__tmp = __dest;

    for (; __n != 0; __n--)
    {
        *__tmp++ = __c;
    }

    return __dest;
}

void kfree(void *__address)
{
    MEMORY_BLOCK *__block = __kmem_global_head;

    while (__block != (void *)0)
    {
        if (__block->__address == __address)
        {
            __block->__metainfo.__is_free = TRUE;

            return;
        }

        __block = __block->__next;
    }
}