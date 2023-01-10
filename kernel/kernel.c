#include <quantum/kernel.h>

#include <drivers/vga.h>

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

void quantum_kernel_init(void)
{
    quantum_vga_init();
    quantum_gdt_init();
    quantum_isr_init();
    quantum_memory_init();

    return;
}
