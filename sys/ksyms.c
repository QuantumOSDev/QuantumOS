/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <core/string.h>

#include <sys/ksyms.h>

static multiboot_elf_section_header_table_t* mb_elf_section_header_table;
static elf32_shdr_t* elf_shdr;

void quantum_ksyms_init(unsigned long addr)
{
    multiboot_info_t* mbinfo = (multiboot_info_t*)addr;

    mb_elf_section_header_table = &mbinfo->u.elf_sec;
    elf_shdr = (elf32_shdr_t*)mb_elf_section_header_table->addr;
}

elf32_sym_t* get_ksym_by_address(unsigned int addr)
{
    void* candidate = NULL;
    int offset = -1;

    for (int i = 0; i < mb_elf_section_header_table->num; i++)
        {
            elf32_sym_t* sym_table = (elf32_sym_t*)elf_shdr[i].addr;

            for (int j = 0; j < elf_shdr[i].size / sizeof(elf32_sym_t); j++)
            {
                if ((offset == -1 || (addr - sym_table[j].value) < offset))
                {
                    candidate = &sym_table[j];
                    offset = addr - sym_table[j].value;
                }
            }
        }

    if (candidate)
        return (elf32_sym_t*)candidate;

    return NULL;
}

char* get_shdr_string(unsigned int index)
{
    for (int i = 0; i < mb_elf_section_header_table->num; i++)
    {
        if (elf_shdr[i].type == 3) 
            return (char*)elf_shdr[i].addr + index;
    }

    return NULL;
}