
enum State {
  IDLE,
  COIN_INSERTED,
  ITEM_SELECTED,
  DISPENSING
};

State currentState = IDLE;

const int coinBtn = 2;
const int selectBtn = 3;
const int cancelBtn = 4;

const int redLED = 8;
const int yellowLED = 9;
const int greenLED = 10;

void setup() {

  pinMode(coinBtn, INPUT_PULLUP);
  pinMode(selectBtn, INPUT_PULLUP);
  pinMode(cancelBtn, INPUT_PULLUP);

  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  Serial.begin(9600);

  Serial.println("Current State : IDLE");
}

void loop() {

  updateLEDs();

  switch (currentState) {

    case IDLE:

      if (digitalRead(coinBtn) == LOW) {
        currentState = COIN_INSERTED;
        Serial.println("IDLE -> COIN_INSERTED");
        delay(300);
      }

      break;

    case COIN_INSERTED:

      if (digitalRead(cancelBtn) == LOW) {
        currentState = IDLE;
        Serial.println("COIN_INSERTED -> IDLE");
        delay(300);
      }

      else if (digitalRead(selectBtn) == LOW) {
        currentState = ITEM_SELECTED;
        Serial.println("COIN_INSERTED -> ITEM_SELECTED");
        delay(300);
      }

      break;

    case ITEM_SELECTED:

      Serial.println("ITEM_SELECTED -> DISPENSING");

      currentState = DISPENSING;

      delay(1000);

      break;

    case DISPENSING:

      Serial.println("Dispensing Item...");
      delay(3000);

      Serial.println("DISPENSING -> IDLE");

      currentState = IDLE;

      break;
  }
}

void updateLEDs() {

  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);

  switch (currentState) {

    case IDLE:
      digitalWrite(redLED, HIGH);
      break;

    case COIN_INSERTED:
      digitalWrite(yellowLED, HIGH);
      break;

    case ITEM_SELECTED:
      digitalWrite(redLED, HIGH);
      digitalWrite(yellowLED, HIGH);
      break;

    case DISPENSING:
      digitalWrite(greenLED, HIGH);
      break;
  }
}
