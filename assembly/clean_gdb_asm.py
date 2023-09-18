#!/usr/bin/python3

def isAlpha(char: str):
    if (char[0] >= 'A' and char <= 'Z') or (char[0] >= 'a' and char <= 'z'):
        return True
    return False

def getSymName(sym: str):
    i = 0
    
    for c in sym:
        if c == "+":
            break
        else:
            i += 1
    return sym[:i]

def findFirstOccurence(line: str, f: object):
    for i, c in enumerate(line):
        if f(c) == True:
            return i
    return -1

def clean_asm():
    first_time = False
    final_file = "section .text\n\tglobal _start\n"
    old_name = ""
    
    with open("tmp.s", "r") as f:
        f.readline()
        for l in f:
            name = getSymName(l[23:])
            if old_name != name and len(name) > 0:
                first_time = True
                final_file += "\n\n"
                
            if first_time == True and len(name) > 0:
                final_file += name + ":\n"
                first_time = False
            end_name_pos = findFirstOccurence(l[23 + len(name):], isAlpha)
            start_index = 23 + len(name) + end_name_pos
            instruction = l[start_index:].strip()
            final_file += "\t" + instruction + "\n"
            old_name = name
    print(final_file)


if __name__ == "__main__":
    clean_asm()