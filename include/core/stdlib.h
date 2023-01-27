#ifndef STDLIB_H
#define STDLIB_H

typedef enum
{
    FALSE,
    TRUE,
} BOOL;

#define OPTIONAL(type) type \

#define OPTIONAL_NIL() NULL \

#define __SET_REGISTER__(reg, value)                           \
    __asm__ __volatile__ ("mov %0, %%" reg : : "r" (value) : ) \

#define __SYSCALL__()                    \
    __asm__ __volatile__ ("int $0x80");  \

int kstrlen(const char *__data);

void kstrcat(char *__dest, const char *__src);

int isalpha(char c);

int isdigit(char c);

int isspace(char c);

void snprintf(char* buf, int buf_size, char* format, ...);

void sprintf(char* buf, char* format, ...);

#endif
