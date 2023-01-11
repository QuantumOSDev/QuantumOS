#ifndef ACPI_H
#define ACPI_H

#define KEYBOARD_CONTROL 0x64
#define KEYBOARD_RESET 0xFE
#define KEYBOARD_DATA 0x60

#define KEYBOARD_BIT_KDATA 0
#define KEYBOARD_BIT_UDATA 1

#define __acpi_set_bit(__n) (1 << (__n))
#define __acpi_check_bit(__flags, __n) ((__flags) & __acpi_set_bit(__n))

void acpi_reboot(void);

#endif
