section .data
    str: db "%c", 0

section .text
    extern dprintf
    global _start

_start:
    nop
    nop
    push rsp
    push rax
    push rdi
    push rsi
    push rdx
    push rcx
    lea rdi, [rel $ - 0x136]
    mov rax, 0xfffffffff000
    and rdi, rax 
    mov rsi, 0x1000
    mov rdx, 7
    mov rax, 10
    syscall

    mov rdi, 1
    lea rsi, [rel $ + 0x38]
    mov rdx, 13
    mov rax, 1
    syscall

    lea rdi, [rel $ - 0x136] ; offset encrypted symbol
    lea rsi, [rel $ - 0x30] ; debut de key
    mov rcx, 0x1234 ; sera change par le code c
    cld
    my_loop:
        lodsb
        xor al, [rdi]
        stosb
        loop my_loop
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    pop rax
    pop rsp