#ifndef INIT_H
#define INIT_H

#include <quantum/multiboot.h>
#include <quantum/kernel.h>

int quantum_get_kernel_mmap(KERNEL_MEMORY_MAP *__map, multiboot_info_t *__mboot);

void quantum_info(char* file, int __status, char* header, char* format, ...);

void quantum_gdt_init(void);

void quantum_idt_init(void);

void quantum_memory_init(void);

void quantum_keyboard_init(void);

void quantum_pmm_init(unsigned long __addr);

void quantum_vfs_init(void);

void quantum_ata_init(void);

void quantum_devmgr_init(void);

void quantum_pci_init(void);

void quantum_migrate_to_kernel_mode(void);

void quantum_migrate_to_userspace(void);

#endif
