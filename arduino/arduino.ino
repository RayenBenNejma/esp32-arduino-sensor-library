/*
 * HC-SR04 Obstacle Avoidance Robot
 * Uses ultrasonic sensor to detect and avoid obstacles
 * 
 * Components:
 * - HC-SR04 Ultrasonic Sensor
 * - L298N Motor Driver
 * - 2x DC Motors
 * - Arduino Uno
 */

// Ultrasonic sensor pins
#define TRIG_PIN 9
#define ECHO_PIN 10

// Motor driver pins
#define MOTOR_LEFT_FWD 2
#define MOTOR_LEFT_BACK 3
#define MOTOR_RIGHT_FWD 4
#define MOTOR_RIGHT_BACK 5
#define MOTOR_LEFT_SPEED 6   // PWM
#define MOTOR_RIGHT_SPEED 7  // PWM

// Constants
#define SAFE_DISTANCE 30     // Stop if object closer than 30cm
#define DANGER_DISTANCE 15   // Emergency stop distance
#define MOTOR_SPEED 200      // PWM value (0-255)

float distance;

void setup() {
  Serial.begin(9600);
  
  // Ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Motor pins
  pinMode(MOTOR_LEFT_FWD, OUTPUT);
  pinMode(MOTOR_LEFT_BACK, OUTPUT);
  pinMode(MOTOR_RIGHT_FWD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACK, OUTPUT);
  pinMode(MOTOR_LEFT_SPEED, OUTPUT);
  pinMode(MOTOR_RIGHT_SPEED, OUTPUT);
  
  Serial.println(F("🤖 Obstacle Avoidance Robot Ready!"));
  delay(2000);
}

void loop() {
  distance = measureDistance();
  
  Serial.print(F("Distance: "));
  Serial.print(distance);
  Serial.print(F(" cm - "));
  
  // Decision making based on distance
  if (distance < 0) {
    // Invalid reading - stop
    stopRobot();
    Serial.println(F("⚠️  No echo - Stopping"));
  }
  else if (distance < DANGER_DISTANCE) {
    // Very close - reverse and turn
    Serial.println(F("🔴 DANGER! Reversing..."));
    reverseRobot();
    delay(500);
    turnRight();
    delay(400);
  }
  else if (distance < SAFE_DISTANCE) {
    // Close - turn to avoid
    Serial.println(F("🟡 Obstacle detected - Turning"));
    turnRight();
    delay(300);
  }
  else {
    // Clear path - move forward
    Serial.println(F("🟢 Clear - Moving forward"));
    moveForward();
  }
  
  delay(100);
}

float measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  
  if (duration == 0) {
    return -1;
  }
  
  return duration * 0.034 / 2.0;
}

void moveForward() {
  digitalWrite(MOTOR_LEFT_FWD, HIGH);
  digitalWrite(MOTOR_LEFT_BACK, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACK, LOW);
  analogWrite(MOTOR_LEFT_SPEED, MOTOR_SPEED);
  analogWrite(MOTOR_RIGHT_SPEED, MOTOR_SPEED);
}

void reverseRobot() {
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_BACK, HIGH);
  digitalWrite(MOTOR_RIGHT_FWD, LOW);
  digitalWrite(MOTOR_RIGHT_BACK, HIGH);
  analogWrite(MOTOR_LEFT_SPEED, MOTOR_SPEED);
  analogWrite(MOTOR_RIGHT_SPEED, MOTOR_SPEED);
}

void turnRight() {
  digitalWrite(MOTOR_LEFT_FWD, HIGH);
  digitalWrite(MOTOR_LEFT_BACK, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, LOW);
  digitalWrite(MOTOR_RIGHT_BACK, HIGH);
  analogWrite(MOTOR_LEFT_SPEED, MOTOR_SPEED);
  analogWrite(MOTOR_RIGHT_SPEED, MOTOR_SPEED);
}

void turnLeft() {
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_BACK, HIGH);
  digitalWrite(MOTOR_RIGHT_FWD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACK, LOW);
  analogWrite(MOTOR_LEFT_SPEED, MOTOR_SPEED);
  analogWrite(MOTOR_RIGHT_SPEED, MOTOR_SPEED);
}

void stopRobot() {
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_BACK, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, LOW);
  digitalWrite(MOTOR_RIGHT_BACK, LOW);
  analogWrite(MOTOR_LEFT_SPEED, 0);
  analogWrite(MOTOR_RIGHT_SPEED, 0);
}