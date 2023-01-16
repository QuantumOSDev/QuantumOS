#include <sys/syscall.h>

unsigned int 
syscall0(unsigned int _syscall)
{
    ASM__SET_REG("eax", _syscall);
    ASM__SYSCALL();
}

unsigned int 
syscall1(unsigned int _syscall, unsigned int arg1)
{
    ASM__SET_REG("ebx", arg1);
    ASM__SET_REG("eax", _syscall);
    ASM__SYSCALL();
}

unsigned int 
syscall2(unsigned int _syscall, unsigned int arg1, unsigned int arg2)
{
    ASM__SET_REG("ebx", arg1);
    ASM__SET_REG("ecx", arg2);
    ASM__SET_REG("eax", _syscall);
    ASM__SYSCALL();
}

unsigned int 
syscall3(unsigned int _syscall, unsigned int arg1, unsigned int arg2, unsigned int arg3)
{
    ASM__SET_REG("ebx", arg1);
    ASM__SET_REG("ecx", arg2);
    ASM__SET_REG("edx", arg3);
    ASM__SET_REG("eax", _syscall);
    ASM__SYSCALL();
}

unsigned int 
syscall4(unsigned int _syscall, unsigned int arg1, unsigned int arg2, unsigned int arg3, unsigned int arg4)
{
    ASM__SET_REG("ebx", arg1);
    ASM__SET_REG("ecx", arg2);
    ASM__SET_REG("edx", arg3);
    ASM__SET_REG("esi", arg4);
    ASM__SET_REG("eax", _syscall);
    ASM__SYSCALL();
}

unsigned int 
syscall5(unsigned int _syscall, unsigned int arg1, unsigned int arg2, unsigned int arg3, unsigned int arg4, unsigned int arg5)
{
    ASM__SET_REG("ebx", arg1);
    ASM__SET_REG("ecx", arg2);
    ASM__SET_REG("edx", arg3);
    ASM__SET_REG("esi", arg4);
    ASM__SET_REG("edi", arg5);
    ASM__SET_REG("eax", _syscall);
    ASM__SYSCALL();
}

unsigned int 
syscall6(unsigned int _syscall, unsigned int arg1, unsigned int arg2, unsigned int arg3, unsigned int arg4, unsigned int arg5, unsigned int arg6)
{
    ASM__SET_REG("ebx", arg1);
    ASM__SET_REG("ecx", arg2);
    ASM__SET_REG("edx", arg3);
    ASM__SET_REG("esi", arg4);
    ASM__SET_REG("edi", arg5);
    ASM__SET_REG("ebp", arg6);
    ASM__SET_REG("eax", _syscall);
    ASM__SYSCALL();
}