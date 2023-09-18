#include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include "../libelf/libftelf.h"

#ifndef VIRUS_H
#define VIRUS_H
#define READ (void *)0
#define WRITE (void *)1
#define OPEN (void *)2
#define CLOSE (void *)3
#define STAT (void *)4
#define FSTAT (void *)5
#define LSEEK (void *)8
#define MMAP (void *)9
#define MUNMMAP (void *)11
#define MPROTECT (void *)10
#define EXIT (void *)60
#define FTRUNCATE (void *)77
#define GETDENTS (void *)78
#define OPENAT (void *)257

#define	__S_IFMT	0170000
#define	__S_IFDIR	0040000
#define	__S_ISTYPE(mode, mask)	(((mode) & __S_IFMT) == (mask))

#define	S_ISDIR(mode)	 __S_ISTYPE((mode), __S_IFDIR)

#define PF_X 0x1
#define PF_W 2
#define PF_R 4
#define SHF_EXECINSTR 0x4

#define SHT_DYNAMIC 6
#define SHT_NOBITS 8
#define SHT_PROGBITS 1	
#define SHF_ALLOC 2
#define SHF_WRITE 1

#define PT_LOAD 1


struct ELFheaders64 {
    unsigned int ei_mag0;
    unsigned char ei_class;
    unsigned char ei_data;
    unsigned char ei_version;
    unsigned char ei_osabi;
    unsigned char ei_abiversion;
    unsigned char ei_pad[7];
    unsigned short int e_type;
    unsigned short int e_machine;
    unsigned int e_version;
    unsigned long int e_entry;
    unsigned long int e_phoff;
    unsigned long int e_shoff;
    unsigned int e_flags;
    unsigned short int e_ehsize;
    unsigned short int e_phentsize;
    unsigned short int e_phnum;
    unsigned short int e_shentsize;
    unsigned short int e_shnum;
    unsigned short int e_shstrndx;
};

struct pheaders64 {
    unsigned int p_type;
    unsigned int p_flags;
    unsigned long int p_offset;
    unsigned long int p_vaddr;
    unsigned long int p_paddr;
    unsigned long int p_filesz;
    unsigned long int p_memsz;
    unsigned long int p_align;
};

struct sheaders64 {
    unsigned int sh_name;
    unsigned int sh_type;
    unsigned long int sh_flags;
    unsigned long int sh_addr;
    unsigned long int sh_offset;
    unsigned long int sh_size;
    unsigned int sh_link;
    unsigned int sh_info;
    unsigned long int sh_addralign;
    unsigned long int sh_entsize;
};

struct stat {
    unsigned long int   st_dev;
    unsigned long int   st_ino;
    unsigned int    st_mode;
    unsigned long  int  st_nlink;
    unsigned int    st_uid;
    unsigned int    st_gid;
    unsigned long int   st_rdev;
    long int        st_size;
    unsigned char osef[92];
};

struct linux_dirent64 {
    unsigned long  d_ino;
    unsigned long  d_off;
    unsigned short d_reclen;
    char           d_name[];
};

unsigned long int ft_syscall(void *arg1, void *arg2, void *arg3, void *arg4, void *arg5);
int ft_strcat(char *dst, char *src);
unsigned long int	ft_strlen(const char *str);
int	ft_strcmp(const char *s1, const char *s2);
int memncat(void *src, unsigned long int index, void *dst, unsigned long int n);
void	*ft_memset(void *s, int c, unsigned long int n);
int check_file(struct ELFheaders64 fHdr);
int checkSignature(int fd, const char *signature);
unsigned char isDir(char *path);
int find_offset_nentry_oentry(unsigned long int oep, unsigned long int nep);
int get_section_index(int flags, unsigned int type, struct ELFheaders64 fHdr, struct sheaders64 *sHdrs);

unsigned long int    writeFile(int fd, struct ELFheaders64 fHdr);

void    findLastExecSection(struct sheaders64 *sHdrs, struct ELFheaders64 fHdr, int *last_sHdr_exec);
int    findLastLoadSection(struct sheaders64 *sHdrs, struct ELFheaders64 fHdr, struct pheaders64 pHdr);
int get_last_load_segment(struct pheaders64 *pHdrs, struct ELFheaders64 fHdr);
unsigned char increaseFileSize(int fd, struct ELFheaders64 fHdr, struct sheaders64 *sHdrs);

int _start();

#endif