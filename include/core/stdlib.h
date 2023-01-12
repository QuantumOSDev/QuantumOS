#ifndef STDLIB_H
#define STDLIB_H

#define O_CLOEXEC      02000000
#define O_CREAT	       0100
#define O_DIRECTORY    0200000
#define O_EXCL		   0200	
#define O_NOCTTY	   0400	
#define O_NOFOLLOW	   0400000
#define O_TMPFILE      (020000000 | __O_DIRECTORY)
#define O_TRUNC	       01000	

typedef enum
{
    FALSE,
    TRUE,
} BOOL;

#define __SET_REGISTER__(reg, value)                           \
    __asm__ __volatile__ ("mov %0, %%" reg : : "r" (value) : ) \

#define __SYSCALL__()                    \
    __asm__ __volatile__ ("int $0x80");  \

int kstrlen(const char *__data);

void kstrcat(char *__dest, const char *__src);

#endif
