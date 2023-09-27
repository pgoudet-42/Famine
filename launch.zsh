#!/usr/bin/zsh

if [ ! -e "/tmp/test/" ]
then
    mkdir /tmp/test/
    mkdir /tmp/test2/
fi
cp /usr/bin/ls /tmp/test/ls
cp /usr/bin/ls /tmp/test2/ls
cp /usr/bin/test /tmp/test/test
cp /usr/bin/test /tmp/test2/test
cp /usr/bin/date /tmp/test/date
cp /usr/bin/date /tmp/test2/date
cp hello /tmp/test/hello
cp hello /tmp/test2/hello

cd assembly
nasm -f elf64 Famine.s && \
gcc -masm=intel -Wno-int-to-pointer-cast -nostartfiles -fPIC  -nostdlib -fno-builtin Famine.o -o Famine -fPIE && \
rm Famine.o

cd ..
cp assembly/Famine ./
if [ $(find ./ -name "peda-session*" | wc -l) -gt 0 ]
then
    rm peda-session*
fi
