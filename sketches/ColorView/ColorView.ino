
int redPin = 9; 
int greenPin = 10; 
int bluePin = 11; 
int sensorPin = 0; 

struct color {
  int r, g, b;
};

void setup() {
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

struct color readColor() {
  struct color rval;
  analogWrite(redPin, 255);
  delay(50);
  rval.r = analogRead(sensorPin);
  analogWrite(redPin, 0);

  analogWrite(greenPin, 255);
  delay(50);
  rval.g = analogRead(sensorPin);
  analogWrite(greenPin, 0);

  analogWrite(bluePin, 255);
  delay(50);
  rval.b = analogRead(sensorPin);
  analogWrite(bluePin, 0);

  return rval;
}

void loop() {
  struct color reflect = readColor();
  Serial.print("Saw color: r:");
  Serial.print(reflect.r);
  Serial.print(" g:");
  Serial.print(reflect.g);
  Serial.print(" b:");
  Serial.println(reflect.b);
}
