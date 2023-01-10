#include <quantum/kernel.h>

#include <drivers/vga.h>
#include <drivers/keyboard.h>

#include <core/stdlib.h>

#include <sys/idt.h>
#include <sys/isr.h>
#include <sys/kgdt.h>
#include <sys/pmm.h>
#include <sys/memory.h>

static inline void quantum_vga_init(void)
{
    vga_clear();
}

static inline void quantum_gdt_init(void)
{
    kgdt_enable();
}

static inline void quantum_isr_init(void)
{
    isr_enable();
}

static inline void quantum_memory_init(void)
{
    void *__start = pmm_allocate_blocks(20);
    void *__end   = __start + (pmm_next_free(1) * PMM_BLOCK_SIZE);

    kmem_initialize(__start, __end);
}

static inline void quantum_keyboard_init(void)
{
    keyboard_enable();
}

void quantum_kernel_init(void)
{
    quantum_vga_init();
    quantum_gdt_init();
    quantum_isr_init();
    quantum_memory_init();
    quantum_keyboard_init();

    char *__result = keyboard_getchar_until('\n');

    for (int i = 0; i < kstrlen(__result); i++)
    {
        vga_putchar(__result[i], 0x07);
    }

    return;
}
