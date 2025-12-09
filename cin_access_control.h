#ifndef CIN_ACCESS_CONTROL_H
#define CIN_ACCESS_CONTROL_H

#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

class CINAccessControl : public QObject
{
    Q_OBJECT

public:
    explicit CINAccessControl(QObject *parent = nullptr);
    ~CINAccessControl();
    
    bool initializeSystem(); // Initialize and start Python
    void startPythonScript(); // Start Python webcam script
    void stopPythonScript(); // Stop Python script
    
private:
    QProcess *pythonProcess;
    QString capturedCIN;
    QString responseFilePath;
    
    bool verifyCINInDatabase(const QString &cin);
    void logAccess(const QString &cin, const QString &status);
    void sendResponseToPython(const QString &response);

private slots:
    void onPythonOutputReady();
    void onPythonError();

signals:
    void accessGranted(QString cin, QString nom, QString prenom);
    void accessDenied(QString reason);
    void systemStatusChanged(QString status);
};

#endif // CIN_ACCESS_CONTROL_H
