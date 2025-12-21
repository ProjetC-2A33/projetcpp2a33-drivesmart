#include "arduino.h"
#include "cin_access_control.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSerialPortInfo>
#include <QRegularExpression>

ArduinoReader::ArduinoReader(QObject *parent) : QObject(parent), cinAccessControl(nullptr) {
    serial = new QSerialPort(this);
    if (!dbConn.opendb()) {
        qDebug() << "AVERTISSEMENT: Impossible d'ouvrir la DB, les fonctionnalités RFID seront limitées";
    } else {
        qDebug() << "DB connectée avec succès pour ArduinoReader";
    }
}

void ArduinoReader::setCINAccessControl(CINAccessControl *cinControl) {
    cinAccessControl = cinControl;
    qDebug() << "CIN Access Control lié au système RFID";
}

bool ArduinoReader::openArduino() {
    // Log available ports for easier troubleshooting
    const auto ports = QSerialPortInfo::availablePorts();
    qDebug() << "Ports série détectés:";
    for (const QSerialPortInfo &info : ports) {
        qDebug() << " -" << info.portName()
                 << "desc=" << info.description()
                 << "manuf=" << info.manufacturer()
                 << "loc=" << info.systemLocation();
    }

    const QString preferredPort = "COM3";
    QList<QString> tryOrder;
    QSet<QString> added;

    auto addPort = [&](const QString &p) {
        if (!added.contains(p)) {
            tryOrder.append(p);
            added.insert(p);
        }
    };

    // 1) Preferred COM3
    addPort(preferredPort);
    // 2) Any port that looks like Arduino by description/manufacturer
    for (const QSerialPortInfo &info : ports) {
        const QString desc = info.description().toLower();
        const QString manuf = info.manufacturer().toLower();
        if (desc.contains("arduino") || manuf.contains("arduino")) {
            addPort(info.portName());
        }
    }
    // 3) All remaining ports
    for (const QSerialPortInfo &info : ports) {
        addPort(info.portName());
    }

    QList<QString> triedPorts;
    bool opened = false;

    auto tryOpen = [&](const QString &portName) -> bool {
        serial->setPortName(portName);
        serial->setBaudRate(QSerialPort::Baud9600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        triedPorts.append(portName);
        if (!serial->open(QIODevice::ReadWrite)) {
            qDebug() << "Echec ouverture" << portName << ":" << serial->errorString();
            return false;
        }
        return true;
    };

    for (const QString &portName : tryOrder) {
        if (tryOpen(portName)) {
            opened = true;
            break;
        }
    }

    if (!opened) {
        qDebug() << "Arduino non détecté. Ports testés:" << triedPorts;
        return false;
    }

    qDebug() << "Arduino connecté sur" << serial->portName();
    connect(serial, &QSerialPort::readyRead, this, &ArduinoReader::readData);
    return true;
}
void ArduinoReader::readData() {
    buffer.append(serial->readAll());

    while (true) {
        int newlineIndex = buffer.indexOf('\n');
        if (newlineIndex < 0) {
            break;
        }

        QByteArray line = buffer.left(newlineIndex);
        buffer.remove(0, newlineIndex + 1);
        QString uid = QString::fromUtf8(line).trimmed();

        if (uid == "READY") {
            continue; // Juste ignorer, pas de log
        }

        // Validation simplifiée
        if (uid.length() >= 8 && uid.length() <= 16) {
            bool isHex = true;
            for (const QChar &c : uid) {
                if (!c.isDigit() && !(c >= 'A' && c <= 'F') && !(c >= 'a' && c <= 'f')) {
                    isHex = false;
                    break;
                }
            }

            if (isHex) {
                uid = uid.toUpper();

                QSqlQuery query;
                query.prepare("SELECT CIN_CONDIDAT, NOM, PRENOM FROM CONDIDAT WHERE \"UID\" = :uid");
                query.bindValue(":uid", uid);

                if (query.exec() && query.next()) {
                    QString cin = query.value(0).toString();

                    // RAPIDE: Pas de logs détaillés, juste le traitement
                    int heures = dbConn.getSessionHoursToday(cin);

                    // ENVOYER DIRECTEMENT LA COMMANDE
                    sendLEDCommand(heures);

                    // Optionnel: Émettre un signal silencieux
                    emit uidDetected(uid);

                } else {
                    // Pas de candidat trouvé avec RFID
                    sendLEDCommand(0);
                    
                    // Demander vérification CIN si système disponible
                    if (cinAccessControl) {
                        qDebug() << "RFID non reconnu, déclenchement vérification CIN";
                        emit requestCINVerification();
                    }
                }
            }
        }
    }
}

void ArduinoReader::sendLEDCommand(int hours) {
    if (!serial || !serial->isOpen()) {
        qDebug() << "Port série non ouvert";
        return;
    }

    QString command;

    if (hours > 0) {
        command = "1"; // carte acceptée: LED verte + bip court
        qDebug() << "Commande: carte acceptée (LED verte + bip court)";
    } else {
        command = "0"; // pas de séance: LED rouge + bip long
        qDebug() << "Commande: carte refusée (LED rouge + bip long)";
    }
    
    // Envoyer à l'Arduino
    command += "\n";
    serial->write(command.toUtf8());
    serial->waitForBytesWritten(1000);
    
    qDebug() << "Commande envoyée à l'Arduino:" << command.trimmed();
}
