/*
 * HC-SR04 Water Level Monitor - ESP32
 * Measures water level in tank
 * Sends alerts when low/high
 */

#define TRIG_PIN 5
#define ECHO_PIN 18

// Tank specifications (adjust for your tank)
#define TANK_HEIGHT 200     // cm (from sensor to bottom)
#define TANK_DIAMETER 100   // cm
#define TANK_CAPACITY 1570  // liters (calculated from diameter)

// Alert thresholds
#define LOW_LEVEL_PERCENT 20
#define HIGH_LEVEL_PERCENT 90

struct TankData {
  float distanceToWater;
  float waterLevel;
  float waterLevelPercent;
  float volumeLiters;
  bool alertLow;
  bool alertHigh;
};

TankData tank;

void setup() {
  Serial.begin(115200);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  Serial.println(F("\n💧 Water Level Monitoring System"));
  Serial.printf("Tank Height: %d cm\n", TANK_HEIGHT);
  Serial.printf("Tank Capacity: %d liters\n\n", TANK_CAPACITY);
  
  delay(2000);
}

void loop() {
  measureWaterLevel();
  displayTankStatus();
  checkAlerts();
  
  delay(5000);  // Update every 5 seconds
}

void measureWaterLevel() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  
  if (duration == 0) {
    tank.distanceToWater = -1;
    return;
  }
  
  tank.distanceToWater = duration * 0.034 / 2.0;
  tank.waterLevel = TANK_HEIGHT - tank.distanceToWater;
  tank.waterLevelPercent = (tank.waterLevel / TANK_HEIGHT) * 100.0;
  
  // Calculate volume (cylindrical tank)
  float radius = TANK_DIAMETER / 2.0;
  float area = 3.14159 * radius * radius;
  tank.volumeLiters = (area * tank.waterLevel) / 1000.0;
  
  // Check alerts
  tank.alertLow = tank.waterLevelPercent < LOW_LEVEL_PERCENT;
  tank.alertHigh = tank.waterLevelPercent > HIGH_LEVEL_PERCENT;
}

void displayTankStatus() {
  Serial.println(F("\n╔═══════════════════════════════════════╗"));
  Serial.println(F("║      WATER TANK STATUS                ║"));
  Serial.println(F("╚═══════════════════════════════════════╝"));
  
  if (tank.distanceToWater < 0) {
    Serial.println(F("❌ ERROR: Unable to measure"));
    return;
  }
  
  Serial.printf("📏 Distance to water: %.1f cm\n", tank.distanceToWater);
  Serial.printf("💧 Water level:       %.1f cm\n", tank.waterLevel);
  Serial.printf("📊 Fill percentage:   %.1f%%\n", tank.waterLevelPercent);
  Serial.printf("🚰 Volume:            %.1f liters\n", tank.volumeLiters);
  
  printTankVisualization();
}

void printTankVisualization() {
  Serial.println(F("\n   TANK VISUALIZATION"));
  Serial.println(F("   ┌────────────┐"));
  
  int waterBars = (int)(tank.waterLevelPercent / 5);  // 20 levels
  
  for (int i = 20; i >= 0; i--) {
    Serial.print(F("   │"));
    if (i <= waterBars) {
      Serial.print(F("▓▓▓▓▓▓▓▓▓▓"));
    } else {
      Serial.print(F("          "));
    }
    Serial.print(F("│"));
    
    if (i == 20) Serial.print(F(" ← 100%"));
    if (i == 10) Serial.print(F(" ← 50%"));
    if (i == 0)  Serial.print(F(" ← 0%"));
    
    Serial.println();
  }
  
  Serial.println(F("   └────────────┘\n"));
}

void checkAlerts() {
  Serial.println(F("⚡ ALERT STATUS:"));
  
  if (tank.alertLow) {
    Serial.println(F("  🔴 LOW WATER LEVEL!"));
    Serial.println(F("  ⚠️  Refill tank soon"));
  } else if (tank.alertHigh) {
    Serial.println(F("  🔵 HIGH WATER LEVEL"));
    Serial.println(F("  ⚠️  Tank nearly full"));
  } else {
    Serial.println(F("  🟢 Water level normal"));
  }
  
  Serial.println(F("═══════════════════════════════════════\n"));
}