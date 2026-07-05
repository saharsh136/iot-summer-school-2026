const int buzzer = 8;

const int doBtn = 2;
const int reBtn = 3;
const int miBtn = 4;
const int faBtn = 5;
const int modeBtn = 6;

bool majorMode = true;
bool lastState = HIGH;

int major[] = {262, 294, 330, 349};
int minor[] = {262, 294, 311, 349};

void setup() {

  pinMode(doBtn, INPUT_PULLUP);
  pinMode(reBtn, INPUT_PULLUP);
  pinMode(miBtn, INPUT_PULLUP);
  pinMode(faBtn, INPUT_PULLUP);
  pinMode(modeBtn, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {

  bool current = digitalRead(modeBtn);

  if (lastState == HIGH && current == LOW) {

    majorMode = !majorMode;

    if (majorMode)
      Serial.println("Major Mode");
    else
      Serial.println("Minor Mode");

    delay(200);
  }

  lastState = current;

  bool d = digitalRead(doBtn) == LOW;
  bool r = digitalRead(reBtn) == LOW;
  bool m = digitalRead(miBtn) == LOW;
  bool f = digitalRead(faBtn) == LOW;

  int pressed = d + r + m + f;

  if (pressed >= 2) {
    tone(buzzer, 392);
    return;
  }

  int *notes = majorMode ? major : minor;

  if (d)
    tone(buzzer, notes[0]);

  else if (r)
    tone(buzzer, notes[1]);

  else if (m)
    tone(buzzer, notes[2]);

  else if (f)
    tone(buzzer, notes[3]);

  else
    noTone(buzzer);
}