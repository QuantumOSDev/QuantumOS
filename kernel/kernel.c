#include <quantum/multiboot.h>
#include <quantum/kernel.h>
#include <quantum/init.h>

#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vesa.h>
#include <drivers/acpi.h>
#include <drivers/cmos.h>

#include <sys/userspace.h>
#include <sys/syscalls.h>
#include <sys/memory.h>

#include <core/stdlib.h>
#include <core/string.h>
#include <core/print.h>

void quantum_kernel_init(unsigned long magic, unsigned long addr)
{
    quantum_vesa_init(addr);
    quantum_print_init();
    quantum_gdt_init();
    quantum_isr_init();
    quantum_pmm_init(addr);
    quantum_memory_init();
    quantum_keyboard_init();
    quantum_mouse_init();
    quantum_vfs_init();
    quantum_syscalls_init();

    printf("QuantumOS has boot up! ");

    date_t date = get_date_cmos();
    printf("Current time: %s%d:%s%d:%s%d %d%d:%s%d:%s%d\n\n", 
           // Time
           date.hour <= 9 ? "0" : "", date.hour, 
           date.minute <= 9 ? "0" : "", date.minute, 
           date.second <= 9 ? "0" : "", date.second,    

           // Date
           date.century, 
           date.year, 
           date.month <= 9 ? "0" : "", date.month, 
           date.day <= 9 ? "0" : "", date.day);

/* DISABLE MOUSE DRIVERS FOR NOW
    for (;;) {
        vesa_draw_circle(get_mouse_x(), get_mouse_y(), 4, 255, 0, 0);
    }
*/

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
