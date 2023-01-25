#ifndef PIO_H
#define PIO_H

void pio_outb(unsigned short __port, unsigned char __data);
void pio_outs(unsigned short __port, unsigned short __data);
void pio_outl(unsigned short __port, unsigned int __data);

unsigned char pio_inb(unsigned short __port);
unsigned short pio_ins(unsigned short __port);
unsigned int pio_inl(unsigned short __port);

void pio_sleep(void);

unsigned short pio_mminw(unsigned int* addr);

void pio_qinl(unsigned short __register, unsigned int *__buffer, int __quads);
void pio_qoutl(unsigned short __register, unsigned int *__buffer, int __quads);

#endif
