import os
import sys
import subprocess
import shutil
from enum import Enum, IntEnum, auto
from glob import glob


# TODO: MSVC debug in builder

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
    def __init__(self, name, files, target, build_dir="build", intermediary_dir="build/obj", flags=get_default_flags(), wasm_stack_size = 10097152):
        self.name = name
        self.files = files
        self.target = target
        self.build_dir = os.path.normpath(build_dir)
        self.intermediary_dir = os.path.normpath(intermediary_dir)
        self.flags = flags
        self.deps = Dependencies()
        self.wasm_stack_size = wasm_stack_size

    def add_compile_flag(self, flag):
        self.flags.append(flag)

    def add_compile_flags(self, flags):
        self.flags = self.flags + flags

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
        for file in self.files:
            print(self.__get_build_object_args(file))
            ps.append(subprocess.Popen(self.__get_build_object_args(file)))
        for p in ps:
            p.wait()

    def __build_single(self):
        if (self.target.compiler == TargetCompilers.Mvsc) or (self.target.compiler == TargetCompilers.ClangCl):
            args = [self.target.compiler_command(), *self.get_compile_flags(), self.files[0], "/link", "/out:" + self.__get_executable_output_path()]
            print(args)
            subprocess.run(args)
        else:
            args = [self.target.compiler_command(), self.files[0], *self.__get_target_specific_compile_flags(), *self.get_compile_flags(), "-o", self.__get_executable_output_path()]
            print(args)
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
                "wasm-ld",
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
            return [self.target.compiler_command(), *self.__get_object_names(), "/link", "/out:" + self.__get_executable_output_path()]
        else:
            return [self.target.compiler_command(), *self.__get_object_names(), "-o", self.__get_executable_output_path()]

    def __link(self):
        print(self.get_link_args())
        subprocess.run(self.get_link_args())

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

    def add_executable(self, exe):
        return self.add_task(exe.name, exe)

    def __run_task_bare(self, task):
        if (callable(task)):
            task()
        else:
            taskdeps = task.deps.get()
            for dep in taskdeps:
                self.__run_task_bare(dep)
            task.run_task()

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

#target = get_default_target()

target = target_win32_clang()
b = Builder()

 #["src/unity_build.c"] #glob("src/**/*.c", recursive=True)

sourcefiles_base = [
        "src/maybe.c",
        "src/result.c",
        "src/format.c",
        "src/math.c"
        ]

sourcefiles_exe = [*sourcefiles_base, "src/platform_win32.c"] if (target.env == TargetEnv.Win32) else sourcefiles_base
sourcefiles_wasm = [*sourcefiles_base, "src/platform_web.c"]

exe = Executable("main", sourcefiles_exe, target)
exe.deps.add(exe.clear())

run = exe.run()
run.deps.add(lambda: print("Build and then run!"))
run.deps.add(lambda: exec(open("other.py").read()))
run.deps.add(exe)

wasmgen = b.add_task("wasm-gen", lambda: exec(open(os.path.join(".", "genwasmjs.py")).read()))

wasm = Executable("main", sourcefiles_wasm, target=target_wasm32())
#wasm.add_compile_flag("-std=c99")
wasm.add_compile_flag("-g")
wasm.add_compile_flag("-Isrc/include")
wasm.add_include_dirs("src\\toolbox", "src\\coffee")
wasm.deps.add(wasmgen)

b.add_task("run", run)
b.add_task("wasm", wasm)

if (sys.platform == "win32"):
    b.add_task("env", lambda: subprocess.run(["C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Community\\Common7\\Tools\\VsDevCmd.bat", "-arch=amd64" ]))

tests = EmptyTask() 
tests.deps.add(create_test_task("maybe", "src/maybe.c", target))
tests.deps.add(create_test_task("result", "src/result.c", target))
tests.deps.add(create_test_task("math", "src/math.c", target, flags=["-lm"]))
tests.deps.add(create_test_task("math", "src/slice.c", target, flags=["-lm"]))
tests.deps.add(create_test_task("heap_wasm", "src/heap.c", target, flags=["-lm"]))
tests.deps.add(create_test_task("heap", "src/heap_wasm.c", target, flags=["-lm"]))

alloc = Executable("alloc", ["alloc.c"], target = target_win32_msvc(), flags=["/Zi"])
allocrun = alloc.run()
allocrun.deps.add(alloc)
b.add_task("alloc", allocrun)

b.add_task("test", tests)

b.run()

