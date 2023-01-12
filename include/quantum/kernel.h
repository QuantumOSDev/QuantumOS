#ifndef KERNEL_H
#define KERNEL_H

// symbols from linker.ld for section addresses
extern unsigned char __kernel_section_start;
extern unsigned char __kernel_section_end;
extern unsigned char __kernel_text_section_start;
extern unsigned char __kernel_text_section_end;
extern unsigned char __kernel_data_section_start;
extern unsigned char __kernel_data_section_end;
extern unsigned char __kernel_rodata_section_start;
extern unsigned char __kernel_rodata_section_end;
extern unsigned char __kernel_bss_section_start;
extern unsigned char __kernel_bss_section_end;

typedef struct
{
    struct
    {
        unsigned int __kernel_start;
        unsigned int __kernel_end;
        unsigned int __kernel_len;

        unsigned int __text_start;
        unsigned int __text_end;
        unsigned int __text_len;

        unsigned int __data_start;
        unsigned int __data_end;
        unsigned int __data_len;

        unsigned int __rodata_start;
        unsigned int __rodata_end;
        unsigned int __rodata_len;

        unsigned int __bss_start;
        unsigned int __bss_end;
        unsigned int __bss_len;
    } __kernel;

    struct
    {
        unsigned int __total_memory;
    } __system;

    struct
    {
        unsigned long __start;
        unsigned long __end;
        unsigned long __size;
    } __available;
} KERNEL_MEMORY_MAP;

#endif
