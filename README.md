# Smart Hub Pro: IoT Security & Environment Controller

A comprehensive, ESP32-powered IoT solution for smart home security and environmental automation. This project features a real-time mobile-friendly dashboard, autonomous safety protocols, and cross-platform control via MQTT.

## 🚀 Project Overview
The Smart Hub Pro is designed to provide a "single pane of glass" view of your home's security and air quality. It utilizes a variety of sensors to detect intruders, fires, and poor air quality, taking autonomous actions like locking doors, opening windows, or sounding high-pitch sirens when necessary.

---

## 🛠️ Hardware Architecture (Wokwi Simulation)
The project uses the following components connected to an **ESP32 DevKit V1**:

- **Core**: ESP32 Microcontroller (WiFi & MQTT enabled)
- **Environment**: 
  - `DHT22`: Temperature and Humidity monitoring.
  - `Potentiometer`: Simulates a Smoke/Gas sensor for air quality.
  - `LDR (Photoresistor)`: Ambient light level detection.
- **Security**:
  - `PIR Sensor`: Motion detection for indoor security.
  - `HC-SR04 Ultrasonic`: Distance sensing for visitor detection and door perimeter monitoring.
- **Actuators**:
  - `Servo 1 (Door)`: Physical lock/unlock mechanism.
  - `Servo 2 (Window)`: Autonomous ventilation mechanism.
  - `Buzzer`: High-decibel alarm system.
  - `Blue LED`: Status indicator and emergency strobe.
- **Feedback**:
  - `LCD 1602 (I2C)`: On-device real-time status display.

---

## ✨ Key Features

### 1. Smart Security System
*   **Dual-Sensor Breach Detection**: Uses both PIR (Motion) and Ultrasonic (Proximity) to detect intruders when the system is **Armed**.
*   **Auto-Lock Timer**: When the door is unlocked via the dashboard, the system starts a **30-second countdown**. If no manual intervention occurs, it automatically re-locks the door and re-arms the security for safety.
*   **Visitor "Doorbell" Alert**: If the ultrasonic sensor detects someone standing at the door for more than **5 seconds**, it triggers a "Visitor" notification without sounding the alarm.

### 2. Environmental Automation
*   **Auto-Lighting**: The Blue LED automatically turns ON when ambient light (LDR) drops below a specific threshold and turns OFF during the day.
*   **Auto-Ventilation**: If the Air Quality Index (AQI) becomes poor (**AQI > 40**), the window servo automatically opens to 90° to vent the room. It closes automatically once the air clears.
*   **Manual Overrides**: Any manual toggle on the dashboard (Lights/Window) temporarily disables the automation until "Auto Mode" is re-enabled.

### 3. Safety & Emergency Protocols
*   **Fire Detection**: Triggers immediately if smoke levels are high or temperature exceeds 50°C.
*   **Panic Mode**: A dedicated "Panic Button" on the dashboard that triggers a rapid alternating siren and flashing LED strobe.
*   **Smart Muting**: Users can "Mute" the buzzer from the dashboard. For safety, muting a fire alarm also stops the flashing LED, but the dashboard maintains an orange warning banner until the danger clears.

### 4. Advanced Mobile Dashboard
*   **Responsive UI**: A mobile-first design with tactile switches and high-visibility status badges.
*   **Real-time Analytics**: A 3-line chart (using Chart.js) that tracks **Temperature, Humidity, and Smoke Levels** over a 1-hour window.
*   **Browser Notifications**: Pushes system-level notifications for critical events (Fire/Breach) even if the dashboard is in a background tab.

---

## 📡 MQTT Communication Protocol
The system communicates using the **HiveMQ Public Broker** (`broker.hivemq.com`):

### Topics:
- **`yaksh/smarthub/telemetry`**: (Outbound) Sends JSON data containing all sensor values, AQI, and current alarm states.
- **`yaksh/smarthub/status`**: (Outbound) Syncs the physical state of the Lock, Window, Light, and Panic mode to the dashboard.
- **`yaksh/smarthub/command`**: (Inbound) Receives control strings from the dashboard:
  - `ARM` / `DISARM`
  - `LIGHT_ON` / `LIGHT_OFF` / `AUTO_LIGHT`
  - `WINDOW_OPEN` / `WINDOW_CLOSE` / `AUTO_WINDOW`
  - `PANIC_ON` / `PANIC_OFF`
  - `MUTE_FIRE` / `MUTE_INTRUDER`

---

## 🔌 Pin Mapping (ESP32)
| Component | Pin | Function |
| :--- | :--- | :--- |
| **DHT22** | 15 | Temp/Humidity Data |
| **PIR** | 13 | Motion Detection |
| **LDR** | 34 | Light Level (Analog) |
| **Smoke Sensor** | 32 | Smoke/Gas Level (Analog) |
| **Ultrasonic Trig** | 5 | Trigger Pulse |
| **Ultrasonic Echo** | 18 | Echo Pulse |
| **Buzzer** | 2 | Alarm Output |
| **Blue LED** | 16 | Status/Strobe |
| **Door Servo** | 4 | PWM Lock Control |
| **Window Servo** | 14 | PWM Ventilation Control |
| **LCD SDA/SCL** | 21 / 22 | I2C Communication |

---

## 🛠️ Setup Instructions
1.  **Wokwi Simulation**: Open the project at [Wokwi Smart Hub](https://wokwi.com/projects/462359743307308033).
2.  **Dashboard**: Open `dashboard.html` in any modern web browser (mobile or desktop).
3.  **Connectivity**: Ensure the ESP32 is connected to the `Wokwi-GUEST` network. The dashboard will automatically connect to the MQTT broker and begin displaying data.

---
**Developer Note**: This project prioritizes low-latency feedback and autonomous safety logic, ensuring the home remains secure even if the internet connection to the dashboard is temporarily lost.
