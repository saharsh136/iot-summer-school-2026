const int LED = 13;
int blinkCount = 0;

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(LED, HIGH);
  delay(500);

  digitalWrite(LED, LOW);
  delay(500);

  blinkCount++;
  Serial.print("Blink count: ");
  Serial.println(blinkCount);
}
