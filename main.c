#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

unsigned char	ft_strstr(const char *str, const char *need, unsigned long int len)
{
	unsigned long int i;
	unsigned long int j;

	i = 0;
	while (i < len) {
		j = 0;
		while ((i + j) < len && (str[i + j] == need[j])) {
			if (need[j + 1] == 0)
				return (i);
			j++;
		}
		i++;
	}
	return (0);
}


int main(void) {
    int fd = 0;
    int bytes_rd = 1;
    char buff[0x1000];
    int res = 0;

    fd = open("/tmp/test/hello", O_RDONLY);
    while ( bytes_rd > 0 ) {
        bytes_rd  = read(fd, buff, 0x1000);
        if (bytes_rd > 0) {
            res = ft_strstr("Famine version 1.0 (c)oded by <pgoudet>-<Mastermind pgoudet>", buff, bytes_rd);
            if (res > 1)
                printf("found at %d\n", res);
            memset(buff, 0, 0x1000);
        }

    }
    return (0);
}