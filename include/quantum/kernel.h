#ifndef KERNEL_H
#define KERNEL_H

// symbols from linker.ld for section addresses
extern unsigned int __kernel_section_start;
extern unsigned int __kernel_section_end;
extern unsigned int __kernel_text_section_start;
extern unsigned int __kernel_text_section_end;
extern unsigned int __kernel_data_section_start;
extern unsigned int __kernel_data_section_end;
extern unsigned int __kernel_rodata_section_start;
extern unsigned int __kernel_rodata_section_end;
extern unsigned int __kernel_bss_section_start;
extern unsigned int __kernel_bss_section_end;

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
        unsigned int __start;
        unsigned int __end;
        unsigned int __size;
    } __available;
} KERNEL_MEMORY_MAP;

extern KERNEL_MEMORY_MAP __kernel_memory_map;

#endif
