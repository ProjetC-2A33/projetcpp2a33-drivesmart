#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include "connection.h"

class CINAccessControl; // Forward declaration

class ArduinoReader : public QObject {
    Q_OBJECT
public:
    explicit ArduinoReader(QObject *parent = nullptr);
    bool openArduino();
    void sendLEDCommand(int hours); // Envoyer commande LED/buzzer
    void setCINAccessControl(CINAccessControl *cinControl); // Lien avec système CIN

signals:
    void uidDetected(const QString &uid);
    void candidateDetected(const QString &nom, const QString &prenom, int hours);
    void requestCINVerification(); // Demande vérification CIN si RFID échoue

private slots:
    void readData();

private:
    QSerialPort *serial;
    Connection dbConn;   // objet pour la DB
    QByteArray buffer;    // accumulateur pour lignes série complètes
    CINAccessControl *cinAccessControl; // Pointeur vers système CIN
};

#endif
