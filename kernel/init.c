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
