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
    $CC $CFLAGS $DRIVERSDIR/pic.c -o $BUILDDIR/pic.c.o
    $CC $CFLAGS $DRIVERSDIR/ata.c -o $BUILDDIR/ata.c.o
    $CC $CFLAGS $DRIVERSDIR/cmos.c -o $BUILDDIR/cmos.c.o

    $CC $CFLAGS $KERNELDIR/string.c -o $BUILDDIR/string.c.o
    $CC $CFLAGS $KERNELDIR/print.c -o $BUILDDIR/print.c.o
    $CC $CFLAGS $KERNELDIR/stdlib.c -o $BUILDDIR/stdlib.c.o
    $CC $CFLAGS $KERNELDIR/memory.c -o $BUILDDIR/memory.c.o
    $CC $CFLAGS $KERNELDIR/pmm.c -o $BUILDDIR/pmm.c.o
    $CC $CFLAGS $KERNELDIR/init.c -o $BUILDDIR/init.c.o
    $CC $CFLAGS $KERNELDIR/syscalls.c -o $BUILDDIR/syscalls.c.o
    $CC $CFLAGS $KERNELDIR/kpanic.c -o $BUILDDIR/kpanic.c.o
    $CC $CFLAGS $KERNELDIR/vfs.c -o $BUILDDIR/vfs.c.o
    $CC $CFLAGS $KERNELDIR/kmode.c -o $BUILDDIR/kmode.c.o
    $CC $CFLAGS $KERNELDIR/userspace.c -o $BUILDDIR/userspace.c.o
    $CC $CFLAGS $KERNELDIR/time.c -o $BUILDDIR/time.c.o
}

function __build_s
{
    $AS $ASFLAGS $BOOTDIR/boot.asm -o $BUILDDIR/boot.asm.o
    $AS $ASFLAGS $KERNELDIR/sys/gdt.asm -o $BUILDDIR/gdt.asm.o
    $AS $ASFLAGS $KERNELDIR/sys/interrupts.asm -o $BUILDDIR/interrupts.asm.o
    $AS $ASFLAGS $KERNELDIR/sys/idt.asm -o $BUILDDIR/idt.asm.o
    $AS $ASFLAGS $KERNELDIR/sys/irq.asm -o $BUILDDIR/irq.asm.o
    $AS $ASFLAGS $KERNELDIR/sys/syscalls.asm -o $BUILDDIR/syscalls.asm.o
    $AS $ASFLAGS $KERNELDIR/sys/bios32_call.asm -o $BUILDDIR/bios32_call.asm.o
}

function __link_k
{
    ld -m elf_i386 -Tconfig/linker.ld -o $BUILDDIR/kernel.elf $BUILDDIR/boot.asm.o $BUILDDIR/kernel.c.o $BUILDDIR/userspace.c.o $BUILDDIR/cmos.c.o $BUILDDIR/vga.c.o $BUILDDIR/string.c.o $BUILDDIR/print.c.o $BUILDDIR/vesa.c.o  $BUILDDIR/pio.c.o $BUILDDIR/isr.c.o $BUILDDIR/idt.c.o $BUILDDIR/syscalls.c.o $BUILDDIR/kgdt.c.o $BUILDDIR/interrupts.asm.o $BUILDDIR/gdt.asm.o $BUILDDIR/pmm.c.o $BUILDDIR/init.c.o $BUILDDIR/memory.c.o $BUILDDIR/keyboard.c.o $BUILDDIR/mouse.c.o $BUILDDIR/stdlib.c.o $BUILDDIR/acpi.c.o $BUILDDIR/kpanic.c.o $BUILDDIR/vfs.c.o $BUILDDIR/kmode.c.o $BUILDDIR/pic.c.o $BUILDDIR/idt.asm.o $BUILDDIR/irq.asm.o $BUILDDIR/ata.c.o $BUILDDIR/time.c.o
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

sudo sh mkdisk.sh
sudo chown $USER:$USER ext2.img

qemu-system-x86_64 -cdrom ./build/quantumos.iso -drive file=ext2.img,format=raw -m 3G -vga vmware -serial stdio
