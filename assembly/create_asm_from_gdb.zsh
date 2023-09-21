#!/usr/bin/zsh

gdb -batch -ex "file ../Famine" -ex "disas 0x0000000000001000 , 0x0000000000002000" > tmp.s
./clean_gdb_asm.py > test.s
if [ -e "tmp.s" ]
then
    rm tmp.s
fi