#!/bin/bash

CC='gcc'
AS='nasm'

SYS_SYSCALL_O='./src/sys/syscall.c.o'
STDIO_O='./src/stdio.c.o'
CTR0_O='./src/crt0.asm.o'

CFLAGS='-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -I include'

$CC $CFLAGS ./test/main.c $STDIO_O $CTR0_O $SYS_SYSCALL_O -o ./test/a.out 