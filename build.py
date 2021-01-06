import os
import sys
import subprocess
import shutil
from enum import Enum, IntEnum, auto
#from glob import glob

# TODO:
#   - Abstract debugging information level flags
#   - Manage command-line options. I.e.,  $ python build.py native -debug


def block_stdout():
    sys.stdout = open(os.devnull, 'w')

# Restore
def enable_stdout():
    sys.stdout = sys.__stdout__

def command_exists(name):
    return shutil.which(name) is not None

def get_wasm_ld_command_name():
    if command_exists("wasm-ld"):
        return "wasm-ld"
    elif command_exists("wasm-ld-10"):
        return "wasm-ld-10"
    elif command_exists("wasm-ld-11"):
        return "wasm-ld-11"
    elif command_exists("wasm-ld-12"):
        return "wasm-ld-12"
    else:
        return None

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

class TargetArch(Enum):
    X86_64 = auto()
    Wasm32 = auto()


class TargetEnv(Enum):
    Win32 = auto()
    Linux = auto()
    Freestanding = auto()


class TargetCompilers(Enum):
    Clang = "clang"
    Gcc = "gcc"
    Mvsc = "cl"
    ClangCl = "clang-cl"


class Target():
    def __init__(self, arch, env, compiler):
        self.arch = arch
        self.env = env
        self.compiler = compiler
        self.validate()

    def validate(self):
        if (self.env == TargetEnv.Win32) and (self.compiler == TargetCompilers.Gcc):
            self.fail("Invalid compiler for Win32 Environment!")

        if ((self.compiler == TargetCompilers.Mvsc) or (self.compiler == TargetCompilers.ClangCl)) and (self.env != TargetEnv.Win32):
            self.fail("Invlid compiler for current Environment!")

        if (self.arch == TargetArch.Wasm32) and (self.compiler != TargetCompilers.Clang):
            self.fail("Invalid compiler for Wasm32 target!")

    def fail(self, msg):
        self.print()
        sys.exit(msg)

    def is_msvc_compatible(self):
        if self.compiler == TargetCompilers.Mvsc or self.compiler == TargetCompilers.ClangCl:
            return True
        else:
            return False

    def print(self):
        print(("Target:  {}, {}, {}  ".format(self.arch, self.env, self.compiler)))

    def compiler_command(self):
        return self.compiler.value


def target_win32_msvc():
    return Target(TargetArch.X86_64, TargetEnv.Win32, TargetCompilers.Mvsc)


def target_win32_clang():
    return Target(TargetArch.X86_64, TargetEnv.Win32, TargetCompilers.Clang)


def target_win32_clang_cl():
    return Target(TargetArch.X86_64, TargetEnv.Win32, TargetCompilers.ClangCl)


def target_wasm32():
    return Target(TargetArch.Wasm32, TargetEnv.Freestanding, TargetCompilers.Clang)


def target_linux_x86_64_gcc():
    return Target(TargetArch.X86_64, TargetEnv.Linux, TargetCompilers.Gcc)


def get_default_target():
    if (os.name == "nt"):
        return target_win32_msvc()
    elif (os.name == "posix"):
        return target_linux_x86_64_gcc()
    else:
        return target_win32_msvc()


def make_dir_if_not_exists(dirname):
    if not os.path.exists(dirname):
        try:
            os.makedirs(dirname)
        except:
            raise


def relative_path_split_all(filepath):
    np = os.path.normpath(filepath)
    return np.split(os.sep)


class CompilerOptimLevel(IntEnum):
    O0 = 0
    O1 = 1
    O2 = 2
    O3 = 3

    def get_flag(self, target):
        if (target.is_msvc_compatible()):
            return ["/Od", "/O1", "/O2", "/O2"][self]
        else:
            return ["-O0", "-O1", "-O2", "-O3"][self]

class IncludeDirFlag():
    def __init__(self, path):
        self.dir = os.path.normpath(path)

    def get_flag(self, target):
        if (target.is_msvc_compatible()):
            return "/I " + self.dir
        else:
            return "-I" + self.dir


class CompilerWarningLevel(IntEnum):
    W0 = 0
    W1 = 1
    W2 = 2
    W3 = 3
    W4 = 4
    Wall = 5
    Wnone = 6

    def get_flag(self, target):
        if (target.is_msvc_compatible()):
            return ["/W0", "/W1", "/W2", "/W3", "/W4", "/Wall", "/w"][self]
        else:
            if (self == CompilerWarningLevel.Wnone):
                return "-w"
            else:
                return "-Wall"


def get_default_flags():
    return [CompilerOptimLevel.O1, CompilerWarningLevel.Wall]


class Dependencies:
    def __init__(self):
        self.deps = []

    def add(self, d):
        self.deps.append(d)

    def get(self):
        return self.deps


class RunTask:
    def __init__(self, args):
        self.deps = Dependencies()
        self.args = args

    def run_task(self):
        subprocess.run(self.args)


class ClearTask:
    def __init__(self, dirs=[]):
        self.dirs = dirs
        self.deps = Dependencies()

    def run_task(self):
        for d in self.dirs:
            try:
                shutil.rmtree(d)
            except:
                pass


class Executable:
    def __init__(self, name, files, target, build_dir="build", intermediary_dir="build/obj", flags=get_default_flags(), link_flags=[], wasm_stack_size = 10097152, verbose = False, silent = False):
        self.name = name
        self.files = files
        self.target = target
        self.build_dir = os.path.normpath(build_dir)
        self.intermediary_dir = os.path.normpath(intermediary_dir)
        self.flags = flags
        self.deps = Dependencies()
        self.wasm_stack_size = wasm_stack_size
        self.verbose = verbose
        self.silent = silent
        self.link_flags = link_flags

    def add_compile_flag(self, flag):
        self.flags.append(flag)

    def add_compile_flags(self, flags):
        self.flags = self.flags + flags

    def add_link_flag(self, flag):
        self.link_flags.append(flag)

    def add_link_flags(self, flags):
        self.link_flags = self.link_flags + flags

    def add_include_dirs(self, *args):
        for d in args:
            self.flags.append(IncludeDirFlag(d))

    def get_compile_flags(self):
        flags = []
        for f in self.flags:
            if (isinstance(f, str)):
                flags.append(f)
            else:
                flags.append(f.get_flag(self.target))
        return flags

    def __create_output_dirs(self):
        try:
            make_dir_if_not_exists(self.build_dir)
            make_dir_if_not_exists(self.intermediary_dir)
        except:
            print("Failed to create output directories!")
            exit()

    def __get_object_names(self):
        os = []
        for file in self.files:
            os.append(self.__get_object_name_for_file(file))

        return os

    def __get_object_name_for_file(self, file):
        split = relative_path_split_all(file)
        joined = "_".join(split)
        joined_no_ext = os.path.join(
            self.intermediary_dir, os.path.splitext(joined)[0])
        if (self.target.is_msvc_compatible()):
            return joined_no_ext + ".obj"
        else:
            return joined_no_ext + ".o"

    def __get_target_specific_compile_flags(self):
        if (self.target.arch == TargetArch.Wasm32):
            return ["--target=wasm32", "-nostdlib", "-fvisibility=hidden"]
        else:
            return []

    def __get_build_object_args(self, file):
        if (self.target.is_msvc_compatible()):
            return [self.target.compiler_command(), "/Fo" + self.__get_object_name_for_file(file), *self.get_compile_flags(), "/c", file]
        else:
            return [self.target.compiler_command(), "-c", file, *self.__get_target_specific_compile_flags(), *self.get_compile_flags(), "-o", self.__get_object_name_for_file(file)]

    def __build_objects(self):
        ps = []
        stdouts = []
        stderrs = []
        print(bcolors.HEADER + "* Building source files..." + bcolors.ENDC)
        for file in self.files:
            if self.verbose:
                print("Building " + file + "...")
                print("Running command: ", self.__get_build_object_args(file))
            subp = subprocess.Popen(self.__get_build_object_args(file), stdout = subprocess.PIPE, stderr = subprocess.PIPE, universal_newlines=True)
            stdouts.append(subp.communicate()[0])
            stderrs.append(subp.communicate()[1])
            ps.append(subp)
        for p in ps:
            p.wait()
        first_error = True
        for errs in stderrs:
            if errs != '':
                if first_error:
                    print()
                    print(bcolors.RED + u'\u2717' + " There were warnings or errors:" + bcolors.ENDC)
                    print()
                    first_error = False
                print(bcolors.WARNING + errs + bcolors.ENDC)
        if self.verbose:
            for out in stdouts:
                print(out)
        if first_error:
            print()
            print(bcolors.OKGREEN +  u'\u2713' + " Compiled all files" + bcolors.ENDC)
            print()

    def __build_single(self):
        if (self.target.compiler == TargetCompilers.Mvsc) or (self.target.compiler == TargetCompilers.ClangCl):
            args = [self.target.compiler_command(), *self.get_compile_flags(), self.files[0], "/link", "/out:" + self.__get_executable_output_path()]
            if self.verbose: print(args)
            subprocess.run(args)
        else:
            args = [self.target.compiler_command(), self.files[0], *self.__get_target_specific_compile_flags(), *self.get_compile_flags(), "-o", self.__get_executable_output_path()]
            if self.verbose: print(args)
            subprocess.run(args)

    def __get_executable_output_path(self):
        if (self.target.arch == TargetArch.Wasm32):
            return os.path.join(self.build_dir, self.name + ".wasm")
        elif (self.target.env == TargetEnv.Win32):
            return os.path.join(self.build_dir, self.name + ".exe")
        else:
            return os.path.join(self.build_dir, self.name)

    def get_link_args(self):
        if (self.target.arch == TargetArch.Wasm32):
            return [
                get_wasm_ld_command_name(),
                "--export-dynamic",
                "--import-table",
                "-no-entry",
                "-allow-undefined",
                "--import-memory",
                "--export=__heap_base",
                "-z",
                "stack-size=" + str(self.wasm_stack_size),
                *self.__get_object_names(),
                "-o",
                self.__get_executable_output_path()
            ]
        elif (self.target.is_msvc_compatible()):
            return [self.target.compiler_command(), *self.__get_object_names(), "/link", "/out:" + self.__get_executable_output_path(), *self.link_flags]
        else:
            return [self.target.compiler_command(), *self.__get_object_names(), "-o", self.__get_executable_output_path(), *self.link_flags]

    def __link(self):
        print(bcolors.HEADER + "* Linking executable..." + bcolors.ENDC)
        if self.verbose:
            print("ARGS = " + ", ".join(self.get_link_args()))
        p = subprocess.run(self.get_link_args())
        if p.stderr is not None:
            print(bcolors.RED + u'\u2717' + " Failed to link executable:" + bcolors.ENDC)
            print(p.stderr)
        else:
            print()
            print(bcolors.OKGREEN +  u'\u2713' + " Executable linked." + bcolors.ENDC)
            print()



    def __build(self):
        self.__create_output_dirs()
        if (len(self.files) == 1) and (self.target.arch != TargetArch.Wasm32):
            self.__build_single()
        else:
            self.__build_objects()
            self.__link()

    def run(self):
        return RunTask([self.__get_executable_output_path()])

    def clear(self):
        return ClearTask([self.build_dir, self.intermediary_dir])

    def run_task(self):
        self.__build()


class Builder:
    def __init__(self):
        self.tasks = {}

    def add_task(self, name, task):
        self.tasks[name] = task
        if "default" not in self.tasks:
            self.tasks["default"] = task
        return task

    def set_default(self, name):
        if name in self.tasks:
            self.tasks["default"] = self.tasks[name]

    def add_executable(self, exe):
        return self.add_task(exe.name, exe)

    def __run_task_bare(self, task):
        if hasattr(task, "silent") and task.silent:
            block_stdout()
        if (callable(task)):
            task()
        else:
            taskdeps = task.deps.get()
            for dep in taskdeps:
                self.__run_task_bare(dep)
            task.run_task()
        if hasattr(task, "silent") and task.silent:
            enable_stdout()

    def run_task(self, taskname="default"):
        if taskname not in self.tasks:
            print("Invlid task '" + taskname + "'.")
            exit()
        self.__run_task_bare(self.tasks[taskname])

    def run(self):
        self.run_task(sys.argv[1] if len(sys.argv) > 1 else "default")


class PrintTask:
    def __init__(self, msg="MESSAGE"):
        self.msg = msg
        self.deps = Dependencies()

    def run_task(self):
        print(self.msg)

class EmptyTask:
    def __init__(self):
        self.deps = Dependencies()

    def run_task(self):
        pass

def create_test_task(name, source, target, flags = []):
    compile_task = Executable(name, [source], target, flags=["-D__RUN_TESTS", "-g", *flags], build_dir=".test_temp")
    run_task = compile_task.run()
    run_task.deps.add(compile_task)
    return run_task

###################  Build script ###########################

verbose = False
for arg in sys.argv:
    if arg == "-v":
        verbose = True

b = Builder()
default_target = get_default_target()

sourcefiles_base = [
        "src/maybe.c",
        "src/format.c",
        "src/math.c",
        "src/heap.c",
        "src/main.c",
        "src/utils.c",
        "src/renderer_gl.c"
        ]


## 'wasm' task
sourcefiles_wasm = [*sourcefiles_base, "src/platform_web.c"]

wasmgen = b.add_task("wasm-gen", lambda: exec(open(os.path.join(".", "genwasmjs.py")).read()))
wasm_task = Executable("main", sourcefiles_wasm, target=target_wasm32(), flags=["-g", CompilerWarningLevel.Wall], verbose=verbose)
wasm_task.deps.add(wasmgen)

b.add_task("wasm", wasm_task)
b.set_default("wasm")

# 'test' task
test_flags = ["/Z7"] if (get_default_target().env == TargetEnv.Win32) else ["-lm"]

tests = EmptyTask()
tests.deps.add(create_test_task("maybe", "src/maybe.c", get_default_target()))
tests.deps.add(create_test_task("result", "src/result.c", get_default_target()))
tests.deps.add(create_test_task("math", "src/math.c", get_default_target(), flags=test_flags))
tests.deps.add(create_test_task("slice", "src/slice.c", get_default_target(), flags=test_flags))
tests.deps.add(create_test_task("heap", "src/heap.c", get_default_target(), flags=test_flags))
#tests.deps.add(create_test_task("heap_wasm", "src/heap_wasm.c", get_default_target(), flags=test_flags))
tests.deps.add(create_test_task("list", "src/list.c", get_default_target(), flags=test_flags))
tests.deps.add(create_test_task("string_intern", "src/string_intern.c", get_default_target(), flags=test_flags))

b.add_task("test", tests)

## Native task

native_source_files = []
if default_target.env == TargetEnv.Win32:
    native_source_files = [*sourcefiles_base, "src/platform_win32.c"]

native_exe_task = Executable("main", native_source_files, default_target, verbose=verbose, flags=[ CompilerOptimLevel.O0 ])

if default_target.env == TargetEnv.Win32:
    native_exe_task.add_compile_flag("/Z7")
    native_exe_task.add_compile_flag("/std:c11")
    native_exe_task.add_compile_flags(["/wd4820", "/wd4204", "/wd4711", "/wd5045"])
    native_exe_task.link_flags = ["/DEBUG:FULL", "user32.lib", "opengl32.lib", "gdi32.lib"]
b.add_task("native", native_exe_task)

## Run build

b.run()

