; Magic header
FLAGS       equ  0x4
MAGIC_HEADER       equ  0x1BADB002
CHECKSUM    equ -(MAGIC_HEADER + FLAGS)

; Bootloader magic number
BOOTLOADER_MAGIC  equ  0x2BADB002

; Section .multiboot
section .multiboot
    align 4
    dd MAGIC_HEADER
    dd FLAGS
    dd CHECKSUM
    dd 0
    dd 0
    dd 0
    dd 0
    dd 0
    dd 0
    dd 1280
    dd 720
    dd 32

section .data
    align 4096

section .initial_stack, nobits
    align 4

stack_bottom:
    resb 104856
stack_top:

section .text
    global _start
    global MAGIC_HEADER
    global BOOTLOADER_MAGIC

_start:
    extern quantum_kernel_init ; Extern c function 
    
    mov esp, stack_top
    mov eax, BOOTLOADER_MAGIC
        
    push ebx
    push eax
    
    call quantum_kernel_init
    jmp $