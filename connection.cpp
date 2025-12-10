#include "connection.h"

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




