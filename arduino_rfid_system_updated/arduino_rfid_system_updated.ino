/*
 * Arduino RFID System with LED and Buzzer Control - Version Simplifiée
 */

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define LED_GREEN 4
#define LED_RED 5
#define BUZZER 6

MFRC522 rfid(SS_PIN, RST_PIN);
bool ledState = false;
unsigned long ledStartTime = 0;
int currentCommand = 0; // 0 = rien, 1 = vert, 2 = rouge

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  noTone(BUZZER);
  
  Serial.println("READY");
}

void loop() {
  // 1. Lire les cartes RFID
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String uid = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      uid += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
      uid += String(rfid.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();
    
    if (uid.length() >= 8 && uid.length() <= 16) {
      Serial.println(uid);
    }
    
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
  
  // 2. Lire les commandes du PC (version simplifiée)
  if (Serial.available() > 0) {
    char command = Serial.read();
    
    if (command == '0') {
      executeCommand(2); // Rouge
    } 
    else if (command == '1') {
      executeCommand(1); // Vert
    }
  }
  
  // 3. Gérer le timer des LEDs
  handleLEDTimer();
  
  delay(10); // Petit délai pour stabilité
}

void executeCommand(int cmd) {
  currentCommand = cmd;
  ledStartTime = millis();
  
  // Arrêter tout d'abord
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  noTone(BUZZER);
  
  // Appliquer la nouvelle commande
  if (cmd == 1) { // VERT
    digitalWrite(LED_GREEN, HIGH);
    tone(BUZZER, 800);
  } 
  else if (cmd == 2) { // ROUGE
    digitalWrite(LED_RED, HIGH);
    tone(BUZZER, 1000);
  }
}

void handleLEDTimer() {
  if (currentCommand == 0) return;
  
  unsigned long currentTime = millis();
  unsigned long elapsed = currentTime - ledStartTime;
  
  if ((currentCommand == 1 && elapsed >= 300) ||  // Vert: 200ms
      (currentCommand == 2 && elapsed >= 1000)) { // Rouge: 1000ms
    // Éteindre tout
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, LOW);
    noTone(BUZZER);
    currentCommand = 0;
  }
}