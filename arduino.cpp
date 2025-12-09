#include "arduino.h"

// Constructor initializes members and sets up the serial port
Arduino::Arduino()
{
    data = "";
    arduino_port_name = "";
    arduino_is_available = false;
    serial = new QSerialPort(this);
    
    // Connect signal for reading data automatically
    connect(serial, &QSerialPort::readyRead, this, &Arduino::readSerialData);
}

// Getter for the Arduino port name
QString Arduino::getarduino_port_name()
{
    return arduino_port_name;
}

// Getter for the QSerialPort object
QSerialPort *Arduino::getserial()
{
    return serial;
}

// Connect to the Arduino via the appropriate port
int Arduino::connect_arduino()
{
    // Search for the port where Arduino is connected
    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts())
    {
        if (serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier())
        {
            if (serial_port_info.vendorIdentifier() == arduino_uno_vendor_id &&
                serial_port_info.productIdentifier() == arduino_uno_product_id)
            {
                arduino_is_available = true;
                arduino_port_name = serial_port_info.portName();
                break;
            }
        }
    }

    qDebug() << "arduino_port_name is :" << arduino_port_name;

    if (arduino_is_available) {
        serial->setPortName(arduino_port_name);
        if (serial->open(QSerialPort::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600); // Set baud rate to 9600
            serial->setDataBits(QSerialPort::Data8);    // Set data bits to 8
            serial->setParity(QSerialPort::NoParity);   // No parity
            serial->setStopBits(QSerialPort::OneStop);  // 1 stop bit
            serial->setFlowControl(QSerialPort::NoFlowControl); // No flow control
            qDebug() << "Arduino connected successfully on" << arduino_port_name;
            return 0; // Success
        }
        return 1; // Error in opening port
    }
    return -1; // Arduino not found
}

// Close the connection to the Arduino
int Arduino::close_arduino()
{
    if (serial->isOpen()) {
        serial->close();
        qDebug() << "Arduino connection closed.";
        return 0; // Success
    }
    return 1; // Failure to close
}

// Read data from the Arduino
QByteArray Arduino::read_from_arduino()
{
    if (serial->isReadable()) {
        data = serial->readAll(); // Retrieve data from serial port
        return data;
    }
    return QByteArray(); // Return empty if no data
}

// Write data to the Arduino
void Arduino::write_to_arduino(QByteArray d)
{
    if (serial->isWritable()) {
        serial->write(d); // Write the data to the serial port
        qDebug() << "Sent to Arduino:" << d;
    } else {
        qDebug() << "Couldn't write to serial!";
    }
}

// Slot to read incoming serial data
void Arduino::readSerialData()
{
    QByteArray receivedData = serial->readAll();
    QString dataString = QString::fromUtf8(receivedData).trimmed();
    
    if (!dataString.isEmpty()) {
        qDebug() << "Received from Arduino:" << dataString;
        emit dataReceived(dataString);
        
        // If Arduino requests CIN verification
        if (dataString == "REQUEST_CIN") {
            qDebug() << "CIN capture requested by Arduino";
            emit cinCaptureRequested();
        }
    }
}

// Send authorization signal to Arduino
void Arduino::sendAuthorizationSignal(bool authorized)
{
    if (authorized) {
        write_to_arduino("AUTHORIZED\n");
        qDebug() << "Authorization GRANTED sent to Arduino";
    } else {
        write_to_arduino("DENIED\n");
        qDebug() << "Authorization DENIED sent to Arduino";
    }
}

// Request CIN capture from Python script
void Arduino::requestCINCapture()
{
    qDebug() << "Requesting CIN capture...";
    emit cinCaptureRequested();
}
