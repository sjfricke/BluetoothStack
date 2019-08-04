import os
import sys

ROOT_DIR = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))

# All directories to run on
WHITE_LIST = [
    os.path.join(ROOT_DIR, "common"),
    os.path.join(ROOT_DIR, "linux", "client"),
    os.path.join(ROOT_DIR, "linux", "server")
]

def ClangFormat():
    if os.name == "nt":
        clang_format_exe = "clang-format.exe"
    elif os.name == "posix":
        clang_format_exe = "clang-format"

    for directory in WHITE_LIST:
        for file in os.listdir(directory):
            if file.endswith(".c") or file.endswith(".cpp") or file.endswith(".h") or file.endswith(".hpp"):
                exe = clang_format_exe + " -i -style=file " + str(os.path.join(directory, file))
                print(exe)
                os.system(exe)

if __name__ == '__main__':
  ClangFormat()
