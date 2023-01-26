/*
 * QuantumOS Copyright (c) 2022-2023
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <quantum/init.h>

#include <drivers/pci.h>
#include <drivers/ac97.h>

#include <net/ethernet.h>

#include <core/print.h>

#include <sys/memory.h>
#include <sys/pio.h>

#include <fs/ata.h>

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA    0xCFC

typedef int (*__device_driver_fpointer) (__pci_device_t *);

typedef struct __pci_known_device
{
    unsigned int __vendor_id;
    unsigned int __device_id;

    __device_driver_fpointer __driver;

    char *__device_name;
} __pci_known_device_t;

typedef struct __pci_generic_device
{
    unsigned int __class_code;
    unsigned int __subclass;

    int __prog_if;

    __device_driver_fpointer __driver;

    char *__device_name;
} __pci_generic_device_t;

const __pci_known_device_t __known_devices[] = {0};
const __pci_generic_device_t __generic_devices[] = {
    {
        .__class_code  = 0x01,
        .__subclass    = 0x01,
        .__prog_if     = -1,
        .__driver      = &ata_initialize,
        .__device_name = "PATA Hard Drive"
    },
    {
        .__class_code  = 0x02,
        .__subclass    = 0x00,
        .__prog_if     = -1,
        .__driver      = &ethernet_initalize,
        .__device_name = "Ethernet"
    },
    {
        .__class_code  = 0x04,
        .__subclass    = 0x01,
        .__prog_if     = -1,
        .__driver      = &ac97_initialize,
        .__device_name = "AC97 Sound card"
    }
};

__pci_device_t *__pcilist;

unsigned int __pci_device_count = 0;

const int __known_device_amount   = sizeof(__known_devices) / sizeof(__pci_known_device_t);
const int __generic_device_amount = sizeof(__generic_devices) / sizeof(__pci_generic_device_t);

unsigned int pci_make_addr(unsigned short __bus, unsigned short __slot, unsigned short __func, unsigned short __offset)
{
    unsigned int __lbus  = (unsigned int) __bus;
    unsigned int __lslot = (unsigned int) __slot;
    unsigned int __lfunc = (unsigned int) __func;

    unsigned short __tmp;

    return (unsigned int) ((__lbus << 16) | (__lslot << 11) | (__lfunc << 8) | (__offset & 0xFC) | ((unsigned int) 0x80000000));
}

unsigned short pci_read_word(unsigned short __bus, unsigned short __slot, unsigned short __func, unsigned short __offset)
{
    unsigned int __addr = pci_make_addr(__bus, __slot, __func, __offset);

    unsigned short __tmp;

    pio_outl(CONFIG_ADDRESS, __addr);

    __tmp = (unsigned short) ((pio_inl(CONFIG_DATA) >> ((__offset & 2) * 8)) & 0xFFFF);

    return __tmp;
}

void pci_write_dword(unsigned short __bus, unsigned short __slot, unsigned short __function,
                     unsigned short __offset, unsigned int __value)
{
    pio_outl(CONFIG_ADDRESS, pci_make_addr(__bus, __slot, __function, __offset));
    pio_outl(CONFIG_DATA, __value);
}

unsigned short pci_read(__pci_device_t *__device, unsigned int __offset)
{
    return pci_read_word(__device->__bus, __device->__device, __device->__function, __offset);
}

unsigned short pci_get_vendor_id(unsigned short __bus, unsigned short __device, unsigned short __function)
{
    return (unsigned short) pci_read_word(__bus, __device, __function, PCI_COMMAND_OFFSET);
}

unsigned short pci_get_device_id(unsigned short __bus, unsigned short __device, unsigned short __function)
{
    return (unsigned short) pci_read_word(__bus, __device, __function, PCI_COMMAND_OFFSET);
}

unsigned short pci_get_class_id(unsigned short __bus, unsigned short __device, unsigned short __function)
{
    return (((unsigned short) pci_read_word(__bus, __device, __function, 0xA)) & ~0x00FF) >> 8;
}

unsigned short pci_get_subclass_id(unsigned short __bus, unsigned short __device, unsigned short __function)
{
    return ((unsigned short) pci_read_word(__bus, __device, __function, 0xA)) & ~0xFF00;
}

unsigned short pci_get_prog_if(unsigned short __bus, unsigned short __device, unsigned short __function)
{
    return (((unsigned short) pci_read_word(__bus, __device, __function, 0x8)) & ~0x00FF) >> 8;
}

unsigned short pci_get_base_address(unsigned short __bus, unsigned short __device, unsigned short __function, int __address)
{
    unsigned int __tmp = ((unsigned int) pci_read_word(__bus, __device, __function, 0xf + __address * 4)) << 16;

    __tmp |= (unsigned int) ((unsigned short) pci_read_word(__bus, __device, __function, 0xf + __address * 4 + 2));

    return __tmp;
}

unsigned short pci_get_header_type(unsigned short __bus, unsigned short __device, unsigned short __function)
{
    return (((unsigned short) pci_read_word(__bus, __device, __function, 0xE)) & ~0xFF00);
}

unsigned short pci_get_rev_id(unsigned short __bus, unsigned short __device, unsigned short __function)
{
    return (((unsigned short) pci_read_word(__bus, __device, __function, 0x8)) & ~0xFF00);
}

unsigned short pci_get_command(unsigned short __bus, unsigned short __device, unsigned short __function)
{
    return (unsigned short) pci_read_word(__bus, __device, __function, PCI_COMMAND_OFFSET);
}

void *pci_parse_header(unsigned short __bus, unsigned short __slot, unsigned short __function, unsigned short __header_type)
{
    if (__header_type == 0x00)
    {
        __pci_common_header_t *__header = kcalloc(0, sizeof(__pci_common_header_t));

        for (int i = 0; i < 6; i++)
        {
            __header->__BAR[i] = pci_get_base_address(__bus, __slot, __function, i);
        }

        __header->__interrupt_pin  = pci_read_word(__bus, __slot, __function, PCI_INTERRUPT_PIN_OFFSET);
        __header->__interrupt_line = pci_read_word(__bus, __slot, __function, PCI_INTERRUPT_LINE_OFFSET);

        return (void *) __header;
    }
    else
    {
        /* Invalid/Unknown header... we don't support it */

        return (void *) 0;
    }

    return (void *) 0;
}

void pci_add_device(__pci_device_t *__device)
{
    if (__pcilist == 0)
    {
        __pcilist = __device;
    }
    else
    {
        volatile __pci_device_t *__tmp = __pcilist;

        while (__tmp->__next != 0)
        {
            __tmp = __tmp->__next;
        }

        __tmp->__next = __device;
    }

    __pci_device_count++;
}

void pci_probe_devices(void)
{
    for (int __bus = 0; __bus < 256; __bus++)
    {
        for (int __slot = 0; __slot < 32; __slot++)
        {
            for (int __function = 0; __function < 8; __function++)
            {
                unsigned short __vendor_id = pci_get_vendor_id(__bus, __slot, __function);

                if (__vendor_id == 0xffff)
                {
                    continue;
                }

                __pci_device_t *__device = (__pci_device_t *) kcalloc(1, sizeof(__pci_device_t));

                __device->__bus         = __bus;
                __device->__device      = __slot;
                __device->__function    = __function;
                __device->__vendor_id   = __vendor_id;
                
                __device->__device_id   = pci_get_device_id(__bus, __slot, __function);
                __device->__command     = pci_get_command(__bus, __slot, __function);
                __device->__class_code  = pci_get_class_id(__bus, __slot, __function);
                __device->__subclass    = pci_get_subclass_id(__bus, __slot, __function);
                __device->__prog_if     = pci_get_prog_if(__bus, __slot, __function);
                __device->__header_type = pci_get_header_type(__bus, __slot, __function);
                
                __device->__header      = pci_parse_header(__bus, __slot, __function, __device->__header_type);

                __device->__rev_id      = pci_get_rev_id(__bus, __slot, __function);
                __device->__name        = "-(PCI)- Unknown Device";

                pci_add_device(__device);

                quantum_info(0, " PIC    ", "PCI Device: [CC: 0x%x | SC: 0x%x | PI: 0x%x]", __device->__class_code, __device->__subclass, __device->__prog_if);

                for (int i = 0; i < __known_device_amount; i++)
                {
                    if (__known_devices[i].__vendor_id == __device->__vendor_id && __known_devices[i].__device_id == __device->__device_id)
                    {
                        __device->__driver = __known_devices[i].__driver;
                        __device->__name   = __known_devices[i].__device_name;

                        quantum_info(0, " PIC    ", "Driver found for PCI Device: [%s] [CC: 0x%x | SC: 0x%x | PI: 0x%x]", __device->__name, __device->__class_code, __device->__subclass, __device->__prog_if);
                    }
                }

                for (int i = 0; i < __generic_device_amount; i++)
                {
                    if (__generic_devices[i].__class_code == __device->__class_code && __generic_devices[i].__subclass == __device->__subclass)
                    {
                        __device->__driver = __generic_devices[i].__driver;
                        __device->__name   = __generic_devices[i].__device_name;

                        quantum_info(0, " PIC    ", "Driver found for PCI Device: [%s] [CC: 0x%x | SC: 0x%x | PI: 0x%x]", __device->__name, __device->__class_code, __device->__subclass, __device->__prog_if);
                        
                        __device->__driver(__device);
                    }
                }
            }
        }
    }
}

__pci_device_t *pci_find_by_class(int __class, int __subclass, int __prog_if)
{
    if (__pcilist = 0)
    {
        return 0;
    }

    __pci_device_t *__tmp = __pcilist;

    do
    {
        if (__tmp->__class_code == __class && __tmp->__subclass == __subclass && __tmp->__prog_if == __prog_if)
        {
            return __tmp;
        }

        __tmp = __tmp->__next;
    } while (__tmp != 0);

    return (void *) 0;
}

__pci_device_t *pci_find_by_vendor(int __vendor, int __device)
{
    if (__pcilist == 0)
    {
        return 0;
    }

    __pci_device_t *__tmp = __pcilist;

    do
    {
        if (__tmp->__vendor_id == __vendor && __tmp->__device_id == __device)
        {
            return __tmp;
        }

        __tmp = __tmp->__next;
    } while (__tmp != 0);

    return (void *) 0;
}

unsigned int pci_get_bar_by_index(__pci_device_t *__device, unsigned int __index)
{
    return (((__pci_common_header_t *) __device->__header)->__BAR[__index]);
}

unsigned int pci_get_bar_by_type(__pci_device_t *__device, unsigned char __type)
{
    unsigned int __bar = 0;

    for (int i = 0; i < 6; i++)
    {
        __bar = ((__pci_common_header_t *) __device->__header)->__BAR[i];

        if ((__bar & 0x1) == __type)
        {
            return __bar;
        }
    }

    /* Where is the bar type? */

    return 0xFFFFFFFF;
}

int pci_initialize_devices(void)
{
    __pci_device_t *__tmp = __pcilist;

    if (__tmp == 0)
    {
        return 0;
    }

    int __exit = 0;

    do
    {
        if (__tmp->__driver != 0)
        {
            __exit = __tmp->__driver(__tmp);

            quantum_info(0, " PCI\t", "Driver initialization of [%s] %s", (char *) __tmp->__name, (__exit ? "failed" : "succeeded"));
        }
    } while ((__tmp = __tmp->__next) != 0);

    return 0;
}

int pci_initialize(void)
{
    pci_probe_devices();

    return 0;
}