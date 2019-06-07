#! /usr/bin/env python3

import argparse
import pathlib
import tempfile
import shutil
import subprocess
import os

parser = argparse.ArgumentParser()
parser.add_argument('sketch', help="A sketch to compile.")
args = parser.parse_args()

arduino = "/opt/arduino/arduino"
arduino_wrapper = "/home/maximus/ArduinoGrader2/arduino"


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
        emu_dir = tempdir / 'emu'
        os.mkdir(emu_dir)
        shutil.copy2(tempdir / 'build' / 'sketch' / (str(sketch.name) + '.cpp'), emu_dir)
        shutil.copytree(arduino_wrapper, emu_dir / 'arduino')
        subprocess.run('tree', cwd=tempdir)

        # Build the emulator.


if __name__ == '__main__':
    main()
