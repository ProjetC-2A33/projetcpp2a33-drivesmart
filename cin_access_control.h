#ifndef CIN_ACCESS_CONTROL_H
#define CIN_ACCESS_CONTROL_H

#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>

class CINAccessControl : public QObject
{
    Q_OBJECT

public:
    explicit CINAccessControl(QObject *parent = nullptr);
    ~CINAccessControl();
    
    bool initializeSystem(); // Initialize and start Python
    void startPythonScript(); // Start Python webcam script
    void stopPythonScript(); // Stop Python script
    void setArduinoSerial(QSerialPort *serial); // Lien avec Arduino pour servo
    
private:
    QProcess *pythonProcess;
    QString capturedCIN;
    QString responseFilePath;
    QSerialPort *arduinoSerial; // Lien vers Arduino pour contrôle servo
    
    bool verifyCINInDatabase(const QString &cin);
    void logAccess(const QString &cin, const QString &status);
    void sendResponseToPython(const QString &response);
    void sendServoCommand(bool open); // Envoyer commande au servo

private slots:
    void onPythonOutputReady();
    void onPythonError();

signals:
    void accessGranted(QString cin, QString nom, QString prenom);
    void accessDenied(QString reason);
    void systemStatusChanged(QString status);
};

#endif // CIN_ACCESS_CONTROL_H
