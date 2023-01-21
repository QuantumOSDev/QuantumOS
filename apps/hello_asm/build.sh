CC="gcc"
ASM="nasm"
LD="ld"

$ASM -f elf32 main.asm -o main.o
$LD -m elf_i386 main.o -o hello_asm
