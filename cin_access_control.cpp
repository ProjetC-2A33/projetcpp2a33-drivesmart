#include "cin_access_control.h"
#include <QFile>
#include <QTextStream>
#include <QDir>

CINAccessControl::CINAccessControl(QObject *parent)
    : QObject(parent),
      pythonProcess(nullptr),
      capturedCIN(""),
      responseFilePath("qt_response.txt")
{
    pythonProcess = new QProcess(this);
    
    // Connect Python process signals
    connect(pythonProcess, &QProcess::readyReadStandardOutput, this, &CINAccessControl::onPythonOutputReady);
    connect(pythonProcess, &QProcess::readyReadStandardError, this, &CINAccessControl::onPythonError);
}

CINAccessControl::~CINAccessControl()
{
    stopPythonScript();
}

bool CINAccessControl::initializeSystem()
{
    qDebug() << "Initializing CIN Access Control System...";
    
    // Clear any old response files
    QFile::remove(responseFilePath);
    
    emit systemStatusChanged("Starting Python script...");
    
    // Start Python script
    startPythonScript();
    
    return true;
}

void CINAccessControl::startPythonScript()
{
    qDebug() << "Starting Python CIN verification script...";
    
    // Get the absolute path to Python script in source directory
    QString scriptPath = "e:/drivesmart-integration/cin_verification.py";
    
    qDebug() << "Script path:" << scriptPath;
    
    // Verify file exists
    if (!QFile::exists(scriptPath)) {
        qDebug() << "ERROR: Python script not found at:" << scriptPath;
        emit systemStatusChanged("Python script not found!");
        return;
    }
    
    // Start Python process with unbuffered output
    QStringList args;
    args << "-u" << scriptPath;  // -u for unbuffered output
    
    pythonProcess->start("python", args);
    
    if (pythonProcess->waitForStarted(3000)) {
        qDebug() << "Python script started successfully";
        emit systemStatusChanged("Python script running - System ready");
    } else {
        qDebug() << "Failed to start Python script:" << pythonProcess->errorString();
        emit systemStatusChanged("Failed to start Python script");
    }
}

void CINAccessControl::stopPythonScript()
{
    if (pythonProcess && pythonProcess->state() == QProcess::Running) {
        qDebug() << "Stopping Python script...";
        pythonProcess->terminate();
        if (!pythonProcess->waitForFinished(3000)) {
            pythonProcess->kill();
        }
        qDebug() << "Python script stopped";
    }
}

void CINAccessControl::onPythonOutputReady()
{
    // Read all available output from Python
    QByteArray output = pythonProcess->readAllStandardOutput();
    QString outputStr = QString::fromUtf8(output);
    
    // Split by lines and process each
    QStringList lines = outputStr.split('\n', Qt::SkipEmptyParts);
    
    for (const QString &line : lines) {
        QString trimmedLine = line.trimmed();
        
        if (trimmedLine.isEmpty()) {
            continue;
        }
        
        qDebug() << "[Python]" << trimmedLine;
        
        // Look for CIN output
        if (trimmedLine.startsWith("CIN:")) {
            QString cin = trimmedLine.mid(4).trimmed();
            
            qDebug() << "=== Received CIN from Python:" << cin << "===";
            
            if (cin == "NOTFOUND") {
                qDebug() << "CIN not detected by webcam";
                sendResponseToPython("DENIED");
                emit accessDenied("CIN not detected");
            }
            else if (cin == "ERROR") {
                qDebug() << "Camera error";
                sendResponseToPython("DENIED");
                emit accessDenied("Camera error");
            }
            else if (cin.length() == 8) {
                // Valid CIN - verify in database
                capturedCIN = cin;
                qDebug() << "Verifying CIN in database...";
                
                if (verifyCINInDatabase(cin)) {
                    qDebug() << ">>> Access GRANTED <<<";
                    sendResponseToPython("AUTHORIZED");
                    logAccess(cin, "AUTHORIZED");
                } else {
                    qDebug() << ">>> Access DENIED <<<";
                    sendResponseToPython("DENIED");
                    logAccess(cin, "DENIED");
                }
            }
        }
    }
}

void CINAccessControl::onPythonError()
{
    QByteArray errorOutput = pythonProcess->readAllStandardError();
    if (!errorOutput.isEmpty()) {
        qDebug() << "[Python Error]" << QString::fromUtf8(errorOutput);
    }
}

void CINAccessControl::sendResponseToPython(const QString &response)
{
    qDebug() << "Sending response to Python:" << response;
    
    QFile responseFile(responseFilePath);
    if (responseFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&responseFile);
        out << response;
        responseFile.close();
        qDebug() << "Response written to" << responseFilePath;
    } else {
        qDebug() << "Failed to write response file!";
    }
}

bool CINAccessControl::verifyCINInDatabase(const QString &cin)
{
    QSqlQuery query;
    // Try EMPLOYEE table first (uppercase), then employes table (lowercase)
    query.prepare("SELECT CIN_EMPLOYEE, NOM, PRENOM, DISPONIBILITE FROM EMPLOYEE WHERE CIN_EMPLOYEE = :cin");
    query.bindValue(":cin", cin);
    
    bool found = false;
    QString nom, prenom, disponibilite;
    
    if (query.exec() && query.next()) {
        found = true;
        nom = query.value(1).toString();
        prenom = query.value(2).toString();
        disponibilite = query.value(3).toString();
    } else {
        // Try alternative table name: employes with CIN_employes
        query.prepare("SELECT CIN_employes, NOM, PRENOM, DISPONIBILITE FROM employes WHERE CIN_employes = :cin");
        query.bindValue(":cin", cin);
        
        if (query.exec() && query.next()) {
            found = true;
            nom = query.value(1).toString();
            prenom = query.value(2).toString();
            disponibilite = query.value(3).toString();
        }
    }
    
    if (found) {
        qDebug() << "Employee found:" << prenom << nom << "- Disponibilite:" << disponibilite;
        
        // Check availability - accept "Disponible", "disponible", "1", or numeric 1
        if (disponibilite == "Disponible" || disponibilite == "disponible" || 
            disponibilite == "1" || disponibilite.toInt() == 1) {
            qDebug() << "Employee is available - Access GRANTED";
            emit accessGranted(cin, nom, prenom);
            return true;
        } else {
            qDebug() << "Employee found but not available (Disponibilite:" << disponibilite << ")";
            emit accessDenied("Employee not available");
            return false;
        }
    }
    
    qDebug() << "CIN" << cin << "not found in database";
    qDebug() << "Last query error:" << query.lastError().text();
    return false;
}

void CINAccessControl::logAccess(const QString &cin, const QString &status)
{
    QSqlQuery query;
    query.prepare("INSERT INTO ACCESS_LOG (CIN, ACCESS_TIME, STATUS) "
                  "VALUES (:cin, SYSDATE, :status)");
    query.bindValue(":cin", cin);
    query.bindValue(":status", status);
    
    if (!query.exec()) {
        qDebug() << "Failed to log access:" << query.lastError().text();
    } else {
        qDebug() << "Access logged for CIN:" << cin << "Status:" << status;
    }
}
