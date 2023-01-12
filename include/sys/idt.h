#ifndef IDT_H
#define IDT_H

#define IDE_DESCRIPTOR_COUNT 256

typedef struct
{
    unsigned short __base_low;
    unsigned short __segment_selector;
    
    unsigned char __zero;
    unsigned char __type;

    unsigned short __base_high;
} __attribute__((packed)) IDT;

typedef struct
{
    unsigned short __limit;

    unsigned int __base;
} __attribute__((packed)) IDT_PTR;

extern void load_idt(unsigned int __idt_ptr);

void idt_set_entry(int __index, unsigned int __base, unsigned short __seg, unsigned char __flags);
void idt_enable(void);

#endif
