#ifndef PCI_H
#define PCI_H

#define PCI_VENDOR_OFFSET         0x0
#define PCI_DEVICE_ID_OFFSET      0x2
#define PCI_COMMAND_OFFSET        0x4
#define PCI_INTERRUPT_LINE_OFFSET 0x3C
#define PCI_INTERRUPT_PIN_OFFSET  0x3E

#define PCI_BAR_MEM  0x0
#define PCI_BAR_IO   0x1
#define PCI_BAR_NONE 0x3

struct __pci_device_s;

typedef struct __pci_device_s __pci_device_t;

typedef int (* __pci_device_initializer_t) (__pci_device_t *__dev);

struct __pci_device_s
{
    unsigned short __bus;
    unsigned short __device;
    unsigned short __function;

    unsigned short __vendor_id;
    unsigned short __device_id;
    unsigned short __command;
    
    unsigned char __class_code;
    unsigned char __subclass;
    unsigned char __prog_if;
    unsigned char __rev_id;
    unsigned char __header_type;

    void *__header;

    __pci_device_initializer_t __driver;

    const char *__name;

    struct __pci_device_s *__next;
};

typedef struct
{
    unsigned int __BAR[6];

    unsigned char __interrupt_line;
    unsigned char __interrupt_pin;
    unsigned char __min_grant;
    unsigned char __max_latency;
} __pci_common_header_t;

void pci_write_dword(unsigned short __bus, unsigned short __slot, unsigned short __function,
                     unsigned short __offset, unsigned int __value);

__pci_device_t *pci_find_by_class(int __class, int __subclass, int __prog_if);
__pci_device_t *pci_find_by_vendor(int __vendor, int __device);

unsigned int pci_get_bar_by_index(__pci_device_t *__device, unsigned int __index);
unsigned int pci_get_bar_by_type(__pci_device_t *__device, unsigned char __type);

int pci_initialize(void);
int pci_initialize_devices(void);

#endif