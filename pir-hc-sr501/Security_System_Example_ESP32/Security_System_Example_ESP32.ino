/*
 * PIR Security System with ESP32
 * Multiple zones, alarm, notifications
 */

#define PIR_ZONE1 27  // Front door
#define PIR_ZONE2 26  // Back door
#define PIR_ZONE3 25  // Living room
#define SIREN_PIN 33
#define ARM_BUTTON 32

enum SystemState {
  DISARMED,
  ARMED_AWAY,
  ARMED_HOME,
  TRIGGERED
};

SystemState systemState = DISARMED;
bool zone1Triggered = false;
bool zone2Triggered = false;
bool zone3Triggered = false;

unsigned long alarmStartTime = 0;
const unsigned long SIREN_DURATION = 300000;  // 5 minutes

void setup() {
  Serial.begin(115200);
  
  pinMode(PIR_ZONE1, INPUT);
  pinMode(PIR_ZONE2, INPUT);
  pinMode(PIR_ZONE3, INPUT);
  pinMode(SIREN_PIN, OUTPUT);
  pinMode(ARM_BUTTON, INPUT_PULLUP);
  
  digitalWrite(SIREN_PIN, LOW);
  
  Serial.println(F("🔒 Security System Initialized"));
  printMenu();
}

void loop() {
  // Check arm/disarm button
  if (digitalRead(ARM_BUTTON) == LOW) {
    handleArmButton();
  }
  
  // Monitor zones if armed
  if (systemState == ARMED_AWAY || systemState == ARMED_HOME) {
    checkZones();
  }
  
  // Handle triggered state
  if (systemState == TRIGGERED) {
    handleAlarm();
  }
  
  // Check for serial commands
  if (Serial.available()) {
    handleSerialCommand();
  }
  
  delay(100);
}

void checkZones() {
  bool zone1 = digitalRead(PIR_ZONE1) == HIGH;
  bool zone2 = digitalRead(PIR_ZONE2) == HIGH;
  bool zone3 = digitalRead(PIR_ZONE3) == HIGH;
  
  if (systemState == ARMED_AWAY) {
    // All zones active
    if (zone1 || zone2 || zone3) {
      triggerAlarm(zone1 ? 1 : (zone2 ? 2 : 3));
    }
  } else if (systemState == ARMED_HOME) {
    // Only perimeter zones (1 and 2)
    if (zone1 || zone2) {
      triggerAlarm(zone1 ? 1 : 2);
    }
  }
}

void triggerAlarm(int zone) {
  systemState = TRIGGERED;
  alarmStartTime = millis();
  
  Serial.println(F("\n🚨🚨🚨 ALARM TRIGGERED! 🚨🚨🚨"));
  Serial.printf("Zone %d: %s\n", zone, 
                zone == 1 ? "Front Door" : 
                zone == 2 ? "Back Door" : "Living Room");
  Serial.println(F("⏰ ") + String(millis() / 1000) + F(" seconds"));
  
  digitalWrite(SIREN_PIN, HIGH);
  
  // Send notification
  sendNotification("ALARM TRIGGERED", zone);
}

void handleAlarm() {
  // Sound siren pattern
  static unsigned long lastToggle = 0;
  if (millis() - lastToggle > 500) {
    digitalWrite(SIREN_PIN, !digitalRead(SIREN_PIN));
    lastToggle = millis();
  }
  
  // Auto-shutoff after duration
  if (millis() - alarmStartTime > SIREN_DURATION) {
    Serial.println(F("⏰ Siren timeout - silencing"));
    digitalWrite(SIREN_PIN, LOW);
    systemState = ARMED_AWAY;  // Re-arm
  }
}

void handleArmButton() {
  delay(50);  // Debounce
  
  if (digitalRead(ARM_BUTTON) == LOW) {
    cycleArmState();
    while (digitalRead(ARM_BUTTON) == LOW) {
      delay(10);
    }
  }
}

void cycleArmState() {
  switch (systemState) {
    case DISARMED:
      systemState = ARMED_HOME;
      Serial.println(F("🏠 Armed: HOME MODE"));
      Serial.println(F("   Perimeter zones active"));
      break;
    case ARMED_HOME:
      systemState = ARMED_AWAY;
      Serial.println(F("🚗 Armed: AWAY MODE"));
      Serial.println(F("   All zones active"));
      break;
    case ARMED_AWAY:
    case TRIGGERED:
      systemState = DISARMED;
      digitalWrite(SIREN_PIN, LOW);
      Serial.println(F("🔓 DISARMED"));
      break;
  }
}

void handleSerialCommand() {
  char cmd = Serial.read();
  
  switch (cmd) {
    case 'd':
    case 'D':
      systemState = DISARMED;
      digitalWrite(SIREN_PIN, LOW);
      Serial.println(F("🔓 System DISARMED"));
      break;
    case 'h':
    case 'H':
      systemState = ARMED_HOME;
      Serial.println(F("🏠 Armed: HOME MODE"));
      break;
    case 'a':
    case 'A':
      systemState = ARMED_AWAY;
      Serial.println(F("🚗 Armed: AWAY MODE"));
      break;
    case 's':
    case 'S':
      printStatus();
      break;
  }
}

void printStatus() {
  Serial.println(F("\n╔════════════════════════════════════╗"));
  Serial.println(F("║      SECURITY SYSTEM STATUS        ║"));
  Serial.println(F("╚════════════════════════════════════╝"));
  
  Serial.print(F("State: "));
  switch (systemState) {
    case DISARMED: Serial.println(F("🔓 DISARMED")); break;
    case ARMED_HOME: Serial.println(F("🏠 ARMED - HOME")); break;
    case ARMED_AWAY: Serial.println(F("🚗 ARMED - AWAY")); break;
    case TRIGGERED: Serial.println(F("🚨 ALARM TRIGGERED")); break;
  }
  
  Serial.println(F("\nZone Status:"));
  Serial.printf("  Zone 1 (Front): %s\n", 
                digitalRead(PIR_ZONE1) ? "MOTION" : "Clear");
  Serial.printf("  Zone 2 (Back):  %s\n", 
                digitalRead(PIR_ZONE2) ? "MOTION" : "Clear");
  Serial.printf("  Zone 3 (Room):  %s\n", 
                digitalRead(PIR_ZONE3) ? "MOTION" : "Clear");
  
  Serial.println(F("════════════════════════════════════\n"));
}

void printMenu() {
  Serial.println(F("\n═══════════════════════════════════"));
  Serial.println(F("     SECURITY SYSTEM COMMANDS      "));
  Serial.println(F("═══════════════════════════════════"));
  Serial.println(F("  D - Disarm system"));
  Serial.println(F("  H - Arm (Home mode)"));
  Serial.println(F("  A - Arm (Away mode)"));
  Serial.println(F("  S - Show status"));
  Serial.println(F("═══════════════════════════════════\n"));
}

void sendNotification(String message, int zone) {
  // Implement notification system:
  // - Send email
  // - Send push notification
  // - MQTT publish
  // - HTTP POST to server
  Serial.println(F("📧 Notification sent: ") + message);
}