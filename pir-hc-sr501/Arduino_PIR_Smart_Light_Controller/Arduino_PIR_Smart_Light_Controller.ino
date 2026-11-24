/*
 * HC-SR501 PIR Motion Sensor - ESP32 Advanced Example
 * Features: WiFi notifications, deep sleep, web dashboard
 * 
 * Connections:
 * PIR VCC  -> ESP32 5V (or 3.3V)
 * PIR GND  -> ESP32 GND
 * PIR OUT  -> ESP32 GPIO 27
 * 
 * Features:
 * - Motion detection with statistics
 * - WiFi event logging
 * - Web server dashboard
 * - Deep sleep mode for battery operation
 * - MQTT publishing (optional)
 */

#include <WiFi.h>
#include <WebServer.h>
#include <esp_sleep.h>

// Pin definitions
#define PIR_PIN 27
#define LED_PIN 2  // Built-in LED

// WiFi credentials (optional - comment out if not using WiFi)
// const char* ssid = "YOUR_SSID";
// const char* password = "YOUR_PASSWORD";

// Web server
WebServer server(80);

// Motion detection variables
struct MotionData {
  unsigned long motionCount;
  unsigned long lastMotionTime;
  unsigned long totalMotionDuration;
  unsigned long sessionStartTime;
  bool currentlyDetecting;
  float motionsPerHour;
};

MotionData motionStats;

// Deep sleep settings
#define SLEEP_DURATION 60  // seconds
bool deepSleepEnabled = false;

// Timing
unsigned long motionStartTime = 0;
int previousPirState = LOW;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  printHeader();
  
  // Initialize pins
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // Initialize stats
  motionStats.motionCount = 0;
  motionStats.lastMotionTime = 0;
  motionStats.totalMotionDuration = 0;
  motionStats.sessionStartTime = millis();
  motionStats.currentlyDetecting = false;
  motionStats.motionsPerHour = 0;
  
  // WiFi setup (optional)
  // setupWiFi();
  // setupWebServer();
  
  // Calibrate PIR
  calibratePIR();
  
  Serial.println(F("✅ System ready!"));
  Serial.println(F("🔍 Monitoring for motion...\n"));
}

void loop() {
  // Handle web server (if WiFi enabled)
  // server.handleClient();
  
  // Read PIR sensor
  int pirState = digitalRead(PIR_PIN);
  
  // Motion detected (rising edge)
  if (pirState == HIGH && previousPirState == LOW) {
    handleMotionDetected();
  }
  
  // Motion ended (falling edge)
  else if (pirState == LOW && previousPirState == HIGH) {
    handleMotionEnded();
  }
  
  // Update statistics
  updateStatistics();
  
  previousPirState = pirState;
  
  // Optional: Deep sleep mode
  if (deepSleepEnabled && !motionStats.currentlyDetecting) {
    enterDeepSleep();
  }
  
  delay(100);
}

void printHeader() {
  Serial.println(F("\n╔════════════════════════════════════════╗"));
  Serial.println(F("║   PIR Motion Sensor - ESP32 Advanced   ║"));
  Serial.println(F("║   Smart Detection System               ║"));
  Serial.println(F("╚════════════════════════════════════════╝"));
}

void calibratePIR() {
  Serial.println(F("\n⏳ Calibrating PIR sensor..."));
  Serial.println(F("   Please remain still"));
  
  digitalWrite(LED_PIN, HIGH);
  
  for (int i = 30; i > 0; i--) {
    Serial.printf("   Calibration: %d seconds remaining\n", i);
    delay(1000);
  }
  
  digitalWrite(LED_PIN, LOW);
  Serial.println(F("✅ PIR sensor calibrated!\n"));
}

void handleMotionDetected() {
  motionStartTime = millis();
  motionStats.lastMotionTime = millis();
  motionStats.motionCount++;
  motionStats.currentlyDetecting = true;
  
  digitalWrite(LED_PIN, HIGH);
  
  Serial.println(F("\n╔════════════════════════════════════════╗"));
  Serial.println(F("║        🚨 MOTION DETECTED!             ║"));
  Serial.println(F("╚════════════════════════════════════════╝"));
  
  Serial.printf("⏰ Time: %lu seconds\n", millis() / 1000);
  Serial.printf("📊 Motion Event #%lu\n", motionStats.motionCount);
  Serial.printf("📈 Rate: %.2f motions/hour\n", motionStats.motionsPerHour);
  
  // Trigger actions
  triggerMotionActions();
}

void handleMotionEnded() {
  unsigned long duration = millis() - motionStartTime;
  motionStats.totalMotionDuration += duration;
  motionStats.currentlyDetecting = false;
  
  digitalWrite(LED_PIN, LOW);
  
  Serial.println(F("\n╔════════════════════════════════════════╗"));
  Serial.println(F("║        ✅ Motion Ended                 ║"));
  Serial.println(F("╚════════════════════════════════════════╝"));
  
  Serial.printf("⏱️  This event duration: %.2f seconds\n", duration / 1000.0);
  Serial.printf("⏰ Total motion time: %.2f seconds\n", 
                motionStats.totalMotionDuration / 1000.0);
  Serial.println(F("─────────────────────────────────────────\n"));
}

void updateStatistics() {
  static unsigned long lastUpdate = 0;
  
  if (millis() - lastUpdate > 10000) {  // Update every 10 seconds
    unsigned long sessionDuration = millis() - motionStats.sessionStartTime;
    float hours = sessionDuration / 3600000.0;
    
    if (hours > 0) {
      motionStats.motionsPerHour = motionStats.motionCount / hours;
    }
    
    lastUpdate = millis();
  }
}

void triggerMotionActions() {
  // Add your custom actions here:
  
  // 1. Send notification
  Serial.println(F("📧 Sending notification..."));
  
  // 2. Turn on lights (via relay)
  // digitalWrite(RELAY_PIN, HIGH);
  
  // 3. Start camera recording
  // startRecording();
  
  // 4. Send MQTT message
  // publishMQTT("motion/detected", "true");
  
  // 5. Log to web server
  // logMotionEvent();
}

void displayStatistics() {
  Serial.println(F("\n╔════════════════════════════════════════╗"));
  Serial.println(F("║         MOTION STATISTICS              ║"));
  Serial.println(F("╚════════════════════════════════════════╝"));
  
  Serial.printf("📊 Total Motions: %lu\n", motionStats.motionCount);
  Serial.printf("⏱️  Total Duration: %.2f seconds\n", 
                motionStats.totalMotionDuration / 1000.0);
  Serial.printf("📈 Motions/Hour: %.2f\n", motionStats.motionsPerHour);
  Serial.printf("⏰ Session Time: %.2f minutes\n", 
                (millis() - motionStats.sessionStartTime) / 60000.0);
  Serial.printf("🔴 Currently Detecting: %s\n", 
                motionStats.currentlyDetecting ? "YES" : "NO");
  
  if (motionStats.motionCount > 0) {
    float avgDuration = motionStats.totalMotionDuration / (float)motionStats.motionCount;
    Serial.printf("📊 Average Duration: %.2f seconds\n", avgDuration / 1000.0);
  }
  
  Serial.println(F("════════════════════════════════════════\n"));
}

String getJSONStatus() {
  String json = "{";
  json += "\"motion_count\":" + String(motionStats.motionCount) + ",";
  json += "\"currently_detecting\":" + String(motionStats.currentlyDetecting ? "true" : "false") + ",";
  json += "\"last_motion\":" + String(motionStats.lastMotionTime) + ",";
  json += "\"total_duration\":" + String(motionStats.totalMotionDuration) + ",";
  json += "\"motions_per_hour\":" + String(motionStats.motionsPerHour, 2) + ",";
  json += "\"uptime\":" + String(millis());
  json += "}";
  return json;
}

void enterDeepSleep() {
  Serial.println(F("\n💤 Entering deep sleep mode..."));
  Serial.printf("⏰ Wake up in %d seconds\n", SLEEP_DURATION);
  
  // Configure wake-up source
  esp_sleep_enable_timer_wakeup(SLEEP_DURATION * 1000000ULL);
  
  // Optional: Wake on PIR motion
  esp_sleep_enable_ext0_wakeup((gpio_num_t)PIR_PIN, HIGH);
  
  Serial.println(F("😴 Good night!\n"));
  delay(100);
  
  esp_deep_sleep_start();
}

// Optional: WiFi setup
/*
void setupWiFi() {
  Serial.print(F("Connecting to WiFi"));
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(F("."));
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("\n✅ WiFi connected!"));
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
  } else {
    Serial.println(F("\n❌ WiFi connection failed"));
  }
}
*/

// Optional: Web server setup
/*
void setupWebServer() {
  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/stats", handleStats);
  server.begin();
  Serial.println(F("🌐 Web server started"));
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<title>PIR Motion Sensor</title>";
  html += "<meta http-equiv='refresh' content='5'>";
  html += "</head><body>";
  html += "<h1>PIR Motion Sensor Dashboard</h1>";
  html += "<h2>Status: " + String(motionStats.currentlyDetecting ? "MOTION DETECTED" : "No Motion") + "</h2>";
  html += "<p>Total Motions: " + String(motionStats.motionCount) + "</p>";
  html += "<p>Motions/Hour: " + String(motionStats.motionsPerHour, 2) + "</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleStatus() {
  server.send(200, "application/json", getJSONStatus());
}

void handleStats() {
  displayStatistics();
  server.send(200, "text/plain", "Statistics printed to serial");
}
*/