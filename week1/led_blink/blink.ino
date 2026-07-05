/*
--------------------------------------------------------
Author      : Saharsh Mishra
Date        : 05-07-2026
Version     : v1.3
Description :
LED Blink with Serial Monitor and Potentiometer Speed Control
--------------------------------------------------------
*/

const int LED = 13;
const int POT = A0;

int blinkCount = 0;

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  // Read potentiometer value
  int potValue = analogRead(POT);

  // Fix: Constrain the reading to avoid invalid values
  potValue = constrain(potValue, 0, 1023);

  // Convert potentiometer value to delay (100 ms - 1000 ms)
  int delayTime = map(potValue, 0, 1023, 100, 1000);

  digitalWrite(LED, HIGH);
  delay(delayTime);

  digitalWrite(LED, LOW);
  delay(delayTime);

  blinkCount++;

  Serial.print("Blink count: ");
  Serial.print(blinkCount);
  Serial.print(", Delay: ");
  Serial.print(delayTime);
  Serial.println(" ms");
}
