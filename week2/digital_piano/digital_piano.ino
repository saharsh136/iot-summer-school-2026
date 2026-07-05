const int buzzer = 8;

const int buttonDo = 2;
const int buttonRe = 3;
const int buttonMi = 4;
const int buttonFa = 5;
const int modeButton = 6;

bool majorMode = true;
bool lastModeState = HIGH;

// Major Scale
int majorNotes[] = {262, 294, 330, 349};

// Minor Scale
int minorNotes[] = {262, 294, 311, 349};

void setup() {
  pinMode(buttonDo, INPUT_PULLUP);
  pinMode(buttonRe, INPUT_PULLUP);
  pinMode(buttonMi, INPUT_PULLUP);
  pinMode(buttonFa, INPUT_PULLUP);
  pinMode(modeButton, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {

  // ---------- Mode Toggle ----------
  bool currentModeState = digitalRead(modeButton);

  if (lastModeState == HIGH && currentModeState == LOW) {
    majorMode = !majorMode;

    if (majorMode)
      Serial.println("Major Mode");
    else
      Serial.println("Minor Mode");

    delay(200);   // Debounce
  }

  lastModeState = currentModeState;

  bool doPressed = digitalRead(buttonDo) == LOW;
  bool rePressed = digitalRead(buttonRe) == LOW;
  bool miPressed = digitalRead(buttonMi) == LOW;
  bool faPressed = digitalRead(buttonFa) == LOW;

  int pressedButtons = doPressed + rePressed + miPressed + faPressed;

  // If two or more buttons are pressed
  if (pressedButtons >= 2) {
    tone(buzzer, 392);   // Sol
    return;
  }

  int *notes;

  if (majorMode)
    notes = majorNotes;
  else
    notes = minorNotes;

  if (doPressed)
    tone(buzzer, notes[0]);

  else if (rePressed)
    tone(buzzer, notes[1]);

  else if (miPressed)
    tone(buzzer, notes[2]);

  else if (faPressed)
    tone(buzzer, notes[3]);

  else
    noTone(buzzer);
}