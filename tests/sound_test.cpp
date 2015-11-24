#include <climits>
#include <iostream>
#include <sstream>
#include <vector>

#include "emulator.h"

using std::cout; 
using std::endl;
using std::string; 
using std::vector; 
using std::stringstream; 

static stringstream input; 

class note {
public: 
  unsigned long ts; 
  int tone; 
};

static vector<note> melody; 

char getChar(int freq) {
  int n;
  switch (freq) {
    case 0:
      return ' ';
    case 261:
      return 'c';
    case 277:
      return 'C';
    case 294:
      return 'd';
    case 311:
      return 'D';
    case 330:
      return 'e';
    case 349:
      return 'f';
    case 370:
      return 'F';
    case 392:
      return 'g';
    case 415:
      return 'G';
    case 440:
      return 'a';
    case 466:
      return 'A';
    case 494:
      return 'b';
    default:
      return '!';
  }
}

void validate() {
  cout << endl << endl << "=== Tone validation pass ===" << endl;
  note playing; 
  playing.tone = 0; 
  playing.ts = 0; 
  for (note n : melody) {
   

    unsigned long duration = n.ts - playing.ts; 
    char note = getChar(playing.tone); 

    //cout << "DEBUG: " << note << " duration: " << (n.ts - playing.ts) << endl;

    if (note == '!') {
      cout << "ERROR: incorrect note: " << playing.tone << endl;
    }else if (note == ' ') {
      int rd = duration % 200;
      if (rd != 10 && rd != 0) {
	cout << "ERROR: malformed rest: " << duration << "ms" << endl;
      }
      if (duration >= 200) {
	cout << endl;
      }
    }else{
      if ((duration % 200) != 190) {
	cout << "ERROR: malformed note: " << duration << "ms" << endl;
      }else{
	cout << note;
	for (int i=0; i<duration/200; i++) {
	  cout << '-';
	}
      }
    }
    playing = n;
  }
  cout << endl << endl << "=== END Tone validation pass ===" << endl;

  // last note here. 
}

void test_pinchange(int pin, const PinState &prev, const PinState &next) {
  if (prev == next)
    return;
  if (next.get_mode() == PinMode::kSound) {
    note n; 
    n.ts = Arduino.get_time() / 1000;
    n.tone = next.get_value();
    melody.push_back(n);
  }
}

void test_propchange(const string &prop, const string &value) {
  if (prop == "stream.timeout" && value == TO_STRING(ULONG_MAX)) {
    cout << "NOTE: stream.timeout seems to be set to -1: enabled NL" << endl;
    input << "\n";
  }
}

void test_setup(void) {
  cout << "TEST: test_setup()" << endl;
  Arduino.set_istream(&input);
  input << "agfgaaa-ggg-aaa agfgaaaaggagf\n";
}

bool test_loop(int count) {
  return (count < 50);
}

void test_exit(void) {
  cout << "TEST: exit()" << endl;
  validate();
}

void test_check(const std::string &what) {
  if (what.substr(0,6) != "micros") { 
    //std::cout << "TEST: trace: " << what << std::endl;
  }
  if (Arduino.get_time() > 10000000) {
    validate();
    throw std::string("Simulator forced to exit after 10 seconds.");
  }
}
