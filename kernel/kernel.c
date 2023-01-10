#include <quantum/kernel.h>

#include <drivers/vesa.h>
#include <drivers/mouse.h>

#include <multiboot.h>
#include <print.h>
#include <init.h>

void quantum_kernel_init(unsigned long magic, unsigned long addr) {
{
    quantum_vesa_init(addr);
    quantum_pmm_init(addr);
    quantum_print_init();
    quantum_gdt_init();
    quantum_isr_init();
    quantum_memory_init();
    quantum_keyboard_init();

    printf("QuantumOS has boot up!\n");

    for (;;) {
        vesa_draw_circle(get_mouse_x(), get_mouse_y(), 4, 255, 0, 0);
    }

    return;
}
