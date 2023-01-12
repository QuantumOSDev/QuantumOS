#ifndef STDLIB_H
#define STDLIB_H

typedef enum
{
    FALSE,
    TRUE,
} BOOL;

#define __SET_REGISTER__(reg, value)                           \
    __asm__ __volatile__ ("mov %0, %%" reg : : "r" (value) : ) \

int kstrlen(const char *__data);

void kstrcat(char *__dest, const char *__src);

#endif
