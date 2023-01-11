#include <quantum/init.h>

#include <sys/kgdt.h>

#define SEGMENT_DESCRIPTOR_COUNT 3

#define SEGMENT_BASE 0
#define SEGMENT_LIMIT 0xFFFFF

#define SEGMENT_CODE_TYPE 0x9A
#define SEGMENT_DATA_TYPE 0x92
#define SEGMENT_FLAGS_PART 0x0C

static struct GDTDescriptor __gdt_descriptors[SEGMENT_DESCRIPTOR_COUNT];

void kgdt_init_descriptor(int __index, unsigned int __baddr, unsigned int __limit, unsigned char __abyte, unsigned char __flags)
{
    quantum_info(" GDT    ", "Initializing %d descriptor", __index);
	__gdt_descriptors[__index].__base_low 			= __baddr & 0xFFFF;
	__gdt_descriptors[__index].__base_middle 		= (__baddr >> 16) & 0xFF;
	__gdt_descriptors[__index].__base_high 			= (__baddr >> 24) & 0xFF;

	__gdt_descriptors[__index].__limit_low 			= __limit & 0xFFFF;
	__gdt_descriptors[__index].__limit_and_flags 	= (__limit >> 16) & 0xF;
	__gdt_descriptors[__index].__limit_and_flags   |= (__flags << 4) & 0xF0;

	__gdt_descriptors[__index].__access_byte 		= __abyte;
}

int kgdt_enable()
{
    quantum_info(" GDT    ", "Enabling GDT");
	/* TODO: Add some kind of error handling here */
	__gdt_descriptors[0].__base_low 		= 0;
	__gdt_descriptors[0].__base_middle 		= 0;
	__gdt_descriptors[0].__base_high 		= 0;
	__gdt_descriptors[0].__limit_low 		= 0;
	__gdt_descriptors[0].__access_byte 		= 0;
	__gdt_descriptors[0].__limit_and_flags  = 0;

	struct GDT* gdt_ptr = (struct GDT *) __gdt_descriptors;
	
	gdt_ptr->__address = (unsigned int) __gdt_descriptors;
	gdt_ptr->__size = (sizeof(struct GDTDescriptor) * SEGMENT_DESCRIPTOR_COUNT) - 1;

	kgdt_init_descriptor(1, SEGMENT_BASE, SEGMENT_LIMIT, SEGMENT_CODE_TYPE, SEGMENT_FLAGS_PART);
	kgdt_init_descriptor(2, SEGMENT_BASE, SEGMENT_LIMIT, SEGMENT_DATA_TYPE, SEGMENT_FLAGS_PART);

	kgdt_load_gdt(*gdt_ptr);
	kgdt_load_registers();

	return 0;
}
