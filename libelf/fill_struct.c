#include "libftelf.h"

struct ELFheaders64 get_elfHeader64_little_endian(unsigned char *buf) {
    struct ELFheaders64 elfHeader;

    elfHeader.ei_mag0 = ((unsigned char)buf[0x0]) + ((unsigned char)buf[0x1] << 8) + ((unsigned char)buf[0x2] << 16) + ((unsigned char)buf[0x3] << 24);
    elfHeader.ei_class = buf[0x04];
    elfHeader.ei_data = buf[0x05];
    elfHeader.ei_version = buf[0x06];
    elfHeader.ei_osabi = buf[0x07];
    elfHeader.ei_abiversion = buf[0x08];
    elfHeader.ei_pad[0] = buf[0x09];
    elfHeader.ei_pad[1] = buf[0x0a];
    elfHeader.ei_pad[2] = buf[0x0b];
    elfHeader.ei_pad[3] = buf[0x0c];
    elfHeader.ei_pad[4] = buf[0x0d];
    elfHeader.ei_pad[5] = buf[0x0e];
    elfHeader.ei_pad[6] = buf[0x0f];
    elfHeader.e_type = ((unsigned char)buf[0x10]) + ((unsigned char)buf[0x11] << 8);
    elfHeader.e_machine = ((unsigned char)buf[0x12]) + ((unsigned char)buf[0x13] << 8);
    elfHeader.e_version = ((unsigned char)buf[0x14]) + ((unsigned char)buf[0x15] << 8) + ((unsigned char)buf[0x16] << 16) + ((unsigned char)buf[0x17] << 24);
    elfHeader.e_entry = ((unsigned char)buf[0x18]) + ((unsigned char)buf[0x19] << 8) + ((unsigned char)buf[0x1a] << 16) + ((unsigned char)buf[0x1b] << 24) + ((unsigned long)buf[0x1c] << 32) + ((unsigned long)buf[0x1d] << 40) + ((unsigned long)buf[0x1e] << 48) + ((unsigned long)buf[0x1f] << 56);
    elfHeader.e_phoff = ((unsigned char)buf[0x20]) + ((unsigned char)buf[0x21] << 8) + ((unsigned char)buf[0x22] << 16) + ((unsigned char)buf[0x23] << 24) + ((unsigned long)buf[0x24] << 32) + ((unsigned long)buf[0x25] << 40) + ((unsigned long)buf[0x26] << 48) + ((unsigned long)buf[0x27] << 56);
    elfHeader.e_shoff = ((unsigned char)buf[0x28]) + ((unsigned char)buf[0x29] << 8) + ((unsigned char)buf[0x2a] << 16) + ((unsigned char)buf[0x2b] << 24) + ((unsigned long)buf[0x2c] << 32) + ((unsigned long)buf[0x2d] << 40) + ((unsigned long)buf[0x2e] << 48) + ((unsigned long)buf[0x2f] << 56);
    elfHeader.e_flags = ((unsigned char)buf[0x30]) + ((unsigned char)buf[0x31] << 8) + ((unsigned char)buf[0x32] << 16) + ((unsigned char)buf[0x33] << 24);
    elfHeader.e_ehsize = ((unsigned char)buf[0x34]) + ((unsigned char)buf[0x35] << 8);
    elfHeader.e_phentsize = ((unsigned char)buf[0x36]) + ((unsigned char)buf[0x37] << 8);
    elfHeader.e_phnum = ((unsigned char)buf[0x38]) + ((unsigned char)buf[0x39] << 8);
    elfHeader.e_shentsize = ((unsigned char)buf[0x3a]) + ((unsigned char)buf[0x3b] << 8);
    elfHeader.e_shnum = ((unsigned char)buf[0x3c]) + ((unsigned char)buf[0x3d] << 8);
    elfHeader.e_shstrndx = ((unsigned char)buf[0x3e]) + ((unsigned char)buf[0x3f] << 8);

    return (elfHeader);
}


struct pheaders64 get_pHeader64_little_endian(unsigned char *buf, struct ELFheaders64 *elfHeader, int nb) {
    struct pheaders64 pheader;
    size_t offset;

    offset = elfHeader->e_phoff + elfHeader->e_phentsize * nb;
    memset(&pheader, 0, sizeof(struct pheaders64));
    if (offset < 0x40)
        return (pheader);
    
    pheader.p_type = ((unsigned char)buf[offset + 0x00]) + ((unsigned char)buf[offset + 0x01] << 8) + ((unsigned char)buf[offset + 0x02] << 16) + ((unsigned char)buf[offset + 0x03] << 24);
    pheader.p_flags = ((unsigned char)buf[offset + 0x04]) + ((unsigned char)buf[offset + 0x05] << 8) + ((unsigned char)buf[offset + 0x06] << 16) + ((unsigned char)buf[offset + 0x07] << 24);
    pheader.p_offset = ((unsigned char)buf[offset + 0x08]) + ((unsigned char)buf[offset + 0x09] << 8) + ((unsigned char)buf[offset + 0x0a] << 16) + ((unsigned char)buf[offset + 0x0b] << 24) +\
    ((unsigned long)buf[offset + 0x0c] << 32) + ((unsigned long)buf[offset + 0x0d] << 40) + ((unsigned long)buf[offset + 0x0e] << 48) + ((unsigned long)buf[offset + 0x0f] << 56); 
    pheader.p_vaddr = ((unsigned char)buf[offset + 0x10]) + ((unsigned char)buf[offset + 0x11] << 8) + ((unsigned char)buf[offset + 0x12] << 16) + ((unsigned char)buf[offset + 0x13] << 24) +\
    ((unsigned long)buf[offset + 0x14] << 32) + ((unsigned long)buf[offset + 0x15] << 40) + ((unsigned long)buf[offset + 0x16] << 48) + ((unsigned long)buf[offset + 0x17] << 56); 
    pheader.p_paddr = ((unsigned char)buf[offset + 0x18]) + ((unsigned char)buf[offset + 0x19] << 8) + ((unsigned char)buf[offset + 0x1a] << 16) + ((unsigned char)buf[offset + 0x1b] << 24) +\
    ((unsigned long)buf[offset + 0x1c] << 32) + ((unsigned long)buf[offset + 0x1d] << 40) + ((unsigned long)buf[offset + 0x1e] << 48) + ((unsigned long)buf[offset + 0x1f] << 56); 
    pheader.p_filesz = ((unsigned char)buf[offset + 0x20]) + ((unsigned char)buf[offset + 0x21] << 8) + ((unsigned char)buf[offset + 0x22] << 16) + ((unsigned char)buf[offset + 0x23] << 24) +\
    ((unsigned long)buf[offset + 0x24] << 32) + ((unsigned long)buf[offset + 0x25] << 40) + ((unsigned long)buf[offset + 0x26] << 48) + ((unsigned long)buf[offset + 0x27] << 56); 
    pheader.p_memsz = ((unsigned char)buf[offset + 0x28]) + ((unsigned char)buf[offset + 0x29] << 8) + ((unsigned char)buf[offset + 0x2a] << 16) + ((unsigned char)buf[offset + 0x2b] << 24) +\
    ((unsigned long)buf[offset + 0x2c] << 32) + ((unsigned long)buf[offset + 0x2d] << 40) + ((unsigned long)buf[offset + 0x2e] << 48) + ((unsigned long)buf[offset + 0x2f] << 56); 
    pheader.p_align = ((unsigned char)buf[offset + 0x30]) + ((unsigned char)buf[offset + 0x31] << 8) + ((unsigned char)buf[offset + 0x32] << 16) + ((unsigned char)buf[offset + 0x33] << 24) +\
    ((unsigned long)buf[offset + 0x34] << 32) + ((unsigned long)buf[offset + 0x35] << 40) + ((unsigned long)buf[offset + 0x36] << 48) + ((unsigned long)buf[offset + 0x37] << 56); 

    return (pheader);
}

struct sheaders64 get_sHeader64_little_endian(unsigned char *buf, struct ELFheaders64 *elfHeader, int nb) {
    struct sheaders64 sheader;
    size_t offset;

    offset = elfHeader->e_shoff + elfHeader->e_shentsize * nb;
    memset(&sheader, 0, sizeof(struct sheaders64));
    if (offset < 0x40)
        return (sheader);


    sheader.sh_name = ((unsigned char)buf[offset + 0x00]) + ((unsigned char)buf[offset + 0x01] << 8) + ((unsigned char)buf[offset + 0x02] << 16) + ((unsigned char)buf[offset + 0x03] << 24);
    sheader.sh_type = ((unsigned char)buf[offset + 0x04]) + ((unsigned char)buf[offset + 0x05] << 8) + ((unsigned char)buf[offset + 0x06] << 16) + ((unsigned char)buf[offset + 0x07] << 24);
    sheader.sh_flags = ((unsigned char)buf[offset + 0x08]) + ((unsigned char)buf[offset + 0x09] << 8) + ((unsigned char)buf[offset + 0x0a] << 16) + ((unsigned char)buf[offset + 0x0b] << 24) +\
    ((unsigned long)buf[offset + 0x0c] << 32) + ((unsigned long)buf[offset + 0x0d] << 40) + ((unsigned long)buf[offset + 0x0e] << 48) + ((unsigned long)buf[offset + 0x0f] << 56);
    sheader.sh_addr = ((unsigned char)buf[offset + 0x10]) + ((unsigned char)buf[offset + 0x11] << 8) + ((unsigned char)buf[offset + 0x12] << 16) + ((unsigned char)buf[offset + 0x13] << 24) +\
    ((unsigned long)buf[offset + 0x14] << 32) + ((unsigned long)buf[offset + 0x15] << 40) + ((unsigned long)buf[offset + 0x16] << 48) + ((unsigned long)buf[offset + 0x17] << 56);
    sheader.sh_offset = ((unsigned char)buf[offset + 0x18]) + ((unsigned char)buf[offset + 0x19] << 8) + ((unsigned char)buf[offset + 0x1a] << 16) + ((unsigned char)buf[offset + 0x1b] << 24) +\
    ((unsigned long)buf[offset + 0x1c] << 32) + ((unsigned long)buf[offset + 0x1d] << 40) + ((unsigned long)buf[offset + 0x1e] << 48) + ((unsigned long)buf[offset + 0x1f] << 56); 
    sheader.sh_size = ((unsigned char)buf[offset + 0x20]) + ((unsigned char)buf[offset + 0x21] << 8) + ((unsigned char)buf[offset + 0x22] << 16) + ((unsigned char)buf[offset + 0x23] << 24) +\
    ((unsigned long)buf[offset + 0x24] << 32) + ((unsigned long)buf[offset + 0x25] << 40) + ((unsigned long)buf[offset + 0x26] << 48) + ((unsigned long)buf[offset + 0x27] << 56);
    sheader.sh_link = ((unsigned char)buf[offset + 0x28]) + ((unsigned char)buf[offset + 0x29] << 8) + ((unsigned char)buf[offset + 0x2a] << 16) + ((unsigned char)buf[offset + 0x2b] << 24);
    sheader.sh_info = ((unsigned char)buf[offset + 0x2c]) + ((unsigned char)buf[offset + 0x2d] << 8) + ((unsigned char)buf[offset + 0x2e] << 16) + ((unsigned char)buf[offset + 0x2f] << 24);
    sheader.sh_addralign = ((unsigned char)buf[offset + 0x30]) + ((unsigned char)buf[offset + 0x31] << 8) + ((unsigned char)buf[offset + 0x32] << 16) + ((unsigned char)buf[offset + 0x33] << 24) +\
    ((unsigned long)buf[offset + 0x34] << 32) + ((unsigned long)buf[offset + 0x35] << 40) + ((unsigned long)buf[offset + 0x36] << 48) + ((unsigned long)buf[offset + 0x37] << 56);
    sheader.sh_entsize = ((unsigned char)buf[offset + 0x38]) + ((unsigned char)buf[offset + 0x39] << 8) + ((unsigned char)buf[offset + 0x3a] << 16) + ((unsigned char)buf[offset + 0x3b] << 24) +\
    ((unsigned long)buf[offset + 0x3c] << 32) + ((unsigned long)buf[offset + 0x3d] << 40) + ((unsigned long)buf[offset + 0x3e] << 48) + ((unsigned long)buf[offset + 0x3f] << 56); 

    return (sheader);
}

struct Elf64_Sym fill_sym64_struct(size_t offset, unsigned char *buf) {
    struct Elf64_Sym sym;

    memset(&sym, 0, sizeof(struct Elf64_Sym));
    sym.st_name = ((unsigned char)buf[offset + 0x00]) + ((unsigned char)buf[offset + 0x01] << 8) + ((unsigned char)buf[offset + 0x02] << 16) + ((unsigned char)buf[offset + 0x03] << 24);
    sym.st_info = (unsigned char)buf[offset + 0x04];
    sym.st_other = (unsigned char)buf[offset + 0x05];
    sym.st_shndx = ((unsigned char)buf[offset + 0x06]) + ((unsigned char)buf[offset + 0x07] << 8);
    sym.st_value =  ((unsigned char)buf[offset + 0x08]) + ((unsigned char)buf[offset + 0x09] << 8) + ((unsigned char)buf[offset + 0x0a] << 16) + ((unsigned char)buf[offset + 0x0b] << 24) +\
    ((unsigned long)buf[offset + 0x0c] << 32) + ((unsigned long)buf[offset + 0x0d] << 40) + ((unsigned long)buf[offset + 0x0e] << 48) + ((unsigned long)buf[offset + 0x0f] << 56);
    sym.st_size = ((unsigned char)buf[offset + 0x10]) + ((unsigned char)buf[offset + 0x11] << 8) + ((unsigned char)buf[offset + 0x12] << 16) + ((unsigned char)buf[offset + 0x13] << 24) +\
    ((unsigned long)buf[offset + 0x14] << 32) + ((unsigned long)buf[offset + 0x15] << 40) + ((unsigned long)buf[offset + 0x16] << 48) + ((unsigned long)buf[offset + 0x17] << 56);
    return (sym);
}

struct Elf64_Sym get_sym64_section_little(unsigned char *buf, struct ELFheaders64 elfHeader, struct sheaders64 *sheaders, int i) {
    struct Elf64_Sym sym;
    size_t offset = 0;
    size_t begin = 0;

    for (int j = 0; j < elfHeader.e_shnum; j++) {
        if (sheaders[j].sh_type == 0x02) {
            begin = sheaders[j].sh_offset;
            break;
        }
    }
    offset = begin + sizeof(struct Elf64_Sym) * i;
    sym = fill_sym64_struct(offset, buf);
    return (sym);
}

Elf64_Rela    get_rela64_little_endian(unsigned char *buf, struct sheaders64 sheader, int i) {
    Elf64_Rela rela;
    size_t offset = 0;

    memset(&rela, 0, sizeof(Elf64_Rela));
    if (i > (int)(sheader.sh_size / sizeof(Elf64_Rela)))
        return (rela);
    offset = sheader.sh_offset + i * sizeof(Elf64_Rela);

    rela.r_offset = ((buf[offset + 0x00] & 0xFF)) + ((buf[offset + 0x01] & 0xFF) << 8) + ((buf[offset + 0x02] & 0xFF) << 16) + ((buf[offset + 0x03] & 0xFF) << 24) +\
    ((unsigned long)buf[offset + 0x04] << 32) + ((unsigned long)buf[offset + 0x05] << 40) + ((unsigned long)buf[offset + 0x06] << 48) + ((unsigned long)buf[offset + 0x07] << 56);
    rela.r_info = ((buf[offset + 0x08] & 0xFF)) + ((buf[offset + 0x09] & 0xFF) << 8) + ((buf[offset + 0x0a] & 0xFF) << 16) + ((buf[offset + 0x0b] & 0xFF) << 24) +\
    ((unsigned long)buf[offset + 0x0c] << 32) + ((unsigned long)buf[offset + 0x0d] << 40) + ((unsigned long)buf[offset + 0x0e] << 48) + ((unsigned long)buf[offset + 0x0f] << 56);
    rela.r_addend = ((buf[offset + 0x10] & 0xFF)) + ((buf[offset + 0x11] & 0xFF) << 8) + ((buf[offset + 0x12] & 0xFF) << 16) + ((buf[offset + 0x13] & 0xFF) << 24) +\
    ((unsigned long)buf[offset + 0x14] << 32) + ((unsigned long)buf[offset + 0x15] << 40) + ((unsigned long)buf[offset + 0x16] << 48) + ((unsigned long)buf[offset + 0x17] << 56);

    return (rela);
}

Elf64_Rel    get_rel64_little_endian(unsigned char *buf, struct sheaders64 sheader, int i) {
    Elf64_Rel rel;
    size_t offset = 0;

    memset(&rel, 0, sizeof(Elf64_Rel));
    if (i > (int)(sheader.sh_size / sizeof(Elf64_Rel)))
        return (rel);
    offset = sheader.sh_offset + i * sizeof(Elf64_Rel);
    rel.r_offset = ((buf[offset + 0x00] & 0xFF)) + ((buf[offset + 0x01] & 0xFF) << 8) + ((buf[offset + 0x02] & 0xFF) << 16) + ((buf[offset + 0x03] & 0xFF) << 24) +\
    ((unsigned long)buf[offset + 0x04] << 32) + ((unsigned long)buf[offset + 0x05] << 40) + ((unsigned long)buf[offset + 0x06] << 48) + ((unsigned long)buf[offset + 0x07] << 56);
    rel.r_info = ((buf[offset + 0x08] & 0xFF)) + ((buf[offset + 0x09] & 0xFF) << 8) + ((buf[offset + 0x0a] & 0xFF) << 16) + ((buf[offset + 0x0b] & 0xFF) << 24) +\
    ((unsigned long)buf[offset + 0x0c] << 32) + ((unsigned long)buf[offset + 0x0d] << 40) + ((unsigned long)buf[offset + 0x0e] << 48) + ((unsigned long)buf[offset + 0x0f] << 56);

    return (rel);
}

Elf64_Dyn fill_Elf64_Dyn_little(size_t offset, unsigned char *buf) {
     Elf64_Dyn dyn;

    dyn.d_tag = ((buf[offset + 0x00] & 0xFF)) | ((buf[offset + 0x01] & 0xFF) << 8) | ((buf[offset + 0x02] & 0xFF) << 16) | ((buf[offset + 0x03] & 0xFF) << 24) |\
    ((unsigned long)buf[offset + 0x04] << 32) | ((unsigned long)buf[offset + 0x05] << 40) | ((unsigned long)buf[offset + 0x06] << 48) | ((unsigned long)buf[offset + 0x07] << 56);
    dyn.d_un = ((buf[offset + 0x08] & 0xFF)) | ((buf[offset + 0x09] & 0xFF) << 8) | ((buf[offset + 0x0a] & 0xFF) << 16) | ((buf[offset + 0x0b] & 0xFF) << 24) |\
    ((unsigned long)buf[offset + 0x0c] << 32 ) | ((unsigned long)buf[offset + 0x0d] << 40) | ((unsigned long)buf[offset + 0x0e] << 48) | ((unsigned long)buf[offset + 0x0f] << 56);

    return (dyn);
}

Elf64_Dyn *get_Elf64_Dyns_little(unsigned char *buf, struct sheaders64 *sheaders, struct ELFheaders64 elfHeader) {
    int i;
    size_t offset;
    Elf64_Dyn *dyns;

    i = get_section_index(".dynamic", buf, sheaders, elfHeader);
    offset = sheaders[i].sh_offset;
    if (offset <= 0)
        return (NULL);
    dyns = malloc(sheaders[i].sh_size + sizeof(void *));
    if (!dyns) {
        perror("Error get_elf64_dyns_little: ");
        exit(1);
    }

    for(long unsigned int j = 0; j < sheaders[i].sh_size / sizeof(Elf64_Dyn); j++) {
        dyns[j] = fill_Elf64_Dyn_little(offset, buf);
        if (dyns[j].d_tag == 0x0) {
            break;
        }
        offset += sizeof(Elf64_Dyn);
    }
    return (dyns);
}