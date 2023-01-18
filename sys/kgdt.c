#include <sys/memory.h>
#include <sys/kgdt.h>
#include <sys/pio.h>

#include <quantum/init.h>

GDT __gdt_descriptors[GDT_DESCRIPTOR_COUNT];
GDT_PTR __gdt;

extern void tss_flush();
extern void load_gdt(unsigned int __gdt_ptr);

tss_entry_t tss_entry;

static void write_tss(int num, unsigned short ss0, unsigned int esp0)
{
   unsigned int base = (unsigned int) &tss_entry;
   unsigned int limit = base + sizeof(tss_entry);

   gdt_set_entry(num, base, limit, 0xE9, 0x00);
   kmemset(&tss_entry, 0, sizeof(tss_entry));

   tss_entry.ss0  = ss0;  
   tss_entry.esp0 = esp0; 

   tss_entry.cs   = 0x0b;
   tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
}

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
	write_tss(5, 0x10, 0x0);

	load_gdt((unsigned int) &__gdt);
	tss_flush();

    quantum_info(0, " GDT    ", "Successfully initialized GDT");
    quantum_info(0, " TSS    ", "Successfully initialized TSS");
}