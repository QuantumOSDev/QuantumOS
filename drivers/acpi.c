#include <drivers/acpi.h>

#include <sys/pio.h>

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
