#include "arduino-integ.h"
#include <QDebug>

ArduinoInteg::ArduinoInteg(QObject *parent) : QObject(parent)
{
    connect(&serial, &QSerialPort::readyRead, this, &ArduinoInteg::handleReadyRead);
}

bool ArduinoInteg::connectArduino(const QString &portName, int baudRate)
{
    serial.setPortName(portName);
    serial.setBaudRate(baudRate);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    if(serial.open(QIODevice::ReadWrite)){
        qDebug() << "Arduino connecté sur" << portName;
        return true;
    }else{
        qDebug() << "Erreur de connexion :" << serial.errorString();
        return false;
    }
}

void ArduinoInteg::sendData(const QString &data)
{
    if(serial.isOpen()) {
        serial.write((data + "\n").toUtf8());
        qDebug() << "Données envoyées à Arduino:" << data;
    }
}

void ArduinoInteg::sendVehicleInfo(const QString &model, const QString &plate, const QString &status)
{
    // Format: VEHICULE:modele|matricule|disponibilite
    QString data = QString("VEHICULE:%1|%2|%3").arg(model).arg(plate).arg(status);
    sendData(data);
}

void ArduinoInteg::handleReadyRead()
{
    while(serial.canReadLine()){
        QString data = QString::fromUtf8(serial.readLine()).trimmed();
        qDebug() << "Données brutes reçues d'Arduino:" << data;

        // Émettre le signal avec toutes les données
        emit dataReceived(data);

        // Si c'est une demande de matricule
        if(data.startsWith("MAT:")) {
            QString matricule = data.mid(4); // Enlever "MAT:"
            matricule = matricule.trimmed(); // Nettoyer les espaces
            qDebug() << "Matricule extrait:" << matricule;

            if(!matricule.isEmpty()) {
                emit matriculeReceived(matricule);
            } else {
                qDebug() << "Matricule vide après extraction!";
            }
        }
    }
}
