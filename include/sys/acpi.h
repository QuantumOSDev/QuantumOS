#ifndef ACPI_H
#define ACPI_H

#define KEYBOARD_CONTROL 0x64
#define KEYBOARD_RESET 0xFE
#define KEYBOARD_DATA 0x60

#define KEYBOARD_BIT_KDATA 0
#define KEYBOARD_BIT_UDATA 1

#define RSD_SIGNATURE "RSD PTR "

typedef struct rsdp_t {
   unsigned char   signature[8];
   unsigned char   check_sum;
   unsigned char   oem_id[6];
   unsigned char   revision;
   unsigned short* rsdp_addr;
} rsdp_t;

typedef struct rsdt_t {
    char          signature[4];
    unsigned int  length;
    unsigned char revision;
    unsigned char checksum;
    char          oem_id[6];
    char          oem_table_id[6];
    unsigned int  oem_revision;
    unsigned int  creator_id;
    unsigned int  creator_revision;
    char          data[];
} rsdt_t;

#define __acpi_set_bit(__n) (1 << (__n))
#define __acpi_check_bit(__flags, __n) ((__flags) & __acpi_set_bit(__n))

void quantum_acpi_init();

void acpi_reboot(void);

void acpi_shutdown();

#endif
