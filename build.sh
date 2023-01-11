#!/bin/bash -x

CC='gcc'
AS='nasm'

CFLAGS='-m32 -c -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -I include'
ASFLAGS='-f elf'

BUILDDIR='build'
KERNELDIR='kernel'
DRIVERSDIR='drivers'
BOOTDIR='boot'

function __setup
{
    mkdir -p $BUILDDIR
}

function __build_c
{
    $CC $CFLAGS $KERNELDIR/kernel.c -o $BUILDDIR/kernel.c.o
    
    $CC $CFLAGS $DRIVERSDIR/vga.c -o $BUILDDIR/vga.c.o
    $CC $CFLAGS $DRIVERSDIR/pio.c -o $BUILDDIR/pio.c.o
    $CC $CFLAGS $DRIVERSDIR/isr.c -o $BUILDDIR/isr.c.o
    $CC $CFLAGS $DRIVERSDIR/idt.c -o $BUILDDIR/idt.c.o
    $CC $CFLAGS $DRIVERSDIR/kgdt.c -o $BUILDDIR/kgdt.c.o
    $CC $CFLAGS $DRIVERSDIR/vesa.c -o $BUILDDIR/vesa.c.o
    $CC $CFLAGS $DRIVERSDIR/keyboard.c -o $BUILDDIR/keyboard.c.o
    $CC $CFLAGS $DRIVERSDIR/mouse.c -o $BUILDDIR/mouse.c.o
    $CC $CFLAGS $DRIVERSDIR/acpi.c -o $BUILDDIR/acpi.c.o

    $CC $CFLAGS $KERNELDIR/string.c -o $BUILDDIR/string.c.o
    $CC $CFLAGS $KERNELDIR/print.c -o $BUILDDIR/print.c.o
    $CC $CFLAGS $KERNELDIR/stdlib.c -o $BUILDDIR/stdlib.c.o
    $CC $CFLAGS $KERNELDIR/memory.c -o $BUILDDIR/memory.c.o
    $CC $CFLAGS $KERNELDIR/pmm.c -o $BUILDDIR/pmm.c.o
    $CC $CFLAGS $KERNELDIR/init.c -o $BUILDDIR/init.c.o
    $CC $CFLAGS $KERNELDIR/kpanic.c -o $BUILDDIR/kpanic.c.o
    $CC $CFLAGS $KERNELDIR/vfs.c -o $BUILDDIR/vfs.c.o
    $CC $CFLAGS $KERNELDIR/kmode.c -o $BUILDDIR/kmode.c.o
}

function __build_s
{
    $AS $ASFLAGS $BOOTDIR/boot.asm -o $BUILDDIR/boot.asm.o
    $AS $ASFLAGS $KERNELDIR/sys/gdt.asm -o $BUILDDIR/gdt.asm.o
    $AS $ASFLAGS $KERNELDIR/sys/interrupts.asm -o $BUILDDIR/interrupts.asm.o
}

function __link_k
{
    ld -m elf_i386 -Tconfig/linker.ld -o $BUILDDIR/kernel.elf $BUILDDIR/boot.asm.o $BUILDDIR/kernel.c.o $BUILDDIR/vga.c.o $BUILDDIR/string.c.o $BUILDDIR/print.c.o $BUILDDIR/vesa.c.o  $BUILDDIR/pio.c.o $BUILDDIR/isr.c.o $BUILDDIR/idt.c.o $BUILDDIR/kgdt.c.o $BUILDDIR/interrupts.asm.o $BUILDDIR/gdt.asm.o $BUILDDIR/pmm.c.o $BUILDDIR/init.c.o $BUILDDIR/memory.c.o $BUILDDIR/keyboard.c.o $BUILDDIR/mouse.c.o $BUILDDIR/stdlib.c.o $BUILDDIR/acpi.c.o $BUILDDIR/kpanic.c.o $BUILDDIR/vfs.c.o $BUILDDIR/kmode.c.o
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

qemu-system-x86_64 -cdrom ./build/quantumos.iso -m 3G -vga vmware -serial stdio
