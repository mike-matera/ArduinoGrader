#include <iostream>

#include "emulator.h"

using std::cout; 
using std::endl;
using std::string; 

static volatile int codes[] = {450, 451, 552, 553};
static volatile int voltage = -1;

int test_getvalue(int pin, const PinState &state) {  
  return codes[voltage];
}

bool test_loop(int count) {
  voltage++;
  return (count < 4);
}

void test_exit() {
  cout << "Test finished, output should be: Low, Inter, Inter, High" << endl;
}
