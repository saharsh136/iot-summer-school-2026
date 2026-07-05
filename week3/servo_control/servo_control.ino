#include <Servo.h>

Servo servo;

const int servoPin = 9;
const int potPin = A0;
const int buttonPin = 2;

void setup() {
  Serial.begin(9600);

  servo.attach(servoPin);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {

    for (int angle = 0; angle <= 180; angle++) {
      servo.write(angle);
      Serial.print("Angle: ");
      Serial.println(angle);
      delay(10);
    }

    for (int angle = 180; angle >= 0; angle--) {
      servo.write(angle);
      Serial.print("Angle: ");
      Serial.println(angle);
      delay(10);
    }

    while (digitalRead(buttonPin) == LOW);
    delay(50);
  }
  else {
    int potValue = analogRead(potPin);
    int angle = map(potValue, 0, 1023, 0, 180);

    servo.write(angle);

    Serial.print("Angle: ");
    Serial.println(angle);

    delay(100);
  }
}
