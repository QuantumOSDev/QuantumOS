/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <quantum/multiboot.h>
#include <quantum/kernel.h>
#include <quantum/kmode.h>
#include <quantum/init.h>

#include <userspace/userspace.h>

#include <core/stdarg.h>
#include <core/string.h>
#include <core/print.h>

#include <drivers/keyboard.h>
#include <drivers/debug.h>
#include <drivers/pci.h>

#include <sys/memory.h>
#include <sys/kgdt.h>
#include <sys/idt.h>
#include <sys/isr.h>
#include <sys/pmm.h>

#include <fs/device.h>
#include <fs/vfs.h>
#include <fs/ata.h>

int quantum_get_kernel_mmap(KERNEL_MEMORY_MAP *__map, multiboot_info_t *__mboot)
{
    unsigned int i;

    if (__map == NULL)
    {
        return -1;
    }

    quantum_info(__FILE__, 0, " MMAP   ", "Trying to get kernel memory map");

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

    for (i = 0; i < __mboot->mmap_length; i += sizeof(__multiboot_memory_map))
    {
        __multiboot_memory_map *__mmap = (__multiboot_memory_map *)(__mboot->mmap_addr + i);

        if (__mmap->__type != MULTIBOOT_MEMORY_AVAILABLE)
        {
            continue;
        }

        if (__mmap->__addr_low == __map->__kernel.__text_start)
        {
            __map->__available.__start = __map->__kernel.__kernel_end + 1024 * 1024;
            __map->__available.__end = __mmap->__addr_low + __mmap->__len_low;

            __map->__available.__size = __map->__available.__end - __map->__available.__start;

            return 0;
        }
    }

    return -1;
}

// #define DEBUG

void quantum_info(char* file, int __status, char* header, char* format, ...)
{
#if defined(DEBUG)
    printf("[");
    if (__status == 0)
    {
        print_set_color(0, 255, 0);
    }
    else if (__status == 2)
    {
        print_set_color(255, 140, 0);
    }
    else if (__status == 1)
    {
        print_set_color(255, 0, 0);
    }
    printf("%s", header);
    print_set_color(255, 255, 255);
    printf("] ");
    print_set_color(50, 168, 164);
    printf("%s", file);
    print_set_color(255, 255, 255);
    printf(": ");

    va_list arg;
    va_start(arg, format);
    printf_va(format, arg);
    va_end(arg);

    insert_newline();
#else
    debug_printf("[");
    if (__status == 0)
    {
        debug_printf("\x1b[1;32m");
    }
    else if (__status == 2)
    {
        debug_printf("\x1b[1;33m");
    }
    else if (__status == 1)
    {
        debug_printf("\x1b[1;31m");
    }
    debug_printf("%s\x1b[0;0m", header);
    debug_printf("] ");
    debug_printf("\x1b[1;36m%s", file);
    debug_printf("\x1b[0;0m");
    debug_printf(": ");

    va_list arg;
    va_start(arg, format);
    debug_printf_va(format, arg);
    va_end(arg);

    debug_printf("\n");
#endif
}

void quantum_gdt_init(void)
{
    gdt_enable();
}

void quantum_idt_init(void)
{
    idt_enable();
}

void quantum_memory_init(void)
{
    void *__start = pmm_allocate_blocks(20);
    void *__end = __start + (pmm_next_free(1) * PMM_BLOCK_SIZE);

    quantum_info(__FILE__, 0, " PMM    ", "Successfully allocated 20 blocks! START: 0x%x END: 0x%x", __start, __end);

    kmem_initialize(__start, __end);
}

void quantum_keyboard_init(void)
{
    keyboard_enable();
}

void display_kernel_memory_map(KERNEL_MEMORY_MAP *kmap)
{
    printf("kernel:\n");
    printf("  kernel-start: 0x%x, kernel-end: 0x%x, TOTAL: %d bytes\n",
           kmap->__kernel.__kernel_start, kmap->__kernel.__kernel_end, kmap->__kernel.__kernel_len);
    printf("  text-start: 0x%x, text-end: 0x%x, TOTAL: %d bytes\n",
           kmap->__kernel.__text_start, kmap->__kernel.__text_end, kmap->__kernel.__text_len);
    printf("  data-start: 0x%x, data-end: 0x%x, TOTAL: %d bytes\n",
           kmap->__kernel.__data_start, kmap->__kernel.__data_end, kmap->__kernel.__data_len);
    printf("  rodata-start: 0x%x, rodata-end: 0x%x, TOTAL: %d\n",
           kmap->__kernel.__rodata_start, kmap->__kernel.__rodata_end, kmap->__kernel.__rodata_len);
    printf("  bss-start: 0x%x, bss-end: 0x%x, TOTAL: %d\n",
           kmap->__kernel.__bss_start, kmap->__kernel.__bss_end, kmap->__kernel.__bss_len);

    printf("total_memory: %d KB\n", kmap->__system.__total_memory);
    printf("available:\n");
    printf("  start_addr: 0x%x\n  end addr: 0x%x\n  size: %d\n",
           kmap->__available.__start, kmap->__available.__end, kmap->__available.__size);
}

void quantum_pmm_init(unsigned long __addr)
{
    KERNEL_MEMORY_MAP __kernel_memory_map;

    multiboot_info_t *__mboot = (multiboot_info_t *)__addr;

    int __status = quantum_get_kernel_mmap(&__kernel_memory_map, __mboot);

    if (__status != 0)
    {
        /* Error */

        quantum_info(__FILE__, 1, " PMM    ", "Failed to initialize PMM!");

        return;
    }

    pmm_initialize(__kernel_memory_map.__available.__start, __kernel_memory_map.__available.__size);
    pmm_initialize_region(__kernel_memory_map.__available.__start, PMM_BLOCK_SIZE * 256);

    quantum_info(__FILE__, 0, " PMM    ", "Initialized PMM from address: 0x%x to 0x%x", __kernel_memory_map.__available.__start, PMM_BLOCK_SIZE * 256);
}

void quantum_vfs_init(void)
{
    vfs_init();

    quantum_info(__FILE__, 0, " VFS    ", "Initialized VFS!");
}

void quantum_ata_init(void)
{
    //ata_initialize(0x1F0, 0x3F6, 0x170, 0x376, 0x000);
}

void quantum_devmgr_init(void)
{
    device_init();

    quantum_info(__FILE__, 0, " Devmgr ", "Successfully initialized devmgr");
}

void quantum_pci_init(void)
{
    int __exit = pci_initialize();
}

void quantum_migrate_to_kernel_mode(void)
{
    kmode_initialize();
}

void quantum_migrate_to_userspace(void)
{
    userspace_initialize();
}
