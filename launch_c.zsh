#!/usr/bin/zsh

if [ $# -eq 1 ] && [ "$1" = "--debug" ]
then
    echo "debug compilation"
    gcc -masm=intel -Wno-int-to-pointer-cast -nostartfiles virus/*.c -Os -o Famine
else
    echo "normal compilation"
    gcc -masm=intel -Wno-int-to-pointer-cast -nostartfiles -nostdlib -fno-builtin virus/*.c -Os -o Famine
fi
