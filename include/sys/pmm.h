#ifndef PMM_H
#define PMM_H

#define PMM_BIT_SIZE 32
#define PMM_BLOCK_SIZE 4096

/* TODO:
* Move this into a new header
*/

typedef unsigned int __pmm_physical_address;

typedef struct __pmm_mem_info
{
    unsigned int __memsize;
    unsigned int __maxblocks;
    unsigned int __usedblocks;
    unsigned int __mapend;

    unsigned int *__map_array;
} __pmm_mem_info_t;

static __pmm_mem_info_t __pmm_mem_info;

void pmm_mmap_set(int __address);
void pmm_mmap_unset(int __address);
char pmm_mmap_get(int __address);

int pmm_next_free(int __size);

void pmm_initialize(__pmm_physical_address __bitmap, unsigned int __memsize);
void pmm_initialize_region(__pmm_physical_address __base, unsigned int __region_size);
void pmm_uninitialize_region(__pmm_physical_address __base, unsigned int __region_addr);

void *pmm_allocate_block(void);
void *pmm_allocate_blocks(unsigned int __size);

void pmm_free_block(void *__address);
void pmm_free_blocks(void *__address, unsigned int __size);

#endif