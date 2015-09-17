
int speakerPin = 3;
  
void setup()
{
}

int getNote(char note) {
  int n;
  switch (note) {
    case 'c':
      n = 261;
      break;
    case 'C':
      n = 277;
      break;
    case 'd':
      n = 294;
      break;
    case 'D':
      n = 311;
      break;
    case 'e':
      n = 330;
      break;
    case 'f':
      n = 349;
      break;
    case 'F':
      n = 370;
      break;
    case 'g':
      n = 392;
      break;
    case 'G':
      n = 415;
      break;
    case 'a':
      n = 440;
      break;
    case 'A':
      n = 466;
      break;
    case 'b':
      n = 494;
      break;
    default:
      n = 0;
  }
  return n;
}

void loop() {  
  String scale = "cdef";
  Serial.println("== Sequential tones, no break =="); 
  for (int i=0; i<scale.length(); i++) {
    tone(speakerPin, getNote(scale[i]));
    delay(100);
  }

  Serial.println("== Sequential tones, manual break =="); 
  for (int i=0; i<scale.length(); i++) {
    tone(speakerPin, getNote(scale[i]));
    delay(50);
    noTone(speakerPin);
    delay(50);
  }

  Serial.println("== Sequential tones, automatic break =="); 
  for (int i=0; i<scale.length(); i++) {
    tone(speakerPin, getNote(scale[i]), 50);
    delay(100);
  }

  Serial.println("== Overlapping tones, break ruined =="); 
  for (int i=0; i<scale.length(); i++) {
    tone(speakerPin, getNote(scale[i]), 100);
    delay(50);
  }

  Serial.println("== done =="); 
  noTone(speakerPin);
  delay(200);
}
