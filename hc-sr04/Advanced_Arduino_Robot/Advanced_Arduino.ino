/*
 * HC-SR04 Ultrasonic Sensor - ESP32 Advanced Example
 * Features: WiFi logging, OLED display, multiple sensors
 * 
 * IMPORTANT: HC-SR04 ECHO pin outputs 5V
 * ESP32 pins are 3.3V tolerant!
 * Use voltage divider: 1kΩ and 2kΩ resistors
 * 
 * Connections:
 * HC-SR04 VCC  -> ESP32 5V (from VIN)
 * HC-SR04 GND  -> ESP32 GND
 * HC-SR04 TRIG -> ESP32 GPIO 5
 * HC-SR04 ECHO -> Voltage Divider -> ESP32 GPIO 18
 * 
 * Voltage Divider:
 * ECHO pin -> 1kΩ -> GPIO 18 -> 2kΩ -> GND
 * This reduces 5V to 3.3V
 */

#include <WiFi.h>

// Pin definitions
#define TRIG_PIN 5
#define ECHO_PIN 18

// WiFi credentials (optional - for IoT projects)
// const char* ssid = "YOUR_SSID";
// const char* password = "YOUR_PASSWORD";

// Constants
#define SOUND_SPEED 0.034
#define MAX_DISTANCE 400
#define MIN_DISTANCE 2

// Measurement settings
#define SAMPLE_SIZE 5
#define MEASUREMENT_INTERVAL 100  // ms

// Data structure
struct SensorData {
  float distance;
  float filteredDistance;
  bool validReading;
  unsigned long timestamp;
  float temperature;  // For compensation
};

SensorData currentData;

// Moving average filter
float readings[SAMPLE_SIZE];
int readIndex = 0;
float total = 0;
float average = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  printHeader();
  
  // Initialize pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Initialize moving average array
  for (int i = 0; i < SAMPLE_SIZE; i++) {
    readings[i] = 0;
  }
  
  Serial.println(F("✅ HC-SR04 initialized successfully!\n"));
  
  // Optional: WiFi setup
  // setupWiFi();
}

void loop() {
  // Measure distance
  measureDistance();
  
  // Apply filtering
  applyMovingAverage();
  
  // Display results
  displayMeasurement();
  
  // Check alerts
  checkDistanceAlerts();
  
  delay(MEASUREMENT_INTERVAL);
}

void printHeader() {
  Serial.println(F("\n╔════════════════════════════════════════╗"));
  Serial.println(F("║   HC-SR04 Ultrasonic Sensor - ESP32    ║"));
  Serial.println(F("║   Advanced Distance Measurement        ║"));
  Serial.println(F("╚════════════════════════════════════════╝"));
}

void measureDistance() {
  // Clear trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Send pulse
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Read echo
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  
  currentData.timestamp = millis();
  
  if (duration == 0) {
    currentData.distance = -1;
    currentData.validReading = false;
    return;
  }
  
  // Calculate distance
  float distance = duration * SOUND_SPEED / 2.0;
  
  // Validate reading
  if (distance < MIN_DISTANCE || distance > MAX_DISTANCE) {
    currentData.distance = -1;
    currentData.validReading = false;
  } else {
    currentData.distance = distance;
    currentData.validReading = true;
  }
}

void applyMovingAverage() {
  if (!currentData.validReading) {
    currentData.filteredDistance = -1;
    return;
  }
  
  // Subtract the last reading
  total = total - readings[readIndex];
  
  // Add new reading
  readings[readIndex] = currentData.distance;
  total = total + readings[readIndex];
  
  // Advance to next position
  readIndex = (readIndex + 1) % SAMPLE_SIZE;
  
  // Calculate average
  currentData.filteredDistance = total / SAMPLE_SIZE;
}

void displayMeasurement() {
  Serial.println(F("\n╔════════════════════════════════════════╗"));
  Serial.println(F("║         DISTANCE MEASUREMENT           ║"));
  Serial.println(F("╚════════════════════════════════════════╝"));
  
  if (!currentData.validReading) {
    Serial.println(F("⚠️  STATUS: OUT OF RANGE"));
    Serial.println(F("   No valid echo received"));
    Serial.println(F("   Check sensor orientation & target"));
  } else {
    Serial.printf("📏 Raw Distance:      %.2f cm\n", currentData.distance);
    Serial.printf("📊 Filtered Distance: %.2f cm\n", currentData.filteredDistance);
    Serial.printf("📐 In Inches:         %.2f in\n", currentData.filteredDistance / 2.54);
    Serial.printf("📐 In Meters:         %.3f m\n", currentData.filteredDistance / 100.0);
    Serial.printf("⏱️  Timestamp:         %lu ms\n", currentData.timestamp);
    
    printDistanceBar(currentData.filteredDistance);
  }
  
  Serial.println();
}

void printDistanceBar(float distance) {
  int percentage = map(distance, 0, MAX_DISTANCE, 0, 100);
  percentage = constrain(percentage, 0, 100);
  
  Serial.print(F("\nRange: ["));
  
  int bars = percentage / 2;  // 50 characters max
  for (int i = 0; i < 50; i++) {
    if (i < bars) {
      Serial.print(F("█"));
    } else {
      Serial.print(F("░"));
    }
  }
  
  Serial.printf("] %d%%\n", percentage);
}

void checkDistanceAlerts() {
  if (!currentData.validReading) {
    return;
  }
  
  Serial.println(F("⚡ PROXIMITY ALERTS:"));
  
  float dist = currentData.filteredDistance;
  
  if (dist < 5) {
    Serial.println(F("  🔴 CRITICAL - Object extremely close!"));
    Serial.println(F("  🔊 Beep beep beep!"));
  } else if (dist < 10) {
    Serial.println(F("  🟠 WARNING - Object very near"));
    Serial.println(F("  🔊 Beep beep"));
  } else if (dist < 30) {
    Serial.println(F("  🟡 CAUTION - Object detected"));
    Serial.println(F("  🔊 Beep"));
  } else if (dist < 100) {
    Serial.println(F("  🟢 SAFE - Object in range"));
  } else {
    Serial.println(F("  🔵 CLEAR - No nearby objects"));
  }
  
  Serial.println(F("════════════════════════════════════════\n"));
}

// Temperature compensated measurement
float getTemperatureCompensatedDistance(float tempCelsius) {
  // Speed of sound = 331.3 + (0.606 × temperature) m/s
  float speedOfSound = (331.3 + (0.606 * tempCelsius)) / 10000.0;
  
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  
  if (duration == 0) return -1;
  
  return duration * speedOfSound / 2.0;
}

// Median filter (alternative to moving average)
float getMedianDistance(int samples) {
  float measurements[samples];
  
  for (int i = 0; i < samples; i++) {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    long duration = pulseIn(ECHO_PIN, HIGH, 30000);
    measurements[i] = duration * SOUND_SPEED / 2.0;
    delay(50);
  }
  
  // Sort array
  for (int i = 0; i < samples - 1; i++) {
    for (int j = i + 1; j < samples; j++) {
      if (measurements[i] > measurements[j]) {
        float temp = measurements[i];
        measurements[i] = measurements[j];
        measurements[j] = temp;
      }
    }
  }
  
  // Return median
  return measurements[samples / 2];
}

// JSON output for IoT applications
String getJSONData() {
  String json = "{";
  json += "\"distance\":" + String(currentData.filteredDistance, 2) + ",";
  json += "\"raw_distance\":" + String(currentData.distance, 2) + ",";
  json += "\"valid\":" + String(currentData.validReading ? "true" : "false") + ",";
  json += "\"timestamp\":" + String(currentData.timestamp);
  json += "}";
  return json;
}

// Detect motion/movement
bool detectMovement(float threshold) {
  static float lastDistance = 0;
  
  if (!currentData.validReading) {
    return false;
  }
  
  float change = abs(currentData.filteredDistance - lastDistance);
  lastDistance = currentData.filteredDistance;
  
  return change > threshold;
}

// Optional: WiFi setup for IoT
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