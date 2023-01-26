/*
 * QuantumOS Copyright (c) 2021-2022
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <core/print.h>

#include <drivers/debug.h>

void kpanic(const char *__message)
{
    debug_printf("KERNEL PANIC: [%s]\nHALTING THE SYSTEM!", __message);
    printf("KERNEL PANIC: [%s]\nHALTING THE SYSTEM!", __message);

    for (;;)
        asm volatile ("hlt");
}
