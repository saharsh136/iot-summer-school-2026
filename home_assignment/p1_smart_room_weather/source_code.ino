#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 4
DHTesp dht;

#define RED_LED 18
#define GREEN_LED 19

#define BUZZER 23

float temperature = 0;
float humidity = 0;

unsigned long lastRead = 0;
unsigned long lastLog = 0;

String statusLabel = "COMFORT";

void setup() {

  Serial.begin(115200);

  dht.setup(DHTPIN, DHTesp::DHT11);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BUZZER, LOW);

  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (1);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  Serial.println("millis,temp,humidity,status");
}

void loop() {

  if (millis() - lastRead >= 2000) {

    lastRead = millis();

    TempAndHumidity data = dht.getTempAndHumidity();

    temperature = data.temperature;
    humidity = data.humidity;

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" C   ");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    determineStatus();

    updateOLED();

    thresholdCheck();
  }

  if (millis() - lastLog >= 5000) {

    lastLog = millis();

    Serial.print(millis());
    Serial.print(",");
    Serial.print(temperature);
    Serial.print(",");
    Serial.print(humidity);
    Serial.print(",");
    Serial.println(statusLabel);
  }
}

void determineStatus() {

  if (temperature > 38 || humidity > 80) {
    statusLabel = "DANGER";
  }

  else if (temperature >= 32 || humidity >= 70) {
    statusLabel = "HOT";
  }

  else if (temperature <= 22 && humidity <= 50) {
    statusLabel = "COOL";
  }

  else {
    statusLabel = "COMFORT";
  }
}

void updateOLED() {

  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temperature, 1);
  display.println(" C");

  display.setCursor(0, 20);
  display.print("Hum : ");
  display.print(humidity, 0);
  display.println(" %");

  display.setCursor(0, 45);
  display.print("Status: ");
  display.println(statusLabel);

  display.display();
}

void thresholdCheck() {

  if (temperature > 38 || humidity > 80) {

    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);

    digitalWrite(BUZZER, HIGH);
    delay(1000);
    digitalWrite(BUZZER, LOW);

  }

  else {

    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);

  }
}
