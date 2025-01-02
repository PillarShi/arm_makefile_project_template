import os,sys,shutil

RUN_TARGET = "morden_cmake_project.elf"
SRC_DIR = "."
BUILD_DIR = "build"

IS_NINJA = False
IS_VERBOSE = False
THREAD_NUM = os.cpu_count()

DEFAULT_MAKE = "only_build"
SHOW_COMMAND = False

################################### rules ####################################
rules = {}

def gen():
    if IS_NINJA:
        run_command(f"cmake -B {BUILD_DIR} -S {SRC_DIR} -GNinja")
    else:
        verbose = "ON" if IS_VERBOSE else "OFF"
        if os.name == 'nt':  # 'nt'表示Windows系统
            run_command(f"cmake -B {BUILD_DIR} -S {SRC_DIR} -G \"MinGW Makefiles\" -DCMAKE_VERBOSE_MAKEFILE={verbose}")
        else:
            run_command(f"cmake -B {BUILD_DIR} -S {SRC_DIR} -DCMAKE_VERBOSE_MAKEFILE={verbose}")
rules["gen"] = gen

def build():
    run_command(f"cmake --build {BUILD_DIR} -j{THREAD_NUM}")
rules["build"] = build

def run():
    run_command(f"./{BUILD_DIR}/{RUN_TARGET}")
rules["run"] = run

def clean():
    if os.path.exists(BUILD_DIR):
        shutil.rmtree(BUILD_DIR)
    if SHOW_COMMAND:
        print(f"clean {BUILD_DIR}")
rules["clean"] = clean

def run_begin():
    print("-------------------------------- RUN TARGET --------------------------------")
rules["run_begin"] = run_begin

def run_end():
    print("----------------------------------------------------------------------------")
rules["run_end"] = run_end

def burn():
    run_command(f"pyocd flash {BUILD_DIR}/{RUN_TARGET} -t apm32f103c8 --pack ../pack/Geehy.APM32F1xx_DFP.1.1.0.pack")
rules["burn"] = burn

def erase():
    run_command(f"pyocd erase -t apm32f103c8 --chip --pack ../pack/Geehy.APM32F1xx_DFP.1.1.0.pack")
rules["erase"] = erase

def full_run():
    only_build()
    run_begin()
    run_program()
    run_end()
rules["full_run"] = full_run

def only_build():
    gen()
    build()
rules["only_build"] = only_build
##############################################################################

def run_command(command):
    """运行 shell 命令并检查结果"""
    if SHOW_COMMAND:
        print(command)
    os.system(command)

def run_rule(rule=None):
    """根据目标执行相应的命令"""
    if rule is None:
        rule = DEFAULT_MAKE
    try:
        rules[rule]()
    except KeyError:
        print(f"Unknown rule: {rule}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) > 1:
        for i in range(1, len(sys.argv)):
            run_rule(sys.argv[i])
    else:
        run_rule()
