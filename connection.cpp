#include "connection.h"
#include <QDebug>
#include <QStringList>

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

int Connection::getSessionHoursToday(int cin)
{
    QSqlQuery query;
    
    // Try multiple variations of the column name to handle different creation methods
    QStringList queryVariations = {
        "SELECT NVL(SUM(DUREE), 0) FROM EXAMEN WHERE CIN_CONDIDAT = :cin AND TRUNC(DATE_EXAMEN) = TRUNC(SYSDATE)",
        "SELECT NVL(SUM(\"DUREE\"), 0) FROM EXAMEN WHERE \"CIN_CONDIDAT\" = :cin AND TRUNC(\"DATE_EXAMEN\") = TRUNC(SYSDATE)",
        "SELECT NVL(SUM(duree), 0) FROM examen WHERE cin_condidat = :cin AND TRUNC(date_examen) = TRUNC(SYSDATE)",
        "SELECT NVL(SUM(Duree), 0) FROM Examen WHERE Cin_Condidat = :cin AND TRUNC(Date_Examen) = TRUNC(SYSDATE)"
    };
    
    for (const QString &sql : queryVariations) {
        query.prepare(sql);
        query.bindValue(":cin", cin);
        
        if (query.exec() && query.next()) {
            qDebug() << "Query succeeded with:" << sql;
            return query.value(0).toInt();
        }
    }
    
    qDebug() << "All query variations failed. Last error:" << query.lastError().text();
    return 0;
}
