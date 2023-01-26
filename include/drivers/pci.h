#ifndef PCI_H
#define PCI_H

typedef union __pci_device
{
    unsigned int __bits;

    struct {
        unsigned int __always_zero    : 2;
        unsigned int __field_num      : 6;
        unsigned int __function_num   : 3;
        unsigned int __device_num     : 5;
        unsigned int __bus_num        : 8;
        unsigned int __reserved       : 7;
        unsigned int __enable         : 1;
    };
} __pci_device_t;

/* PIO Ports */
#define PCI_CONFIG_ADDRESS  0xCF8
#define PCI_CONFIG_DATA     0xCFC

/* Config Address Register */

/* Offset */
#define PCI_VENDOR_ID            0x00
#define PCI_DEVICE_ID            0x02
#define PCI_COMMAND              0x04
#define PCI_STATUS               0x06
#define PCI_REVISION_ID          0x08
#define PCI_PROG_IF              0x09
#define PCI_SUBCLASS             0x0a
#define PCI_CLASS                0x0b
#define PCI_CACHE_LINE_SIZE      0x0c
#define PCI_LATENCY_TIMER        0x0d
#define PCI_HEADER_TYPE          0x0e
#define PCI_BIST                 0x0f
#define PCI_BAR0                 0x10
#define PCI_BAR1                 0x14
#define PCI_BAR2                 0x18
#define PCI_BAR3                 0x1C
#define PCI_BAR4                 0x20
#define PCI_BAR5                 0x24
#define PCI_INTERRUPT_LINE       0x3C
#define PCI_SECONDARY_BUS        0x09

/* Device Type */
#define PCI_HEADER_TYPE_DEVICE  0
#define PCI_HEADER_TYPE_BRIDGE  1
#define PCI_HEADER_TYPE_CARDBUS 2
#define PCI_TYPE_BRIDGE 0x0604
#define PCI_TYPE_SATA   0x0106
#define PCI_NONE 0xFFFF

#define DEVICE_PER_BUS           32
#define FUNCTION_PER_DEVICE      32

unsigned int pci_read(__pci_device_t __device, unsigned int __field);
unsigned int pci_get_device_type(__pci_device_t __device);
unsigned int pci_get_secondary_bus(__pci_device_t __device);
unsigned int pci_reach_end(__pci_device_t __device);

__pci_device_t pci_scan_function(unsigned short __vendor_id, unsigned short __device_id, unsigned int __device,
                                 unsigned int __bus, unsigned int __function, int __device_type);

__pci_device_t pci_scan_device(unsigned short __vendor_id, unsigned short __device_id, unsigned int __bus,
                               unsigned int __device, int __device_type);

__pci_device_t pci_scan_bus(unsigned short __vendor_id, unsigned short __device_id, unsigned int __bus,
                            int __device_type);

__pci_device_t pci_get_device(unsigned short __vendor_id, unsigned short __device_id, int __device_type);

void pci_write(__pci_device_t __device, unsigned int __field, unsigned int __value);

int pci_initialize(void);

#endif