#ifndef ARDUINO_H
#define ARDUINO_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <QObject>

class Arduino : public QObject
{
    Q_OBJECT

public:
    Arduino(); // Constructor

    QString getarduino_port_name(); // Get the port name
    QSerialPort *getserial(); // Get the serial port object

    int connect_arduino(); // Connect to the Arduino
    int close_arduino(); // Close the connection
    QByteArray read_from_arduino(); // Read data from Arduino
    void write_to_arduino(QByteArray d); // Write data to Arduino
    
    // New methods for CIN verification system
    void sendAuthorizationSignal(bool authorized); // Send AUTHORIZE/DENY to Arduino
    void requestCINCapture(); // Request Python script to capture CIN

signals:
    void dataReceived(QString data); // Signal when Arduino sends data
    void cinCaptureRequested(); // Signal to trigger webcam capture

private slots:
    void readSerialData(); // Slot to read incoming data

private:
    QString arduino_port_name; // Port name
    bool arduino_is_available; // Arduino availability
    QSerialPort *serial; // Serial object to handle communication
    QByteArray data; // Data to be read or written
    static const quint16 arduino_uno_vendor_id = 0x2341; // Arduino vendor ID
    static const quint16 arduino_uno_product_id = 0x0043; // Arduino UNO product ID
};

#endif // ARDUINO_H
