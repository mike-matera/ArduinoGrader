
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
  __check("Serial.begin(%d, $x)", baud, params);
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

  int rval;
  if (got == EOF)
    rval = 0;
  else
    rval = 1;
  
  __check("Serial.available() == %d", rval); 	  
  return rval;
}

int SerialEmulator::peek() {
  istream *input = Arduino.get_istream();
  int got = input->get();
  input->unget();
  input->clear();

  int rval;
  if (got == EOF) 
    rval = -1;
  else
    rval = got;

  __check("Serial.peek() == %d", rval); 	  
  return rval;
}

int SerialEmulator::read() {
  istream *input = Arduino.get_istream();
  int got = input->get();
  input->clear();

  int rval;
  if (got == EOF)
    rval = -1;
  else
    rval = got;
  
  __check("Serial.read() == %d", rval);
  return rval;
}

int SerialEmulator::availableForWrite() {
  __check("Serial.availableForWrite() == %d", 1);
  return 1;
}

void SerialEmulator::flush() {
  __check("Serial.flush()");
  ostream *output = Arduino.get_ostream();
  output->flush();
}

size_t SerialEmulator::write(uint8_t c) {
  __check("Serial.write(%d)", c);
  ostream *output = Arduino.get_ostream();
  output->put(c);
}

SerialEmulator Serial;
