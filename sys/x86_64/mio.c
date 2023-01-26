/*
 * QuantumOS Copyright (c) 2022-2023
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <sys/mio.h>

void mio_out8(void *__ptr, unsigned char __data)
{
    *(volatile unsigned char *) (__ptr) = __data;
}

void mio_out16(void *__ptr, unsigned short __data)
{
    *(volatile unsigned short *) (__ptr) = __data;
}

void mio_out32(void *__ptr, unsigned int __data)
{
    *(volatile unsigned int *) (__ptr) = __data;
}

unsigned char mio_in8(void *__ptr)
{
    return *(volatile unsigned char *) (__ptr);
}

unsigned short mio_in16(void *__ptr)
{
    return *(volatile unsigned short *) (__ptr);
}

unsigned int mio_in32(void *__ptr)
{
    return *(volatile unsigned int *) (__ptr);
}