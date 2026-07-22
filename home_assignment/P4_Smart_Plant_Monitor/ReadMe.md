## Components

- ESP32
- Capacitive Soil Moisture Sensor
- DHT11
- SSD1306 OLED
- Relay Module
- Red LED
- Green LED
- Push Button

## Connections

### Soil Sensor
- VCC -> 3.3V
- GND -> GND
- AO -> GPIO34

### DHT11
- VCC -> 3.3V
- GND -> GND
- DATA -> GPIO4

### OLED
- SDA -> GPIO21
- SCL -> GPIO22
- VCC -> 3.3V
- GND -> GND

### Relay
- IN -> GPIO26

### LEDs
- Red -> GPIO25
- Green -> GPIO27

### Push Button
- One side -> GPIO0
- Other side -> GND

## Libraries

- DHTesp
- Adafruit GFX
- Adafruit SSD1306

## Working

- Reads soil moisture and temperature.
- Displays values on the OLED.
- Turns ON the relay when soil moisture is below 30%.
- Green LED indicates normal condition.
- Red LED indicates dry soil.
- Push button manually runs the pump (relay) for 5 seconds.
- 
## Serial_Montior

<img width="523" height="180" alt="WhatsApp Image 2026-07-20 at 18 22 30" src="https://github.com/user-attachments/assets/4f7e3c66-d72d-414c-9c21-0fb74bd3fecd" />
