global test_syscall

hello db "Hello", 10
num resb 5

test_syscall:
    mov eax, 4
    mov ebx, 1
    mov ecx, hello
    mov edx, 6
    int 0x80

    mov eax, 3
    mov ebx, 1
    mov ecx, num  
    mov edx, 5        
    int 0x80
        
    mov eax, 4
    mov ebx, 1
    mov ecx, hello
    mov edx, 5
    int 0x80  