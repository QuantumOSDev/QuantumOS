#include <quantum/kernel.h>

#include <drivers/vga.h>

#include <sys/idt.h>
#include <sys/isr.h>
#include <sys/kgdt.h>

void quantum_kernel_init(void)
{
    vga_clear();
    
    const char *__str = "Hello, World!\n";

    int i = 0;

    while (__str[i])
    {
        vga_putchar(__str[i], 0x07);

        i++;
    }

    kgdt_enable();
    isr_enable();

    return;
}
