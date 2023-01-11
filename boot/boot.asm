; Now we are running in 32-bits

[bits 32]
; Setup multiboot variables
MULTIBOOT_HEADER equ 0x1BADB002
MULTIBOOT_FLAGS  equ 0x04
MULTIBOOT_CHECK  equ -(MULTIBOOT_HEADER + MULTIBOOT_FLAGS)

BOOTLOADER_MAGIC equ 0x2BADB002

section .multiboot
    align 4
    
    dd MULTIBOOT_HEADER
    dd MULTIBOOT_FLAGS
    dd MULTIBOOT_CHECK
    
    align 4
    dd 0
    dd 0
    dd 0
    dd 0
    dd 0
    dd 0
    dd 1280 ; width
    dd 720  ; height
    dd 32   ; pitch

section .data
    align 4096

section .initial_stack, nobits
    align 4

__kernel_stack_bottom:
    resb 104856
__kernel_stack_top:

; Align and allocate the MULTIBOOT header
section .text
    global MULTIBOOT_HEADER
    global BOOTLOADER_MAGIC

; Linker (_start) entry point
global _start

; Extern kernel initialization function
extern quantum_kernel_init

; Definition of _start function
_start:
    cli                            ; Disable interrupts

    mov esp, __kernel_stack_top    ; Allocate stack space
    mov eax, BOOTLOADER_MAGIC      ; Set eax to bootloader magic

    push ebx                       ; Pass bootloader magic
    push eax                       ; Pass multiboot structure pointer

    call quantum_kernel_init       ; Call c function 
    hlt                            ; Halt the CPU
