const int LED = 13;
const int POT = A0;

int blinkCount = 0;

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  int potValue = analogRead(POT);

  int delayTime = map(potValue, 0, 1023, 100, 1000);

  digitalWrite(LED, HIGH);
  delay(delayTime);

  digitalWrite(LED, LOW);
  delay(delayTime);

  blinkCount++;

  Serial.print("Blink count: ");
  Serial.print(blinkCount);
  Serial.print(" | Delay: ");
  Serial.println(delayTime);
}
