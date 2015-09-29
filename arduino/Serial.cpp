
#include <fcntl.h>
#include <stdio.h>

#include "emulator.h"
#include "Serial.h" 

#include <iostream>
#include <string>

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
  Arduino.set_property("serial.baud", TO_STRING(baud));
  Arduino.set_property("serial.params", TO_STRING((int) params));
  test_propchange("serial.baud", TO_STRING(baud));
  test_propchange("serial.params", TO_STRING((int) params));
}

void SerialEmulator::end() {
}

int SerialEmulator::available() {
  istream *input = Arduino.get_istream();
  int got = input->get();
  input->unget();
  input->clear();

  if (got == EOF)
    return 0;
  else
    return 1;

}

int SerialEmulator::peek() {
  istream *input = Arduino.get_istream();
  int got = input->get();
  input->unget();
  input->clear();

  if (got == EOF) 
    return -1;
  else
    return got;
}

int SerialEmulator::read() {
  istream *input = Arduino.get_istream();
  int got = input->get();
  input->clear();

  if (got == EOF)
    return -1;
  else
    return got;
}

int SerialEmulator::availableForWrite() {
  return 1;
}

void SerialEmulator::flush() {
  ostream *output = Arduino.get_ostream();
  output->flush();
}

size_t SerialEmulator::write(uint8_t c) {
  ostream *output = Arduino.get_ostream();
  output->put(c);
}

SerialEmulator Serial;
