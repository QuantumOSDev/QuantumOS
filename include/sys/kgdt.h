#ifndef GDT_H
#define GDT_H

#define GDT_DESCRIPTOR_COUNT 8

struct tss_entry_struct
{
   unsigned int prev_tss;  
   unsigned int esp0;       
   unsigned int ss0;        
   unsigned int esp1;       
   unsigned int ss1;
   unsigned int esp2;
   unsigned int ss2;
   unsigned int cr3;
   unsigned int eip;
   unsigned int eflags;
   unsigned int eax;
   unsigned int ecx;
   unsigned int edx;
   unsigned int ebx;
   unsigned int esp;
   unsigned int ebp;
   unsigned int esi;
   unsigned int edi;
   unsigned int es;         
   unsigned int cs;
   unsigned int ss;         
   unsigned int ds;
   unsigned int fs;         
   unsigned int gs;
   unsigned int ldt;  
   unsigned short trap;
   unsigned short iomap_base;
} __attribute__((packed));

typedef struct tss_entry_struct tss_entry_t;

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