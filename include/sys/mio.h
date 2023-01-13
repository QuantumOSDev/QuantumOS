#ifndef MIO_H
#define MIO_H

void mio_out8(void *__ptr, unsigned char __data);
void mio_out16(void *__ptr, unsigned short __data);
void mio_out32(void *__ptr, unsigned int __data);

unsigned char mio_in8(void *__ptr);
unsigned short mio_in16(void *__ptr);
unsigned int mio_in32(void *__ptr);

#endif