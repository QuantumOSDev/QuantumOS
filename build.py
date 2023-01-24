CFLAGS='-m32 -c -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -I include '
ASFLAGS='-f elf '

csources = []
ssources = []

import fnmatch
import os

for root, dirnames, filenames in os.walk('.'):
    for filename in fnmatch.filter(filenames, '*.c'):
        if "apps" not in os.path.join(root, filename) and "clib" not in os.path.join(root, filename):
            csources.append(os.path.join(root, filename))

for root, dirnames, filenames in os.walk('.'):
    for filename in fnmatch.filter(filenames, '*.asm'):
        if "apps" not in os.path.join(root, filename) and "clib" not in os.path.join(root, filename):
            ssources.append(os.path.join(root, filename))

for c in csources:
    print("Compiling: [" + c + "]")

    os.system("gcc " + CFLAGS + c + " -o build/" + c + ".o")

for s in ssources:
    print("Compiling: [" + c + "]")

    os.system("nasm " + ASFLAGS + s + " -o build/" + s + ".o")

total = ""

for c in csources:
    total += "build/"
    total += c
    total += ".o "

for s in ssources:
    total += "build/"
    total += s
    total += ".o "

print("\nLinking kernel...\n")

os.system("ld -m elf_i386 -Tconfig/linker.ld -o build/iso/boot/kernel.elf " + total)
os.system("cp config/grub.cfg build/iso/boot/grub/")

print("Done!\nRunning kernel...")

os.system("qemu-system-x86_64 -cdrom ./build/quantumos.iso -drive file=ext2.img,format=raw -m 3G -vga vmware -serial stdio -device ac97 ")