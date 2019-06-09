#! /usr/bin/env python3

import argparse
import pathlib
import tempfile
import shutil
import subprocess
import os
import multiprocessing

parser = argparse.ArgumentParser()
parser.add_argument('sketch', help="A sketch to compile.")
args = parser.parse_args()

arduino = "/opt/arduino/arduino"
arduino_wrapper = "/home/maximus/ArduinoGrader/emulator"


def main():
    sketch = pathlib.Path(args.sketch)
    if not sketch.exists():
        raise ValueError('The sketch does not exist.')

    sketchdir = sketch.parent
    sketchonly = False
    if sketchdir.name != sketch.stem:
        sketchonly = True

    with tempfile.TemporaryDirectory() as d:
        tempdir = pathlib.Path(d)
        tempsketch = tempdir / sketch.stem

        # Copy the sketch into the temporary directory
        if sketchonly:
            os.mkdir(tempsketch)
            shutil.copy2(sketch, tempsketch)
        else:
            shutil.copytree(sketchdir, tempsketch)

        # Run Arduino to produce the processed CPP file.
        subprocess.run(f'{arduino} --verify --preserve-temp-files --pref build.path={tempdir}/build {tempsketch}/{sketch.name}', cwd=tempdir, check=True, shell=True)

        # Copy files to build the emulator.
        emu_dir = tempdir / 'emulator'
        shutil.copytree(arduino_wrapper, emu_dir)
        shutil.copy2(tempdir / 'build' / 'sketch' / (str(sketch.name) + '.cpp'), emu_dir)

        # Build the emulator.
        ncpus = multiprocessing.cpu_count()
        subprocess.run(f'make -j {ncpus}', shell=True, cwd=emu_dir, check=True)
        subprocess.run('tree', cwd=tempdir)
        subprocess.run('./test', shell=True, cwd=emu_dir, check=True)

if __name__ == '__main__':
    main()
