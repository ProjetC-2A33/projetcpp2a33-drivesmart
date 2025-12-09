# Intégration des Systèmes Arduino - DriveSmart

## 📋 Vue d'ensemble

Ce document explique l'intégration des **deux systèmes Arduino** dans DriveSmart :

1. **Système RFID** (`arduino.cpp/h` + `ultrasonic_servo.ino`)
2. **Système CIN avec Caméra** (`cin_access_control.cpp/h` + ultrasonic/servo)

## 🔗 Architecture Intégrée

```
┌─────────────────────────────────────────────────────────┐
│                    MainWindow (Qt)                      │
│  ┌────────────────────┐      ┌────────────────────┐    │
│  │  ArduinoReader     │◄────►│  CINAccessControl  │    │
│  │  (RFID System)     │      │  (Camera System)   │    │
│  └────────┬───────────┘      └──────────┬─────────┘    │
│           │                              │              │
└───────────┼──────────────────────────────┼──────────────┘
            │                              │
            │ Port série partagé           │
            ▼                              ▼
    ┌───────────────────────────────────────────────┐
    │    ultrasonic_servo.ino (Arduino Uno)        │
    │  ┌──────────────┐     ┌──────────────────┐   │
    │  │ RFID RC522   │     │ Servo + Ultrason │   │
    │  │ + LED/Buzzer │     │ + LED            │   │
    │  └──────────────┘     └──────────────────┘   │
    └───────────────────────────────────────────────┘
                           +
            ┌───────────────────────────┐
            │  Python (OpenCV)          │
            │  cin_verification.py      │
            │  Reconnaissance CIN       │
            └───────────────────────────┘
```

## 🎯 Fonctionnalités Intégrées

### Mode 1 : RFID (Prioritaire)
1. L'utilisateur présente sa carte RFID
2. L'Arduino lit l'UID et l'envoie au PC
3. `ArduinoReader` vérifie dans la base de données
4. **Si trouvé** :
   - LED verte + bip court
   - Accès accordé
5. **Si non trouvé** :
   - LED rouge + bip long
   - Déclenchement automatique du **Mode 2**

### Mode 2 : CIN avec Caméra (Backup)
1. Déclenché automatiquement si RFID échoue
2. Le capteur ultrasonique détecte une personne
3. La caméra capture l'image du CIN
4. Python (OpenCV) extrait les données OCR
5. `CINAccessControl` vérifie dans la base
6. **Si trouvé et disponible** :
   - Servo ouvre la porte
   - LED verte
7. **Si refusé** :
   - Servo reste fermé
   - LED rouge

## 📡 Communication Série

### Format des Commandes PC → Arduino

| Commande | Système | Action |
|----------|---------|--------|
| `"0\n"` | RFID | LED rouge + bip long (carte refusée) |
| `"1\n"` | RFID | LED verte + bip court (carte acceptée) |
| `"OPEN\n"` | CIN | Ouvrir le servo (autorisation accordée) |
| `"CLOSE\n"` | CIN | Fermer le servo (accès refusé) |

### Format des Messages Arduino → PC

| Message | Système | Signification |
|---------|---------|---------------|
| `"A1B2C3D4\n"` | RFID | UID de carte détecté (8-16 caractères hex) |
| `"READY\n"` | Startup | Arduino prêt |
| `"Distance: XX cm\n"` | Ultrason | Distance mesurée |

## 🔌 Branchement Arduino

### Pins Utilisés

```cpp
// RFID RC522
#define SS_PIN 10
#define RST_PIN 9

// Capteur Ultrasonique
#define TRIG_PIN 8
#define ECHO_PIN 7

// Servo Moteur
#define SERVO_PIN 6

// LEDs et Buzzer
#define LED_GREEN 3
#define LED_RED 4
#define BUZZER 5
```

### Schéma de Connexion

```
Arduino Uno
├─ RFID RC522
│  ├─ SDA  → Pin 10
│  ├─ SCK  → Pin 13
│  ├─ MOSI → Pin 11
│  ├─ MISO → Pin 12
│  ├─ RST  → Pin 9
│  ├─ VCC  → 3.3V
│  └─ GND  → GND
│
├─ Capteur HC-SR04
│  ├─ TRIG → Pin 8
│  ├─ ECHO → Pin 7
│  ├─ VCC  → 5V
│  └─ GND  → GND
│
├─ Servo SG90
│  ├─ Signal → Pin 6
│  ├─ VCC    → 5V
│  └─ GND    → GND
│
└─ Indicateurs
   ├─ LED Verte  → Pin 3 (+ résistance 220Ω)
   ├─ LED Rouge  → Pin 4 (+ résistance 220Ω)
   └─ Buzzer     → Pin 5
```

## 🚀 Démarrage

### 1. Téléverser le Sketch Arduino

```bash
# Ouvrir Arduino IDE
# Charger: ultrasonic_servo.ino
# Installer les bibliothèques:
#   - MFRC522 (pour RFID)
#   - Servo (incluse)
# Sélectionner: Arduino Uno
# Port: COM3 (ou autre selon votre système)
# Téléverser
```

### 2. Démarrer l'Application Qt

```bash
# L'application démarre automatiquement:
# 1. ArduinoReader (RFID)
# 2. CINAccessControl (Python + Caméra)
# 3. Liaison entre les deux systèmes
```

### 3. Test de Fonctionnement

**Test RFID:**
- Présenter une carte RFID
- Vérifier LED verte/rouge
- Écouter le bip

**Test CIN:**
- Approcher du capteur sans carte
- La caméra doit s'activer
- Présenter le CIN devant la caméra
- Observer l'ouverture du servo si valide

## 🔧 Configuration

### Fichier `connection.cpp`
```cpp
// Base de données Oracle
db.setDatabaseName("projet");
db.setUserName("projet");
db.setPassword("123");
```

### Fichier `cin_verification.py`
```python
# Chemin de configuration
TESSERACT_PATH = r"C:\Program Files\Tesseract-OCR\tesseract.exe"
```

## 📊 Tables de Base de Données

### Table CONDIDAT (pour RFID)
```sql
CREATE TABLE CONDIDAT (
    CIN_CONDIDAT VARCHAR2(50) PRIMARY KEY,
    NOM VARCHAR2(20),
    PRENOM VARCHAR2(20),
    UID VARCHAR2(50)  -- UID de la carte RFID
);
```

### Table EMPLOYEE (pour CIN)
```sql
CREATE TABLE EMPLOYEE (
    CIN_EMPLOYEE VARCHAR2(50) PRIMARY KEY,
    NOM VARCHAR2(20),
    PRENOM VARCHAR2(20),
    DISPONIBILITE VARCHAR2(20)  -- 'Disponible' ou 'Indisponible'
);
```

## 🐛 Dépannage

### Problème: Arduino non détecté
**Solution:**
- Vérifier le port COM dans le Gestionnaire de périphériques
- Essayer un autre câble USB
- Redémarrer l'application Qt

### Problème: RFID ne répond pas
**Solution:**
- Vérifier les connexions SPI
- Tester avec l'exemple MFRC522
- Alimenter le RC522 en 3.3V (pas 5V!)

### Problème: Servo ne bouge pas
**Solution:**
- Vérifier l'alimentation 5V du servo
- Tester manuellement avec `doorServo.write(90);`
- Vérifier le pin SERVO_PIN

### Problème: Python/Caméra ne démarre pas
**Solution:**
- Vérifier l'installation de Tesseract OCR
- Vérifier les permissions de la caméra
- Lire les logs Python dans la console Qt

## 📝 Logs et Debugging

### Dans Qt Creator (Console d'application)
```
✓ Arduino RFID System connected successfully
✓ CIN Access Control System initialized
✓ Arduino serial linked to CIN Access Control
✓ Integrated Access Control System ready!
  - RFID cards will be checked first
  - CIN camera verification as backup
```

### Dans le Moniteur Série Arduino
```
=== INTEGRATED ACCESS CONTROL SYSTEM ===
RFID + CIN Camera Ready
READY
```

## 👥 Crédits

- **Système RFID**: Amine & Wael
- **Système CIN**: Amine & Wael  
- **Intégration**: Assistant IA
- **Projet**: DriveSmart 2A33

---

**Date d'intégration**: 10 Décembre 2025  
**Version**: 1.0 - Système Unifié
