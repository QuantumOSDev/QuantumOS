#include "pio.h"

void __pio_outb(unsigned short __port, unsigned char __data)
{
    asm volatile ("outb %1, %0" :: "dN"(__port), "a"(__data));   
}

void __pio_outs(unsigned short __port, unsigned short __data)
{
    asm volatile ("outw %1, %0" :: "dN" (__port), "a" (__data));
}

void __pio_outl(unsigned short __port, unsigned int __data)
{
    asm volatile ("outl %%eax, %%dx" :: "dN" (__port), "a" (__data));
}

unsigned char __pio_inb(unsigned short __port)
{
    unsigned char __result;

    asm volatile ("inb %1, %0" : "=a" (__result) : "Nd"(__port));

    return __result;
}

unsigned short __pio_ins(unsigned short __port)
{
    unsigned short __result;

    asm volatile ("inw %1, %0" : "=a" (__result) : "dN" (__port));

    return __result;
}

unsigned int __pio_inl(unsigned short __port)
{
    unsigned int __result;

    asm volatile ("inl %%dx, %%eax" : "=a" (__result) : "dN" (__port));

    return __result;
}

void __pio_sleep(void)
{
    __pio_outb(0x80, 0);
}
