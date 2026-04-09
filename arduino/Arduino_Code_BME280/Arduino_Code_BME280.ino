/*
 * BME280 Sensor - Complete Example for Arduino
 * Reads Temperature, Humidity, and Pressure
 * 
 * Connections:
 * BME280 VCC -> Arduino 3.3V
 * BME280 GND -> Arduino GND
 * BME280 SCL -> Arduino A5 (or SCL pin)
 * BME280 SDA -> Arduino A4 (or SDA pin)
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// I2C Address (default is 0x76, some modules use 0x77)
#define BME280_ADDRESS 0x76
#define SEALEVELPRESSURE_HPA (1013.25)

// Create BME280 object
Adafruit_BME280 bme;

// Variables to store sensor data
float temperature;
float humidity;
float pressure;
float altitude;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println(F("BME280 Sensor Test"));
  
  // Initialize BME280
  if (!bme.begin(BME280_ADDRESS)) {
    Serial.println(F("Could not find BME280 sensor!"));
    Serial.println(F("Check wiring and I2C address"));
    while (1) delay(10);
  }
  
  Serial.println(F("BME280 sensor initialized successfully!"));
  Serial.println();
  
  // Configure sensor settings
  bme.setSampling(Adafruit_BME280::MODE_NORMAL,
                  Adafruit_BME280::SAMPLING_X16,  // temperature
                  Adafruit_BME280::SAMPLING_X16,  // pressure
                  Adafruit_BME280::SAMPLING_X16,  // humidity
                  Adafruit_BME280::FILTER_X16,
                  Adafruit_BME280::STANDBY_MS_500);
}

void loop() {
  // Read sensor values
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F; // Convert Pa to hPa
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  
  // Display readings
  Serial.println(F("=== BME280 Readings ==="));
  
  Serial.print(F("Temperature: "));
  Serial.print(temperature);
  Serial.println(F(" °C"));
  
  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.println(F(" %"));
  
  Serial.print(F("Pressure: "));
  Serial.print(pressure);
  Serial.println(F(" hPa"));
  
  Serial.print(F("Approx. Altitude: "));
  Serial.print(altitude);
  Serial.println(F(" m"));
  
  Serial.println();
  
  delay(2000); // Read every 2 seconds
}

// Function to check if readings are valid
bool isValidReading() {
  if (isnan(temperature) || isnan(humidity) || isnan(pressure)) {
    return false;
  }
  return true;
}

// Function to print formatted data
void printFormattedData() {
  if (isValidReading()) {
    char buffer[100];
    sprintf(buffer, "T:%.2f°C | H:%.2f%% | P:%.2fhPa | Alt:%.2fm", 
            temperature, humidity, pressure, altitude);
    Serial.println(buffer);
  } else {
    Serial.println(F("Error: Invalid sensor reading"));
  }
}