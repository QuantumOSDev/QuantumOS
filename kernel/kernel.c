/*
 * QuantumOS Copyright (c) 2021-2022
 *  - Solindek <solindeklive.biznes@gmail.com>
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <quantum/kernel_menu.h>
#include <quantum/multiboot.h>
#include <quantum/module.h>
#include <quantum/kernel.h>
#include <quantum/init.h>

#include <userspace/syscalls.h>

#include <drivers/sound_blaster.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vesa.h>

#include <sys/memory.h>
#include <sys/paging.h>
#include <sys/task.h>
#include <sys/acpi.h>
#include <sys/cmos.h>
#include <sys/pit.h>

#include <fs/ext2.h>

#include <core/stdlib.h>
#include <core/string.h>
#include <core/print.h>

void quantum_kernel_init(unsigned long magic, unsigned long addr)
{
    quantum_vesa_init(addr);
    quantum_print_init();
    quantum_gdt_init();
    quantum_idt_init();
    quantum_pmm_init(addr);
    quantum_memory_init();
    quantum_paging_init();
    quantum_keyboard_init();
    quantum_mouse_init();
    quantum_devmgr_init();
    quantum_vfs_init();
    quantum_pci_init();
    quantum_pit_init();
    quantum_acpi_init();
    quantum_time_init(TIME_ZONES_UTC_PLUS_1); 
    quantum_syscalls_init();
    quantum_sound_blaster_init();
    ata_handler(0x1F0, 0x3F6, 0x170, 0x376, 0x000);
    quantum_ext2_init();

    printf("KERNEL: Essential systems bootup... [DONE]\n");
    printf("(Press any key to continue...)\n");

    keyboard_getchar();

    vesa_clear();

    quantum_menu();
    
    return;
}
