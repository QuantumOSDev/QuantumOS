/*
 * QuantumOS Copyright (c) 2022-2023
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <quantum/init.h>
#include <drivers/pci.h>

#include <sys/pio.h>

unsigned int __pci_size_map[100];

__pci_device_t __pci_dev_zero;

unsigned int pci_read(__pci_device_t __device, unsigned int __field)
{
    __device.__field_num = (__field & 0xFC) >> 2;

    __device.__enable = 1;

    pio_outl(PCI_CONFIG_ADDRESS, __device.__bits);

    unsigned int __size = __pci_size_map[__field];

    if (__size == 1)
    {
        unsigned char __target = pio_inb(PCI_CONFIG_DATA + (__field & 3));

        return __target;
    }
    else if (__size == 2)
    {
        unsigned short __target = pio_ins(PCI_CONFIG_DATA + (__field & 2));

        return __target;
    }
    else if (__size == 4)
    {
        unsigned int __target = pio_inl(PCI_CONFIG_DATA);

        return __target;
    }

    return 0xFFFF;
}

unsigned int pci_get_device_type(__pci_device_t __device)
{
    unsigned int __type = pci_read(__device, PCI_CLASS) << 8;

    return __type | pci_read(__device, PCI_SUBCLASS);
}

unsigned int pci_get_secondary_bus(__pci_device_t __device)
{
    return pci_read(__device, PCI_SECONDARY_BUS);
}

unsigned int pci_reach_end(__pci_device_t __device)
{
    unsigned int __end = pci_read(__device, PCI_HEADER_TYPE);

    return !__end;
}

__pci_device_t pci_scan_function(unsigned short __vendor_id, unsigned short __device_id, unsigned int __device,
                                 unsigned int __bus, unsigned int __function, int __device_type)
{
    __pci_device_t __dev = {0};

    __dev.__bus_num      = __bus;
    __dev.__device_num   = __device;
    __dev.__function_num = __function;

    if (pci_get_device_type(__dev) == PCI_TYPE_BRIDGE)
    {
        pci_scan_bus(__vendor_id, __device_id, pci_get_secondary_bus(__dev), __device_type);
    }

    if (__device_type == -1 || __device_type == pci_get_device_type(__dev))
    {
        unsigned int __devid = pci_read(__dev, PCI_DEVICE_ID);
        unsigned int __venid = pci_read(__dev, PCI_VENDOR_ID);

        if (__devid == __device_id && __venid == __vendor_id)
        {
            return __dev;
        }
    }

    return __pci_dev_zero;
}

__pci_device_t pci_scan_device(unsigned short __vendor_id, unsigned short __device_id, unsigned int __bus,
                               unsigned int __device, int __device_type)
{
    __pci_device_t __dev = {0};

    __dev.__bus_num    = __bus;
    __dev.__device_num = __device;

    if (pci_read(__dev, PCI_VENDOR_ID) == PCI_NONE)
    {
        return __pci_dev_zero;
    }

    __pci_device_t __target = pci_scan_function(__vendor_id, __device_id, __bus, __device, 0, __device_type);

    if (__target.__bits)
    {
        return __target;
    }

    if (pci_reach_end(__dev))
    {
        return __pci_dev_zero;
    }

    for (int f = 1; f < FUNCTION_PER_DEVICE; f++)
    {
        if (pci_read(__dev, PCI_VENDOR_ID) != PCI_NONE)
        {
            __target = pci_scan_function(__vendor_id, __device_id, __bus, __device, f, __device_type);

            if (__target.__bits)
            {
                return __target;
            }
        }
    }

    return __pci_dev_zero;
}

__pci_device_t pci_scan_bus(unsigned short __vendor_id, unsigned short __device_id, unsigned int __bus,
                            int __device_type)
{
    for (int d = 0; d < DEVICE_PER_BUS; d++)
    {
        __pci_device_t __target = pci_scan_bus(__vendor_id, __device_id, d, __device_type);

        if (__target.__bits)
        {
            return __target;
        }
    }

    return __pci_dev_zero;
}

__pci_device_t pci_get_device(unsigned short __vendor_id, unsigned short __device_id, int __device_type)
{
    __pci_device_t __target = pci_scan_bus(__vendor_id, __device_id, 0, __device_type);

    if (__target.__bits)
    {
        return __target;
    }

    if (pci_reach_end(__pci_dev_zero))
    {
        /* Failed */

#ifdef DEBUG_PCI
        printf("PCI Get Device Failed\n");
#endif
    }

    for (int f = 1; f < FUNCTION_PER_DEVICE; f++)
    {
        __pci_device_t __dev = {0};

        __dev.__function_num = f;

        if (pci_read(__dev, PCI_VENDOR_ID) == PCI_NONE)
        {
            break;
        }

        __target = pci_scan_bus(__vendor_id, __device_id, f, __device_type);

        if (__target.__bits)
        {
            return __target;
        }
    }

    return __pci_dev_zero;
}

void pci_write(__pci_device_t __device, unsigned int __field, unsigned int __value)
{
    __device.__field_num = (__field & 0xFC) >> 2;
    __device.__enable    = 1;

    pio_outl(PCI_CONFIG_ADDRESS, __device.__bits);
    pio_outl(PCI_CONFIG_DATA, __value);
}

int pci_initialize(void)
{
    __pci_size_map[PCI_VENDOR_ID]       = 2;
    __pci_size_map[PCI_DEVICE_ID]       = 2;
    __pci_size_map[PCI_COMMAND]         = 2;
    __pci_size_map[PCI_STATUS]          = 2;
    __pci_size_map[PCI_SUBCLASS]        = 1;
    __pci_size_map[PCI_CLASS]           = 1;
    __pci_size_map[PCI_CACHE_LINE_SIZE] = 1;
    __pci_size_map[PCI_LATENCY_TIMER]   = 1;
    __pci_size_map[PCI_HEADER_TYPE]     = 1;
    __pci_size_map[PCI_BIST]            = 1;
    __pci_size_map[PCI_BAR0]            = 4;
    __pci_size_map[PCI_BAR1]            = 4;
    __pci_size_map[PCI_BAR2]            = 4;
    __pci_size_map[PCI_BAR3]            = 4;
    __pci_size_map[PCI_BAR4]            = 4;
    __pci_size_map[PCI_BAR5]            = 4;
    __pci_size_map[PCI_INTERRUPT_LINE]  = 1;
    __pci_size_map[PCI_SECONDARY_BUS]   = 1;

    return 0;
} 