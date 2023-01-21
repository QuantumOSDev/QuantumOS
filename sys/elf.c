/*
 * QuantumOS Copyright (c) 2021-2022
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <sys/elf.h>

#include <quantum/init.h>
#include <core/stdlib.h>

int is_elf_magic(elf32_ehdr_t* header)
{
    if (!(header->e_ident[0] == ELFMAG0 && header->e_ident[1] == ELFMAG1 &&
          header->e_ident[2] == ELFMAG2 && header->e_ident[3] == ELFMAG3)) 
        return 0;
}

int is_elf_correct(elf32_ehdr_t* header)
{
    if (header->e_ident[EI_CLASS] != ELFCLASS32) {
        quantum_info(1, " ELF    ", "Unsupported ELF File Class");
		return 0;
	}
	else if (header->e_ident[EI_DATA] != ELFDATA2LSB) {
        quantum_info(1, " ELF    ", "Unsupported ELF File byte order");
		return 0;
	}
	else if (header->e_machine != EM_386) {
        quantum_info(1, " ELF    ", "Unsupported ELF File target");
		return 0;
	}
	else if (header->e_ident[EI_VERSION] != EV_CURRENT) {
        quantum_info(1, " ELF    ", "Unsupported ELF File version");
		return 0;
	}
	else if (header->e_type != ET_REL && header->e_type != ET_EXEC) {
        quantum_info(1, " ELF    ", "Unsupported ELF File type");
		return 0;
	}
}

int load_elf(char* data, elf32_ehdr_t* header)
{
    header = (elf32_ehdr_t*)data;

    if (!is_elf_magic(header))
    {
        quantum_info(1, " ELF    ", "This is not an ELF File");
        return INVALID_ELF;
    }

    if (!is_elf_correct(header))
    {
        quantum_info(1, " ELF    ", "This is not an correct ELF File");
        return NCORRECT_ELF;
    }

    quantum_info(0, " ELF    ", "Fully correct file");

    // Load this program better please :)
    for (int i = 0; i < header->e_phnum; i++) {
        elf32_phdr_t* program_header;
        program_header = (elf32_phdr_t*)(data + (header->e_phoff + (i * header->e_phentsize)));
        
        if (program_header->p_type == PT_LOAD) {
            unsigned char* mem = (unsigned char*)program_header->p_paddr;
            kmemcpy(mem, data + program_header->p_offset, program_header->p_memsz);
        }
    }
}