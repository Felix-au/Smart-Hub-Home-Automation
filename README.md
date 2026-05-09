<h1 align="center">🏠 Smart Hub: Home Automation</h1>
<p align="center">
  <strong>ESP32-powered IoT security &amp; environmental automation with a real-time mobile dashboard</strong><br/>
  <em>Autonomous safety protocols • MQTT control • Wokwi-simulated hardware</em>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/platform-ESP32-000000?style=flat-square&logo=espressif&logoColor=white" alt="ESP32" />
  <img src="https://img.shields.io/badge/simulation-Wokwi-4B32C3?style=flat-square&logo=wokwi&logoColor=white" alt="Wokwi" />
  <img src="https://img.shields.io/badge/protocol-MQTT-660066?style=flat-square&logo=mqtt&logoColor=white" alt="MQTT" />
  <img src="https://img.shields.io/badge/dashboard-HTML%2FJS-E34F26?style=flat-square&logo=html5&logoColor=white" alt="Dashboard" />
  <img src="https://img.shields.io/badge/deploy-Vercel-000000?style=flat-square&logo=vercel&logoColor=white" alt="Vercel" />
  <img src="https://img.shields.io/badge/license-MIT-green?style=flat-square" alt="License" />
</p>

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Why Smart Hub?](#-why-smart-hub)
- [Features](#-features)
- [System Architecture](#-system-architecture)
- [Hardware Components](#-hardware-components)
- [Pin Mapping](#-pin-mapping)
- [MQTT Protocol](#-mqtt-communication-protocol)
- [Quick Start](#-quick-start)
- [Dashboard](#-mobile-dashboard)
- [Project Structure](#-project-structure)
- [Dependencies](#-dependencies)
- [Improvement Ideas](#-improvement-ideas)
- [Author](#-author)

---

## 🔍 Overview

**Smart Hub: Home Automation** is a comprehensive IoT solution that combines an ESP32 microcontroller with a suite of environmental and security sensors to create an autonomous smart home system. It provides a "single pane of glass" for monitoring your home's security, air quality, and climate — with the intelligence to take action on its own when danger is detected.

The ESP32 firmware runs autonomously, processing sensor data and executing safety protocols (locking doors, opening windows, sounding alarms) even if the dashboard is offline. A mobile-first web dashboard connects over MQTT for real-time monitoring and manual overrides from anywhere in the world.

> The entire system is simulated on [Wokwi](https://wokwi.com) — no physical hardware required to run and test.

---

## 🎯 Why Smart Hub?

> **Most IoT classroom projects are passive data loggers — they read sensors and display values. Smart Hub actually *acts* on its environment.**

| | Typical IoT Projects | Smart Hub: Home Automation |
|---|---|---|
| **Sensor data** | Displayed on LCD or serial monitor | Streamed in real-time to a mobile dashboard via MQTT, with historical charts |
| **Safety response** | Manual — user reads a value and decides | **Autonomous** — fire auto-triggers alarm + opens windows; intruder auto-locks doors |
| **Control** | Physical buttons on the breadboard | Remote mobile dashboard with toggle switches, panic button, and mute controls |
| **Automation** | None | LDR-based auto-lighting, AQI-based auto-ventilation, 30s auto-relock timer |
| **Intelligence** | Single-sensor triggers | Multi-sensor fusion — PIR + Ultrasonic for breach detection, Smoke + Temp for fire confirmation |
| **Visitor detection** | ❌ | ✅ 5-second dwell-time logic distinguishes visitors from passing objects |
| **Panic mode** | ❌ | ✅ Dedicated panic button with alternating siren and LED strobe |

---

## ✨ Features

### 🔒 Smart Security System
| Feature | Description |
|---|---|
| **Dual-Sensor Breach Detection** | PIR (motion) + Ultrasonic (proximity) — both must indicate a threat when the system is armed |
| **Auto-Lock Timer** | When unlocked via dashboard, the system starts a **30-second countdown** then automatically re-locks and re-arms |
| **Visitor "Doorbell" Alert** | Ultrasonic detects someone standing at the door for >5 seconds — triggers a notification, not an alarm |
| **Arm / Disarm Toggle** | One-tap security toggle from the mobile dashboard |

### 🌡️ Environmental Automation
| Feature | Description |
|---|---|
| **Auto-Lighting** | LED turns ON when ambient light (LDR) drops below threshold, OFF during daytime |
| **Auto-Ventilation** | Window servo opens to 90° when AQI > 40 (poor air), closes automatically when air clears |
| **Manual Overrides** | Any manual toggle on the dashboard disables automation for that device until "Auto Mode" is re-enabled |
| **Climate Monitoring** | Real-time temperature, humidity, and smoke levels with AQI calculation |

### 🚨 Safety & Emergency Protocols
| Feature | Description |
|---|---|
| **Fire Detection** | Triggers immediately when smoke levels are high OR temperature exceeds 50°C |
| **Panic Mode** | Dedicated panic button — triggers rapid alternating siren (1.5kHz/3kHz) and LED strobe |
| **Smart Muting** | Users can mute individual alarms (fire/intruder) from the dashboard; mute auto-resets when the danger clears |
| **Unified Alarm Logic** | Priority system: Panic > Fire > Intruder — prevents alarm conflicts |

### 📱 Real-Time Dashboard
| Feature | Description |
|---|---|
| **Mobile-First Design** | Responsive, touch-optimized UI with toggle switches and status badges |
| **Live Telemetry** | Temperature, humidity, AQI with color-coded suggestions (Clean / Moderate / POOR) |
| **Historical Charts** | Chart.js line graph tracking temp, humidity, and smoke over a rolling 20-point window |
| **Alert Banners** | Fire (flashing orange), Intruder (red), and Visitor (blue) banners with mute controls |
| **State Sync** | Dashboard reflects the physical state of every actuator in real-time via MQTT status topic |
| **Vercel Deployment** | Dashboard can be hosted on Vercel for remote access from anywhere |

---

## 🏗 System Architecture

```
┌──────────────────────────────────────────────────────────────┐
│                     ESP32 DevKit V1                          │
│                                                              │
│  SENSORS (Input)                    ACTUATORS (Output)       │
│  ┌────────────┐                     ┌──────────────┐        │
│  │ DHT22      │ Temp + Humidity     │ Door Servo   │ Lock   │
│  │ LDR        │ Light Level         │ Window Servo │ Vent   │
│  │ PIR        │ Motion              │ Buzzer       │ Alarm  │
│  │ HC-SR04    │ Distance            │ LED          │ Strobe │
│  │ Smoke Pot  │ Gas/AQI             │ LCD 1602     │ Status │
│  └─────┬──────┘                     └──────┬───────┘        │
│        │                                   │                │
│  ┌─────▼───────────────────────────────────▼──────────┐     │
│  │              Autonomous Logic Engine                │     │
│  │                                                    │     │
│  │  • Fire: smoke>2000 || temp>50 → alarm + vent      │     │
│  │  • Intruder: armed && (PIR || dist<50) → siren     │     │
│  │  • Visitor: dist<100 for >5s → notify              │     │
│  │  • Auto-light: LDR>2000 → LED on                  │     │
│  │  • Auto-vent: AQI>40 → window open                │     │
│  │  • Auto-lock: 30s after unlock → re-arm            │     │
│  └─────────────────────┬──────────────────────────────┘     │
│                        │                                    │
│                   WiFi + MQTT                               │
└────────────────────────┼────────────────────────────────────┘
                         │
              ┌──────────▼──────────┐
              │  HiveMQ Broker      │
              │  broker.hivemq.com  │
              └──────────┬──────────┘
                         │
              ┌──────────▼──────────┐
              │  📱 Web Dashboard   │
              │  (dashboard.html)   │
              │                     │
              │  • Toggle controls  │
              │  • Alert banners    │
              │  • Live charts      │
              │  • Panic button     │
              └─────────────────────┘
```

---

## 🔧 Hardware Components

### 🧠 Controller
| Component | Role |
|---|---|
| **ESP32 DevKit V1** | Central processing — WiFi connectivity, MQTT communication, sensor reading, actuator control |

### 🌡️ Environmental Sensors
| Sensor | Pin | Function |
|---|---|---|
| **DHT22** | GPIO 15 | Temperature and humidity monitoring. Fire detection when temp > 50°C |
| **Potentiometer** (Smoke Simulator) | GPIO 32 | Simulates a gas/smoke sensor. Measures "Air Health" (AQI 0–100) |
| **LDR (Photoresistor)** | GPIO 34 | Ambient light detection for auto-lighting logic |

### 🛡️ Security Sensors
| Sensor | Pin | Function |
|---|---|---|
| **PIR Motion Sensor** | GPIO 13 | Infrared heat movement detection — primary intruder trigger |
| **HC-SR04 Ultrasonic** | GPIO 5 (Trig), 18 (Echo) | Distance measurement for visitor detection and door perimeter monitoring |

### ⚙️ Actuators
| Actuator | Pin | Function |
|---|---|---|
| **Servo Motor 1** (Door Lock) | GPIO 4 | 0–90° rotation simulating lock/unlock |
| **Servo Motor 2** (Window) | GPIO 14 | 0–90° rotation for emergency ventilation |
| **Active Buzzer** | GPIO 2 | Audible alarm with distinct tones: Fire (2kHz), Intruder (1kHz), Panic (alternating 1.5/3kHz) |
| **Blue LED** | GPIO 16 | Night light (via LDR) and emergency strobe |

### 📺 Display
| Component | Pins | Function |
|---|---|---|
| **LCD 1602 (I2C)** | GPIO 21 (SDA), 22 (SCL) | On-device display showing temperature, AQI, and system status |

---

## 📌 Pin Mapping

| Component | Pin | Type |
|---|---|---|
| DHT22 | 15 | Digital (Data) |
| PIR | 13 | Digital (Input) |
| LDR | 34 | Analog (Input) |
| Smoke Sensor | 32 | Analog (Input) |
| Ultrasonic Trig | 5 | Digital (Output) |
| Ultrasonic Echo | 18 | Digital (Input) |
| Buzzer | 2 | Digital (Output) |
| Blue LED | 16 | Digital (Output) |
| Door Servo | 4 | PWM |
| Window Servo | 14 | PWM |
| LCD SDA | 21 | I2C |
| LCD SCL | 22 | I2C |

---

## 📡 MQTT Communication Protocol

Broker: **HiveMQ Public** (`broker.hivemq.com`, port 1883 TCP / 8884 WSS)

### Topics

| Topic | Direction | Payload |
|---|---|---|
| `yaksh/smarthub/telemetry` | ESP32 → Dashboard | JSON: `temp`, `hum`, `intruder`, `fire`, `smoke`, `visitor`, `aqi`, `window`, `intruderMuted`, `fireMuted` |
| `yaksh/smarthub/status` | ESP32 → Dashboard | JSON: `armed`, `light`, `window`, `buzzerMuted`, `fireMuted`, `panic` |
| `yaksh/smarthub/command` | Dashboard → ESP32 | String commands (see below) |

### Command Reference

| Command | Action |
|---|---|
| `ARM` / `DISARM` | Arm/disarm security system (lock/unlock door) |
| `LIGHT_ON` / `LIGHT_OFF` | Manual light control |
| `AUTO_LIGHT` | Re-enable LDR-based auto-lighting |
| `WINDOW_OPEN` / `WINDOW_CLOSE` | Manual window control |
| `AUTO_WINDOW` | Re-enable AQI-based auto-ventilation |
| `PANIC_ON` / `PANIC_OFF` | Activate/deactivate panic mode |
| `MUTE_FIRE` | Silence fire alarm (auto-resets when fire clears) |
| `MUTE_INTRUDER` | Silence intruder alarm (auto-resets when breach clears) |

---

## 🚀 Quick Start

### 1. Run the Simulation

Open the Wokwi project:

👉 **[Launch on Wokwi](https://wokwi.com/projects/462033205041634305)**

Click **"Start Simulation"** — the ESP32 will connect to WiFi and begin publishing telemetry.

### 2. Open the Dashboard

**Option A — Local**
```
Open dashboard.html in any modern browser
```

**Option B — Deployed on Vercel**

Deploy to Vercel for remote access:
```bash
npm i -g vercel
vercel deploy
```

The `vercel.json` rewrites `/` to `dashboard.html` automatically.

### 3. Interact

- The dashboard auto-connects to the MQTT broker over WebSocket (port 8884, SSL).
- Toggle switches, press the panic button, and watch real-time telemetry flow in.
- Interact with Wokwi sensor controls (adjust potentiometer for smoke, click PIR for motion, change DHT values) to trigger automation.

---

## 📱 Mobile Dashboard

The dashboard (`dashboard.html`) is a single-file, mobile-first web app:

- **Panic Button** — Full-width, pulsing red when active
- **Security Toggle** — Armed (red badge) / Open (green badge) with auto-lock countdown
- **Light Toggle** — Manual ON/OFF with "Auto-light (LDR)" checkbox
- **Window Toggle** — Manual Open/Close with "Auto-ventilation" checkbox
- **Environment Card** — Temperature, Humidity, AQI with color-coded health suggestions
- **Live Chart** — Rolling Chart.js line graph (temp / humidity / smoke)
- **Alert Banners** — Fire (flashing orange), Intruder (red), Visitor (blue) with inline mute links

---

## 📁 Project Structure

```
smart-hub/
├── sketch.ino           # ESP32 Arduino firmware (229 lines)
│                        #   - Sensor reading, autonomous logic, MQTT pub/sub
├── dashboard.html       # Mobile-first web dashboard (438 lines)
│                        #   - MQTT over WebSocket, Chart.js, toggle controls
├── diagram.json         # Wokwi circuit schematic (all components + wiring)
├── libraries.txt        # Arduino library dependencies for Wokwi
├── vercel.json          # Vercel deployment config (rewrites / → dashboard)
├── wokwi-project.txt    # Wokwi project link reference
├── summary.md           # Component-by-component hardware summary
├── guide.md             # Quick-start guide
├── README.md            # This file
└── LICENSE              # MIT License
```

---

## 📚 Dependencies

### ESP32 Firmware (Arduino)
| Library | Purpose |
|---|---|
| `WiFi.h` | ESP32 WiFi connectivity |
| `PubSubClient` | MQTT client for publishing telemetry and receiving commands |
| `Wire.h` | I2C communication for LCD |
| `LiquidCrystal_I2C` | 16×2 LCD driver |
| `DHT` | DHT22 temperature/humidity sensor driver |
| `ESP32Servo` | Servo motor control on ESP32 (door lock + window) |

### Dashboard (Browser)
| Library | Purpose |
|---|---|
| [Paho MQTT](https://www.eclipse.org/paho/) | MQTT over WebSocket client (CDN) |
| [Chart.js](https://www.chartjs.org/) | Real-time line charts for telemetry (CDN) |

---

## 💡 Improvement Ideas

> Suggestions for future enhancements — no code changes required.

### High Impact
- **Push Notifications** — Use the Web Push API or a Telegram bot to send fire/intruder alerts to the user's phone even when the dashboard is closed.
- **Multi-Room Support** — Extend the MQTT topic namespace (`yaksh/smarthub/room1/`, `room2/`) with multiple ESP32 nodes, each with its own sensor suite.
- **Historical Data Storage** — Log telemetry to a time-series database (InfluxDB or Firebase) for long-term analytics and trend detection.
- **OTA Firmware Updates** — Add ArduinoOTA support so the ESP32 firmware can be updated wirelessly without re-flashing.

### Medium Impact
- **Custom Alert Thresholds** — Let users configure fire temp threshold, AQI vent threshold, and auto-lock timer duration from the dashboard.
- **Camera Integration** — Add an ESP32-CAM module for live video feed when a visitor or intruder is detected.
- **Energy Monitoring** — Add a current sensor (ACS712) to track power consumption of connected appliances.
- **Voice Control** — Integrate with Google Home or Alexa via MQTT bridge for voice-activated commands.

### Polish
- **Dark Mode Dashboard** — Add a theme toggle (the current dashboard is light-only).
- **Dashboard Authentication** — Add a PIN or password to prevent unauthorized control.
- **Offline Indicator** — Show a warning banner when the MQTT connection drops.
- **Sensor Calibration UI** — Let users calibrate smoke and light sensor thresholds from the dashboard.
- **PWA Support** — Add a service worker and manifest for "Add to Home Screen" on mobile.

---

## 👤 Author

**Felix-au** (Harshit Soni)

- 🔗 GitHub: [github.com/Felix-au](https://github.com/Felix-au)
- 📧 Email: [harshit.soni.23cse@bmu.edu.in](mailto:harshit.soni.23cse@bmu.edu.in)

---

> **Developer Note:** The firmware prioritizes autonomous safety logic — the home remains secure even if the internet connection to the dashboard is temporarily lost. All safety decisions (fire response, intruder lockdown, auto-relock) execute locally on the ESP32 with zero cloud dependency.

<p align="center">
  <sub>Built for homes that think for themselves.</sub>
</p>
