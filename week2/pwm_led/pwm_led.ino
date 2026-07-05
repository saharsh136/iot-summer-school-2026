const int led = 9;
const int button = 2;

int mode = 1;
bool lastButtonState = HIGH;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Mode 1 : Slow Breathing");
}

void loop() {

  bool currentButton = digitalRead(button);

  if (lastButtonState == HIGH && currentButton == LOW) {

    mode++;

    if (mode > 3)
      mode = 1;

    if (mode == 1)
      Serial.println("Mode 1 : Slow Breathing");

    else if (mode == 2)
      Serial.println("Mode 2 : Fast Pulse");

    else
      Serial.println("Mode 3 : SOS");

    delay(250);
  }

  lastButtonState = currentButton;

  if (mode == 1)
    slowBreathing();

  else if (mode == 2)
    fastPulse();

  else
    sosPattern();
}

void slowBreathing() {

  for (int i = 0; i <= 255; i++) {
    analogWrite(led, i);
    delay(6);
  }

  for (int i = 255; i >= 0; i--) {
    analogWrite(led, i);
    delay(6);
  }
}

void fastPulse() {

  for (int i = 0; i <= 255; i++) {
    analogWrite(led, i);
    delay(1);
  }

  for (int i = 255; i >= 0; i--) {
    analogWrite(led, i);
    delay(1);
  }
}

void dot() {
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
  delay(200);
}

void dash() {
  digitalWrite(led, HIGH);
  delay(600);
  digitalWrite(led, LOW);
  delay(200);
}

void sosPattern() {

  dot();
  dot();
  dot();

  dash();
  dash();
  dash();

  dot();
  dot();
  dot();

  delay(1000);
}
