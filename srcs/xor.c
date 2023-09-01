#include "../include/famine.h"

unsigned char *generate_random_key(size_t len) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789,.-#'?!";
    unsigned char *str = NULL;

    str = malloc(len);
    if (!str)
        return (NULL);
    if (len) {
        --len;
        for (size_t n = 0; n < len; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = (unsigned char)(charset[key]);
        }
        str[len] = '\0';
    }
    return str;
}

void    apply_xor(unsigned char *buf, size_t len, char *key) {
    size_t i = 0;
    
    while(i < len) {
        buf[i] = buf[i] ^ key[i % len];
        i++;
    }
}


