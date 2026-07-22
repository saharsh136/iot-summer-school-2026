## Components
- Arduino Uno
- HC-SR04 Ultrasonic Sensor
- Active Buzzer
- Green, Yellow and Red LEDs

## Connections

HC-SR04
- VCC -> 5V
- GND -> GND
- TRIG -> D9
- ECHO -> D10

Green LED -> D2

Yellow LED -> D3

Red LED -> D4

Buzzer -> D6

## Libraries
No external libraries required.

## Working

- Distance > 60 cm : Green LED ON
- 30–60 cm : Yellow LED + Slow Beep
- 15–30 cm : Yellow + Red LED + Fast Beep
- Distance < 15 cm : Red LED + Continuous Buzzer

## Serial Monitor

<img width="579" height="356" alt="WhatsApp Image 2026-07-20 at 18 25 50" src="https://github.com/user-attachments/assets/5f9336e3-4a37-49c2-8ade-1d72b77df711" />
