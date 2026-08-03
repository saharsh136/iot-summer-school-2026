# Smart Home Automation System using HTTP Client–Server Integration

## Project Overview

The *Smart Home Automation System using HTTP Client–Server Integration* is an IoT-based project developed as part of the *IoT and Drones Online Internship at IIT Jammu* under the guidance of *Majid Sir*.

The system is designed to monitor environmental conditions and automate home appliances in real time using *ESP32* microcontrollers and HTTP communication over Wi-Fi. It continuously collects data from multiple sensors and displays the information on both an OLED display and a web-based dashboard.

The project integrates safety and automation features such as motion detection, gas leakage detection, flame detection, live temperature and humidity monitoring, automatic fan control, and audio-visual alerts.

---

# Components Used

## Hardware

* 2 × ESP32 Dev Module (38-pin)
* DHT11 Temperature and Humidity Sensor
* PIR Motion Sensor
* MQ Gas Sensor
* Flame Sensor
* Relay Module
* Stepper Motor
* Small DC Fan
* SSD1306 OLED Display (0.96 inch)
* Active Buzzer
* Red LED
* Green LED
* Breadboard
* Jumper Wires
* USB Power Supply

## Software

* Arduino IDE
* ESP32 Board Package
* Embedded C/C++
* Wi-Fi Library
* HTTP Client Library
* HTML
* CSS
* JavaScript

---

# Setup and Installation Instructions

## 1. Install Arduino IDE

Download and install the latest version of Arduino IDE.

## 2. Install ESP32 Board Support

* Open *Boards Manager*.
* Search for *ESP32*.
* Install the ESP32 board package.

## 3. Install Required Libraries

Install the following libraries through the Library Manager:

* WiFi
* HTTPClient
* Adafruit GFX
* Adafruit SSD1306
* DHT Sensor Library
* Adafruit Unified Sensor
* Stepper Library (if required)

## 4. Hardware Connections

Connect all sensors and modules to the ESP32 boards according to the circuit diagram:

* DHT11
* PIR Sensor
* MQ Gas Sensor
* Flame Sensor
* Relay Module
* Stepper Motor
* OLED Display
* LEDs
* Buzzer
* Fan

## 5. Configure Wi-Fi

Wifi Credentials is given to the code.


Update the server URL/IP address used for HTTP communication.

## 6. Upload the Code

* Select the correct ESP32 board.
* Choose the correct COM port.
* Upload the firmware to both ESP32 boards.

## 7. Start the Server

Run the HTTP server and ensure both ESP32 devices are connected to the same network.

## 8. Open the Dashboard

Open the web dashboard in a browser to monitor live sensor data and system status.

---

# Working Methodology

1. ESP32 connects to the local Wi-Fi network.
2. DHT11 continuously measures temperature and humidity.
3. PIR sensor detects human motion.
4. When motion is detected:

   * The buzzer beeps.
   * The stepper motor operates.
   * The relay switches ON the connected fan.
5. MQ Gas Sensor continuously monitors gas concentration.
6. Flame Sensor detects the presence of fire.
7. During hazardous conditions:

   * Red LED turns ON.
   * Buzzer alerts the user.
   * Dashboard displays a danger notification.
8. During normal conditions:

   * Green LED remains ON.
   * Dashboard indicates SAFE status.
9. Sensor readings are transmitted using HTTP requests.
10. The web dashboard updates live values including:

    * Temperature
    * Humidity
    * Motion Status
    * Gas Status
    * Flame Status
    * Fan Status
11. The OLED display shows important system information locally.

---

# Team Members

This project was developed by:

* *Anant Priyadarshi*
* *Saharsh Mishra*

*Internship:* IoT and Drones Online Internship, IIT Jammu

*Project Guide:* Majid Sir

---

# Additional Information

## Key Features

* Real-time Temperature Monitoring
* Real-time Humidity Monitoring
* Motion Detection
* Gas Leakage Detection
* Flame Detection
* Automatic Fan Control
* Relay-Based Appliance Switching
* Stepper Motor Automation
* OLED Status Display
* Audio and Visual Alerts
* HTTP Client–Server Communication
* Live Web Dashboard

## Applications

* Smart Home Automation
* Industrial Safety Monitoring
* Fire Detection Systems
* Gas Leakage Detection
* Energy-Efficient Homes
* Elderly Assistance Systems
* Educational IoT Projects

## Future Enhancements

* Mobile Application Integration
* MQTT-Based Communication
* Cloud Data Storage
* Push Notifications
* Voice Assistant Integration
* AI-Based Predictive Automation
* Remote Access Over the Internet
* Multi-Room Smart Home Expansion
