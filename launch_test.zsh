#!/usr/bin/zsh

cp /usr/bin/ls tmp/ls
cp /usr/bin/test tmp/test
cp /usr/bin/date tmp/date
cp hello tmp/hello
cd assembly
nasm -f elf64 test.s && \
gcc -masm=intel -Wno-int-to-pointer-cast -nostartfiles -fPIC -nostdlib -fno-builtin test.o -o test && \
rm test.o

cd ..
cp assembly/test ./
