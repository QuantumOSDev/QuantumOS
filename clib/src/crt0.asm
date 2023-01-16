section .text
    global _start

extern main

_start:
    ; TODO: get the arguments from the stack etc.
    call main

    ; Exit syscall
    mov ebx, eax ; Move value returned from main to ebx
    mov eax, 1
    int 0x80
