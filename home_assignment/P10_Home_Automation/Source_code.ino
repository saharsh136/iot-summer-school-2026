#include <WiFi.h>
#include <DHTesp.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ================= WIFI =================

const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ================= PIN DEFINITIONS =================

#define DHT_PIN 4
#define GAS_PIN 35
#define LDR_PIN 34
#define PIR_PIN 13

#define FAN_RELAY 26
#define LIGHT_RELAY 27

#define BUZZER_PIN 14

#define RED_LED 25
#define GREEN_LED 2

#define FAN_BUTTON 0
#define LIGHT_BUTTON 32

// ================= OLED =================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

// ================= DHT =================

DHTesp dht;

// ================= SENSOR VARIABLES =================

float temperature = 0;
float humidity = 0;

int gasLevel = 0;
int lightLevel = 0;
int pirStatus = 0;

// ================= SYSTEM STATES =================

bool fanOn = false;
bool lightOn = false;
bool gasAlert = false;

bool fanManualOverride = false;
bool lightManualOverride = false;

// ================= TIMERS =================

unsigned long lastSensorRead = 0;
unsigned long lastOLEDUpdate = 0;

unsigned long lastWiFiAttempt = 0;

unsigned long lastMotionTime = 0;

unsigned long fanOverrideStart = 0;
unsigned long lightOverrideStart = 0;

// ================= TIME INTERVALS =================

const unsigned long SENSOR_INTERVAL = 5000;

const unsigned long OLED_INTERVAL = 5000;

const unsigned long WIFI_RETRY_INTERVAL = 30000;

const unsigned long MANUAL_OVERRIDE_TIME = 600000;

const unsigned long NO_MOTION_TIME = 180000;

// OLED screen number

int oledScreen = 0;

// =====================================================
// FUNCTION DECLARATIONS
// =====================================================

void connectWiFi();

void readSensors();

void applyAutomationRules();

void setFan(bool state);

void setLight(bool state);

void checkButtons();

void updateOLED();

String getUptime();

// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  // -----------------------------
  // PIN MODES
  // -----------------------------

  pinMode(GAS_PIN, INPUT);

  pinMode(LDR_PIN, INPUT);

  pinMode(PIR_PIN, INPUT);

  pinMode(FAN_RELAY, OUTPUT);

  pinMode(LIGHT_RELAY, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(RED_LED, OUTPUT);

  pinMode(GREEN_LED, OUTPUT);

  pinMode(FAN_BUTTON, INPUT_PULLUP);

  pinMode(LIGHT_BUTTON, INPUT_PULLUP);

  // -----------------------------
  // RELAYS OFF
  // ACTIVE LOW
  // HIGH = OFF
  // LOW = ON
  // -----------------------------

  digitalWrite(
    FAN_RELAY,
    HIGH
  );

  digitalWrite(
    LIGHT_RELAY,
    HIGH
  );

  // -----------------------------
  // ALARM OFF
  // -----------------------------

  digitalWrite(
    BUZZER_PIN,
    LOW
  );

  digitalWrite(
    RED_LED,
    LOW
  );

  digitalWrite(
    GREEN_LED,
    HIGH
  );

  // -----------------------------
  // DHT INITIALIZATION
  // -----------------------------

  dht.setup(
    DHT_PIN,
    DHTesp::DHT22
  );

  // -----------------------------
  // OLED INITIALIZATION
  // -----------------------------

  Wire.begin(
    21,
    22
  );

  if (
    !display.begin(
      SSD1306_SWITCHCAPVCC,
      0x3C
    )
  ) {

    Serial.println(
      "OLED not found"
    );
  }

  display.clearDisplay();

  display.setTextSize(1);

  display.setTextColor(
    SSD1306_WHITE
  );

  display.setCursor(
    0,
    0
  );

  display.println(
    "HOME AUTOMATION"
  );

  display.setCursor(
    0,
    20
  );

  display.println(
    "Starting..."
  );

  display.display();

  delay(2000);

  // -----------------------------
  // WIFI
  // -----------------------------

  connectWiFi();

  Serial.println();

  Serial.println(
    "HOME AUTOMATION HUB READY"
  );
}

// =====================================================
// LOOP
// =====================================================

void loop() {

  unsigned long currentMillis =
    millis();

  // =================================================
  // WIFI RECONNECTION
  // =================================================

  if (
    WiFi.status() != WL_CONNECTED
  ) {

    digitalWrite(
      GREEN_LED,
      LOW
    );

    if (
      currentMillis -
      lastWiFiAttempt
      >= WIFI_RETRY_INTERVAL
    ) {

      Serial.println(
        "WiFi disconnected"
      );

      WiFi.disconnect();

      WiFi.begin(
        ssid,
        password
      );

      lastWiFiAttempt =
        currentMillis;
    }
  }

  else {

    if (
      !gasAlert
    ) {

      digitalWrite(
        GREEN_LED,
        HIGH
      );
    }
  }

  // =================================================
  // BUTTONS
  // =================================================

  checkButtons();

  // =================================================
  // READ SENSORS EVERY 5 SECONDS
  // =================================================

  if (
    currentMillis -
    lastSensorRead
    >= SENSOR_INTERVAL
  ) {

    readSensors();

    applyAutomationRules();

    lastSensorRead =
      currentMillis;
  }

  // =================================================
  // OLED UPDATE
  // =================================================

  if (
    currentMillis -
    lastOLEDUpdate
    >= OLED_INTERVAL
  ) {

    updateOLED();

    lastOLEDUpdate =
      currentMillis;
  }
}

// =====================================================
// WIFI CONNECTION
// =====================================================

void connectWiFi() {

  Serial.print(
    "Connecting to WiFi"
  );

  WiFi.begin(
    ssid,
    password
  );

  unsigned long startTime =
    millis();

  while (
    WiFi.status() != WL_CONNECTED &&
    millis() - startTime < 15000
  ) {

    delay(500);

    Serial.print(
      "."
    );
  }

  Serial.println();

  if (
    WiFi.status() == WL_CONNECTED
  ) {

    Serial.println(
      "WiFi Connected"
    );

    Serial.print(
      "IP Address: "
    );

    Serial.println(
      WiFi.localIP()
    );
  }

  else {

    Serial.println(
      "WiFi connection failed"
    );
  }
}

// =====================================================
// READ SENSORS
// =====================================================

void readSensors() {

  // -----------------------------
  // DHT22
  // -----------------------------

  TempAndHumidity data =
    dht.getTempAndHumidity();

  if (
    !isnan(
      data.temperature
    ) &&
    !isnan(
      data.humidity
    )
  ) {

    temperature =
      data.temperature;

    humidity =
      data.humidity;
  }

  // -----------------------------
  // GAS SENSOR SIMULATION
  // POTENTIOMETER
  // -----------------------------

  int gasRaw =
    analogRead(
      GAS_PIN
    );

  gasLevel =
    map(
      gasRaw,
      0,
      4095,
      0,
      100
    );

  // -----------------------------
  // LDR
  // -----------------------------

  int ldrRaw =
    analogRead(
      LDR_PIN
    );

  lightLevel =
    map(
      ldrRaw,
      0,
      4095,
      0,
      100
    );

  // -----------------------------
  // PIR
  // -----------------------------

  pirStatus =
    digitalRead(
      PIR_PIN
    );

  if (
    pirStatus == HIGH
  ) {

    lastMotionTime =
      millis();
  }

  // -----------------------------
  // SERIAL MONITOR
  // -----------------------------

  Serial.println();

  Serial.println(
    "========== SENSOR DATA =========="
  );

  Serial.print(
    "Temperature: "
  );

  Serial.print(
    temperature
  );

  Serial.println(
    " C"
  );

  Serial.print(
    "Humidity: "
  );

  Serial.print(
    humidity
  );

  Serial.println(
    " %"
  );

  Serial.print(
    "Gas: "
  );

  Serial.print(
    gasLevel
  );

  Serial.println(
    " %"
  );

  Serial.print(
    "Light: "
  );

  Serial.print(
    lightLevel
  );

  Serial.println(
    " %"
  );

  Serial.print(
    "PIR: "
  );

  if (
    pirStatus == HIGH
  ) {

    Serial.println(
      "MOTION"
    );
  }

  else {

    Serial.println(
      "NO MOTION"
    );
  }

  Serial.println(
    "================================="
  );
}

// =====================================================
// AUTOMATION RULES
// =====================================================

void applyAutomationRules() {

  // =================================================
  // GAS ALERT
  // =================================================

  if (
    gasLevel > 60
  ) {

    gasAlert = true;

    // SAFETY SHUTDOWN

    setFan(
      false
    );

    setLight(
      false
    );

    // BUZZER ON

    digitalWrite(
      BUZZER_PIN,
      HIGH
    );

    // RED LED ON

    digitalWrite(
      RED_LED,
      HIGH
    );

    // GREEN LED OFF

    digitalWrite(
      GREEN_LED,
      LOW
    );

    Serial.println(
      "!!! GAS ALERT !!!"
    );

    return;
  }

  else {

    gasAlert = false;

    digitalWrite(
      BUZZER_PIN,
      LOW
    );

    digitalWrite(
      RED_LED,
      LOW
    );

    digitalWrite(
      GREEN_LED,
      HIGH
    );
  }

  // =================================================
  // FAN MANUAL OVERRIDE TIMEOUT
  // =================================================

  if (
    fanManualOverride &&
    millis() -
    fanOverrideStart
    >= MANUAL_OVERRIDE_TIME
  ) {

    fanManualOverride =
      false;

    Serial.println(
      "Fan override expired"
    );
  }

  // =================================================
  // LIGHT MANUAL OVERRIDE TIMEOUT
  // =================================================

  if (
    lightManualOverride &&
    millis() -
    lightOverrideStart
    >= MANUAL_OVERRIDE_TIME
  ) {

    lightManualOverride =
      false;

    Serial.println(
      "Light override expired"
    );
  }

  // =================================================
  // FAN AUTOMATION
  // =================================================

  if (
    !fanManualOverride
  ) {

    // Temperature above 32°C

    if (
      !fanOn &&
      temperature > 32
    ) {

      setFan(
        true
      );

      Serial.println(
        "FAN AUTO ON"
      );
    }

    // Temperature below 28°C

    if (
      fanOn &&
      temperature < 28
    ) {

      setFan(
        false
      );

      Serial.println(
        "FAN AUTO OFF"
      );
    }
  }

  // =================================================
  // LIGHT AUTOMATION
  // =================================================

  if (
    !lightManualOverride
  ) {

    // DARK + MOTION

    if (
      lightLevel < 20 &&
      pirStatus == HIGH
    ) {

      setLight(
        true
      );

      Serial.println(
        "LIGHT AUTO ON"
      );
    }

    // DAYLIGHT

    if (
      lightLevel > 60
    ) {

      setLight(
        false
      );

      Serial.println(
        "LIGHT AUTO OFF"
      );
    }

    // NO MOTION FOR 3 MINUTES

    if (
      millis() -
      lastMotionTime
      >= NO_MOTION_TIME
    ) {

      setLight(
        false
      );

      Serial.println(
        "LIGHT OFF - NO MOTION"
      );
    }
  }
}

// =====================================================
// FAN CONTROL
// =====================================================

void setFan(
  bool state
) {

  fanOn =
    state;

  if (
    state
  ) {

    digitalWrite(
      FAN_RELAY,
      LOW
    );
  }

  else {

    digitalWrite(
      FAN_RELAY,
      HIGH
    );
  }
}

// =====================================================
// LIGHT CONTROL
// =====================================================

void setLight(
  bool state
) {

  lightOn =
    state;

  if (
    state
  ) {

    digitalWrite(
      LIGHT_RELAY,
      LOW
    );
  }

  else {

    digitalWrite(
      LIGHT_RELAY,
      HIGH
    );
  }
}

// =====================================================
// BUTTON CONTROL
// =====================================================

void checkButtons() {

  static bool previousFanButton =
    HIGH;

  static bool previousLightButton =
    HIGH;

  bool currentFanButton =
    digitalRead(
      FAN_BUTTON
    );

  bool currentLightButton =
    digitalRead(
      LIGHT_BUTTON
    );

  // -----------------------------
  // FAN BUTTON
  // -----------------------------

  if (
    previousFanButton == HIGH &&
    currentFanButton == LOW
  ) {

    if (
      !gasAlert
    ) {

      setFan(
        !fanOn
      );

      fanManualOverride =
        true;

      fanOverrideStart =
        millis();

      Serial.println(
        "FAN MANUAL OVERRIDE"
      );
    }

    delay(200);
  }

  // -----------------------------
  // LIGHT BUTTON
  // -----------------------------

  if (
    previousLightButton == HIGH &&
    currentLightButton == LOW
  ) {

    if (
      !gasAlert
    ) {

      setLight(
        !lightOn
      );

      lightManualOverride =
        true;

      lightOverrideStart =
        millis();

      Serial.println(
        "LIGHT MANUAL OVERRIDE"
      );
    }

    delay(200);
  }

  previousFanButton =
    currentFanButton;

  previousLightButton =
    currentLightButton;
}

// =====================================================
// OLED DISPLAY
// =====================================================

void updateOLED() {

  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(
    0,
    0
  );

  display.println(
    "HOME AUTOMATION"
  );

  display.drawLine(
    0,
    10,
    128,
    10,
    SSD1306_WHITE
  );

  // =================================================
  // SCREEN 1
  // =================================================

  if (
    oledScreen == 0
  ) {

    display.setCursor(
      0,
      18
    );

    display.print(
      "Temp: "
    );

    display.print(
      temperature,
      1
    );

    display.println(
      " C"
    );

    display.setCursor(
      0,
      35
    );

    display.print(
      "Humidity: "
    );

    display.print(
      humidity,
      1
    );

    display.println(
      " %"
    );
  }

  // =================================================
  // SCREEN 2
  // =================================================

  else if (
    oledScreen == 1
  ) {

    display.setCursor(
      0,
      18
    );

    display.print(
      "Gas: "
    );

    display.print(
      gasLevel
    );

    display.println(
      " %"
    );

    display.setCursor(
      0,
      32
    );

    display.print(
      "Light: "
    );

    display.print(
      lightLevel
    );

    display.println(
      " %"
    );

    display.setCursor(
      0,
      46
    );

    display.print(
      "PIR: "
    );

    if (
      pirStatus == HIGH
    ) {

      display.println(
        "MOTION"
      );
    }

    else {

      display.println(
        "NO MOTION"
      );
    }
  }

  // =================================================
  // SCREEN 3
  // =================================================

  else {

    display.setCursor(
      0,
      18
    );

    display.print(
      "Fan: "
    );

    display.println(
      fanOn
      ? "ON"
      : "OFF"
    );

    display.setCursor(
      65,
      18
    );

    display.print(
      "Light: "
    );

    display.println(
      lightOn
      ? "ON"
      : "OFF"
    );

    display.setCursor(
      0,
      35
    );

    display.println(
      "Uptime:"
    );

    display.setCursor(
      0,
      49
    );

    display.println(
      getUptime()
    );
  }

  display.display();

  oledScreen++;

  if (
    oledScreen > 2
  ) {

    oledScreen = 0;
  }
}

// =====================================================
// UPTIME
// =====================================================

String getUptime() {

  unsigned long totalSeconds =
    millis() / 1000;

  unsigned long days =
    totalSeconds / 86400;

  unsigned long hours =
    (
      totalSeconds % 86400
    ) / 3600;

  unsigned long minutes =
    (
      totalSeconds % 3600
    ) / 60;

  unsigned long seconds =
    totalSeconds % 60;

  String result = "";

  result += String(
    days
  );

  result += "d ";

  if (
    hours < 10
  ) {

    result += "0";
  }

  result += String(
    hours
  );

  result += "h ";

  if (
    minutes < 10
  ) {

    result += "0";
  }

  result += String(
    minutes
  );

  result += "m ";

  if (
    seconds < 10
  ) {

    result += "0";
  }

  result += String(
    seconds
  );

  result += "s";

  return result;
}
