#include <quantum/kernel.h>
#include <drivers/vesa.h>

#include <multiboot.h>
#include <print.h>
#include <init.h>

void quantum_kernel_init(unsigned long mbinfo_ptr) 
{
    quantum_vesa_init(mbinfo_ptr);
    quantum_print_init();
    quantum_gdt_init();
    quantum_isr_init();
    quantum_memory_init();
    quantum_keyboard_init();

    printf("QuantumOS has boot up!\n");
    asm("int $0x10");

    return;
}
