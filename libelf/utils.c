#include "libftelf.h"

int open_file(char *path) {
    int fd = -1;
    
    fd = open(path, O_RDWR);
    return (fd);
}

void    sort_tab(int **tab, size_t size) {
    int tmp = 0;

    for (size_t j = 0; j < size; j++) {
        for (size_t i = 0; i < size; i++) {
            if (i < size - 1) {
                if ((*tab)[i] > (*tab)[i + 1]) {
                    tmp = (*tab)[i];
                    (*tab)[i] = (*tab)[i + 1];
                    (*tab)[i + 1] = tmp;
                }
            }
        }
    }
}

void *ft_mmap(size_t lengthint, int prot, int flags, int fd, off_t offset) {
    unsigned char *buf = NULL;

    buf = mmap(NULL, lengthint, prot, flags, fd, offset);
    return (buf);
}

int get_fd(int argc, char **argv, struct stat **buf) {
    int fd;

    if (argc == 1)
        argv[1] = "./a.out";
    fd = open_file(argv[1]);
    if (fd == -1) {
        perror("Error open:");
        return (-1);
    }
    *buf = malloc(sizeof(struct stat));
    if (*buf == NULL)
        return(-1);
    if (get_file_info(fd, *buf) == -1) {
        perror("Error fstat:");
        return (-1);
    }
    return (fd);
}

int memncat(void *src, size_t index, void *dst, size_t n) {
    size_t i = -1;
    
    while (++i < n)
        ((unsigned char *)src)[index + i] = ((unsigned char *)dst)[i];
    return (0);
}

int check_file(unsigned char *buf, size_t size) {
    struct ELFheaders64 file_header;
    unsigned int ref_bytes = 1179403647;

    if(size < 0x40)
        return (1);
    file_header = get_elfHeader64_little_endian(buf);
    if (file_header.ei_mag0 != ref_bytes) {
        write(1, "Magic bytes aren't good\n", strlen("Magic bytes aren't good\n"));
        return (1);
    }
    else if (file_header.ei_class != 2) {
        write(1, "Must be a 64 bytes arch file\n", strlen("Must be a 64 bytes arch file\n"));
        return (1);
    }
    else if (file_header.ei_data != 1) {
        write(1, "Must be in little endian\n", strlen("Must be in little endian\n"));
        return (1);
    }
    else if (file_header.ei_osabi != 0) {
        write(1, "Not the good arch. Must be build for SysV\n", strlen("Not the good arch. Must be build for SysV\n"));
        return (1);
    }
    return (0);
}

size_t find_opcode(unsigned char *buf, size_t size, unsigned char *opcodes, size_t size_opcodes, int num) {
    size_t j = 0;
    size_t positions[100];
    int k = 0;

    for (size_t i = 0; i < size; i++) {
        if (buf[i] == opcodes[0]) {
            while (j < size_opcodes && buf[i + j] == opcodes[j]) {
                j++;
            }
            if (j == size_opcodes) {
                positions[k++] = i;
                i = i + j;
                j = 0;
            }
        }
    }
    return (positions[num]);
}

void additionSurOctets(unsigned char *buffer, size_t taille, unsigned int nombre) {
    unsigned int retenue = nombre;
    
    for (size_t i = 0; i < taille; ++i) {
        unsigned int somme = buffer[i] + retenue;
        retenue = somme >> 8;
        buffer[i] = (unsigned char)somme;
    }
}

int is_in_luint_table(long unsigned int wanted, long unsigned int *table, size_t size) {
    for (size_t i = 0; i < size; i++)
        if (table[i] == wanted)
            return (1);
    return (0);
}