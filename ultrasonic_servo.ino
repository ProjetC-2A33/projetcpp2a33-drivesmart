#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

// RFID Pin definitions
#define SS_PIN 10
#define RST_PIN 9

// Ultrasonic and Servo Pin definitions
#define TRIG_PIN 8
#define ECHO_PIN 7
#define SERVO_PIN 6

// LED and Buzzer Pin definitions (pour RFID)
#define LED_GREEN 3
#define LED_RED 4
#define BUZZER 5

// Distance threshold (in cm)
#define DISTANCE_THRESHOLD 50

// Servo positions
#define CLOSED_ANGLE 0
#define OPEN_ANGLE 180

MFRC522 rfid(SS_PIN, RST_PIN);
Servo doorServo;

bool doorOpen = false;
bool isAuthorized = false;
unsigned long authTimeout = 0;
String incomingData = "";

// Variables pour RFID LED control
bool ledState = false;
unsigned long ledStartTime = 0;
int currentCommand = 0; // 0 = rien, 1 = vert, 2 = rouge

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Initialize RFID
  SPI.begin();
  rfid.PCD_Init();
  
  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Initialize LED and Buzzer pins
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  // Attach and initialize servo
  doorServo.attach(SERVO_PIN);
  doorServo.write(CLOSED_ANGLE);
  
  // Turn off LEDs and buzzer
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  noTone(BUZZER);
  
  Serial.println("=== INTEGRATED ACCESS CONTROL SYSTEM ===");
  Serial.println("RFID + CIN Camera Ready");
  Serial.println("READY");
  
  delay(1000); // Let servo settle
}

void loop() {
  // 1. RFID Card Reading
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String uid = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      uid += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
      uid += String(rfid.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();
    
    if (uid.length() >= 8 && uid.length() <= 16) {
      Serial.println(uid); // Envoyer UID au PC
    }
    
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
  
  // 2. Check for incoming serial commands from Qt (pour RFID et CIN)
  if (Serial.available() > 0) {
    incomingData = Serial.readStringUntil('\n');
    incomingData.trim();
    
    // Commandes RFID (simples: '0' ou '1')
    if (incomingData == "0") {
      executeRFIDCommand(2); // Rouge
    } 
    else if (incomingData == "1") {
      executeRFIDCommand(1); // Vert
    }
    // Commandes CIN Servo
    else if (incomingData == "OPEN") {
      isAuthorized = true;
      authTimeout = millis() + 15000; // 15 second window
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, LOW);
      Serial.println("Door authorization GRANTED");
    }
    else if (incomingData == "CLOSE") {
      isAuthorized = false;
      digitalWrite(RED_LED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      Serial.println("Access DENIED");
      delay(2000);
      digitalWrite(RED_LED, LOW);
    }
  }
  
  // 3. Gérer le timer des LEDs RFID
  handleRFIDLEDTimer();
  
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

// ===== FONCTIONS RFID =====
void executeRFIDCommand(int cmd) {
  currentCommand = cmd;
  ledStartTime = millis();
  
  // Arrêter tout d'abord
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  noTone(BUZZER);
  
  // Appliquer la nouvelle commande
  if (cmd == 1) { // VERT - carte acceptée
    digitalWrite(LED_GREEN, HIGH);
    tone(BUZZER, 800); // Bip aigu court
  } 
  else if (cmd == 2) { // ROUGE - carte refusée
    digitalWrite(LED_RED, HIGH);
    tone(BUZZER, 1000); // Bip grave long
  }
}

void handleRFIDLEDTimer() {
  if (currentCommand == 0) return;
  
  unsigned long currentTime = millis();
  unsigned long elapsed = currentTime - ledStartTime;
  
  if ((currentCommand == 1 && elapsed >= 300) ||  // Vert: 300ms
      (currentCommand == 2 && elapsed >= 1000)) { // Rouge: 1000ms
    // Éteindre tout
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, LOW);
    noTone(BUZZER);
    currentCommand = 0;
  }
}
