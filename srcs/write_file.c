#include "../include/famine.h"


int write_file(unsigned char *buf, size_t size) {
    int fd = open("packed_file", O_WRONLY | O_CREAT, S_IRWXU);

    if (fd == -1)
        return (-1);

    return (write(fd, buf, size));
}