
#include <fcntl.h>
#include <stdio.h>

#include "Emulator.h"
#include "Serial.h" 

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
  Arduino.setProperty("serial.baud", std::to_string(params));
}

void SerialEmulator::end() {
}

int SerialEmulator::available() {
  int got = fgetc(stdin); 
  ungetc(got, stdin);
  if (got == EOF) 
    return 0;
  else
    return 1;
}

int SerialEmulator::peek() {
  int got = fgetc(stdin); 
  ungetc(got, stdin);
  if (got == EOF) 
    return -1;
  else
    return got;
}

int SerialEmulator::read() {
  int got = fgetc(stdin);
  if (got == EOF)
    return -1;
  else
    return got;
}

int SerialEmulator::availableForWrite() {
  return 1;
}

void SerialEmulator::flush() {
  fflush(stdin); 
}

size_t SerialEmulator::write(uint8_t c) {
  fputc(c, stdout);
}

SerialEmulator Serial;
