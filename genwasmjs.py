from pathlib import Path
import fileinput
import re
import os

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    RED = '\033[1;31m'  

entries = Path('src/')
js = ''
isjs = False

macro_name = "WASM_JS("

print()
print(bcolors.HEADER + "* Generating javascript code for WASM_JS definitions..." + bcolors.ENDC)

files = (os.path.join(p, f) for p, _, fs in os.walk(entries) for f in fs)
with fileinput.input(files) as f:
    for line in f:
        if not isjs:
            if line.lstrip().startswith(macro_name):
                isjs = True
        else:
            if line.lstrip().startswith(")"):
                isjs = False
            else:
                js += line

js += "\n"
functions = []
for line in js.split("\n"):
    if line.lstrip().startswith("async function"):
        idx = 2
    else: 
        idx = 1
    if line.lstrip().startswith("function") or line.lstrip().startswith("async function"):
        fname = re.findall(r"[\w']+", line)[idx]
        if fname not in functions:
            functions.append(fname)
        else:
            print(f"Warning: duplicated function '{fname}'")

print()
print("Imported JS functions: ", ", ".join(functions))
print()

with open("build/tg_wasm_js.js", "w") as file:
    with open("js/preface.js", "r") as preface:
        contents = preface.read()
        print(f"{contents}", file=file)
    with open("js/gl.js", "r") as gl:
        contents = gl.read()
        print(f"{contents}", file=file)
    print(f"{js}", file=file)
    print(f"let funcs = {{\n", file=file)
    for func in functions:
        print(f"    {func},\n", file=file)
    print(f"}}\n", file=file)
    with open("js/postface.js", "r") as preface:
        contents = preface.read()
        print(f"{contents}", file=file)


print(bcolors.OKGREEN +  u'\u2713' + " Generated 'build/tg_wasm_js.js' file." + bcolors.ENDC)
print()
