# LED Blink Project

## Project Title

LED Blink using Arduino

---

## Hardware Required

- Arduino Uno
- LED
- 220Ω Resistor
- Breadboard
- Jumper Wires
- USB Cable
- 10kΩ Potentiometer (used in Version 1.2)

---

## Circuit Connections

### LED

- Arduino Pin 13 → 220Ω Resistor → LED (+)
- LED (−) → GND

### Potentiometer

- One side → 5V
- Other side → GND
- Middle pin → A0

---

## How to Upload the Code

1. Connect the Arduino board to your computer.
2. Open the Arduino IDE.
3. Open the `blink.ino` file.
4. Select **Arduino Uno** from the **Tools > Board** menu.
5. Select the correct COM Port.
6. Click **Verify** to compile the code.
7. Click **Upload**.
8. After uploading, the program will start automatically.
9. Open the Serial Monitor (9600 baud) if using Version 1.1 or later.

---

## Expected Output

- The LED blinks continuously.
- The Serial Monitor displays the blink count.
- In Version 1.2, rotating the potentiometer changes the blinking speed.

---

## Troubleshooting Tips

- Check that the correct COM port is selected.
- Make sure the LED is connected in the correct direction.
- Verify all jumper wire connections.
- Check that the potentiometer is connected to A0.
- If upload fails, press the Reset button and try again.

---

## Author

Saharsh Mishra

IoT Summer School 2026
