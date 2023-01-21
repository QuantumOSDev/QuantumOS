/*
 * QuantumOS Copyright (c) 2021-2022
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <sys/pic.h>
#include <sys/pio.h>

#include <quantum/init.h>

void pic_init(void)
{
    unsigned char __a1, __a2;

    __a1 = pio_inb(PIC1_DATA);
    __a2 = pio_inb(PIC2_DATA);

    pio_outb(PIC1_COMMAND, ICW1);
    pio_outb(PIC2_COMMAND, ICW1);

    pio_outb(PIC1_DATA, 0x20);
    pio_outb(PIC2_DATA, 0x28);

    pio_outb(PIC1_DATA, 4);
    pio_outb(PIC2_DATA, 2);

    pio_outb(PIC1_DATA, ICW4_8086);
    pio_outb(PIC2_DATA, ICW4_8086);

    pio_outb(PIC1_DATA, __a1);
    pio_outb(PIC2_DATA, __a2);

    quantum_info(0, " PIC    ", "Successfully initialized PIC");
}

void pic_eoi(unsigned char __irq)
{
    if (__irq >= 0x28)
    {
        pio_outb(PIC2, PIC_EOI);
    }

    pio_outb(PIC1, PIC_EOI);
}