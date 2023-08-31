#include "libft.h"

void ft_error_exit(int error, char *mess) {
    write(2, mess, ft_strlen(mess));
    exit(error);
}