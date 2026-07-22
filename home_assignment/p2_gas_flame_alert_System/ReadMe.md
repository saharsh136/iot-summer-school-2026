# Project 2 - Gas & Fire Safety Alert System

## Components
- Arduino Uno
- MQ2 Gas Sensor
- Flame Sensor
- Active Buzzer
- Red, Yellow and Green LEDs

## Connections

MQ2
- VCC -> 5V
- GND -> GND
- A0 -> A0

Flame Sensor
- VCC -> 5V
- GND -> GND
- DO -> D7

Green LED -> D10

Yellow LED -> D11

Red LED -> D12

Buzzer -> D9

## Libraries
No external libraries required.

## Working

- Gas <30% : Green LED ON
- Gas 30-60% : Yellow LED + Beep
- Gas >60% : Red LED + Continuous Buzzer
- Flame detected : Red LED + Continuous Buzzer
- Status is displayed on Serial Monitor.

## Serial_Monitor
<img width="482" height="372" alt="image" src="https://github.com/user-attachments/assets/95e41328-8e5d-4aad-8cde-24c18fe6d842" />
