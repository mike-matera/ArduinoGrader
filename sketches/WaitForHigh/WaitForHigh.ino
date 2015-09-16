
void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("Waiting for pin 0 to go HIGH");
  while (digitalRead(0) == LOW) {
  }
  Serial.println("Done!");
}
