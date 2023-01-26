/*
 * QuantumOS Copyright (c) 2021-2022
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <core/print.h>

void kpanic(const char *__message)
{
    printf("KERNEL PANIC: [%s]\nHALTING THE SYSTEM!", __message);

    for (;;)
        asm volatile ("hlt");
}
