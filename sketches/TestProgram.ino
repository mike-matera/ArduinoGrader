#include <Servo.h>

#define LED0 2 
#define LED1 4 
#define LED2 7 
#define LED3 8 

#define RED_LED 11
#define GREEN_LED 10
#define BLUE_LED 9

#define BUTTON0 12
#define BUTTON1 13

#define SPEAKER 3 
#define SERVO 6 

#define LIGHT A0
#define PRESSURE A1

#define SCOPEINT 20

Servo serv; 
static int st = 1;
static bool hasPressure = false;
static bool hasLight = false;

void sample(unsigned int value) {

  int i;
  unsigned int lead_sym = SCOPEINT * value / 1024;
  if (lead_sym > SCOPEINT)
    lead_sym = SCOPEINT;
  unsigned int trail_sym = SCOPEINT - lead_sym;

  Serial.print("|#"); 
  for (i=0; i<lead_sym; i++) 
    Serial.print('#');
  for (i=0; i<trail_sym; i++) 
    Serial.print(' ');

  if (value > 1024) {
    Serial.print("!");
  }
  else{
    Serial.print("|");
  }

}

int trail(bool reverse=true, int time = 200) {
  static int pos = 0;
  int n = 0;

  if (reverse) {
    st--;
    if (st == -1)
      st = 4; 
  }
  else{
    st = (st + 1) % 5;
  }

  digitalWrite(LED0, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);

  if (st == 0) {
    digitalWrite(LED0, HIGH);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    tone(SPEAKER, 98);
    n = 98;
    pos = 0;
  }
  else if (st == 1) {
    digitalWrite(LED0, HIGH);
    digitalWrite(RED_LED, LOW);
    n = 440;
    pos = 5;
  }
  else if (st == 2) {
    digitalWrite(LED1, HIGH);
    digitalWrite(GREEN_LED, LOW);
    n = 493;
    pos = 10;
  }
  else if (st == 3) {
    digitalWrite(LED2, HIGH);
    digitalWrite(BLUE_LED, LOW);
    n = 587;
    pos = 15;
  }
  else if (st == 4) {
    digitalWrite(LED3, HIGH);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    n = 698; 
    pos = 20;
  }

  serv.write(pos);
  tone(SPEAKER, n);
  delay(time/3);
  noTone(SPEAKER);
  delay(2*time/3);

  return time;
}

unsigned int buttons_and_switches() {
  int d = 0, pres = 0;
  if (digitalRead(BUTTON0) == LOW) {  
    d += trail();
  }  
  if (digitalRead(BUTTON1) == LOW) {  
    d += trail(false);
  }

  if (hasPressure) {
    pres = analogRead(PRESSURE);
    if (pres > 768) {
      d += trail(true, 100);
    }
    else if (pres > 200) {
      d += trail(true, ((768-pres)/2)+100);
    }
  }

  if (d == 0)
    delay(200);

  return pres;
}

void setup() {
  Serial.begin(9600);

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);

  digitalWrite(LIGHT, HIGH);
  delay(100);
  for (int i=0; i<10; i++) {
    if (analogRead(LIGHT) < 1000) {
      hasLight = true;
      break;
    }
  }
  digitalWrite(LIGHT, LOW);
  
  digitalWrite(PRESSURE, HIGH);
  delay(100);
  for (int i=0; i<10; i++) {
    if (analogRead(PRESSURE) < 1000) {
      hasPressure = true;
      break;
    }
  }
  digitalWrite(PRESSURE, LOW);

  pinMode(LED0, OUTPUT); 
  pinMode(LED1, OUTPUT); 
  pinMode(LED2, OUTPUT); 
  pinMode(LED3, OUTPUT); 
  pinMode(RED_LED, OUTPUT); 
  pinMode(GREEN_LED, OUTPUT); 
  pinMode(BLUE_LED, OUTPUT); 
  pinMode(BUTTON0, INPUT_PULLUP); 
  pinMode(BUTTON1, INPUT_PULLUP);   

  serv.attach(SERVO);
  trail();
}

void loop() {

  Serial.print("LEDs: ");
  for (int i=1; i<5; i++) {
    if (i == st)
      Serial.print('#');
    else
      Serial.print('_');
  }
  Serial.print(' ');
  if (st == 0)
    Serial.print('_');
  else if (st == 1)
    Serial.print('R');
  else if (st == 2)
    Serial.print('G');
  else if (st == 3)
    Serial.print('B');
  else if (st == 4)
    Serial.print('W');

  if (hasLight) {
    Serial.print("    Brightness:");
    sample(analogRead(LIGHT));
  }

  int s = buttons_and_switches();
  if (hasPressure) {
    Serial.print("    Pressure:");
    sample(s);
  }
  Serial.println();
}

