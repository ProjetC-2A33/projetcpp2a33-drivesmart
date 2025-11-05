#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("DriveSmart");//inserer le nom de la source de données
db.setUserName("seifeddine");//inserer nom de l'utilisateur
db.setPassword("03201999");//inserer mot de passe de cet utilisateur

if (db.open()) {
    test=true;
} else {
    // Output the detailed error
    qDebug() << "Connection Failed:" << db.lastError().text();
}
    return  test;
}
