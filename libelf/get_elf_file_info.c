#include "libftelf.h"


size_t calcul_file_size(size_t code_size, unsigned char *buf, struct sheaders64 *sheaders, struct ELFheaders64 elfHeader) {
    size_t final_size;
    size_t size = 0;
    struct Elf64_Sym *syms;

    syms = get_syms_64(elfHeader, sheaders, buf, &size);

    for (int i = 0; i < elfHeader.e_shnum; i++) {
        if (sheaders[i].sh_offset > size)
            final_size = sheaders[i].sh_offset + sheaders[i].sh_size;
    }
    for (size_t i = 0; i < size; i++) {
        if (syms[i].st_value > final_size)
            final_size = syms[i].st_value + syms[i].st_size;
    }
    final_size += code_size;
    return (final_size);
}

unsigned long int find_p_align(struct pheaders64 ph) {
    unsigned long int alignement, offset, vadrr;

    if (ph.p_offset == 0 || ph.p_vaddr == 0)
        return (1);
    alignement = 1;
    offset = ph.p_offset % alignement;
    vadrr = ph.p_vaddr % alignement;
    while (offset == vadrr && alignement < 0x1000) {
        alignement = alignement * 2;
        offset = ph.p_offset % alignement;
        vadrr = ph.p_vaddr % alignement;
    }
    return (alignement);
}