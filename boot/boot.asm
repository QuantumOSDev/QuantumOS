; Setup multiboot variables

__MULTIBOOT_HEADER equ 0x1BADB002
__MULTIBOOT_FLAGS  equ 0x00
__MULTIBOOT_CHECK  equ -(__MULTIBOOT_HEADER + __MULTIBOOT_FLAGS)

; We run in 32-bit mode for now

[bits 32]

; Align and allocate the MULTIBOOT header

section .text
    align 4

    dd __MULTIBOOT_HEADER
    dd __MULTIBOOT_FLAGS
    dd __MULTIBOOT_CHECK

; Linker (_start) entry point

global _start

; Extern kernel initialization function

extern __quantum_kernel_init

; Definition of _start function

_start:
    ; Disable interrupts

    cli

    ; Allocate stack space

    mov esp, __quantum_stack_pointer

    ; Call kernel initialization

    call __quantum_kernel_init

    ; If the kernel returns control to here we loop forever

    jmp $

    ; Halt the CPU

    hlt

; Reserve (1MB for uninitialized stack space)

section .bss
    resb 104856

    __quantum_stack_pointer:
