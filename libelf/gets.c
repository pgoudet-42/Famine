#include "libftelf.h"

int get_file_info(int fd, struct stat *buf) {
    int res = 0;

    res = fstat(fd, buf);
    return (res);
}

char *get_dynsym_name_64(unsigned char *buf, struct ELFheaders64 elfHeader ,struct sheaders64 *sheaders, unsigned int name) {
    char *str = NULL;
    size_t offset = 0;
    size_t i, j;

    i = get_section_index(".dynstr", buf, sheaders, elfHeader);

    offset = sheaders[i].sh_offset + name;
    j = 0;
    while (j <= sheaders[i].sh_size) {
        if (buf[offset + j] == 0 && j > 0)
            break;
        j++;
    }
    if (sheaders[i].sh_size <= j)
        return (NULL);
    str = malloc(j + 1);
    strncpy(str, (char *)&(buf[offset]), j + 1);
    return (str);
}

char *get_sym_name_64(unsigned char *buf, struct ELFheaders64 elfHeader ,struct sheaders64 *sheaders, unsigned int name) {
    char *str = NULL;
    size_t offset = 0;
    size_t i, j;

    i = elfHeader.e_shstrndx - 1;
    if (sheaders[i].sh_type != 0x03)
        return (NULL);

    offset = sheaders[i].sh_offset + name;
    j = 0;
    while (j <= sheaders[i].sh_size) {
        if (buf[offset + j] == 0 && j > 0)
            break;
        j++;
    }
    if (sheaders[i].sh_size <= j)
        return (NULL);
    str = malloc(j + 1);
    strncpy(str, (char *)&(buf[offset]), j + 1);
    return (str);
}

char *get_sym_name_from_offset(unsigned char *buf, size_t offset, struct sheaders64 *sheaders, struct ELFheaders64 elfheader) {
    struct Elf64_Sym *syms;
    size_t nb;
    size_t value = 0xFFFFFFFFFFFFFFFF;
    int index = -1;
    char *name;

    syms = get_syms_64(elfheader, sheaders, buf, &nb);
    for (size_t i = 0; i < nb; i++) {
        if (offset > syms[i].st_value + syms[i].st_size) {
            if (offset - syms[i].st_value + syms[i].st_size < value) {
                value = offset - syms[i].st_value + syms[i].st_size;
                index = (int)i;
            }
        }

    }
    if (index == -1)
        return (NULL);
    name = get_sym_name_64(buf, elfheader, sheaders, syms[index].st_name);
    free(syms);
    return (name);
}


char *get_section_name_64(unsigned char *buf, struct ELFheaders64 elfHeader ,struct sheaders64 *sheaders, unsigned int name) {
    char *str = NULL;
    size_t offset = 0;
    size_t i, j;

    if (name == 0)
        return (NULL);
    i = elfHeader.e_shstrndx;
    if (sheaders[i].sh_type != 0x03)
        return (NULL);

    offset = sheaders[i].sh_offset + name;
    j = 0;
    while (j <= sheaders[i].sh_size) {
        if (buf[offset + j] == 0 && j > 0)
            break;
        j++;
    }
    if (sheaders[i].sh_size <= j)
        return (NULL);
    str = malloc(j + 1);
    strncpy(str, (char *)&(buf[offset]), j + 1);
    return (str);
}

struct sheaders64 *get_section_headers_64(unsigned char *buf, struct ELFheaders64 elfHeader) {
    struct sheaders64 *sheaders;

    sheaders = malloc(sizeof(struct sheaders64) * elfHeader.e_shnum);
    if (!sheaders)
        return (NULL);
    
    for (int i = 0; i < elfHeader.e_shnum; i++)
        sheaders[i] = get_sHeader64_little_endian(buf, &elfHeader, i);
    return (sheaders);
}

struct pheaders64 *get_program_headers_64( unsigned char *buf, struct ELFheaders64 elfHeader) {
    struct pheaders64 *pheaders;

    pheaders = malloc(sizeof(struct pheaders64) * elfHeader.e_phnum);
    if (!pheaders)
        return (NULL);
    
    for (int i = 0; i < elfHeader.e_phnum; i++)
        pheaders[i] = get_pHeader64_little_endian(buf, &elfHeader, i);
    return (pheaders);
}

struct Elf64_Sym *get_syms_64(struct ELFheaders64 elfHeader, struct sheaders64 *sheaders, unsigned char *buf, size_t *size) {
    struct Elf64_Sym *syms;

    for (int i = 0; i < elfHeader.e_shnum; i++) {
        if (sheaders[i].sh_type == 0x02) {
            *size = sheaders[i].sh_size / sheaders[i].sh_entsize;
            syms = malloc(sizeof(struct Elf64_Sym) * (*size));
            if (!syms)
                return (NULL);
            break;
        }
    }
    if (*size == 0)
      return (NULL);
    for (size_t i = 0; i < (*size); i++) 
        syms[i] = get_sym64_section_little(buf, elfHeader, sheaders, i);
    return (syms);
}

struct sym_name *get_sys_tab_64(struct ELFheaders64 elfHeader, struct sheaders64 *sheaders, unsigned char *buf, int *j) {
  struct Elf64_Sym *syms;
  size_t size = 0;
  struct sym_name *sym_tab = NULL;

  syms = get_syms_64(elfHeader, sheaders, buf, &size);
  if (!syms)
    return (NULL);
  sym_tab = malloc(sizeof(struct sym_name) * size);
  if (!sym_tab)
    return (NULL);
  for (size_t i = 0; i < size; i++) {
    if ((syms[i].st_name != 0 || i == 0)) {
      sym_tab[*j].name = get_section_name_64(buf, elfHeader, sheaders, syms[i].st_name);
      sym_tab[*j].sym.st_info = syms[i].st_info;
      sym_tab[*j].sym.st_name = syms[i].st_name;
      sym_tab[*j].sym.st_other = syms[i].st_other;
      sym_tab[*j].sym.st_shndx = syms[i].st_shndx;
      sym_tab[*j].sym.st_size = syms[i].st_size;
      sym_tab[*j].sym.st_value = syms[i].st_value;
      if (sym_tab[*j].name == NULL)
        (*j)--;
      (*j)++;
    }
  }
  free(syms);
  return (sym_tab);
}

size_t get_section_header_offset(int index, struct ELFheaders64 file_header) {
    size_t offset = 0;

    offset = file_header.e_shoff + file_header.e_shentsize * index;
    return (offset);
}

size_t get_program_header_offset(int index, struct ELFheaders64 file_header) {
    size_t offset = 0;

    offset = file_header.e_phoff + file_header.e_phentsize * index;
    return (offset);
}

int get_section_index(char *section, unsigned char *buf, struct sheaders64 *sheaders, struct ELFheaders64 file_header) {
    int i = 0, index = -1;
    char *name = NULL;

    while(i < file_header.e_shnum) {
        name = get_section_name_64(buf, file_header, sheaders, sheaders[i].sh_name);
        if (name && strcmp(name, section) == 0) {
            index = i;
        }
        free(name);
        i++;
    }
    return (index);
}

size_t get_symbol_offset(unsigned char *buf, char *sym, struct ELFheaders64 elfHeader, struct sheaders64 *sheaders) {
    struct Elf64_Sym *syms;
    size_t size = 0;
    size_t offset = -1;
    char *name;


    syms = get_syms_64(elfHeader, sheaders, buf, &size);
    for (size_t i = 0; i < size; i++) {
      if ((syms[i].st_name != 0 || i == 0)) {
        name = get_sym_name_64(buf, elfHeader, sheaders, syms[i].st_name);
        if (name && strcmp(name, sym) == 0) {
            offset = syms[i].st_value;
            break;
        }
        free(name);
      }
    }
    return (offset);
}

struct Elf64_Sym get_sym_by_name(char *name, unsigned char *buf, struct sheaders64 *sheaders, struct ELFheaders64 elfheader) {
    struct Elf64_Sym *syms;
    size_t size;
    char *sym_name;
    struct Elf64_Sym sym;

    memset(&sym, 0, sizeof(struct Elf64_Sym));
    syms = get_syms_64(elfheader, sheaders, buf, &size);
    for (size_t i = 0; i < size; i++ ) {
        sym_name = get_sym_name_64(buf, elfheader, sheaders, syms[i].st_name);
        if (sym_name && strcmp(sym_name, name) == 0) {
            sym = syms[i];
            free(syms);
            free(sym_name);
            return (sym);
        }
        free(sym_name);
    }
    free(syms);
    return (sym);
}

int get_rel_as_count(unsigned char *buf, char *section_name, struct sheaders64 *sheaders, struct ELFheaders64 elfHeader, unsigned long size_of_struct) {
    int nb = 0;

    for (int i = 0; i < elfHeader.e_shnum; i++) {
        char *name = get_section_name_64(buf, elfHeader, sheaders, sheaders[i].sh_name);
        if (name && strcmp(name, section_name) == 0)
            nb = sheaders[i].sh_size / size_of_struct;
    }
    return (nb);
}


size_t get_rel_a_offset(struct sheaders64 sheader, int i, unsigned long size_of_struct) {
    size_t offset;

    offset = sheader.sh_offset + i * size_of_struct;
    return (offset);
}

Elf64_Rel *get_rels(unsigned char *buf, struct sheaders64 sheader) {
    Elf64_Rel *rel;
    int nb;

    nb = sheader.sh_size / sizeof(Elf64_Rel);
    rel = malloc(sheader.sh_size);
    if (!rel) {
        perror("Error get_relas:");
        exit(1);
    }
    for (int i = 0; i < nb; i++)
        rel[i] = get_rel64_little_endian(buf, sheader, 0);

    return (rel);
}

Elf64_Rela *get_relas(unsigned char *buf, struct sheaders64 sheader) {
    Elf64_Rela *rela;
    int nb;

    nb = sheader.sh_size / sizeof(Elf64_Rela);
    rela = malloc(sheader.sh_size);
    if (!rela) {
        perror("Error get_relas:");
        exit(1);
    }
    for (int i = 0; i < nb; i++)
        rela[i] = get_rela64_little_endian(buf, sheader, i);

    return (rela);
}