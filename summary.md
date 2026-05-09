# Project Summary: Components & Sensors

This document provides a concise overview of every hardware component used in the Smart Hub Pro project and its specific purpose in the ecosystem.

## 🧠 Controller
- **ESP32 DevKit V1**: The central processing unit that handles WiFi connectivity, MQTT communication, sensor data processing, and actuator control.

## 🌡️ Environmental Sensors
- **DHT22 (Temp & Humidity)**: Monitors the indoor climate. Used to detect overheating (fire safety) and display comfort levels on the dashboard.
- **Potentiometer (Smoke Simulator)**: Acting as a Gas/Smoke sensor. It measures "Air Health" (AQI). If values are high, it triggers the fire alarm and opens the window.
- **Photoresistor (LDR)**: Detects ambient light intensity. Used for "Auto-Light" logic to turn on the hub's LED when the room gets dark.

## 🛡️ Security & Proximity Sensors
- **PIR Motion Sensor**: Detects infrared heat movement. The primary trigger for intruder alerts when the system is armed.
- **HC-SR04 Ultrasonic Sensor**: Measures distance using sound waves. Used for:
    - **Intruder detection**: If someone is too close to the door.
    - **Visitor detection**: If someone stands at the door for >5 seconds (Doorbell logic).

## ⚙️ Actuators (Outputs)
- **Servo Motor 1 (Door Lock)**: Rotates 0-90 degrees to physically simulate locking and unlocking the main entrance.
- **Servo Motor 2 (Window)**: Automatically opens to 90 degrees when air quality is poor to simulate emergency ventilation.
- **Active Buzzer**: Provides audible feedback. It has distinct tones for Fire (Siren), Intruder (Fast beep), and Panic (Alternating strobe).
- **Blue LED**: Acts as a night light (via LDR) and a high-visibility strobe during emergencies.

## 📺 Feedback & Interface
- **LCD 1602 (I2C)**: A 2-line display that shows real-time Temperature, AQI, and system status (Armed/Venting/Alert) directly on the hardware hub.
- **MQTT Dashboard**: A web-based mobile-friendly interface for remote monitoring and manual control from anywhere in the world.
