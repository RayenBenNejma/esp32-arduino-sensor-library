/*
 * BME280 Sensor - Complete Example for ESP32
 * Reads Temperature, Humidity, and Pressure
 * Supports Deep Sleep and WiFi reporting
 * 
 * Connections:
 * BME280 VCC -> ESP32 3.3V
 * BME280 GND -> ESP32 GND
 * BME280 SCL -> ESP32 GPIO 22
 * BME280 SDA -> ESP32 GPIO 21
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// I2C Pins for ESP32
#define I2C_SDA 21
#define I2C_SCL 22

// BME280 I2C Address
#define BME280_ADDRESS 0x76
#define SEALEVELPRESSURE_HPA (1013.25)

// Create BME280 object
Adafruit_BME280 bme;

// Sensor data structure
struct SensorData {
  float temperature;
  float humidity;
  float pressure;
  float altitude;
  unsigned long timestamp;
};

SensorData currentReading;

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println(F("\n=== BME280 ESP32 Advanced Example ==="));
  
  // Initialize I2C with custom pins
  Wire.begin(I2C_SDA, I2C_SCL);
  
  // Initialize BME280
  if (!bme.begin(BME280_ADDRESS, &Wire)) {
    Serial.println(F("ERROR: Could not find BME280 sensor!"));
    Serial.println(F("Please check:"));
    Serial.println(F("1. Wiring connections"));
    Serial.println(F("2. I2C address (0x76 or 0x77)"));
    Serial.println(F("3. Power supply (3.3V)"));
    while (1) delay(10);
  }
  
  Serial.println(F("✓ BME280 sensor initialized successfully!"));
  
  // Configure sensor for optimal performance
  bme.setSampling(Adafruit_BME280::MODE_NORMAL,
                  Adafruit_BME280::SAMPLING_X16,  // temperature
                  Adafruit_BME280::SAMPLING_X16,  // pressure
                  Adafruit_BME280::SAMPLING_X16,  // humidity
                  Adafruit_BME280::FILTER_X16,
                  Adafruit_BME280::STANDBY_MS_500);
  
  printSensorDetails();
  Serial.println();
}

void loop() {
  // Read all sensor values
  readSensorData();
  
  // Display readings
  displayReadings();
  
  // Check for alerts
  checkAlerts();
  
  delay(2000);
}

void readSensorData() {
  currentReading.temperature = bme.readTemperature();
  currentReading.humidity = bme.readHumidity();
  currentReading.pressure = bme.readPressure() / 100.0F;
  currentReading.altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  currentReading.timestamp = millis();
}

void displayReadings() {
  Serial.println(F("\n╔════════════════════════════════╗"));
  Serial.println(F("║     BME280 Readings            ║"));
  Serial.println(F("╚════════════════════════════════╝"));
  
  Serial.printf("🌡️  Temperature:  %.2f °C\n", currentReading.temperature);
  Serial.printf("💧 Humidity:     %.2f %%\n", currentReading.humidity);
  Serial.printf("📊 Pressure:     %.2f hPa\n", currentReading.pressure);
  Serial.printf("⛰️  Altitude:     %.2f m\n", currentReading.altitude);
  Serial.printf("⏱️  Timestamp:    %lu ms\n", currentReading.timestamp);
  Serial.println();
}

void checkAlerts() {
  // Temperature alerts
  if (currentReading.temperature > 35.0) {
    Serial.println(F("⚠️  WARNING: High temperature detected!"));
  } else if (currentReading.temperature < 10.0) {
    Serial.println(F("⚠️  WARNING: Low temperature detected!"));
  }
  
  // Humidity alerts
  if (currentReading.humidity > 80.0) {
    Serial.println(F("⚠️  WARNING: High humidity detected!"));
  } else if (currentReading.humidity < 20.0) {
    Serial.println(F("⚠️  WARNING: Low humidity detected!"));
  }
  
  // Pressure alerts (weather changes)
  if (currentReading.pressure < 1000.0) {
    Serial.println(F("🌧️  Low pressure - Rain possible"));
  } else if (currentReading.pressure > 1020.0) {
    Serial.println(F("☀️  High pressure - Clear weather"));
  }
}

void printSensorDetails() {
  Serial.println(F("\n--- Sensor Configuration ---"));
  Serial.printf("I2C Address: 0x%02X\n", BME280_ADDRESS);
  Serial.printf("SDA Pin: GPIO %d\n", I2C_SDA);
  Serial.printf("SCL Pin: GPIO %d\n", I2C_SCL);
  Serial.println(F("Sampling: 16x oversampling"));
  Serial.println(F("Filter: 16x coefficient"));
  Serial.println(F("Mode: Normal (continuous)"));
}

// Function to get JSON formatted data
String getJSONData() {
  String json = "{";
  json += "\"temperature\":" + String(currentReading.temperature, 2) + ",";
  json += "\"humidity\":" + String(currentReading.humidity, 2) + ",";
  json += "\"pressure\":" + String(currentReading.pressure, 2) + ",";
  json += "\"altitude\":" + String(currentReading.altitude, 2) + ",";
  json += "\"timestamp\":" + String(currentReading.timestamp);
  json += "}";
  return json;
}

// Function for deep sleep mode (battery powered applications)
void enterDeepSleep(int seconds) {
  Serial.printf("Entering deep sleep for %d seconds...\n", seconds);
  esp_sleep_enable_timer_wakeup(seconds * 1000000ULL);
  esp_deep_sleep_start();
}