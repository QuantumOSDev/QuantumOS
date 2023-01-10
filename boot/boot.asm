; Now we are running in 32-bits

[bits 32]
; Setup multiboot variables
MULTIBOOT_HEADER equ 0x1BADB002
MULTIBOOT_FLAGS  equ 0x04
MULTIBOOT_CHECK  equ -(MULTIBOOT_HEADER + MULTIBOOT_FLAGS)

; Align and allocate the MULTIBOOT header
section .text
    align 4

    dd MULTIBOOT_HEADER
    dd MULTIBOOT_FLAGS
    dd MULTIBOOT_CHECK
    dd 0
    dd 0
    dd 0
    dd 0
    dd 0
    dd 0
    dd 1280 ; width
    dd 720  ; height
    dd 32   ; pitch

; Linker (_start) entry point
global _start

; Extern kernel initialization function
extern quantum_kernel_init

; Definition of _start function
_start:
    cli                            ; Disable interrupts

    mov esp, quantum_stack_pointer ; Allocate stack space
    mov eax, 0x2BADB002            ; Set eax to bootloader magic

    push eax                       ; Pass bootloader magic
    push ebx                       ; Pass multiboot structure pointer

    call quantum_kernel_init       ; Call c function 
    hlt                            ; Halt the CPU

section .bss
    resb 104856
    quantum_stack_pointer:
