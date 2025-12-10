#ifndef ARDUINO_INTEG_H
#define ARDUINO_INTEG_H

#include <QObject>
#include <QSerialPort>
#include <QString>

class ArduinoInteg : public QObject
{
    Q_OBJECT
public:
    explicit ArduinoInteg(QObject *parent = nullptr);

    bool connectArduino(const QString &portName, int baudRate = 9600);
    void sendData(const QString &data);

    // Nouvelle méthode pour envoyer les informations du véhicule
    void sendVehicleInfo(const QString &model, const QString &plate, const QString &status);

signals:
    void dataReceived(const QString &data);
    void matriculeReceived(const QString &matricule);

private slots:
    void handleReadyRead();

private:
    QSerialPort serial;
};

#endif // ARDUINO_INTEG_H
