## 🔧 Components Used

* ESP32 Development Board
* DHT11 Temperature and Humidity Sensor
* 0.96-inch SSD1306 OLED Display
* Active Buzzer
* Red LED
* Green LED
* 2 × 220Ω Resistors
* 10kΩ Pull-up Resistor
* Breadboard
* Jumper Wires
* USB Cable

---

## 🔌 Wiring Connections

### DHT11 Sensor

| DHT11 Pin | ESP32  |
| --------- | ------ |
| VCC       | 3.3V   |
| GND       | GND    |
| DATA      | GPIO 4 |

A **10kΩ pull-up resistor** is connected between the DHT11 DATA pin and 3.3V.

---

### SSD1306 OLED Display

| OLED Pin | ESP32   |
| -------- | ------- |
| VCC      | 3.3V    |
| GND      | GND     |
| SDA      | GPIO 21 |
| SCL      | GPIO 22 |

OLED I2C Address:

```text
0x3C
```

---

### Red LED

```text
ESP32 GPIO25 → 220Ω Resistor → Red LED Anode
Red LED Cathode → GND
```

---

### Green LED

```text
ESP32 GPIO26 → 220Ω Resistor → Green LED Anode
Green LED Cathode → GND
```

---

### Active Buzzer

```text
Buzzer (+) → ESP32 GPIO27
Buzzer (–) → GND
```

---

## 📚 Libraries Used

The following libraries are required:

### 1. DHTesp

Used to read temperature and humidity from the DHT11 sensor.

```cpp
#include <DHTesp.h>
```

### 2. Adafruit SSD1306

Used to control the OLED display.

```cpp
#include <Adafruit_SSD1306.h>
```

### 3. Adafruit GFX Library

Required by the Adafruit SSD1306 library for graphics and text display.

```cpp
#include <Adafruit_GFX.h>
```

### 4. Wire Library

Used for I2C communication with the OLED display.


### 5. Values is CSV table format

<img width="193" height="121" alt="image" src="https://github.com/user-attachments/assets/f1ac09e4-e883-4b26-811d-a9811c779e6a" />


## Expected Ouput

The system determines the room status based on temperature and humidity.

| Condition                                    | Status  |
| -------------------------------------------- | ------- |
| Temperature below 20°C or humidity below 40% | COOL    |
| Temperature 20–31°C and humidity 40–69%      | COMFORT |
| Temperature ≥ 32°C or humidity ≥ 70%         | HOT     |
| Temperature > 38°C or humidity > 80%         | DANGER  |
