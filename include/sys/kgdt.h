#ifndef GDT_H
#define GDT_H

struct GDT 
{
	unsigned short __size;
	unsigned int __address;
} __attribute__((packed));

struct GDTDescriptor
{
	unsigned short __limit_low;
	unsigned short __base_low;

	unsigned char __base_middle;
	unsigned char __access_byte;
	unsigned char __limit_and_flags;
	unsigned char __base_high;
} __attribute__((packed));

void kgdt_init_descriptor(int __index, unsigned int __baddr, unsigned int __limit, unsigned char __abyte, unsigned char __flags);

int kgdt_enable();

void kgdt_load_gdt(struct GDT gdt);
void kgdt_load_registers();

#endif
