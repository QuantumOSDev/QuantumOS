#!/bin/bash

CC='gcc'
AS='nasm'

C_SOURCES=`find ./src/ -name "*.c"`
AS_SOURCES=`find ./src/ -name "*.asm"`

CFLAGS='-m32 -c -nostdlib -nostdinc -fno-pie -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -I include'
ASFLAGS='-f elf32'

for c_file in $C_SOURCES
do
    $CC $CFLAGS $c_file -o $c_file.o
done

for asm_file in $AS_SOURCES
do
    $AS $ASFLAGS $asm_file -o $asm_file.o
done