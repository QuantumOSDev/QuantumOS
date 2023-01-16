#ifndef _SYS_SYSCALL_H
#define _SYS_SYSCALL_H

#define SYS_read   3
#define SYS_write  4
#define SYS_open   5
#define SYS_creat  8
#define SYS_time   13

#define MAX_SYSCALLS SYS_rseq

#define ASM__SET_REG(reg, value) asm volatile ("mov %0, %%" reg : : "r" (value) : ); 

#define ASM__SYSCALL() asm volatile("int $0x80");

#ifndef _SYSCALL0_FUNC__
# define _SYSCALL0_FUNC__
  unsigned int 
  syscall0(unsigned int _syscall);
#endif /* _SYSCALL0_FUNC__ */

#ifndef _SYSCALL1_FUNC__
# define _SYSCALL1_FUNC__
  unsigned int 
  syscall1(unsigned int _syscall, unsigned int arg1);
#endif /* _SYSCALL1_FUNC__ */

#ifndef _SYSCALL2_FUNC__
# define _SYSCALL2_FUNC__
  unsigned int 
  syscall2(unsigned int _syscall, unsigned int arg1, unsigned int arg2);
#endif /* _SYSCALL2_FUNC__ */

#ifndef _SYSCALL3_FUNC__
# define _SYSCALL3_FUNC__
  unsigned int 
  syscall3(unsigned int _syscall, unsigned int arg1, unsigned int arg2, unsigned int arg3);
#endif /* _SYSCALL3_FUNC__ */

#ifndef _SYSCALL4_FUNC__
# define _SYSCALL4_FUNC__
  unsigned int 
  syscall4(unsigned int _syscall, unsigned int arg1, unsigned int arg2, unsigned int arg3, unsigned int arg4);
#endif /* _SYSCALL4_FUNC__ */

#ifndef _SYSCALL5_FUNC__
# define _SYSCALL5_FUNC__
  unsigned int 
  syscall5(unsigned int _syscall, unsigned int arg1, unsigned int arg2, unsigned int arg3, unsigned int arg4, unsigned int arg5);
#endif /* _SYSCALL5_FUNC__ */

#ifndef _SYSCALL6_FUNC__
# define _SYSCALL6_FUNC__
  unsigned int 
  syscall6(unsigned int _syscall, unsigned int arg1, unsigned int arg2, unsigned int arg3, unsigned int arg4, unsigned int arg5, unsigned int arg6);
#endif /* _SYSCALL6_FUNC__ */

#endif /* _SYS_SYSCALL_H */