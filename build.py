from os import listdir as os_listdir
from os.path import isfile as os_isfile, isdir as os_isdir, join as os_join
import subprocess

def recursive_files(mydir=None):
    if mydir is None:
        mydir = '.'
    current = [f for f in os_listdir(mydir) if os_isfile(os_join(mydir, f))]
    sub = [f for f in os_listdir(mydir) if os_isdir(os_join(mydir, f))]

    subs = [current]
    for s in sub:
        subs.append(recursive_files(s))

    return [os_join(mydir, j) for i in subs for j in i]

def files():
    return [f for f in recursive_files() if f.endswith('.cpp') or f.endswith('.c')]

def safefiles():
    return [f"\"{f}\"" if ' ' in f else f for f in files()]

def command():
    return f"emcc -o main.js -s WASM=1 -O3 -std=c++17 -lembind {' '.join(safefiles())}"

def run_command(cmd):
    try:
        subprocess.run(cmd, shell=True, check=True)
        return True
    except:
        return False

def main():
    if run_command(command()):
        print("\nBuild successful!")
    else:
        print("\nBuild failed!")

if __name__ == '__main__':
    main()
