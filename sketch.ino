#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <ESP32Servo.h>

// --- Pin Definitions ---
#define DHTPIN 15
#define DHTTYPE DHT22
#define PIR_PIN 13
#define LDR_PIN 34
#define TRIG_PIN 5
#define ECHO_PIN 18
#define BUZZER_PIN 2
#define LED_PIN 16
#define DOOR_SERVO_PIN 4
#define WINDOW_SERVO_PIN 14
#define SMOKE_PIN 32 

// --- Objects & Network ---
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);
Servo doorLock;
Servo windowServo;

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
unsigned long visitorStartTime = 0;
unsigned long autoLockStartTime = 0;
unsigned long flashTime = 0;

bool systemArmed = true;
bool buzzerMuted = false;
bool fireMuted = false;
bool manualLight = false;
bool manualWindow = false;
bool lastLightState = false;
bool lastWindowState = false;
bool visitorDetected = false;
bool panicMode = false;
bool ledFlashState = false;

void publishStatus() {
  String statusPayload = "{\"armed\":" + String(systemArmed ? "true" : "false") + 
                         ", \"light\":" + String(digitalRead(LED_PIN) == HIGH ? "true" : "false") + 
                         ", \"window\":" + String(windowServo.read() > 45 ? "true" : "false") +
                         ", \"buzzerMuted\":" + String(buzzerMuted ? "true" : "false") + 
                         ", \"fireMuted\":" + String(fireMuted ? "true" : "false") + 
                         ", \"panic\":" + String(panicMode ? "true" : "false") + "}";
  client.publish("yaksh/smarthub/status", statusPayload.c_str());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) { message += (char)payload[i]; }
  
  if (message == "UNLOCK" || message == "DISARM") {
    doorLock.write(90);
    systemArmed = false;
    buzzerMuted = false;
    autoLockStartTime = millis(); 
    digitalWrite(BUZZER_PIN, LOW);
    panicMode = false;
  } 
  else if (message == "LOCK" || message == "ARM") {
    doorLock.write(0);
    systemArmed = true;
    autoLockStartTime = 0;
    panicMode = false;
  }
  else if (message == "LIGHT_ON") { digitalWrite(LED_PIN, HIGH); manualLight = true; }
  else if (message == "LIGHT_OFF") { digitalWrite(LED_PIN, LOW); manualLight = true; }
  else if (message == "AUTO_LIGHT") { manualLight = false; }
  else if (message == "WINDOW_OPEN") { windowServo.write(90); manualWindow = true; }
  else if (message == "WINDOW_CLOSE") { windowServo.write(0); manualWindow = true; }
  else if (message == "AUTO_WINDOW") { manualWindow = false; }
  else if (message == "MUTE_INTRUDER") { buzzerMuted = true; }
  else if (message == "MUTE_FIRE") { fireMuted = true; noTone(BUZZER_PIN); }
  else if (message == "PANIC_ON") { panicMode = true; }
  else if (message == "PANIC_OFF") { panicMode = false; noTone(BUZZER_PIN); }

  publishStatus();
}

void setup_wifi() {
  WiFi.begin(ssid, password, 6);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_SmartHub_Yaksh")) {
      client.subscribe("yaksh/smarthub/command");
      publishStatus();
    } else { delay(5000); }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(PIR_PIN, INPUT); pinMode(LDR_PIN, INPUT); pinMode(SMOKE_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT); pinMode(ECHO_PIN, INPUT); pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  doorLock.attach(DOOR_SERVO_PIN); doorLock.write(0); 
  windowServo.attach(WINDOW_SERVO_PIN); windowServo.write(0);
  lcd.init(); lcd.backlight();
  setup_wifi();
  client.setServer(mqtt_server, 1883); client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  unsigned long now = millis();

  // Read Sensors
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int lightLevel = analogRead(LDR_PIN);
  int motionDetected = digitalRead(PIR_PIN);
  int smokeLevel = analogRead(SMOKE_PIN);
  
  // Ultrasonic
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = (duration > 0) ? (duration * 0.034 / 2) : 0;

  bool fireAlert = (smokeLevel > 2000 || temp > 50);
  bool intruderAlert = (systemArmed && (motionDetected == HIGH || (distance > 0 && distance < 50)));
  int aqi = map(smokeLevel, 0, 4095, 0, 100);

  // Mute logic cleanup
  if (!fireAlert) fireMuted = false;
  if (!intruderAlert) buzzerMuted = false;

  // --- UNIFIED ALARM & LED LOGIC ---
  bool fireActive = (fireAlert && !fireMuted);
  if (panicMode || fireActive) {
    if (now - flashTime > 250) {
      flashTime = now;
      ledFlashState = !ledFlashState;
      digitalWrite(LED_PIN, ledFlashState);
      if (panicMode) {
        if (ledFlashState) tone(BUZZER_PIN, 3000); else tone(BUZZER_PIN, 1500);
      } else {
        tone(BUZZER_PIN, 2000);
      }
    }
  } 
  else {
    if (!fireActive && !panicMode) {
        if (intruderAlert && !buzzerMuted) tone(BUZZER_PIN, 1000);
        else noTone(BUZZER_PIN);
    }
    // Light Automation
    if (!manualLight) {
      if (lightLevel > 2000) digitalWrite(LED_PIN, HIGH); else digitalWrite(LED_PIN, LOW);
    }
  }

  // --- WINDOW AUTOMATION ---
  if (!manualWindow) {
    if (aqi > 40) windowServo.write(90); // Open if poor
    else windowServo.write(0); // Close if good
  }

  // --- 1s INTERVAL TASKS ---
  if (now - lastMsg > 1000) {
    lastMsg = now;

    // Visitor Detection
    if (distance > 0 && distance < 100) {
      if (visitorStartTime == 0) visitorStartTime = now;
      else if (now - visitorStartTime > 5000) visitorDetected = true;
    } else {
      visitorStartTime = 0;
      visitorDetected = false;
    }

    // LCD
    lcd.setCursor(0, 0); lcd.print("T:"); lcd.print(temp, 1); lcd.print("C AQI:"); lcd.print(aqi); lcd.print("  ");
    lcd.setCursor(0, 1);
    if (panicMode) lcd.print("!! PANIC !!    ");
    else if (fireAlert) lcd.print(fireMuted ? "FIRE (SILENCED) " : "!! FIRE ALERT !!");
    else if (intruderAlert) lcd.print(buzzerMuted ? "BREACH (MUTED) " : "!! BREACH !!   ");
    else if (visitorDetected) lcd.print("Visitor at Door");
    else lcd.print(systemArmed ? "System Armed   " : "System Disarmed");

    // Telemetry
    String payload = "{\"temp\":" + String(temp) + ",\"hum\":" + String(hum) + 
                     ",\"intruder\":" + String(intruderAlert ? "1" : "0") + 
                     ",\"fire\":" + String(fireAlert ? "1" : "0") + 
                     ",\"smoke\":" + String(smokeLevel) + 
                     ",\"visitor\":" + String(visitorDetected ? "1" : "0") + 
                     ",\"aqi\":" + String(aqi) + 
                     ",\"window\":" + String(windowServo.read() > 45 ? "1" : "0") +
                     ",\"intruderMuted\":" + String(buzzerMuted ? "1" : "0") + 
                     ",\"fireMuted\":" + String(fireMuted ? "1" : "0") + "}";
    client.publish("yaksh/smarthub/telemetry", payload.c_str());
    
    // Status Sync
    bool currentLight = (digitalRead(LED_PIN) == HIGH);
    bool currentWindow = (windowServo.read() > 45);
    if (currentLight != lastLightState || currentWindow != lastWindowState) { 
        lastLightState = currentLight; 
        lastWindowState = currentWindow;
        publishStatus(); 
    }
  }

  // Auto-Lock Timer
  if (autoLockStartTime > 0 && now - autoLockStartTime > 30000) {
    doorLock.write(0);
    systemArmed = true;
    autoLockStartTime = 0;
    publishStatus();
  }
}