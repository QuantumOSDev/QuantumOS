#ifndef IDT_H
#define IDT_H

#define KERNEL_CS 0x08
#define IDT_ENTRIES 256

typedef struct {
    unsigned short low_offset;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short high_offset;
} __attribute__((packed)) idt_gate_t;

typedef struct {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed)) idt_register_t;

void set_idt_gate(int __index, unsigned int __handler);

int load_idt(void);

#endif
