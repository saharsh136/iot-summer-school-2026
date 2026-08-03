/*
=========================================
ESP32 Fire & Gas Automation System
=========================================

Relay 1 -> Exhaust Fan
Relay 2 -> Emergency Pump / Light

Logic:

Gas only
Fan ON

Fire only
Emergency ON

Gas + Fire
Fan OFF
Emergency ON
=========================================
*/

#include <WiFi.h>
#include <HTTPClient.h>

// ================= WIFI + SERVER CONFIG =================
// Set these to match your network and your laptop's IP address.
// Find your laptop's IP with `ipconfig` (Windows) or `ifconfig`/`ip addr` (Mac/Linux).
// The ESP32 and your laptop must be on the SAME WiFi network.
const char* WIFI_SSID     = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const char* SERVER_URL    = "http://192.168.1.100:3000/api/update/gas"; // <-- change IP
// ==========================================================

#define MQ2_PIN        34
#define FLAME_PIN      27

#define RELAY1_PIN     25
#define RELAY2_PIN     26

// Adjust after observing Serial Monitor
const int GAS_THRESHOLD = 1800;

unsigned long lastPost = 0;
const unsigned long POST_INTERVAL_MS = 500; // matches the existing loop delay -> real-time feed
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

void sendData(int gasValue, bool gasLeak, bool flame, const char* status) {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.begin(SERVER_URL);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(2000); // don't let a slow/dead server block the safety loop for long

  String payload = "{";
  payload += "\"gasValue\":" + String(gasValue) + ",";
  payload += "\"gasLeak\":" + String(gasLeak ? "true" : "false") + ",";
  payload += "\"flame\":" + String(flame ? "true" : "false") + ",";
  payload += "\"status\":\"" + String(status) + "\"";
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

    pinMode(FLAME_PIN, INPUT);

    pinMode(RELAY1_PIN, OUTPUT);
    pinMode(RELAY2_PIN, OUTPUT);

    // Relay OFF (Most relay modules are Active LOW)
    digitalWrite(RELAY1_PIN, HIGH);
    digitalWrite(RELAY2_PIN, HIGH);

    Serial.println("Fire & Gas Automation Started");

    connectWiFi();
}

void loop()
{
    int gasValue = analogRead(MQ2_PIN);

    // Most flame sensors:
    // LOW = Flame detected
    // HIGH = No flame
    bool flameDetected = (digitalRead(FLAME_PIN) == LOW);

    bool gasLeak = (gasValue > GAS_THRESHOLD);

    Serial.print("Gas Value : ");
    Serial.print(gasValue);

    Serial.print("    Flame : ");

    if(flameDetected)
        Serial.print("YES");
    else
        Serial.print("NO");

    Serial.print("    Status : ");

    const char* statusText;

    // ----------------------------
    // MANUAL OVERRIDE (dashboard Disconnect button)
    // NOTE: this is a safety system (gas + fire). Disabling it means the
    // fan/emergency relays will NOT respond to a real gas leak or flame.
    // Only use this for testing - don't leave it disconnected in normal use.
    // ----------------------------
    if(!actuatorsEnabled)
    {
        digitalWrite(RELAY1_PIN, HIGH);   // relays forced to safe/idle state
        digitalWrite(RELAY2_PIN, HIGH);

        statusText = "PAUSED (dashboard override)";
        Serial.println(statusText);
    }

    // ----------------------------
    // CASE 1
    // Normal
    // ----------------------------
    else if(!gasLeak && !flameDetected)
    {
        digitalWrite(RELAY1_PIN, HIGH);
        digitalWrite(RELAY2_PIN, HIGH);

        statusText = "NORMAL";
        Serial.println(statusText);
    }

    // ----------------------------
    // CASE 2
    // Gas Leak Only
    // ----------------------------
    else if(gasLeak && !flameDetected)
    {
        digitalWrite(RELAY1_PIN, LOW);      // Fan ON
        digitalWrite(RELAY2_PIN, HIGH);     // Emergency OFF

        statusText = "GAS LEAK -> FAN ON";
        Serial.println(statusText);
    }

    // ----------------------------
    // CASE 3
    // Fire Only
    // ----------------------------
    else if(!gasLeak && flameDetected)
    {
        digitalWrite(RELAY1_PIN, HIGH);     // Fan OFF
        digitalWrite(RELAY2_PIN, LOW);      // Emergency ON

        statusText = "FIRE -> EMERGENCY ON";
        Serial.println(statusText);
    }

    // ----------------------------
    // CASE 4
    // Fire + Gas
    // ----------------------------
    else
    {
        digitalWrite(RELAY1_PIN, HIGH);     // Fan OFF
        digitalWrite(RELAY2_PIN, LOW);      // Emergency ON

        statusText = "EMERGENCY: GAS + FIRE";
        Serial.println(statusText);
    }

    // ---- Send live data to the dashboard server ----
    if (millis() - lastPost >= POST_INTERVAL_MS)
    {
        lastPost = millis();
        sendData(gasValue, gasLeak, flameDetected, statusText);
    }

    delay(500);
}
