#include <quantum/multiboot.h>
#include <quantum/kernel.h>
#include <quantum/init.h>

#include <userspace/syscalls.h>

#include <drivers/sound_blaster.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vesa.h>

#include <sys/memory.h>
#include <sys/acpi.h>
#include <sys/cmos.h>

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
    quantum_keyboard_init();
    quantum_mouse_init();
    quantum_devmgr_init();
    quantum_vfs_init();
    quantum_ata_init();
    quantum_syscalls_init();
    quantum_sound_blaster_init();
    quantum_ext2_init();

    printf("QuantumOS has boot up!\n");
    printf("Press any key to continue booting into userspace!\nHit 'k' to enter kernel-mode...\n");

    char __boot_mode = keyboard_getchar();

    if (__boot_mode == 'k')
    {
        printf("\nBailing out!\nGood luck you're on your own...\n\n");
        quantum_migrate_to_kernel_mode();
    }
    else
    {
        printf("\n");
        quantum_migrate_to_userspace();
    }

    return;
}
