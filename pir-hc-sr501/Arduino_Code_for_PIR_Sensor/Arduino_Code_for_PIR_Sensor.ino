/*
 * PIR Motion-Activated Smart Light
 * Automatic lighting with adjustable timeout
 * Energy-saving automatic shutoff
 * 
 * Components:
 * - HC-SR501 PIR Sensor
 * - Relay Module (to control AC lights)
 * - LED for status indication
 */

#define PIR_PIN 2
#define RELAY_PIN 7
#define STATUS_LED 13
#define MANUAL_BUTTON 3  // Optional manual override

// Settings
#define LIGHT_TIMEOUT 60000      // 60 seconds (1 minute)
#define OVERRIDE_TIMEOUT 300000  // 5 minutes manual override

// States
enum LightState {
  OFF,
  AUTO_ON,
  MANUAL_ON
};

LightState currentState = OFF;
unsigned long lastMotionTime = 0;
unsigned long manualOverrideTime = 0;
bool pirCalibrated = false;

void setup() {
  Serial.begin(9600);
  
  pinMode(PIR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  pinMode(MANUAL_BUTTON, INPUT_PULLUP);
  
  digitalWrite(RELAY_PIN, LOW);  // Light OFF
  
  Serial.println(F("💡 Smart Light System Initializing..."));
  
  // Calibrate PIR
  calibratePIR();
  
  Serial.println(F("✅ System Ready!\n"));
  printMenu();
}

void loop() {
  // Check manual button
  if (digitalRead(MANUAL_BUTTON) == LOW) {
    handleManualButton();
  }
  
  // Read PIR sensor
  bool motionDetected = digitalRead(PIR_PIN) == HIGH;
  
  // State machine
  switch (currentState) {
    case OFF:
      if (motionDetected) {
        turnLightOn(AUTO_ON);
      }
      break;
      
    case AUTO_ON:
      if (motionDetected) {
        lastMotionTime = millis();  // Reset timeout
      }
      // Check timeout
      if (millis() - lastMotionTime > LIGHT_TIMEOUT) {
        turnLightOff();
      }
      break;
      
    case MANUAL_ON:
      // Check manual override timeout
      if (millis() - manualOverrideTime > OVERRIDE_TIMEOUT) {
        Serial.println(F("⏰ Manual override timeout"));
        currentState = OFF;
        turnLightOff();
      }
      break;
  }
  
  // Status LED blink pattern
  updateStatusLED();
  
  delay(100);
}

void calibratePIR() {
  Serial.println(F("⏳ Calibrating PIR sensor..."));
  Serial.println(F("   Stay still for 30 seconds"));
  
  for (int i = 30; i > 0; i--) {
    Serial.print(F("   "));
    Serial.print(i);
    Serial.println(F("s"));
    delay(1000);
  }
  
  pirCalibrated = true;
  Serial.println(F("✅ PIR calibrated!\n"));
}

void turnLightOn(LightState state) {
  currentState = state;
  digitalWrite(RELAY_PIN, HIGH);
  lastMotionTime = millis();
  
  Serial.println(F("\n╔═══════════════════════════════════╗"));
  Serial.println(F("║     💡 LIGHT TURNED ON            ║"));
  Serial.println(F("╚═══════════════════════════════════╝"));
  
  if (state == AUTO_ON) {
    Serial.println(F("🤖 Mode: AUTOMATIC (Motion Detected)"));
    Serial.print(F("⏱️  Auto-off in: "));
    Serial.print(LIGHT_TIMEOUT / 1000);
    Serial.println(F(" seconds"));
  } else {
    Serial.println(F("👆 Mode: MANUAL OVERRIDE"));
    Serial.print(F("⏱️  Override duration: "));
    Serial.print(OVERRIDE_TIMEOUT / 1000);
    Serial.println(F(" seconds"));
  }
  Serial.println();
}

void turnLightOff() {
  currentState = OFF;
  digitalWrite(RELAY_PIN, LOW);
  
  Serial.println(F("\n╔═══════════════════════════════════╗"));
  Serial.println(F("║     🌙 LIGHT TURNED OFF           ║"));
  Serial.println(F("╚═══════════════════════════════════╝"));
  Serial.println(F("💤 Waiting for motion...\n"));
}

void handleManualButton() {
  delay(50);  // Debounce
  if (digitalRead(MANUAL_BUTTON) == LOW) {
    if (currentState == OFF) {
      manualOverrideTime = millis();
      turnLightOn(MANUAL_ON);
    } else {
      turnLightOff();
    }
    // Wait for button release
    while (digitalRead(MANUAL_BUTTON) == LOW) {
      delay(10);
    }
  }
}

void updateStatusLED() {
  static unsigned long lastBlink = 0;
  static bool ledState = false;
  
  unsigned long blinkInterval;
  
  switch (currentState) {
    case OFF:
      blinkInterval = 2000;  // Slow blink
      break;
    case AUTO_ON:
      blinkInterval = 500;   // Medium blink
      break;
    case MANUAL_ON:
      digitalWrite(STATUS_LED, HIGH);  // Solid on
      return;
  }
  
  if (millis() - lastBlink > blinkInterval) {
    ledState = !ledState;
    digitalWrite(STATUS_LED, ledState);
    lastBlink = millis();
  }
}

void printMenu() {
  Serial.println(F("═══════════════════════════════════"));
  Serial.println(F("        SMART LIGHT SYSTEM         "));
  Serial.println(F("═══════════════════════════════════"));
  Serial.println(F("Features:"));
  Serial.println(F("  • Automatic motion detection"));
  Serial.println(F("  • Manual override button"));
  Serial.println(F("  • Energy-saving auto-off"));
  Serial.println(F("  • Status LED indicator"));
  Serial.println(F("\nSettings:"));
  Serial.print(F("  Auto-off timeout: "));
  Serial.print(LIGHT_TIMEOUT / 1000);
  Serial.println(F(" seconds"));
  Serial.print(F("  Manual timeout: "));
  Serial.print(OVERRIDE_TIMEOUT / 1000);
  Serial.println(F(" seconds"));
  Serial.println(F("═══════════════════════════════════\n"));
}