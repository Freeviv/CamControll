#!/usr/bin/python3

import os

def file_len(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1

def buildWebPages():
    fileList = os.listdir()
    htmlList = []
    for file in fileList:
        if ".html" in file:
            htmlList.append(file)

    src = "#ifndef WEBPAGES_H\n#define WEBPAGES_H\n"
    src += "\n//Warning: This file is automaticly generated! Changes will be overwritten!\n\n"

    for file in htmlList:
        num_lines = file_len(file)
        print(num_lines)
        cur_line = 0
        src += "const char *" + file.replace('.','_') + " = \n"
        with open(file) as of:
            for line in of:
                if cur_line + 1 < num_lines:
                    src += "    \"" + line.strip().replace('\n','').replace("\"","\\\"") + "\\n\" \\\n"
                else:
                    src += "    \"" + line.strip().replace('\n','').replace("\"","\\\"") + "\\n\";\n"
                cur_line += 1
    src += "\n#endif // WEBPAGES_H\n"


    with open("../webpages.cpp",'w+') as source:
        source.write(src)
