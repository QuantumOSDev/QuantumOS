/*
 * QuantumOS Copyright (c) 2021-2022
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <core/print.h>

#include <sys/isr.h>
#include <sys/pio.h>
#include <sys/pic.h>

ISR __interrupt_handlers[INTERRUPT_HANDLERS_COUNT];

char *exception_messages[32] = {
   "Division By Zero",
   "Debug",
   "Non Maskable Interrupt",
   "Breakpoint",
   "Overflow",
   "BOUND Range Exceeded",
   "Invalid Opcode",
   "Device Not Available (No Math Coprocessor)",
   "Double Fault",
   "Coprocessor Segment Overrun",
   "Invalid TSS",
   "Segment Not Present",
   "Stack-Segment Fault",
   "General Protection",
   "Page Fault",
   "Unknown Interrupt (intel reserved)",
   "x87 FPU Floating-Point Error (Math Fault)",
   "Alignment Check",
   "Machine Check",
   "SIMD Floating-Point Exception",
   "Virtualization Exception",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved"
};

void isr_register_interrupt_handler(int __index, ISR __handler)
{
    if (__index < INTERRUPT_HANDLERS_COUNT)
    {
        __interrupt_handlers[__index] = __handler;
    }
}

void isr_end_interrupt(int __index)
{
    pic_eoi((unsigned char) __index);
}

void isr_irq_handler(__registers_t *__regs)
{
    if (__interrupt_handlers[__regs->int_no] != NULL)
    {
        ISR __handler = __interrupt_handlers[__regs->int_no];

        __handler(__regs);
    }

    pic_eoi((unsigned char) __regs->int_no);
}

void isr_exception_handler(__registers_t __regs)
{
    if (__regs.int_no < 32)
    {
        printf("ISR ERROR 0x%x: %s\n", __regs.int_no, exception_messages[__regs.int_no]);

        asm volatile ("hlt");
    }

    if (__interrupt_handlers[__regs.int_no] != NULL)
    {
        ISR __handler = __interrupt_handlers[__regs.int_no];

        __handler(&__regs);
    }
}