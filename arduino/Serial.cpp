
#include "emulator.h"
#include "Serial.h" 

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

using std::cin; 

SerialEmulator::SerialEmulator()  {
  if (tcgetattr(fileno(stdin), &ttysave) == 0) {
    is_tty = true;
  }else{
    is_tty = false;
  }
}

/*
// We must make STDIN non-blocking to emulate the behavior of Arduino.
// Different things must happen depending on whether stdin is a TTY or not...
*/
void SerialEmulator::setNonblocking(bool nb) {
  if (is_tty) {
    // STDIN is a TTY
    if (nb) {
      struct termios ttystate = ttysave;  
      ttystate.c_lflag &= ~ICANON;
      ttystate.c_cc[VMIN] = 0;
      ttystate.c_cc[VTIME] = 0;
      tcsetattr(fileno(stdin), TCSANOW, &ttystate);
    }else{
      tcsetattr(fileno(stdin), TCSANOW, &ttysave);
    }
  }else{
    // FIXME: This isn't tested...
    // STDIN is a pipe or something else.
    int flags = fcntl(fileno(stdin), F_GETFL);
    if (nb) {
      flags &= ~O_NONBLOCK; 
    }else{
      flags |= O_NONBLOCK; 
    }
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
  setNonblocking(true);
  int got = cin.get();
  cin.unget();
  cin.clear();
  setNonblocking(false);

  int rval;
  if (got == EOF)
    rval = 0;
  else
    rval = 1;
  
  return rval;
}

int SerialEmulator::peek() {
  if (available() == 0) {
    return -1;
  }
  return cin.peek();
}

int SerialEmulator::read() {
  if (available() == 0) {
    return -1;
  }
  return cin.get();
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
