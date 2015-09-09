
static int ledPin = 13; 

void setup() {
  Serial.begin(9600);
  Serial.println("Hello World");
  pinMode(ledPin, OUTPUT); 
}

void loop() {
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW); 
  delay(500);
  Serial.println("Blink!");
}

