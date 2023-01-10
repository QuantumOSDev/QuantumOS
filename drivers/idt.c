#include <sys/idt.h>

#define low_16(address)  (unsigned short)((address) & 0xFFFF)
#define high_16(address) (unsigned short)(((address) >> 16) & 0xFFFF)

idt_gate_t __idt[IDT_ENTRIES];
idt_register_t __idt_reg;

void set_idt_gate(int __index, unsigned int __handler)
{
    __idt[__index].low_offset  = low_16(__handler);
    __idt[__index].sel         = KERNEL_CS;
    __idt[__index].always0     = 0;
    __idt[__index].flags       = 0x8E;
    __idt[__index].high_offset = high_16(__handler);
}

int load_idt(void)
{
    __idt_reg.base  = (unsigned int) &__idt;
    __idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;

    asm volatile("lidt (%0)" : : "r" (&__idt_reg));

    return 0;
}
