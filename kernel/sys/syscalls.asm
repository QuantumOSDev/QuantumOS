global test_syscall

hello db "Hello", 0

test_syscall:
    mov eax, 4
    mov ebx, 1
    mov ecx, hello
    mov edx, 5
    int 0x80
    ret