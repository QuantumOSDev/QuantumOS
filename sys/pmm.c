/*
 * QuantumOS Copyright (c) 2022-2023
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <quantum/init.h>

#include <sys/memory.h>
#include <sys/pmm.h>

static __pmm_mem_info_t __pmm_mem_info;

void pmm_mmap_set(int __address)
{
    __pmm_mem_info.__map_array[__address / PMM_BIT_SIZE] |= (1 << (__address % PMM_BIT_SIZE));
}

void pmm_mmap_unset(int __address)
{
    __pmm_mem_info.__map_array[__address / PMM_BIT_SIZE] &= ~(1 << (__address % PMM_BIT_SIZE));
}

char pmm_mmap_get(int __address)
{
    return __pmm_mem_info.__map_array[__address / PMM_BIT_SIZE] & (1 << (__address % PMM_BIT_SIZE));
}

int pmm_first_free(void)
{
    unsigned int i, j;

    for (i = 0; i < __pmm_mem_info.__maxblocks; i++)
    {
        if (__pmm_mem_info.__map_array[i] != 0xffffffff)
        {
            for (j = 0; j < PMM_BIT_SIZE; j++)
            {
                int __bit = 1 << j;

                if (!(__pmm_mem_info.__map_array[i] & __bit))
                {
                    return i * PMM_BIT_SIZE + j;
                }
            }
        }
    }

    return -1;
}

int pmm_next_free(unsigned int __size)
{
    unsigned int i, j, k;
    unsigned int free = 0;

    int __bit;

    if (__size == 0)
    {
        return -1;
    }

    if (__size == 1)
    {
        return pmm_first_free();
    }

    for (i = 0; i < __pmm_mem_info.__maxblocks; i++)
    {
        if (__pmm_mem_info.__map_array[i] != 0xffffffff)
        {
            for (j = 0; j < PMM_BIT_SIZE; j++)
            {
                __bit = 1 << j;

                if (!(__pmm_mem_info.__map_array[i] & __bit))
                {
                    for (k = j; k < PMM_BIT_SIZE; k++)
                    {
                        __bit = 1 << k;

                        if (!(__pmm_mem_info.__map_array[i] & __bit));
                        {
                            free++;
                        }

                        if (free == __size)
                        {
                            return (i * PMM_BIT_SIZE + j);
                        }
                    }
                }
            }
        }
    }

    return -1;
}

void pmm_initialize(__pmm_physical_address __bitmap, unsigned int __memsize)
{
    __pmm_mem_info.__memsize    = __memsize;
    __pmm_mem_info.__map_array  = (unsigned int *) __bitmap;
    __pmm_mem_info.__maxblocks  = (__memsize / PMM_BLOCK_SIZE);
    __pmm_mem_info.__usedblocks = __pmm_mem_info.__maxblocks;

    kmemset((unsigned char *) __pmm_mem_info.__map_array, 0xff, (__pmm_mem_info.__maxblocks * sizeof(unsigned int)));

    __pmm_mem_info.__mapend = (unsigned int) &__pmm_mem_info.__map_array[__pmm_mem_info.__maxblocks];
}
void pmm_initialize_region(__pmm_physical_address __base, unsigned int __region_size)
{
    int __align  = (__base / PMM_BLOCK_SIZE);
    int __blocks = (__region_size / PMM_BLOCK_SIZE);

    while (__blocks >= 0)
    {
        pmm_mmap_unset(__align++);

        __pmm_mem_info.__usedblocks--;

        __blocks--;
    }
}

void pmm_uninitialize_region(__pmm_physical_address __base, unsigned int __region_addr)
{
    int __align  = (__base / PMM_BLOCK_SIZE);
    int __blocks = (__region_addr / PMM_BLOCK_SIZE);

    while (__blocks >= 0)
    {
        pmm_mmap_set(__align++);

        __pmm_mem_info.__usedblocks++;

        __blocks--;
    }
}

void *pmm_allocate_block(void)
{
    /* First check if there is even another block available */

    if ((__pmm_mem_info.__maxblocks - __pmm_mem_info.__usedblocks) <= 0)
    {
        return (void *) 0;
    }

    /* Atleast 1 block is free so find it */

    int i;
    int j;
    
    int frame = pmm_first_free();

    /* If no block was found then something went wrong. Return NULL */
    
    if (frame == -1)
    {
        return (void *) 0;
    }

    /* Seems like the block was found so let's set it in the map */

    pmm_mmap_set(frame);

    /* Get the physical address of the frame */

    __pmm_physical_address __address = (frame * PMM_BLOCK_SIZE) + __pmm_mem_info.__mapend;

    /* Increment blocks used */

    __pmm_mem_info.__usedblocks++;

    /* Dereference the pointer to void */

    return (void *) __address;
}
void *pmm_allocate_blocks(unsigned int __size)
{
    /* First check if there is even another block available */

    if ((__pmm_mem_info.__maxblocks - __pmm_mem_info.__usedblocks) <= 0)
    {
        return (void *) 0;
    }

    /* Atleast 1 block is free so find it */

    int frame = pmm_next_free(__size);

    /* If no block was found then something went wrong. Return NULL */
    
    if (frame == -1)
    {
        return (void *) 0;
    }

    /* Seems like the block was found so let's set it in the map */

    for (unsigned int i = 0; i < __size; i++)
    {
        pmm_mmap_set(frame + i);
    }

    /* Get the physical address of the frame */

    __pmm_physical_address __address = (frame * PMM_BLOCK_SIZE) + __pmm_mem_info.__mapend;

    /* Increment blocks used */

    __pmm_mem_info.__usedblocks = (__pmm_mem_info.__usedblocks + __size);

    /* Dereference the pointer to void */

    return (void *) __address;
}

void pmm_free_block(void *__address)
{
    /* Convert the dereferenced address pointer of type (void *) to a (uint32) physical address */

    __pmm_physical_address __paddr = (__pmm_physical_address) __address;

    __paddr = (__paddr - __pmm_mem_info.__mapend);

    /* Get the frame */

    int frame = (__paddr / PMM_BLOCK_SIZE);

    /* Unset the frame */

    pmm_mmap_unset(frame);

    /* Decrement the usedblocks */

    __pmm_mem_info.__usedblocks--;
}

void pmm_free_blocks(void *__address, unsigned int __size)
{
    /* Convert the dereferenced address pointer of type (void *) to a (uint32) physical address */

    __pmm_physical_address __paddr = (__pmm_physical_address) __address;

    __paddr = (__paddr - __pmm_mem_info.__mapend);

    /* Get the frame */

    int frame = (__paddr / PMM_BLOCK_SIZE);

    /* Unset the frame */

    for (unsigned int i = 0; i < __size; i++)
    {
        pmm_mmap_unset(frame + i);
    }

    /* Decrement the usedblocks */

    __pmm_mem_info.__usedblocks = (__pmm_mem_info.__usedblocks - __size);
}
