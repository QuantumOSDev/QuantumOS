/*
 * QuantumOS Copyright (c) 2022-2023
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <sys/pio.h>

void pio_outb(unsigned short __port, unsigned char __data)
{
    asm volatile ("outb %1, %0" :: "dN"(__port), "a"(__data));   
}

void pio_outs(unsigned short __port, unsigned short __data)
{
    asm volatile ("outw %1, %0" :: "dN" (__port), "a" (__data));
}

void pio_outl(unsigned short __port, unsigned int __data)
{
    asm volatile ("outl %%eax, %%dx" :: "dN" (__port), "a" (__data));
}

unsigned char pio_inb(unsigned short __port)
{
    unsigned char __result;

    asm volatile ("inb %1, %0" : "=a" (__result) : "Nd"(__port));

    return __result;
}

unsigned short pio_ins(unsigned short __port)
{
    unsigned short __result;

    asm volatile ("inw %1, %0" : "=a" (__result) : "dN" (__port));

    return __result;
}

unsigned int pio_inl(unsigned short __port)
{
    unsigned int __result;

    asm volatile ("inl %%dx, %%eax" : "=a" (__result) : "dN" (__port));

    return __result;
}

void pio_sleep(void)
{
    pio_outb(0x80, 0);
}

unsigned short pio_mminw(unsigned int* addr) 
{
    unsigned short ret;
    asm volatile (
        "movw (%1), %0"
        : "=r" (ret)
        : "r"  (addr)
        : "memory"
    );
    return ret;
}

void pio_qinl(unsigned short __register, unsigned int *__buffer, int __quads)
{
    for (int i = 0; i < __quads; i++)
    {
        __buffer[i] = pio_inl(__register);
    }
}

void pio_qoutl(unsigned short __register, unsigned int *__buffer, int __quads)
{
    for (int i = 0; i < __quads; i++)
    {
        pio_outl(__register, __buffer[i]);
    }
}