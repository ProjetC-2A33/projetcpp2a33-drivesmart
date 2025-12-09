#include "connection.h"
#include <QDateTime>
#include <QDebug>

Connection::Connection()
{
}

bool Connection::createconnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("projet");
    db.setUserName("projet");
    db.setPassword("123");

    // Set connection options for proper character encoding
    db.setConnectOptions("SQL_ATTR_METADATA_ID=SQL_FALSE");

    if (db.open())
    {
        // Set session encoding to handle French characters
        QSqlQuery query(db);
        query.exec("ALTER SESSION SET NLS_LANGUAGE='FRENCH'");
        query.exec("ALTER SESSION SET NLS_TERRITORY='FRANCE'");
        query.exec("ALTER SESSION SET NLS_CHARACTERSET='AL32UTF8'");
        test = true;
    }

    return test;
}

bool Connection::opendb()
{
    return createconnect();
}

int Connection::getSessionHoursToday(const QString &cin)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Database not open in getSessionHoursToday";
        return 0;
    }

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM PLANNING WHERE CIN_CONDIDAT = :cin AND TRUNC(DATE_SEANCE) = TRUNC(SYSDATE)");
    query.bindValue(":cin", cin);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }

    qDebug() << "Error getting session hours:" << query.lastError().text();
    return 0;
}
