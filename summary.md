# Smart Hub: Home Automation — Component Summary

This document provides a concise overview of every hardware component used in the Smart Hub: Home Automation project and its specific purpose in the ecosystem.

## 🧠 Controller
- **ESP32 DevKit V1**: The central processing unit that handles WiFi connectivity, MQTT communication, sensor data processing, and actuator control. All autonomous safety logic runs locally on this chip — no cloud dependency.

## 🌡️ Environmental Sensors
- **DHT22 (Temp & Humidity)**: Monitors the indoor climate. Used to detect overheating (fire safety trigger at >50°C) and display comfort levels on both the LCD and the remote dashboard.
- **Potentiometer (Smoke Simulator)**: Simulates a gas/smoke sensor in the Wokwi environment. Measures "Air Health" (AQI 0–100). If values exceed the threshold, it triggers the fire alarm and auto-opens the window for emergency ventilation.
- **Photoresistor (LDR)**: Detects ambient light intensity. Drives the "Auto-Light" logic — the LED turns ON when the room gets dark and OFF during daytime. Can be overridden manually from the dashboard.

## 🛡️ Security & Proximity Sensors
- **PIR Motion Sensor**: Detects infrared heat movement from humans. The primary trigger for intruder alerts when the system is in Armed mode.
- **HC-SR04 Ultrasonic Sensor**: Measures distance using sound waves. Serves dual purposes:
    - **Intruder detection**: Triggers breach alert if an object is within 50cm when armed.
    - **Visitor detection**: If someone stands at the door for >5 seconds continuously, it triggers a "Visitor at Door" notification instead of a full alarm.

## ⚙️ Actuators (Outputs)
- **Servo Motor 1 (Door Lock)**: Rotates 0–90° to physically simulate locking and unlocking the main entrance. Features a 30-second auto-relock timer after being unlocked remotely.
- **Servo Motor 2 (Window)**: Automatically opens to 90° when air quality is poor (AQI > 40) to simulate emergency ventilation. Closes automatically when air quality improves.
- **Active Buzzer**: Provides audible alarm feedback with distinct tones per alert type:
    - Fire: Continuous 2kHz siren
    - Intruder: 1kHz fast beep
    - Panic: Alternating 1.5kHz / 3kHz strobe pattern
- **Blue LED**: Dual-purpose — acts as an automated night light (via LDR sensor) during normal operation and a high-visibility emergency strobe during fire/panic events.

## 📺 Feedback & Interface
- **LCD 1602 (I2C)**: A 2-line on-device display showing real-time temperature, AQI, and system status (Armed / Disarmed / FIRE ALERT / BREACH / Visitor at Door / PANIC) directly on the hardware hub.
- **MQTT Web Dashboard**: A mobile-first web interface (`dashboard.html`) for remote monitoring and control. Features real-time telemetry, toggle switches, alert banners, a panic button, and live Chart.js graphs — accessible from anywhere via MQTT over WebSocket.
