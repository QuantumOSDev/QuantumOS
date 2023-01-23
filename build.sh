#!/bin/bash

CC='gcc'
AS='nasm'

BUILDDIR='build'

C_SOURCES=`find ./core/ ./gui/ ./drivers/ ./net/ ./fs/ ./kernel/ ./sys/ ./userspace/ ./boot/ -name "*.c"`
AS_SOURCES=`find ./core/ ./drivers/ ./net/ ./fs/ ./kernel/ ./sys/ ./userspace/ ./boot/ -name "*.asm"`

CFLAGS='-m32 -c -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -I include'
ASFLAGS='-f elf'

mkdir -p $BUILDDIR/iso/boot/grub

for c_file in $C_SOURCES
do
    mkdir -p $(dirname $BUILDDIR/$c_file)

    $CC $CFLAGS $c_file -o $BUILDDIR/$c_file.o
done

for s_file in $AS_SOURCES
do
    mkdir -p $(dirname $BUILDDIR/$s_file)

    $AS $ASFLAGS $s_file -o $BUILDDIR/$s_file.o
done

LINKS=""

for l_file in $(find build -name "*.o")
do
    LINKS+=" "
    LINKS+=$l_file
    LINKS+=" "
done

ld -m elf_i386 -Tconfig/linker.ld -o $BUILDDIR/iso/boot/kernel.elf $LINKS

cp -f config/grub.cfg $BUILDDIR/iso/boot/grub

grub-mkrescue -o build/quantumos.iso $BUILDDIR/iso

qemu-system-x86_64 -cdrom ./build/quantumos.iso -drive file=ext2.img,format=raw -m 3G -vga vmware -serial stdio -device sb16 