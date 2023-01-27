/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <core/kpanic.h>
#include <core/string.h>
#include <core/print.h>

#include <sys/ksyms.h>

#include <drivers/debug.h>

void kpanic(const char *__message)
{
    debug_printf("KERNEL PANIC: [%s]\nHALTING THE SYSTEM!", __message);
    print_set_color(220, 0, 0);
    printf("KERNEL PANIC: [%s]\nHALTING THE SYSTEM!\n", __message);

    stack_frame_t* call_stack;
    asm volatile ("mov %%ebp, %0" : "=r"(call_stack));

    printf("Call stack:\n");
    for (int frame = 0; call_stack && (frame < 10); frame++)
    {
        elf32_sym_t* sym = get_ksym_by_address(call_stack->eip);
        if (sym != NULL)
            printf("- 0x%x [%s] \n", call_stack->eip, (char*)get_shdr_string(sym->name));
        else
            printf("- 0x%x [unknown_function] \n", call_stack->eip);
        call_stack = (stack_frame_t*)call_stack->ebp;
    }

    for (;;)
        asm volatile ("hlt");
}
