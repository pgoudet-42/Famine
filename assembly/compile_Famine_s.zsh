nasm -f elf64 Famine.s && gcc -masm=intel -Wno-int-to-pointer-cast -nostartfiles -nostdlib -fno-builtin Famine.o -o Famine
rm Famine.o