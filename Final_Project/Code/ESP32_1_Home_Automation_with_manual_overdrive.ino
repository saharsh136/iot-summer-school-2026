#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Stepper.h>
#include <WiFi.h>
#include <HTTPClient.h>

// ================= WIFI + SERVER CONFIG =================
// Set these to match your network and your laptop's IP address.
// Find your laptop's IP with `ipconfig` (Windows) or `ifconfig`/`ip addr` (Mac/Linux).
// The ESP32 and your laptop must be on the SAME WiFi network.
const char* WIFI_SSID     = "OnePlus";
const char* WIFI_PASSWORD = "6789012345";
const char* SERVER_URL    = "http://10.168.142.217:3000/api/update/home"; // <-- change IP
// ==========================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DHT11
#define DHTPIN 4
#define DHTTYPE DHT11

// PIR
#define PIR_PIN 27

// LEDs
#define LED1 26
#define LED2 25

// Buzzer
#define BUZZER 33

DHT dht(DHTPIN, DHTTYPE);

// Stepper
const int stepsPerRevolution = 2048;
Stepper fanStepper(stepsPerRevolution, 18, 23, 19, 5);

float temp = 0;
float hum = 0;

unsigned long lastDHTRead = 0;
unsigned long lastPost = 0;
const unsigned long POST_INTERVAL_MS = 1000; // send data every 1s -> low latency, won't flood the server

bool previousMotion = false;
bool actuatorsEnabled = true; // set by the dashboard's Connect/Disconnect override buttons

void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

void sendData(float t, float h, bool motion, bool fan) {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.begin(SERVER_URL);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(2000); // don't let a slow/dead server block the loop for long

  // Small, dependency-free JSON build (avoids needing ArduinoJson)
  String payload = "{";
  payload += "\"temp\":" + String(t, 1) + ",";
  payload += "\"hum\":" + String(h, 1) + ",";
  payload += "\"motion\":" + String(motion ? "true" : "false") + ",";
  payload += "\"fan\":" + String(fan ? "true" : "false");
  payload += "}";

  int code = http.POST(payload);
  if (code > 0) {
    String response = http.getString();
    // Simple substring check for "enabled":true/false (no ArduinoJson dependency).
    // The dashboard's Connect/Disconnect buttons control this flag on the server.
    if (response.indexOf("\"enabled\":false") != -1) {
      actuatorsEnabled = false;
    } else if (response.indexOf("\"enabled\":true") != -1) {
      actuatorsEnabled = true;
    }
  } else {
    Serial.print("HTTP POST failed: ");
    Serial.println(http.errorToString(code));
  }
  http.end();
}

void setup()
{
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(BUZZER, LOW);

  fanStepper.setSpeed(12);

  dht.begin();

  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED Failed");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  display.setCursor(10, 20);
  display.println("SMART");

  display.setCursor(18, 42);
  display.println("HOME");

  display.display();

  delay(2000);

  connectWiFi();
}

void loop()
{
  if (millis() - lastDHTRead >= 2000)
  {
    lastDHTRead = millis();

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t) && !isnan(h))
    {
      temp = t;
      hum = h;
    }
  }

  int motion = digitalRead(PIR_PIN);
  bool fanOn = false;

  if (!actuatorsEnabled)
  {
    // Manually disconnected from the dashboard: force everything to a safe, idle state.
    // Sensors keep being read and reported so the board can be reconnected remotely.
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(BUZZER, LOW);
  }
  else if (motion)
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);

    if (!previousMotion)
    {
      digitalWrite(BUZZER, HIGH);
      delay(100);
      digitalWrite(BUZZER, LOW);
    }

    // Fan rotates
    fanStepper.step(50);
    fanOn = true;
  }
  else
  {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
  }

  previousMotion = motion;

  Serial.println("-------------------------");

  Serial.print("Temperature : ");
  Serial.print(temp);
  Serial.println(" C");

  Serial.print("Humidity : ");
  Serial.print(hum);
  Serial.println(" %");

  Serial.print("Motion : ");
  Serial.println(motion ? "Detected" : "No Motion");

  Serial.print("Fan : ");
  Serial.println(fanOn ? "ON" : "OFF");

  if (!actuatorsEnabled) {
    Serial.println("** MANUALLY DISCONNECTED FROM DASHBOARD - ACTUATORS PAUSED **");
  }

  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("SMART HOME");

  display.setCursor(0, 14);
  display.print("Temp : ");
  display.print(temp);
  display.print(" C");

  display.setCursor(0, 26);
  display.print("Hum : ");
  display.print(hum);
  display.print(" %");

  display.setCursor(0, 38);
  display.print("Motion : ");
  display.print(motion ? "YES" : "NO");

  display.setCursor(0, 50);
  display.print("Fan : ");
  display.print(fanOn ? "ON" : "OFF");
  if (!actuatorsEnabled) {
    display.print(" (PAUSED)");
  }

  display.display();

  // ---- Send live data to the dashboard server ----
  if (millis() - lastPost >= POST_INTERVAL_MS)
  {
    lastPost = millis();
    sendData(temp, hum, motion, fanOn);
  }

  delay(100);
}
