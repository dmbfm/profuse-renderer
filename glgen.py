import os
f = open(os.path.join("src", "platform_gl.h"))
idx = 0

def read_until_char(s, c):
    idx = 0
    while idx < len(s):
        if s[idx] == c:
            return s[:idx]
        idx += 1

lines = f.readlines()
parsed_funcs = []
while idx < len(lines):
    line = lines[idx]
    if line.lstrip().startswith("GLFUNC"):
        final = ""
        inner = line[8:].rstrip('\n')
        r = read_until_char(inner, ')')
        if r is None:
            r2 = read_until_char(lines[idx+1], ')')
            if r2 is not None:
                final = inner + r2.rstrip('\n')
            else:
                continue
        else:
            final = r
        func = list(map(lambda x: x.lstrip().rstrip(), final.split(",")))
        parsed_funcs.append(func)
    idx += 1

w = open(os.path.join("src", "gen", "win32_gl.c"), "w")

w.write("void *gl_get_proc_adress(const char *name);\n")
w.write("#define GL_GETPROC(name) name = (gl_##name *)gl_get_proc_adress(#name)\n");

for func in parsed_funcs:
    line = "gl_{0} *{0};\n".format(func[1])
    w.write(line)

w.write("\n")
w.write("static void platform_win32_gl_init_function_pointers(void)\n")
w.write("{\n")
for func in parsed_funcs:
    w.write("    GL_GETPROC({0});\n".format(func[1]))
w.write("\n")
for func in parsed_funcs:
    w.write("    assert({0});\n".format(func[1]))
w.write("}\n")

