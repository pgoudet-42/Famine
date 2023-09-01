

import sys

def custom_opcodes(line: str):
    str = ""
    for i, c in enumerate(line):
        if i > 9 and i < 32:
            if c == "\n" or (c.isspace() == True and line[i + 1].isspace() == True):
                break
            if i == 10:
                str += "0x"
            str += c
            if c == " " and line[i + 1].isspace() == False:
                str += "0x"
            elif c.isspace() == False and line[i + 1].isspace() == True:
                str += ","
    return str + "\n"

def add_c_words(code, size: int):
    code = code.split("\n")
    tabs = " " * len("unsigned char code[] = {  ")
    for i, c in enumerate(code):
        # if i == 0:
            # code[i] = "#include \"../include/famine.h\"\n\nunsigned char code[] = {  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n" + tabs + c + "\n"
        # else:
        code[i] = tabs + c + "\n"
    code[i] += " " * len("unsigned char code[] = {")  + "};\n"
    code = "".join(code)
    code = "#include \"../include/famine.h\"\n\nunsigned char code[] = {  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,\n" + code
    return code

def get_func(size):
    str = ""
    str = '''
unsigned char *get_code(size_t *size) {
    unsigned char *code_local;

    *size = %d;
    code_local = malloc(*size + 13);
    if (!code_local)
        return (NULL);
    memcpy(code_local, code, *size);
    return (code_local);
}''' % size
    return str

def main(file):
    start:bool = False
    code: str = ""

    with open(file) as f:
        line = f.readline()
        while line:
            if start == True:
                str = custom_opcodes(line)
                code += str
            if line[9:32].find("<") != -1:
                start = True
            line = f.readline()
            if line[9:32].find("<") != -1:
                start = False
        code += "0xe9, 0x10, 0x50, 0x00, 0x00"
        code = add_c_words(code, len(code.split(",")))
    str = get_func(len(code.split(",")))
    with open("../srcs/create_code.c", "wt") as f:
        f.write(code)
        f.write(str)


if __name__ == "__main__":
    main(sys.argv[1])