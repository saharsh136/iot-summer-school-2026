# 🎯 ESP32 Stepper Motor Precision Positioner

A precision motion control system built using an **ESP32** and a **28BYJ-48 stepper motor**. The project demonstrates accurate angular positioning, manual motor control, and real-time position monitoring using push buttons, a potentiometer, and the Serial Monitor.

---

# 📌 Features

- 🎛️ Potentiometer-based Target Angle Selection (0°–360°)
- 🔄 Precision Stepper Motor Positioning
- ➡️ 45° Clockwise Rotation
- ⬅️ 45° Counter-Clockwise Rotation
- 🏠 Home Position Reset
- 📟 Real-Time Position Monitoring on Serial Monitor
- ⚙️ Accurate Angle-to-Step Conversion
- 🚀 ESP32-Based Motion Control

---

# Hardware Used

| Component | Quantity |
|-----------|----------|
| ESP32 Dev Board | 1 |
| 28BYJ-48 Stepper Motor | 1 |
| ULN2003 Driver Module | 1 |
| 10kΩ Potentiometer | 1 |
| Push Buttons | 3 |
| Breadboard | 1 |
| Jumper Wires | As Required |

---

# System Architecture

```
        Potentiometer
              │
              ▼
           ESP32
              │
      Motion Control Logic
              │
      ┌───────┴────────┐
      │                │
      ▼                ▼
 Push Buttons     ULN2003 Driver
      │                │
      └────────► Stepper Motor
                       │
                       ▼
               Serial Monitor
```

---

# Pin Configuration

| Component | ESP32 GPIO |
|------------|-----------|
| ULN2003 IN1 | GPIO18 |
| ULN2003 IN2 | GPIO19 |
| ULN2003 IN3 | GPIO21 |
| ULN2003 IN4 | GPIO22 |
| Potentiometer | GPIO34 |
| CW Button | GPIO25 |
| CCW Button | GPIO26 |
| HOME Button | GPIO27 |

> **Note:** Update the GPIO numbers if your hardware connections are different.

---

# Software Requirements

Install the following libraries from the Arduino IDE Library Manager:

- Stepper *(or AccelStepper if used)*
- ESP32 Arduino Core

---

# Working Principle

The ESP32 continuously reads the potentiometer value and maps it to a target angle between **0° and 360°**.

Push buttons allow manual rotation of the motor or resetting the current position to the home reference.

The current and target angles are continuously displayed on the Serial Monitor.

---

# Functional Overview

## 🎛️ Target Angle Selection

```
Potentiometer
      │
      ▼
Analog Value (0–4095)
      │
      ▼
Map to 0°–360°
```

---

## ➡️ Clockwise Rotation

```
CW Button Pressed
        │
        ▼
Rotate +45°
```

---

## ⬅️ Counter-Clockwise Rotation

```
CCW Button Pressed
        │
        ▼
Rotate -45°
```

---

## 🏠 Home Position

```
HOME Button
      │
      ▼
Reset Current Position to 0°
```

---

# Angle Calculation

For a 28BYJ-48 stepper motor:

- Approximate Steps per Revolution: **2048**

Formula:

```
Steps = (Desired Angle × 2048) / 360
```

Example:

| Angle | Steps |
|-------:|------:|
| 45° | 256 |
| 90° | 512 |
| 180° | 1024 |
| 270° | 1536 |
| 360° | 2048 |

---

# Serial Monitor Output

```
======================================
ESP32 Stepper Precision Positioner
======================================

Target Angle : 180°

Current Angle : 135°

Motor Status : Moving Clockwise

--------------------------------------

Target Angle : 180°

Current Angle : 180°

Motor Status : Target Reached
```

---

# Project Workflow

```
Read Potentiometer
        │
        ▼
Read Push Buttons
        │
        ▼
Calculate Target Position
        │
        ▼
Rotate Stepper Motor
        │
        ▼
Update Current Position
        │
        ▼
Print Status to Serial Monitor
        │
        ▼
Repeat
```

---

# Folder Structure

```
ESP32_Stepper_Positioner/
│
├── ESP32_Stepper_Positioner.ino
├── README.md
├── circuit_diagram.png
├── images/
│   ├── prototype.jpg
│   ├── wiring.jpg
│   └── serial_output.png
└── docs/
    └── project_report.pdf
```

---

# Applications

- Robotic Arm Positioning
- Camera Pan-Tilt Systems
- CNC & Automation Demonstrations
- Smart Mechatronics Projects
- Educational Motion Control Systems
- Industrial Automation Learning

---

# Advantages

- Accurate angular positioning
- Low-cost implementation
- Easy manual operation
- Expandable design
- Ideal for learning stepper motor control
- ESP32 provides higher processing power and future IoT compatibility

---

# Future Improvements

- 📺 OLED Display Integration
- 📱 Wi-Fi Based Remote Control
- 📊 Position Data Logging
- ⚡ AccelStepper for Smooth Motion
- 🎚️ Rotary Encoder Feedback
- 📡 MQTT/Blynk Integration
- 🎯 Closed-Loop Position Control
- 🤖 Robotic Arm Integration

## Serial Monitor
<img width="223" height="156" alt="image" src="https://github.com/user-attachments/assets/c8d85111-9aa8-4359-a000-5be8c2f34fd9" />
