#ifndef KSYMS_H
#define KSYMS_H

#include <quantum/multiboot.h>

typedef struct elf32_shdr_t {
    unsigned int name;
    unsigned int type;
    unsigned int flags;
    unsigned int addr;
    unsigned int offset;
    unsigned int size;
    unsigned int link;
    unsigned int info;
    unsigned int addralign;
    unsigned int entsize;
} elf32_shdr_t;

typedef struct elf32_sym_t {
    unsigned int   name;
    unsigned int   value;
    unsigned int   size;
    unsigned char  info;
    unsigned char  other;
    unsigned short shndx;
} elf32_sym_t;

void quantum_ksyms_init(unsigned long addr);

elf32_sym_t* get_ksym_by_address(unsigned int addr);

char* get_shdr_string(unsigned int index);

#endif