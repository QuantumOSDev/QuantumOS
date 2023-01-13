#include <sys/kgdt.h>
#include <sys/pio.h>

GDT __gdt_descriptors[GDT_DESCRIPTOR_COUNT];
GDT_PTR __gdt;

extern void load_gdt(unsigned int __gdt_ptr);

void gdt_set_entry(int __index, unsigned int __base, unsigned int __limit, unsigned char __access, unsigned char __gran)
{
	GDT *__current = &__gdt_descriptors[__index];

	__current->__segment_limit = (__limit & 0xFFFF);
	__current->__base_low	   = (__base & 0xFFFF);
	__current->__base_mid	   = (__base >> 16) & 0xFF;
	__current->__access		   = __access;

	__current->__granularity   = (__limit >> 16) & 0x0F;
	__current->__granularity   = (__current->__granularity | (__gran & 0xF0));

	__current->__base_high	   = (__base >> 24 & 0xFF);
}

void gdt_enable(void)
{
	__gdt.__limit = sizeof(__gdt_descriptors) - 1;
	__gdt.__base  = (unsigned int) __gdt_descriptors;

	gdt_set_entry(0, 0, 0, 0, 0);
	gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	gdt_set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	gdt_set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

	load_gdt((unsigned int) &__gdt);
}