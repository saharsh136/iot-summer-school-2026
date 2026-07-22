#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define TRIG_PIN 5
#define ECHO_PIN 18

#define GREEN_LED 25
#define YELLOW_LED 26
#define RED_LED 27

#define BUZZER 23

float distance = 0;
String zone = "SAFE";

unsigned long lastMeasure = 0;
unsigned long lastSerial = 0;

unsigned long lastBeep = 0;
bool buzzerState = false;
int beepInterval = -1;

#define BUZZER_CHANNEL 0
#define BUZZER_RESOLUTION 8

void setup() {

  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  digitalWrite(BUZZER, LOW);

  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED Failed");
    while (1);
  }

  display.clearDisplay();
  display.display();
}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - lastMeasure >= 100) {

    lastMeasure = currentMillis;

    distance = getAverageDistance();

    determineZone();

    updateLEDs();

    updateOLED();
  }

  if (currentMillis - lastSerial >= 500) {

    lastSerial = currentMillis;

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" cm    Zone: ");
    Serial.println(zone);
  }

  updateBuzzer();
}

float getAverageDistance() {

  float total = 0;

  for (int i = 0; i < 5; i++) {

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000);

    float d = duration * 0.034 / 2.0;

    total += d;
  }

  return total / 5.0;
}

void determineZone() {

  if (distance > 60) {

    zone = "SAFE";
    beepInterval = -1;

  }

  else if (distance > 30) {

    zone = "CAUTION";
    beepInterval = 800;

  }

  else if (distance > 15) {

    zone = "CLOSE";
    beepInterval = 300;

  }

  else {

    zone = "DANGER";
    beepInterval = 0;
  }
}

void updateLEDs() {

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);

  if (zone == "SAFE") {

    digitalWrite(GREEN_LED, HIGH);

  }

  else if (zone == "CAUTION") {

    digitalWrite(YELLOW_LED, HIGH);

  }

  else if (zone == "CLOSE") {

    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, HIGH);

  }

  else {

    digitalWrite(RED_LED, HIGH);
  }
}

void updateBuzzer() {

  unsigned long currentMillis = millis();

  if (beepInterval == -1) {

    digitalWrite(BUZZER, LOW);
    buzzerState = false;
    return;
  }

  if (beepInterval == 0) {

    digitalWrite(BUZZER, HIGH);
    return;
  }

  if (currentMillis - lastBeep >= beepInterval) {

    lastBeep = currentMillis;

    buzzerState = !buzzerState;

    digitalWrite(BUZZER, buzzerState);
  }
}

void updateOLED() {

  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(10, 0);
  display.print((int)distance);
  display.print(" cm");

  display.setTextSize(1);
  display.setCursor(0, 28);
  display.print("Zone: ");
  display.println(zone);

  int barWidth = map(constrain((int)distance, 0, 100), 100, 0, 0, 120);

  display.drawRect(4, 50, 120, 10, SSD1306_WHITE);
  display.fillRect(4, 50, barWidth, 10, SSD1306_WHITE);

  display.display();
}
