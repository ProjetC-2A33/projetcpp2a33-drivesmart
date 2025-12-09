#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>

class Connection
{
public:
    Connection();
    bool createconnect();
    bool opendb();
    int getSessionHoursToday(const QString &cin);
};

#endif // CONNECTION_H
