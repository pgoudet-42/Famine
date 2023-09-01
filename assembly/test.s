section .text
    global _start

_handleFiles:
    sub rsp, 0x1000
    mov rdi, rax        ; Descripteur de fichier (retourné par l'appel système 'open')
    mov rax, 78h        ; Numéro de l'appel système pour 'getdents'
    mov rsi, rsp         ; Pointeur vers le tampon
    mov rdx, 0x1000       ; Taille du tampon
    syscall
    jmp _fini

_start:
    push rsp
    push rax
    push rdi
    push rsi
    push rdx
    push rcx
    push rbp


    mov rax, 1
    mov rdi, 1
    lea rsi, [rel $ - 27]
    mov rdx, 5
    syscall

    mov rax, 2
    lea rdi, [rel $ - 46]
    mov rsi, 2
    syscall
    test rax, rax
    je _handleFiles

_fini:
    pop rbp
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    pop rax
    pop rsp
