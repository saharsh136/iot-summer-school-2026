#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

#define RELAY_PIN 8
#define BUTTON_PIN 3

DHT dht(DHTPIN, DHTTYPE);

bool relayState = false;
bool manualMode = false;
bool lastButton = HIGH;

void setup() {
  Serial.begin(9600);

  dht.begin();

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(RELAY_PIN, LOW);
}

void loop() {

  float temp = dht.readTemperature();

  bool button = digitalRead(BUTTON_PIN);

  if (lastButton == HIGH && button == LOW) {
    manualMode = !manualMode;
    relayState = !relayState;

    digitalWrite(RELAY_PIN, relayState);

    Serial.print("Manual Override -> Relay ");
    Serial.print(relayState ? "ON" : "OFF");
    Serial.print(" | Temp: ");
    Serial.print(temp, 1);
    Serial.println(" C");

    delay(200);
  }

  lastButton = button;

  if (!manualMode && !isnan(temp)) {

    bool prevState = relayState;

    if (temp > 32.0)
      relayState = true;
    else if (temp < 28.0)
      relayState = false;

    if (relayState != prevState) {
      digitalWrite(RELAY_PIN, relayState);

      Serial.print("Relay ");
      Serial.print(relayState ? "ON" : "OFF");
      Serial.print(" | Temp: ");
      Serial.print(temp, 1);
      Serial.println(" C");
    }
  }

  delay(1000);
}
