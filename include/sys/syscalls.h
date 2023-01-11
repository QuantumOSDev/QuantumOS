#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <sys/isr.h>

#define MAX_SYSCALLS 325

typedef void (*syscall_func_t) (
    int*,  // return (eax)
    int,   // arg0 (eax)
    int,   // arg1 (ebx)
    int,   // arg2 (ecx)
    int,   // arg3 (edx)
    int,   // arg4 (esi)
    int,   // arg5 (edi)
    int);  // arg6 (ebp)

void syscall_interrupt_handler(registers_t* regs);

void quantum_syscalls_init();

// Syscalls
void sys_write(int* syscall_return, int eax, int fd, int buf, int count, int esi, int edi, int ebp);

#endif
