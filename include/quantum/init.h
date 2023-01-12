#ifndef INIT_H
#define INIT_H

#define DEBUG

#include <quantum/kernel.h>

void quantum_info(int __status, char* header, char* format, ...);

void quantum_gdt_init(void);

void quantum_idt_init(void);

void quantum_memory_init(void);

void quantum_keyboard_init(void);

void quantum_pmm_init(unsigned long __addr);

void quantum_vfs_init(void);

void quantum_migrate_to_kernel_mode(void);

void quantum_migrate_to_userspace(void);
 
KERNEL_MEMORY_MAP* get_kernel_mm();

#endif
