#!/usr/bin/zsh
# gcc -Wall -Wextra -Werror srcs/*.c -L$(pwd)/libft -lft  -L$(pwd)/libelf -lft_elf -o packer
if [ $1 = "debug" ]
then
    gcc -masm=intel -Wno-int-to-pointer-cast -nostartfiles -fno-builtin virus/*.c -Os -o Famine
else
    gcc -masm=intel -Wno-int-to-pointer-cast -nostartfiles -nostdlib -fno-builtin virus/*.c -Os -o Famine
fi
cp /usr/bin/ls tmp/ls
cp /usr/bin/test tmp/test
cp /usr/bin/date tmp/date
cp hello tmp/hello
