from pathlib import Path
import fileinput
import re

entries = Path('src/')
js = ''
isjs = False

with fileinput.input(entries.iterdir()) as f:
    for line in f:
        if not isjs:
            if line.lstrip().startswith("TG_WASM_JS("):
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
        fname = re.findall(r"[\w']+", line)[idx];
        if fname not in functions:
            functions.append(fname)
        else:
            print(f"Warning: duplicated function '{fname}'")

print("")
print("Imported JS functions:")
print(functions)

with open("build/tg_wasm_js.js", "w") as file:
    with open("js/preface.js", "r") as preface:
        contents = preface.read()
        print(f"{contents}", file=file)
    print(f"{js}", file=file)
    print(f"let funcs = {{\n", file=file)
    for func in functions:
        print(f"    {func},\n", file=file)
    print(f"}}\n", file=file)
    with open("js/postface.js", "r") as preface:
        contents = preface.read()
        print(f"{contents}", file=file)


print("\nGenerated 'build/tg_wasm_js.js' file.")
