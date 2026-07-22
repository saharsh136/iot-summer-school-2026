#include <DHTesp.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SOIL_PIN 34
#define DHT_PIN 4

#define RELAY 26
#define RED_LED 25
#define GREEN_LED 27

#define BUTTON 0

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHTesp dht;

void setup() {
  Serial.begin(115200);

  pinMode(RELAY, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  dht.setup(DHT_PIN, DHTesp::DHT11);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
}

void loop() {

  int raw = analogRead(SOIL_PIN);

  // Adjust these values after calibration
  int moisture = map(raw, 3200, 1200, 0, 100);
  moisture = constrain(moisture, 0, 100);

  TempAndHumidity data = dht.getTempAndHumidity();

  // Manual watering
  if (digitalRead(BUTTON) == LOW) {
    digitalWrite(RELAY, LOW);
    delay(5000);
    digitalWrite(RELAY, HIGH);
  }

  // Automatic watering
  if (moisture < 30) {
    digitalWrite(RELAY, LOW);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  }
  else {
    digitalWrite(RELAY, HIGH);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
  }

  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(0,0);
  display.print("Soil: ");
  display.print(moisture);
  display.println("%");

  display.print("Temp: ");
  display.print(data.temperature);
  display.println(" C");

  display.print("Hum : ");
  display.print(data.humidity);
  display.println("%");

  display.display();

  Serial.print("Moisture: ");
  Serial.print(moisture);
  Serial.print("%  Temp: ");
  Serial.print(data.temperature);
  Serial.print(" C");

  Serial.print("  Humidity: ");
  Serial.println(data.humidity);

  delay(2000);
}
