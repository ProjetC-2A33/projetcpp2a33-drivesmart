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
    void dataReceived(QString data);  // For webcam system
    void cinCaptureRequested();       // For webcam system

public slots:
    void sendAuthorizationSignal(bool authorized);  // For webcam system
    void requestCINCapture();                        // For webcam system

private slots:
    void readData();
    void readSerialData();  // For webcam system

private:
    QSerialPort *serial;
    Connection dbConn;   // objet pour la DB
    QByteArray buffer;    // accumulateur pour lignes série complètes
};

#endif
