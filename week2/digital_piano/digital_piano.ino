const int buzzer = 8;

const int doBtn = 2;
const int reBtn = 3;
const int miBtn = 4;
const int faBtn = 5;

void setup() {
  pinMode(doBtn, INPUT_PULLUP);
  pinMode(reBtn, INPUT_PULLUP);
  pinMode(miBtn, INPUT_PULLUP);
  pinMode(faBtn, INPUT_PULLUP);
}

void loop() {

  bool d = digitalRead(doBtn) == LOW;
  bool r = digitalRead(reBtn) == LOW;
  bool m = digitalRead(miBtn) == LOW;
  bool f = digitalRead(faBtn) == LOW;

  int pressed = d + r + m + f;

  if (pressed >= 2) {
    tone(buzzer, 392);
  }

  else if (d)
    tone(buzzer, 262);

  else if (r)
    tone(buzzer, 294);

  else if (m)
    tone(buzzer, 330);

  else if (f)
    tone(buzzer, 349);

  else
    noTone(buzzer);
}