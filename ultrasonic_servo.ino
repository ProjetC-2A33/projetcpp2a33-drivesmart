#include <Servo.h>

// Pin definitions
#define TRIG_PIN 9
#define ECHO_PIN 10
#define SERVO_PIN 7
#define GREEN_LED 3
#define RED_LED 4

// Distance threshold (in cm)
#define DISTANCE_THRESHOLD 50

// Servo positions
#define CLOSED_ANGLE 0
#define OPEN_ANGLE 180

Servo doorServo;
bool doorOpen = false;
bool isAuthorized = false;
unsigned long authTimeout = 0;
String incomingData = "";

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Initialize LED pins
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  
  // Attach and initialize servo
  doorServo.attach(SERVO_PIN);
  doorServo.write(CLOSED_ANGLE);
  
  // Turn off LEDs
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  
  Serial.println("=== CIN Access Control System Started ===");
  Serial.println("Waiting for person...");
  
  delay(1000); // Let servo settle
}

void loop() {
  // Check for incoming serial commands from Qt
  if (Serial.available() > 0) {
    incomingData = Serial.readStringUntil('\n');
    incomingData.trim();
    
    if (incomingData == "AUTHORIZED") {
      isAuthorized = true;
      authTimeout = millis() + 15000; // 15 second window to approach
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, LOW);
      Serial.println("Authorization GRANTED - Please approach the sensor");
    }
    else if (incomingData == "DENIED") {
      isAuthorized = false;
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
      Serial.println("Access DENIED - CIN not found in database");
      delay(3000);
      digitalWrite(RED_LED, LOW);
    }
  }
  
  // Check authorization timeout
  if (isAuthorized && millis() > authTimeout) {
    isAuthorized = false;
    digitalWrite(GREEN_LED, LOW);
    Serial.println("Authorization expired");
  }
  
  // Measure distance
  long distance = getDistance();
  
  // Only proceed if authorized
  if (isAuthorized) {
    // Only print if distance is valid
    if (distance > 0 && distance < 400) {
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");
      
      // If person detected within threshold
      if (distance < DISTANCE_THRESHOLD) {
        if (!doorOpen) {
          Serial.println(">>> AUTHORIZED PERSON DETECTED! Opening door...");
          openDoor();
          doorOpen = true;
          isAuthorized = false; // Reset authorization after use
          
          // Wait for person to pass through (3 seconds)
          delay(3000);
          
          // Close door automatically
          Serial.println(">>> Auto-closing door...");
          closeDoor();
          doorOpen = false;
        }
      }
    }
  } else {
    // Not authorized - just detect presence and request CIN verification
    // Don't request if door is currently open (prevents duplicate requests)
    if (!doorOpen && distance > 0 && distance < DISTANCE_THRESHOLD) {
      static unsigned long lastRequest = 0;
      if (millis() - lastRequest > 5000) { // Request every 5 seconds
        Serial.println("REQUEST_CIN");
        lastRequest = millis();
      }
    }
  }
  
  delay(200); // Delay between readings
}

long getDistance() {
  // Clear the trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  
  // Send 10 microsecond pulse
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Read the echo pin
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
  
  // If no echo received
  if (duration == 0) {
    return -1;
  }
  
  // Calculate distance in cm
  // Speed of sound = 343 m/s = 0.0343 cm/µs
  // Distance = (duration / 2) * 0.0343
  long distance = duration * 0.034 / 2;
  
  return distance;
}

void openDoor() {
  Serial.println("Moving servo to OPEN position...");
  doorServo.write(OPEN_ANGLE);
  delay(500); // Wait for servo to move
}

void closeDoor() {
  Serial.println("Moving servo to CLOSED position...");
  doorServo.write(CLOSED_ANGLE);
  delay(500); // Wait for servo to move
}
