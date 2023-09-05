#!/usr/bin/zsh
# gcc -Wall -Wextra -Werror srcs/*.c -L$(pwd)/libft -lft  -L$(pwd)/libelf -lft_elf -o packer
gcc -masm=intel -Wno-int-to-pointer-cast virus/*.c -nostartfiles  -o test
# cp -f /usr/bin/ls tmp/