#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// -------- Keypad --------
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// -------- Pins --------
const int greenLED = 10;
const int redLED = 11;
const int buzzer = 12;

// -------- Password --------
String correctPIN = "1234";
String enteredPIN = "";

int attempts = 0;
bool locked = false;
unsigned long lockStart = 0;

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  showEnterPin();
}

void loop() {

  if (locked) {

    unsigned long elapsed = (millis() - lockStart) / 1000;

    if (elapsed >= 10) {
      locked = false;
      attempts = 0;
      showEnterPin();
    } else {
      lcd.setCursor(0, 0);
      lcd.print("LOCKED        ");
      lcd.setCursor(0, 1);
      lcd.print("Wait ");
      lcd.print(10 - elapsed);
      lcd.print(" sec   ");
    }

    return;
  }

  char key = keypad.getKey();

  if (key) {

    if (key >= '0' && key <= '9') {

      enteredPIN += key;

      lcd.setCursor(enteredPIN.length() - 1, 1);
      lcd.print("*");

      if (enteredPIN.length() == 4) {

        delay(200);

        if (enteredPIN == correctPIN) {

          lcd.clear();
          lcd.print("ACCESS");
          lcd.setCursor(0, 1);
          lcd.print("GRANTED");

          digitalWrite(greenLED, HIGH);

          delay(2000);

          digitalWrite(greenLED, LOW);

          attempts = 0;

        } else {

          lcd.clear();
          lcd.print("ACCESS");
          lcd.setCursor(0, 1);
          lcd.print("DENIED");

          digitalWrite(redLED, HIGH);

          tone(buzzer, 1000);
          delay(1000);
          noTone(buzzer);

          digitalWrite(redLED, LOW);

          attempts++;

          if (attempts >= 3) {
            locked = true;
            lockStart = millis();
          }
        }

        enteredPIN = "";

        if (!locked)
          showEnterPin();
      }
    }

    if (key == '*') {
      enteredPIN = "";
      showEnterPin();
    }
  }
}

void showEnterPin() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ENTER PIN:");
  lcd.setCursor(0, 1);
}
