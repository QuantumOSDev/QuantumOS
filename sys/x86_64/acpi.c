/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <sys/memory.h>
#include <sys/acpi.h>
#include <sys/pio.h>
#include <sys/pit.h>

#include <drivers/debug.h>

#include <quantum/init.h>

#include <core/stdlib.h>
#include <core/string.h>
#include <core/print.h>

static BOOL machine_support_acpi = TRUE;

static rsdp_t* rsdp = NULL;
static rsdp_t* rsdt = NULL;

/* Took from limine-bootloader/limine, it did hep me a lot, thanks */
unsigned char acpi_checksum(void*ptr, unsigned long size)
{
    unsigned char* _ptr = ptr;
    unsigned char sum = 0;

    for (unsigned long i = 0; i < size; i++)
        sum += _ptr[i];

    return sum;
}

unsigned long acpi_get_ebda()
{
    unsigned int* ebda = ((unsigned int*)(pio_mminw((unsigned int*)0x40e) << 4));
    if (ebda < (unsigned int*)0x80000 || ebda >= (unsigned int*)0xa0000)
        ebda = (unsigned int*)0x80000;
    return (unsigned long)ebda;
}

void* acpi_get_rsdp(void) 
{
    unsigned long ebda = acpi_get_ebda();

    for (unsigned long i = ebda; i < 0x100000; i += 16) 
    {
        if (i == ebda + 1024) 
            i = 0xe0000;
        
        if (!kmemcmp((char*)i, RSD_SIGNATURE, 8) && !acpi_checksum((void*)i, 20)) 
        {
            quantum_info(__FILE__, 0, " ACPI   ", "Found RSDP at 0x%x", i);
            return (void*)i;
        }
    }

    return NULL;
}

BOOL is_rsdp_header_validate(rsdp_t* rsdp_header)
{
    signed char* rsdp_header_ptr = (signed char*)rsdp_header;
    int checksum = 0;

    for (int i = 0; i < sizeof(rsdp_t); i++)
        checksum += (int)rsdp_header_ptr[i];

    if ((checksum & 0x000000ff) != 0)
        return FALSE;

    if (rsdp->revision == 0)
        return TRUE;
    else if (rsdp->revision != 2)
        return FALSE;
    
    // TODO: check chcecksum of extended rsdp part
    return FALSE;
}

void quantum_acpi_init()
{
    rsdp = (rsdp_t*)acpi_get_rsdp();
    if (rsdp == NULL)
    {
        quantum_info(__FILE__, 1, " ACPI   ", "Your machine don't support ACPI");
        machine_support_acpi = FALSE;
        return;
    }

    if (is_rsdp_header_validate(rsdp) == FALSE)
    {
        quantum_info(__FILE__, 1, " ACPI   ", "RSDP Header is not validate");
        machine_support_acpi = FALSE;
        return;
    }

    quantum_info(__FILE__, 0, " ACPI   ", "Successfully initialized ACPI");
}

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
    // If machine supports ACPI, shutdown with ACPI
    /* if (machine_support_acpi == TRUE)
    {
        // acpi_enable();
        pio_outs((unsigned int)PM1a_CNT, SLP_TYPa | SLP_EN);
        if (PM1b_CNT != 0)
            pio_outs((unsigned int)PM1b_CNT, SLP_TYPb | SLP_EN);
    } */

    printf("Something gone wrong while trying to shutdown with ACPI\n");
    printf("Wait 1s to shutdown\n");

    pit_sleep(1000);

    pio_outs(0xB004, 0x2000);
    pio_outs(0x604, 0x2000);
    pio_outs(0x4004, 0x3400);
}