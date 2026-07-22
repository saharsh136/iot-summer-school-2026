# 🏠 ESP32 Smart Home Automation Hub

A complete IoT-based Smart Home Automation System built using the ESP32 that monitors environmental conditions and automatically controls household appliances while providing real-time status on an OLED display.

---

## 📌 Features

- 🌡️ Real-time Temperature & Humidity Monitoring
- 🔥 Gas Leakage Detection with Emergency Shutdown
- 🚶 Motion-Based Smart Lighting
- 🌞 Automatic Day/Night Detection using LDR
- 🌀 Intelligent Fan Control with Hysteresis
- 🔘 Manual Override using Push Buttons
- 📺 Live System Status on OLED Display
- 🚨 Audible and Visual Safety Alerts
- ⚡ Dual Relay Control for Home Appliances

---

# Hardware Used

| Component | Quantity |
|-----------|----------|
| ESP32 Dev Board | 1 |
| DHT11 Sensor | 1 |
| MQ-2 Gas Sensor | 1 |
| PIR Motion Sensor | 1 |
| LDR | 1 |
| 10kΩ Resistor | 1 |
| SSD1306 OLED (I2C) | 1 |
| 2-Channel Relay Module | 1 |
| Active Buzzer | 1 |
| Red LED | 1 |
| Green LED | 1 |
| Push Buttons | 2 |
| Jumper Wires | As Required |
| Breadboard | 1 |

---

# System Architecture

```
          Sensors
 ┌─────────────────────────────┐
 │ DHT11                       │
 │ MQ-2                        │
 │ PIR                         │
 │ LDR                         │
 └──────────────┬──────────────┘
                │
                ▼
        ESP32 Decision Engine
                │
    ┌───────────┼────────────┐
    │           │            │
    ▼           ▼            ▼
 Fan Relay  Light Relay   OLED Display
    │
    ▼
 LEDs + Buzzer
```

---

# Pin Configuration

| Component | ESP32 GPIO |
|------------|------------|
| DHT11 | GPIO 4 |
| MQ-2 Analog | GPIO 35 |
| PIR Sensor | GPIO 13 |
| LDR | GPIO 34 |
| Fan Relay | GPIO 26 |
| Light Relay | GPIO 27 |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |
| Buzzer | GPIO 14 |
| Red LED | GPIO 25 |
| Green LED | GPIO 2 |
| Fan Override Button | GPIO 0 |
| Light Override Button | GPIO 32 |

---

# Software Libraries

Install the following Arduino libraries:

- WiFi
- DHTesp
- Adafruit GFX
- Adafruit SSD1306
- Wire
- PubSubClient *(Optional if MQTT is used)*

---

# Automation Logic

## 🌡️ Fan Control

```
Temperature > 32°C
        ↓
     Fan ON

Temperature < 28°C
        ↓
     Fan OFF
```

Hysteresis prevents rapid relay switching near the threshold.

---

## 💡 Smart Lighting

```
Dark Room
     +
Motion Detected
        ↓
   Light ON
```

If no motion is detected for a predefined duration, the light automatically turns OFF.

---

## 🚨 Gas Safety

```
Gas Level Above Threshold
          ↓
     Buzzer ON
     Red LED ON
     Fan OFF
     Light OFF
```

The system enters Emergency Mode until gas levels return to safe values.

---

## 🔘 Manual Override

Dedicated push buttons allow manual control of:

- Fan Relay
- Light Relay

Manual commands temporarily override automatic operation.

---

# OLED Information

The OLED continuously displays:

- Temperature
- Humidity
- Gas Sensor Reading
- Motion Detection Status
- Ambient Light Level
- Fan Status
- Light Status
- System Uptime

---

# Project Flow

```
Read Sensors
      │
      ▼
Process Automation Logic
      │
      ▼
Safety Check
      │
      ▼
Control Relays
      │
      ▼
Update OLED
      │
      ▼
Repeat
```

---

# Folder Structure

```
HomeAutomationHub/
│
├── HomeAutomationHub.ino
├── README.md
├── circuit_diagram.png
├── images/
│   ├── prototype.jpg
│   ├── oled.jpg
│   └── setup.jpg
└── docs/
    └── project_report.pdf
```

---

# Future Improvements

- 📱 Blynk IoT Dashboard
- ☁️ ThingSpeak Cloud Logging
- 📲 Telegram Alerts
- 📊 MQTT Integration
- 🎤 Voice Control
- 📈 Historical Sensor Graphs
- 🔐 User Authentication
- 🏡 Multi-Room Expansion

---

# Applications

- Smart Homes
- Hostels
- Offices
- Laboratories
- Small Industries
- Safety Monitoring Systems

---

# Advantages

- Reduces manual intervention
- Improves household safety
- Energy efficient
- Real-time environmental monitoring
- Modular and expandable design
- Low-cost implementation

---

<img width="384" height="754" alt="image" src="https://github.com/user-attachments/assets/4f801fb9-06f6-4039-aeb4-ccf1365b586e" />
