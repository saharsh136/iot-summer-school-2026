#include <Wire.h>
#include <Stepper.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int STEPS_PER_REV = 4096;

Stepper stepper(
  4096,
  18,
  21,
  19,
  22
);

#define CW_BTN 25
#define CCW_BTN 26
#define HOME_BTN 27

#define LED_PIN 2

long currentSteps = 0;
long targetSteps = 0;

String direction = "STOP";

unsigned long lastButtonTime = 0;
const int debounce = 250;

void setup()
{
  pinMode(CW_BTN, INPUT_PULLUP);
  pinMode(CCW_BTN, INPUT_PULLUP);
  pinMode(HOME_BTN, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Wire.begin(23, 5);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
  display.setTextColor(WHITE);

  stepper.setSpeed(8);
}

void loop()
{

  if (millis() - lastButtonTime > debounce)
  {

    if (digitalRead(CW_BTN) == LOW)
    {
      targetSteps += 512;
      lastButtonTime = millis();
    }

    if (digitalRead(CCW_BTN) == LOW)
    {
      targetSteps -= 512;
      lastButtonTime = millis();
    }

    if (digitalRead(HOME_BTN) == LOW)
    {
      currentSteps = 0;
      targetSteps = 0;

      direction = "HOME";

      lastButtonTime = millis();
    }
  }

  if (currentSteps < targetSteps)
  {

    digitalWrite(LED_PIN, HIGH);

    stepper.step(1);

    currentSteps++;

    direction = "CW";
  }

  else if (currentSteps > targetSteps)
  {

    digitalWrite(LED_PIN, HIGH);

    stepper.step(-1);

    currentSteps--;

    direction = "CCW";
  }

  else
  {

    digitalWrite(LED_PIN, LOW);

    direction = "STOP";
  }

  long currentMod = currentSteps % STEPS_PER_REV;

  if (currentMod < 0)
    currentMod += STEPS_PER_REV;

  long targetMod = targetSteps % STEPS_PER_REV;

  if (targetMod < 0)
    targetMod += STEPS_PER_REV;

  float currentAngle =
      (currentMod * 360.0) / STEPS_PER_REV;

  float targetAngle =
      (targetMod * 360.0) / STEPS_PER_REV;

  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(0, 0);
  display.print("Stepper Controller");

  display.drawLine(0, 10, 127, 10, WHITE);

  display.setCursor(0, 15);
  display.print("Current:");
  display.print(currentAngle, 1);
  display.print((char)247);

  display.setCursor(0, 28);
  display.print("Target :");
  display.print(targetAngle, 1);
  display.print((char)247);

  display.setCursor(0, 41);
  display.print("Dir    :");
  display.print(direction);

  display.setCursor(0, 54);
  display.print("Steps  :");
  display.print(currentSteps);

  display.display();
}
