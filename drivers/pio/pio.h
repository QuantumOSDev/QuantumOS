#ifndef PIO_H
#define PIO_H

void __pio_outb(unsigned short __port, unsigned char __data);
void __pio_outs(unsigned short __port, unsigned short __data);
void __pio_outl(unsigned short __port, unsigned int __data);

unsigned char __pio_inb(unsigned short __port);
unsigned short __pio_ins(unsigned short __port);
unsigned int __pio_inl(unsigned short __port);

void __pio_sleep(void);

#endif
