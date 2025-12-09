#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include "connection.h"

class ArduinoReader : public QObject {
    Q_OBJECT
public:
    explicit ArduinoReader(QObject *parent = nullptr);
    bool openArduino();
    void sendLEDCommand(int hours); // Envoyer commande LED/buzzer

signals:
    void uidDetected(const QString &uid);
    void candidateDetected(const QString &nom, const QString &prenom, int hours);

private slots:
    void readData();

private:
    QSerialPort *serial;
    Connection dbConn;   // objet pour la DB
    QByteArray buffer;    // accumulateur pour lignes série complètes
};

#endif
