#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QHeaderView>
#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{
    
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    // Correction de la chaîne de connexion : pas d'espaces avant "Driver="
    QString connectionString = "Driver={Oracle in XE};Dbq=XE;Uid=projet;Pwd=123;";
    db.setDatabaseName(connectionString);
    
    qDebug() << "Chaîne de connexion : " << connectionString;

    if (!db.open()) {
        QString errorMsg = db.lastError().text();
        qDebug() << " ERREUR DE CONNEXION";
        qDebug() << "   Message : " << errorMsg;
        qDebug() << "=== CONNEXION ÉCHOUÉE ===";

        return false;
    }

    qDebug() << " CONNEXION RÉUSSIE !";
    qDebug() << "   Base de données : " << db.databaseName();
    qDebug() << "   Driver : " << db.driverName();
    qDebug() << "=== CONNEXION ÉTABLIE ===";
    
    return true;
}
