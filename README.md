# 🚗 DriveSmart - Auto-École Management System

[![Qt](https://img.shields.io/badge/Qt-6.7.3-green.svg)](https://www.qt.io/)
[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![Oracle](https://img.shields.io/badge/Database-Oracle-red.svg)](https://www.oracle.com/)
[![Arduino](https://img.shields.io/badge/Arduino-RFID%20%2B%20OCR-teal.svg)](https://www.arduino.cc/)

**DriveSmart** is a comprehensive driving school management system featuring real-time scheduling, Arduino-based access control, interactive maps, and advanced statistics visualization.

---

## 📑 Table of Contents

- [Features](#-features)
- [Technologies](#-technologies)
- [Architecture](#-architecture)
- [Prerequisites](#-prerequisites)
- [Installation](#-installation)
- [Database Setup](#-database-setup)
- [Hardware Setup](#-hardware-setup)
- [Configuration](#-configuration)
- [Usage](#-usage)
- [Project Structure](#-project-structure)
- [API Integration](#-api-integration)
- [Contributing](#-contributing)
- [License](#-license)

---

## ✨ Features

### 👥 User Management
- **Candidates**: Complete CRUD operations with RFID card assignment
- **Employees/Instructors**: Real-time availability tracking with statistics
- **Role-based Access**: Secure authentication system

### 📅 Intelligent Planning
- **Session Scheduling**: Drive and theory session management
- **Conflict Detection**: Automatic validation of instructor/vehicle/candidate availability
- **Calendar Integration**: Local calendar system with visual date picker
- **Advanced Search & Filters**: Multi-criteria search with real-time filtering
- **PDF Export**: Generate professional planning reports

### 🚗 Vehicle Management
- Vehicle registration and tracking (matricule, brand, model)
- Availability status monitoring
- Assignment to driving sessions

### 📝 Exam Management
- Exam scheduling and tracking
- Result recording
- Historical data management

### 🛠️ Equipment Management
- Equipment inventory system
- Condition monitoring
- Assignment tracking

### 🔐 Arduino Access Control System
- **RFID RC522**: Employee card authentication with working hours verification
- **OCR Recognition**: Automatic National ID Card (CIN) verification using webcam
- **Ultrasonic Sensor HC-SR04**: Proximity detection for automatic capture
- **Servo Motor Control**: Automated door access
- **LED Indicators**: Visual feedback (green=access granted, red=denied)
- **Real-time Availability Updates**: Auto-update employee status in database

### 🗺️ Map Integration
- **OpenStreetMap Service**: 100% FREE - No API key required
- **Geocoding**: Convert addresses to coordinates
- **Static Maps**: Display location previews
- **Circuit Selection**: Predefined driving circuits with custom address support

### 📊 Advanced Statistics
- **Interactive Charts**: Pie charts and bar graphs using QtCharts
- **Session Analytics**: 
  - Session type distribution (driving vs theory)
  - Top 5 instructors by session count
  - Monthly session trends (last 6 months)
- **Real-time Metrics**: Active candidates, instructors, total sessions
- **Employee Statistics**: Availability distribution with visual pie charts

### 🔍 Search & Export
- **Multi-criteria Search**: Real-time text search across all fields
- **Advanced Sorting**: Sort by date, candidate, instructor, type, time, circuit
- **PDF Generation**: Export filtered/sorted planning with professional formatting

---

## 🛠️ Technologies

### Frontend & Framework
- **Qt 6.7.3**: Cross-platform GUI framework
  - Qt Widgets: UI components
  - Qt SQL: Database integration
  - Qt Network: HTTP requests for maps
  - Qt Charts: Data visualization
  - Qt SerialPort: Arduino communication

### Backend & Database
- **C++17**: Modern C++ standards
- **Oracle Database**: Enterprise-grade RDBMS with ODBC connectivity
- **SQL**: Complex queries, joins, aggregations, stored procedures

### Hardware & IoT
- **Arduino Uno/Mega**: Microcontroller platform
- **RFID-RC522**: 13.56MHz contactless card reader
- **HC-SR04**: Ultrasonic distance sensor
- **Servo Motor**: Door control mechanism
- **LEDs**: Visual indicators
- **Webcam**: Image capture for OCR

### Additional Libraries
- **Tesseract OCR**: Optical character recognition (Python integration)
- **QR Code Generator**: QR code generation for cards (qrcodegen library)
- **OpenStreetMap Nominatim API**: Free geocoding and mapping

---

## 🏗️ Architecture

```
DriveSmart Application
│
├── UI Layer (Qt Widgets)
│   ├── Login Dialog
│   ├── Main Window (Navigation Hub)
│   ├── Candidate Management
│   ├── Employee Management
│   ├── Planning Management
│   ├── Vehicle Management
│   ├── Exam Management
│   └── Equipment Management
│
├── Business Logic Layer (C++)
│   ├── CRUD Operations
│   ├── Validation Logic
│   ├── Conflict Detection
│   ├── Statistics Calculation
│   └── Calendar Management
│
├── Data Access Layer
│   ├── Oracle Database Connection
│   ├── SQL Query Execution
│   └── Transaction Management
│
├── External Services
│   ├── OpenStreetMap Service (Maps & Geocoding)
│   ├── Local Calendar Service (Event Management)
│   └── Arduino Communication (Access Control)
│
└── Hardware Layer
    ├── RFID Reader
    ├── Webcam + OCR
    ├── Ultrasonic Sensor
    └── Servo + LEDs
```

---

## 📋 Prerequisites

### Software Requirements
- **Qt 6.7.3 or higher** with components:
  - Qt Widgets
  - Qt SQL
  - Qt Network
  - Qt Charts
  - Qt SerialPort
- **C++ Compiler**: GCC 7+ / MSVC 2019+ / Clang 5+
- **Oracle Database 11g or higher**
- **Oracle Instant Client** (for ODBC connectivity)
- **Arduino IDE 1.8.19+** (for uploading firmware)
- **Python 3.8+** with Tesseract OCR (for CIN verification - optional)

### Hardware Requirements (Optional - for Access Control)
- Arduino Uno or Mega
- RFID-RC522 module
- HC-SR04 ultrasonic sensor
- SG90 servo motor
- LEDs (red & green)
- Webcam
- Resistors, jumper wires, breadboard

---

## 🚀 Installation

### 1. Clone the Repository

```bash
git clone https://github.com/ProjetC-2A33/projetcpp2a33-drivesmart.git
cd projetcpp2a33-drivesmart
```

### 2. Install Qt Dependencies

**Windows:**
```bash
# Download and install Qt from https://www.qt.io/download
# Make sure to select Qt 6.7.3 and required components
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt update
sudo apt install qt6-base-dev qt6-charts-dev qt6-serialport-dev
sudo apt install libqt6sql6-odbc
```

### 3. Install Oracle Client

**Windows:**
1. Download Oracle Instant Client from [Oracle Downloads](https://www.oracle.com/database/technologies/instant-client/downloads.html)
2. Extract to `C:\oracle\instantclient_21_3`
3. Add to PATH: `C:\oracle\instantclient_21_3`

**Linux:**
```bash
sudo apt install libaio1
wget https://download.oracle.com/otn_software/linux/instantclient/instantclient-basic-linux.x64-21.1.0.0.0.zip
sudo unzip instantclient-basic-linux.x64-21.1.0.0.0.zip -d /opt/oracle
export LD_LIBRARY_PATH=/opt/oracle/instantclient_21_1:$LD_LIBRARY_PATH
```

### 4. Configure Database Connection

Edit `connection.cpp` with your Oracle credentials:

```cpp
bool Connection::createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("ORACLE_DB"); // Your TNS name or connection string
    db.setUserName("your_username");
    db.setPassword("your_password");
    
    return db.open();
}
```

### 5. Build the Project

**Using Qt Creator:**
1. Open `drivesmart.pro`
2. Configure kit (Qt 6.7.3)
3. Build → Rebuild All (Ctrl+B)
4. Run (Ctrl+R)

**Using Command Line:**
```bash
qmake drivesmart.pro
make
./DriveSmart  # Linux
# or
DriveSmart.exe  # Windows
```

---

## 💾 Database Setup

### 1. Create Tables

Run the SQL scripts in your Oracle database:

```sql
-- Connect to your database
sqlplus your_username/your_password@your_database

-- Run table creation scripts
@create_calendar_table.sql
@fix_planning_table.sql
```

### 2. Required Tables

The application requires the following tables:

- **CONDIDAT**: Candidate information
- **EMPLOYEE**: Employee/instructor data
- **VEHICULE**: Vehicle registry
- **PLANNING**: Session scheduling
- **EXAMEN**: Exam records
- **EQUIPEMENT**: Equipment inventory
- **CALENDAR_EVENTS**: Local calendar events (auto-created by script)

### 3. Verify Setup

```sql
-- Check tables exist
SELECT table_name FROM user_tables;

-- Check sample data
SELECT COUNT(*) FROM EMPLOYEE;
SELECT COUNT(*) FROM PLANNING;
```

---

## 🔧 Hardware Setup (Optional)

### Arduino Wiring Diagram

```
RFID-RC522          Arduino
SDA         ------> Pin 10
SCK         ------> Pin 13
MOSI        ------> Pin 11
MISO        ------> Pin 12
RST         ------> Pin 9
3.3V        ------> 3.3V
GND         ------> GND

HC-SR04            Arduino
VCC         ------> 5V
TRIG        ------> Pin 7
ECHO        ------> Pin 6
GND         ------> GND

Servo Motor        Arduino
Signal      ------> Pin 3
VCC         ------> 5V
GND         ------> GND

LEDs               Arduino
Green LED   ------> Pin 4 (with 220Ω resistor)
Red LED     ------> Pin 5 (with 220Ω resistor)
```

### Upload Arduino Sketch

1. Open Arduino IDE
2. Install required libraries:
   - MFRC522 (for RFID)
   - Servo
3. Open `arduino_rfid_system_updated/arduino_rfid_system_updated.ino`
4. Select board and port
5. Upload

### Python OCR Setup (Optional)

```bash
# Install Tesseract OCR
# Windows: Download from https://github.com/UB-Mannheim/tesseract/wiki
# Linux:
sudo apt install tesseract-ocr python3-opencv python3-pytesseract

# Configure Python script
python cin_verification.py
```

---

## ⚙️ Configuration

### 1. Serial Port Configuration

In `arduino.h` or UI settings, configure the Arduino COM port:

```cpp
// Windows
QString portName = "COM3";

// Linux
QString portName = "/dev/ttyUSB0";  // or /dev/ttyACM0
```

### 2. Map Service (OpenStreetMap)

No API key required! The application uses the free OpenStreetMap Nominatim service.

For custom circuit locations, edit in `planning.cpp`:

```cpp
QString Planning::getCircuitAddress(const QString &circuitName)
{
    if (circuitName == "Circuit A") return "123 Rue Example, Tunis";
    if (circuitName == "Circuit B") return "456 Avenue Test, Ariana";
    // Add your circuits here
    return circuitName;
}
```

### 3. Calendar Configuration

The local calendar system stores events in your Oracle database. No additional configuration needed!

---

## 📖 Usage

### First Launch

1. **Login**: Use your employee credentials
2. **Main Dashboard**: Navigate using the sidebar menu

### Managing Candidates

1. Navigate to **Candidat** section
2. Fill in candidate information (CIN, Name, Age, etc.)
3. Click **Ajouter** to register
4. Use **Search** bar for quick lookup
5. Click **Modifier** to edit or **Supprimer** to delete

### Scheduling Sessions

1. Navigate to **Planning** section
2. Select:
   - Date and time (start/end)
   - Candidate
   - Session type (Drive/Theory)
   - Instructor
   - Vehicle
   - Circuit location
3. Click **🔍 Search Location** to preview map
4. Click **Ajouter** - system validates availability automatically
5. View sessions in calendar widget
6. Export to PDF using **Export PDF** button

### Viewing Statistics

1. Navigate to any module (Planning, Employee)
2. Click **📊 Stats** button
3. View interactive charts:
   - Session distribution
   - Top instructors
   - Monthly trends
   - Summary metrics

### Arduino Access Control (Optional)

1. Connect Arduino via USB
2. System auto-detects on startup
3. **RFID Mode**:
   - Employee scans card
   - System validates working hours
   - Door opens if authorized
4. **CIN Mode**:
   - Candidate approaches sensor
   - Webcam captures ID card
   - OCR verifies CIN number
   - Access granted if valid

---

## 📁 Project Structure

```
projetcpp2a33-drivesmart/
│
├── *.cpp, *.h              # Main application source files
│   ├── main.cpp            # Application entry point
│   ├── mainwindow.*        # Main navigation window
│   ├── logindialog.*       # Authentication dialog
│   ├── connection.*        # Database connection
│   ├── condidat.*          # Candidate management
│   ├── employes.*          # Employee management
│   ├── pageemploye.*       # Employee UI
│   ├── planning.*          # Planning/scheduling
│   ├── vehicule.*          # Vehicle management
│   ├── examen.*            # Exam management
│   ├── gestion_equipement.* # Equipment management
│   ├── arduino.*           # Arduino communication
│   ├── cin_access_control.* # Access control logic
│   ├── openstreetmap_service.* # Map integration
│   ├── local_calendar.*    # Calendar service
│   └── smtp.*              # Email notifications
│
├── *.ui                    # Qt Designer UI files
│
├── arduino_rfid_system_updated/
│   └── *.ino               # Arduino firmware
│
├── vendor/
│   └── qrcodegen.*         # QR code library
│
├── images/                 # Application icons/images
│
├── *.sql                   # Database scripts
│
├── cin_verification.py     # Python OCR script
├── drivesmart.pro          # Qt project file
└── README.md               # This file
```

---

## 🔗 API Integration

### OpenStreetMap Nominatim API

**Geocoding (Address → Coordinates):**
```cpp
mapService->geocodeAddress("123 Rue Example, Tunis");
// Returns: latitude, longitude, formatted_address
```

**Static Map Generation:**
```cpp
mapService->getStaticMap(latitude, longitude, zoomLevel, width, height);
// Returns: QPixmap image of the map
```

**No API Key Required** - Uses free OpenStreetMap service with rate limiting.

### Local Calendar API

**Create Event:**
```cpp
int eventId = localCalendar->createEvent(
    planningId,
    "Driving Session - John Doe",
    startDateTime,
    endDateTime,
    "Circuit A",
    "Instructor: Jane Smith"
);
```

**Query Events:**
```cpp
QList<CalendarEvent> events = localCalendar->getEventsByDate(QDate::currentDate());
```

**Delete Event:**
```cpp
localCalendar->deleteEventByPlanningId(planningId);
```

---

## 🤝 Contributing

Contributions are welcome! Please follow these steps:

1. **Fork** the repository
2. **Create** a feature branch: `git checkout -b feature/AmazingFeature`
3. **Commit** changes: `git commit -m 'Add AmazingFeature'`
4. **Push** to branch: `git push origin feature/AmazingFeature`
5. **Open** a Pull Request

### Code Style Guidelines
- Follow Qt naming conventions
- Use meaningful variable names
- Add comments for complex logic
- Test thoroughly before committing

---

## 📝 License

This project is developed as part of an academic course at **ESPRIT - École Supérieure Privée d'Ingénierie et de Technologies**.

**Course:** Atelier C++ (2A)  
**Academic Year:** 2024-2025

---

## 👥 Authors

**Projet C - 2A33 Team**
- GitHub: [@ProjetC-2A33](https://github.com/ProjetC-2A33)
- Repository: [projetcpp2a33-drivesmart](https://github.com/ProjetC-2A33/projetcpp2a33-drivesmart)

---

## 🐛 Known Issues

- Serial port auto-detection may need manual configuration on some systems
- OCR accuracy depends on ID card image quality

---

## 🚧 Future Enhancements

- [ ] Mobile app integration (Qt for Android/iOS)
- [ ] SMS notifications for session reminders
- [ ] Online payment integration
- [ ] Multi-language support (French/Arabic/English)
- [ ] Cloud backup and synchronization
- [ ] Advanced reporting with custom date ranges
- [ ] Student progress tracking dashboard
- [ ] Automated invoice generation

---

## 📞 Support

For issues, questions, or suggestions:
- Open an issue on [GitHub](https://github.com/ProjetC-2A33/projetcpp2a33-drivesmart/issues)
- Contact the development team through the course platform

---

## 🙏 Acknowledgments

- **Qt Framework** - Cross-platform development
- **OpenStreetMap** - Free mapping service
- **Oracle Database** - Enterprise data management
- **Arduino Community** - Hardware integration support
- **ESPRIT** - Academic support and resources

---

**Made with ❤️ by ProjetC-2A33 Team | © 2024-2025**
