#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <sys/isr.h>

#define MAX_SYSCALLS 325

typedef int (*syscall_func_t) (
    int,   // arg0 (eax)
    int,   // arg1 (ebx)
    int,   // arg2 (ecx)
    int,   // arg3 (edx)
    int,   // arg4 (esi)
    int,   // arg5 (edi)
    int);  // arg6 (ebp)

void syscall_interrupt_handler(__registers_t* regs);

void quantum_syscalls_init();

// Syscalls
int sys_read(int eax, int fd, int buf, int count, int esi, int edi, int ebp);

int sys_write(int eax, int fd, int buf, int count, int esi, int edi, int ebp);

int sys_creat(int eax, int filename, int mode, int edx, int esi, int edi, int ebp);

int sys_open(int eax, int filename, int mode, int edx, int esi, int edi, int ebp);

int sys_time(int eax, int time_ptr, int ecx, int edx, int esi, int edi, int ebp);

int sys_uname(int eax, int uname_ptr, int ecx, int edx, int esi, int edi, int ebp);

#endif
