#ifndef GDT_H
#define GDT_H

#define GDT_DESCRIPTOR_COUNT 8

typedef struct
{
	unsigned short __segment_limit;
	unsigned short __base_low;

	unsigned char __base_mid;
	unsigned char __access;
	unsigned char __granularity;
	unsigned char __base_high;
} __attribute__((packed)) GDT;

typedef struct
{
	unsigned short __limit;

	unsigned int __base;
} __attribute__((packed)) GDT_PTR;

void gdt_set_entry(int __index, unsigned int __base, unsigned int __limit, unsigned char __access, unsigned char __gran);

void gdt_enable(void);

#endif