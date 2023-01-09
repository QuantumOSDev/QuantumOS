#include "../include/kernel.h"

#include "../drivers/vga/vga.h"

void __quantum_kernel_init(void)
{
    __vga_clear();
    __vga_putchar('h', 0x07);

    return;
}
