#include <init.h>
#include <print.h>
#include <multiboot.h>

#include <quantum/kernel.h>

#include <drivers/keyboard.h>

#include <sys/idt.h>
#include <sys/isr.h>
#include <sys/kgdt.h>
#include <sys/pmm.h>
#include <sys/memory.h>

#include <stdarg.h>

KERNEL_MEMORY_MAP __kernel_memory_map;

static inline int quantum_get_kernel_mmap(KERNEL_MEMORY_MAP *__map, multiboot_info_t *__mboot)
{
    unsigned int i;

    if (__map == NULL) return -1;
    
    __map->__kernel.__kernel_start = (unsigned int)&__kernel_section_start;
    __map->__kernel.__kernel_end = (unsigned int)&__kernel_section_end;
    __map->__kernel.__kernel_len = ((unsigned int)&__kernel_section_end - (unsigned int)&__kernel_section_start);

    __map->__kernel.__text_start = (unsigned int)&__kernel_text_section_start;
    __map->__kernel.__text_end = (unsigned int)&__kernel_text_section_end;
    __map->__kernel.__text_len = ((unsigned int)&__kernel_text_section_end - (unsigned int)&__kernel_text_section_start);

    __map->__kernel.__data_start = (unsigned int)&__kernel_data_section_start;
    __map->__kernel.__data_end = (unsigned int)&__kernel_data_section_end;
    __map->__kernel.__data_len = ((unsigned int)&__kernel_data_section_end - (unsigned int)&__kernel_data_section_start);

    __map->__kernel.__rodata_start = (unsigned int)&__kernel_rodata_section_start;
    __map->__kernel.__rodata_end = (unsigned int)&__kernel_rodata_section_end;
    __map->__kernel.__rodata_len = ((unsigned int)&__kernel_rodata_section_end - (unsigned int)&__kernel_rodata_section_start);

    __map->__kernel.__bss_start = (unsigned int)&__kernel_bss_section_start;
    __map->__kernel.__bss_end = (unsigned int)&__kernel_bss_section_end;
    __map->__kernel.__bss_len = ((unsigned int)&__kernel_bss_section_end - (unsigned int)&__kernel_bss_section_start);

    __map->__system.__total_memory = __mboot->mem_lower + __mboot->mem_upper;

    for (i = 0; i < __mboot->mmap_length; i += sizeof(multiboot_memory_map_t))
    {
        multiboot_memory_map_t *__mmap = (multiboot_memory_map_t *) (__mboot->mmap_addr + i);

        if (__mmap->type != MULTIBOOT_MEMORY_AVAILABLE)
        {
            continue;
        }

        if (__mmap->addr == __map->__kernel.__kernel_end)
        {
            __map->__available.__start = __map->__kernel.__kernel_end + 1024 * 1024;
            __map->__available.__end   = __mmap->addr + __mmap->len;

            __map->__available.__size = __map->__available.__end - __map->__available.__start;

            return 0;
        }
    }

    return -1;
}

void quantum_info(char* header, char* format, ...) 
{
#if defined(DEBUG)
    printf("[");
    print_set_color(0, 255, 0);
    printf("%s", header);
    print_set_color(255, 255, 255);
    printf("] ");

    va_list arg;
    va_start(arg, format);
    printf_va(format, arg);
    va_end(arg);

    insert_newline();
#endif
}

void quantum_gdt_init(void) 
{
    kgdt_enable();
}

void quantum_isr_init(void)  
{
    isr_enable();
}

void quantum_memory_init(void)
{
    void *__start = pmm_allocate_blocks(20);
    void *__end   = __start + (pmm_next_free(1) * PMM_BLOCK_SIZE);

    kmem_initialize(__start, __end);
}

void quantum_keyboard_init(void)
{
    keyboard_enable();
}

void quantum_pmm_init(unsigned long __addr)
{
    multiboot_info_t *__mboot = (multiboot_info_t *) __addr;

    if (quantum_get_kernel_mmap(&__kernel_memory_map, __mboot) < 0)
    {
        /* Error */
        return;
    }

    pmm_initialize(__kernel_memory_map.__available.__start, __kernel_memory_map.__available.__size);
    pmm_initialize_region(__kernel_memory_map.__available.__start, PMM_BLOCK_SIZE * 256);
}
