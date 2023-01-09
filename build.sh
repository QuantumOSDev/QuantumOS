#!/bin/bash -x

CC='gcc'
AS='nasm'

CFLAGS='-m32 -c -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs'
ASFLAGS='-f elf'

BUILDDIR='build'
KERNELDIR='kernel'
BOOTDIR='boot'

function __setup
{
    mkdir -p $BUILDDIR
}

function __build_c
{
    $CC $CFLAGS $KERNELDIR/kernel.c -o $BUILDDIR/kernel.c.o
}

function __build_s
{
    $AS $ASFLAGS $BOOTDIR/boot.asm -o $BUILDDIR/boot.asm.o
}

function __link_k
{
    ld -m elf_i386 -Tconfig/linker.ld -o $BUILDDIR/kernel.elf $BUILDDIR/boot.asm.o $BUILDDIR/kernel.c.o
}

function __gen_iso
{
    mkdir -p $BUILDDIR/iso/boot/grub

    cp -f config/grub.cfg $BUILDDIR/iso/boot/grub/
    cp -f $BUILDDIR/kernel.elf $BUILDDIR/iso/boot/

    grub-mkrescue -o $BUILDDIR/quantumos.iso $BUILDDIR/iso
}

__setup
__build_c
__build_s
__link_k
__gen_iso
