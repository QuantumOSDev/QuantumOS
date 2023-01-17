#include <sys/memory.h>
#include <sys/acpi.h>
#include <sys/pio.h>

#include <core/stdlib.h>
#include <core/string.h>
#include <core/print.h>

void acpi_reboot(void)
{
    unsigned char __value;

    asm volatile ("cli");

    do
    {
        __value = pio_inb(KEYBOARD_CONTROL);

        if (__acpi_check_bit(__value, KEYBOARD_BIT_KDATA) != 0)
        {
            pio_inb (KEYBOARD_DATA);
        }
    } while (__acpi_check_bit(__value, KEYBOARD_BIT_UDATA) != 0);

    pio_outb(KEYBOARD_CONTROL, KEYBOARD_RESET);

__rescue_loop:
    asm volatile ("hlt");

    goto __rescue_loop;
}

void acpi_shutdown()
{
    printf("ACPI Shutdown is not implemented, trying to shutdown by virtual machines");

    /* Wait XD */
    for (long i = 0; i < 100000000; i++)
        continue;

    pio_outs(0xB004, 0x2000);
    pio_outs(0x604, 0x2000);
    pio_outs(0x4004, 0x3400);
}