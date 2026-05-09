# Smart Hub: Home Automation — Quick Guide

An ESP32-powered IoT solution for smart home security and environmental automation, with a real-time mobile dashboard connected over MQTT.

> [!IMPORTANT]
> **Unlike typical IoT classroom projects** that passively log sensor data to a serial monitor, Smart Hub **autonomously acts on its environment** — locking doors on breach detection, opening windows on poor air quality, and sounding multi-tone alarms on fire — all while syncing state to a remote mobile dashboard over MQTT.

## 🚀 How to Run

### 1. Start the Simulation

Open the Wokwi project in your browser:

👉 **[Launch on Wokwi](https://wokwi.com/projects/462033205041634305)**

Click **"Start Simulation"** — the ESP32 will connect to WiFi and begin publishing telemetry to the MQTT broker.

### 2. Open the Dashboard

Open `dashboard.html` in any modern browser (mobile or desktop). It will auto-connect to the HiveMQ public broker and begin displaying real-time data.

> [!NOTE]
> Both the Wokwi simulation and the dashboard must be running simultaneously for data to flow. The dashboard connects to `broker.hivemq.com` over WebSocket (port 8884, SSL).

### 3. Deploy the Dashboard (Optional)

To host the dashboard remotely on Vercel:

```bash
npm i -g vercel
vercel deploy
```

The included `vercel.json` rewrites `/` to `dashboard.html` automatically.

## 🛠 Features

- **Dual-Sensor Breach Detection**: PIR + Ultrasonic for armed security with auto-lock timer.
- **Fire Detection**: Smoke sensor + temperature threshold trigger an immediate alarm and open windows.
- **Panic Mode**: Dedicated panic button with alternating siren and LED strobe.
- **Auto-Lighting**: LDR-based automatic lights that turn on at night.
- **Auto-Ventilation**: Window opens automatically when air quality drops.
- **Visitor Doorbell**: 5-second dwell-time detection distinguishes visitors from passing objects.
- **Mobile Dashboard**: Real-time telemetry, toggle controls, charts, and alert banners.
- **Smart Muting**: Mute individual alarms from the dashboard; auto-resets when danger clears.

## 📡 MQTT Commands

Send these strings to `yaksh/smarthub/command`:

| Command | Action |
|---|---|
| `ARM` / `DISARM` | Arm/disarm security |
| `LIGHT_ON` / `LIGHT_OFF` / `AUTO_LIGHT` | Light control |
| `WINDOW_OPEN` / `WINDOW_CLOSE` / `AUTO_WINDOW` | Window control |
| `PANIC_ON` / `PANIC_OFF` | Panic mode |
| `MUTE_FIRE` / `MUTE_INTRUDER` | Silence alarms |

## ⚠️ Important Notes

- **Wokwi Simulation**: No physical hardware needed — everything runs in the browser simulator.
- **Public Broker**: Uses HiveMQ's free public broker. Anyone on the same topic namespace can see/send messages.
- **Autonomous Safety**: All safety logic runs locally on the ESP32 — the home stays secure even if the dashboard goes offline.
- **Auto-Lock**: The door automatically re-locks 30 seconds after being unlocked if no manual action is taken.
