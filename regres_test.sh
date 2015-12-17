#! /bin/bash

# This script does regression testing
export PATH=/opt/arduino-1.6.5:$PATH

set -e

./run_test.sh sketches/WaitForHigh/WaitForHigh.ino tests/wait_for_high.cpp 
#./run_test.sh sketches/Blink/Blink.ino tests/blink_test.cpp
#./run_test.sh sketches/ColorView/ColorView.ino tests/color_view_test.cpp
#./run_test.sh sketches/sound.ino tests/sound_test.cpp 
