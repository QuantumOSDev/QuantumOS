#include <quantum/kernel.h>

#include <multiboot.h>
#include <print.h>

#include <drivers/vesa.h>
#include <drivers/keyboard.h>

#include <core/stdlib.h>

#include <sys/idt.h>
#include <sys/isr.h>
#include <sys/kgdt.h>
#include <sys/pmm.h>
#include <sys/memory.h>

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

void quantum_kernel_init(unsigned long mbinfo_ptr) {
    quantum_vesa_init(mbinfo_ptr);
    quantum_print_init();
    quantum_gdt_init();
    quantum_isr_init();
    quantum_memory_init();
    quantum_keyboard_init();

    printf("Hello, World\nformating\n\tworks\n");
    printf("Num: %d, Str: %s, Hex: 0x%x, Octal: 0o%o", 255, "Hello!", 255, 255);

    char *__result = keyboard_getchar_until('\n');

    for (int i = 0; i < kstrlen(__result); i++)
    {
        putc(__result[i]);
    }

    return;
}
