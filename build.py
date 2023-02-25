from os import listdir as os_listdir
from os.path import isfile as os_isfile, isdir as os_isdir, join as os_join
import subprocess

src_dirs = [
    './src'
]

inc_dirs = [
    './vendor/glm'
]

def recursive_files(mydir='.'):
    if mydir is None:
        mydir = '.'
    current = [f for f in os_listdir(mydir) if os_isfile(os_join(mydir, f))]
    sub = [f for f in os_listdir(mydir) if os_isdir(os_join(mydir, f))]

    subs = [current]
    for s in sub:
        subs.append(recursive_files(s))

    return [os_join(mydir, j) for i in subs for j in i]

def files():
    return [f for f in [f for sf in src_dirs for f in recursive_files(sf)] if f.endswith('.cpp') or f.endswith('.c')]

def inc_dir(f):
    if f.startswith('./'): f = f[2:]
    if not f.endswith('/'): f = f"{f}/"
    return f

def includes():
    return ' '.join([f"-I{inc_dir(f)}" for f in inc_dirs])

def safefiles():
    return [(f"\"{f}\"" if ' ' in f else f).replace('\\', '/') for f in files()]

def command():
    return f"emcc -o dist/main.js -sALLOW_MEMORY_GROWTH -s WASM=1 -O3 -std=c++17 -lembind {includes()} {' '.join(safefiles())}"

def run_command(cmd):
    try:
        subprocess.run(cmd, shell=True, check=True)
        return True
    except:
        return False

def main():
    cmd = command()
    print(cmd)
    if run_command(cmd):
        print("\nBuild successful!")
    else:
        print("\nBuild failed!")

if __name__ == '__main__':
    main()
