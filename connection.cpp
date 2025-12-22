#include "connection.h"
#include <QDebug>
#include <QDate>

Connection::Connection()
{
}

bool Connection::createconnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DriveSmart");
    db.setUserName("seifeddine");
    db.setPassword("03201999");

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

int Connection::getSessionHoursToday(const QString& cin)
{
    QSqlQuery query;
    query.prepare("SELECT COALESCE(SUM(DUREE), 0) FROM SEANCE WHERE CIN_CONDIDAT = :cin AND DATE_SEANCE = :date");
    query.bindValue(":cin", cin);
    query.bindValue(":date", QDate::currentDate());
    
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}
