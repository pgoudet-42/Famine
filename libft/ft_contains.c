#include "libft.h"

int ft_contains(char *str, char **tab) {
    int i;

    i = 0;
    if (!str || !tab)
        return (-1);
    while(tab[i]) {
        if (ft_strcmp(str, tab[i]) == 0) {
            return (i);
        }
        i++;
    }
    return (-1);
}