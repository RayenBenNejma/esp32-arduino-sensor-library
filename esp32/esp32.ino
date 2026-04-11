/*
 * BME680 Sensor - Complete Example for Arduino
 * Reads Temperature, Humidity, Pressure, and Gas Resistance
 * Calculates IAQ (Indoor Air Quality) Index
 * 
 * Connections:
 * BME680 VCC -> Arduino 3.3V
 * BME680 GND -> Arduino GND
 * BME680 SCL -> Arduino A5 (or SCL)
 * BME680 SDA -> Arduino A4 (or SDA)
 * 
 * Library: Adafruit BME680
 * Optional: BSEC library for IAQ calculation
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>

// BME680 I2C address (0x76 or 0x77)
#define BME680_ADDRESS 0x77
#define SEALEVELPRESSURE_HPA (1013.25)

// Create BME680 object
Adafruit_BME680 bme;

// Sensor data variables
float temperature;
float humidity;
float pressure;
float gasResistance;
float altitude;

// For air quality calculation
float baselineGasResistance = 0;
int   gasReadingsCount = 0;
bool  baselineEstablished = false;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println(F("BME680 Air Quality Sensor Test"));
  Serial.println(F("================================"));
  
  // Initialize BME680
  if (!bme.begin(BME680_ADDRESS)) {
    Serial.println(F("Could not find BME680 sensor!"));
    Serial.println(F("Check wiring and I2C address (0x76 or 0x77)"));
    while (1) delay(10);
  }
  
  Serial.println(F("✓ BME680 found!"));
  
  // Configure sensor oversampling and filter
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320°C for 150 ms
  
  Serial.println(F("\nEstablishing baseline gas resistance..."));
  Serial.println(F("Please wait 5 minutes for accurate readings"));
  Serial.println();
}

void loop() {
  // Perform reading
  if (!bme.performReading()) {
    Serial.println(F("Failed to perform reading"));
    delay(2000);
    return;
  }
  
  // Read all sensor values
  temperature = bme.temperature;
  humidity = bme.humidity;
  pressure = bme.pressure / 100.0; // Convert Pa to hPa
  gasResistance = bme.gas_resistance / 1000.0; // Convert to kΩ
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  
  // Establish baseline (average of first 50 readings)
  if (!baselineEstablished) {
    if (gasReadingsCount < 50) {
      baselineGasResistance += gasResistance;
      gasReadingsCount++;
      Serial.print(F("Calibrating... "));
      Serial.print(gasReadingsCount);
      Serial.println(F("/50"));
    } else {
      baselineGasResistance /= 50.0;
      baselineEstablished = true;
      Serial.println(F("\n✓ Baseline established!"));
      Serial.print(F("Baseline Gas Resistance: "));
      Serial.print(baselineGasResistance);
      Serial.println(F(" kΩ\n"));
    }
  }
  
  // Display readings
  printReadings();
  
  // Calculate and display air quality
  if (baselineEstablished) {
    calculateAirQuality();
  }
  
  Serial.println(F("-----------------------------------"));
  delay(2000);
}

void printReadings() {
  Serial.println(F("\n=== BME680 Sensor Readings ==="));
  
  Serial.print(F("Temperature:      "));
  Serial.print(temperature);
  Serial.println(F(" °C"));
  
  Serial.print(F("Humidity:         "));
  Serial.print(humidity);
  Serial.println(F(" %"));
  
  Serial.print(F("Pressure:         "));
  Serial.print(pressure);
  Serial.println(F(" hPa"));
  
  Serial.print(F("Gas Resistance:   "));
  Serial.print(gasResistance);
  Serial.println(F(" kΩ"));
  
  Serial.print(F("Approx. Altitude: "));
  Serial.print(altitude);
  Serial.println(F(" m"));
}

void calculateAirQuality() {
  // Simple air quality calculation based on gas resistance
  float gasRatio = gasResistance / baselineGasResistance;
  
  Serial.println(F("\n=== Air Quality Analysis ==="));
  Serial.print(F("Gas Ratio: "));
  Serial.println(gasRatio, 2);
  
  // Air quality interpretation
  if (gasRatio > 1.0) {
    Serial.println(F("Air Quality: GOOD"));
    Serial.println(F("Status: Clean air, low VOC levels"));
  } else if (gasRatio > 0.8) {
    Serial.println(F("Air Quality: MODERATE"));
    Serial.println(F("Status: Acceptable air quality"));
  } else if (gasRatio > 0.6) {
    Serial.println(F("Air Quality: POOR"));
    Serial.println(F("Status: Increased VOC detected"));
  } else {
    Serial.println(F("Air Quality: VERY POOR"));
    Serial.println(F("Status: High VOC levels - ventilate!"));
  }
  
  // Additional indicators
  if (humidity > 70) {
    Serial.println(F("⚠ High humidity - risk of mold"));
  }
  if (temperature > 30) {
    Serial.println(F("⚠ High temperature"));
  }
}

// Function to get formatted CSV data
void printCSVData() {
  Serial.print(millis());
  Serial.print(",");
  Serial.print(temperature);
  Serial.print(",");
  Serial.print(humidity);
  Serial.print(",");
  Serial.print(pressure);
  Serial.print(",");
  Serial.print(gasResistance);
  Serial.print(",");
  Serial.println(altitude);
}