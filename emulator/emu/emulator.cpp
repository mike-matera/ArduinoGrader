#include "emulator.h"
#include "Arduino.h"

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <fcntl.h>
#include <stdarg.h>

#include <string>
#include <map>
#include <csignal>

extern "C" {
#include "stdlib.h"
}

#define TIME_DILATION_FACTOR 10

unsigned long get_time() {
    return 0;
}

string emu_get_property(const string &key) {
    return "";
}

void emu_set_property(const string &key, const string &val) {
}

void emu_set_pinmode(int pin, PinMode mode) {
}

void emu_set_pinvalue(int pin, int value) {
}

PinMode emu_get_pinmode(int pin) {
  return static_cast<PinMode>(0);
}

int emu_get_pinvalue(int pin) {
  return 0;
}

