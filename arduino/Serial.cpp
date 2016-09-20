
#include "emulator.h"
#include "Serial.h" 

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

using std::cin; 

SerialEmulator::SerialEmulator()  {
  // We must make STDIN non-blocking to emulate the behavior of Arduino.
  // Different things must happen depending on whether stdin is a TTY or not...
  struct termios ttystate;
  if (tcgetattr(fileno(stdin), &ttystate) == 0) {
    // tty...
    ttysave = ttystate; 
    ttystate.c_lflag &= ~ICANON;
    ttystate.c_cc[VMIN] = 0;
    ttystate.c_cc[VTIME] = 0;
    tcsetattr(fileno(stdin), TCSANOW, &ttystate);
  }else{
    // pipe or something else...
    int flags = fcntl(fileno(stdin), F_GETFL);
    flags &= ~O_NONBLOCK; 
    fcntl(fileno(stdin), F_SETFL, flags);
  }
}

SerialEmulator::~SerialEmulator() {
  struct termios ttystate;
  if (tcgetattr(fileno(stdin), &ttystate) == 0) {
    tcsetattr(fileno(stdin), TCSANOW, &ttysave);    
  }
}

void SerialEmulator::begin(unsigned long baud, uint8_t params) {
  emu_set_property("serial.baud", std::to_string(baud));
  emu_set_property("serial.params", std::to_string((int) params));
}

void SerialEmulator::end() {
}

int SerialEmulator::available() {
  int got = cin.get();
  cin.unget();
  cin.clear();

  int rval;
  if (got == EOF)
    rval = 0;
  else
    rval = 1;
  
  return rval;
}

int SerialEmulator::peek() {
  int got = cin.get();
  cin.unget();
  cin.clear();

  int rval;
  if (got == EOF) 
    rval = -1;
  else
    rval = got;

  return rval;
}

int SerialEmulator::read() {
  int got = cin.get();
  cin.clear();

  int rval;
  if (got == EOF)
    rval = -1;
  else
    rval = got;
  
  return rval;
}

int SerialEmulator::availableForWrite() {
  return 1;
}

void SerialEmulator::flush() {
  cout.flush();
}

size_t SerialEmulator::write(uint8_t c) {
  cout.put(c);
  return 1;
}

SerialEmulator Serial;
