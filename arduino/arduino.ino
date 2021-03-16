int secondPin = A1;

void setup() {
  pinMode(secondPin, OUTPUT);
  
}

void loop() {
  analogWrite(secondPin, 255);
}
