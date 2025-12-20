#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class Connection
{
public:
    Connection();
    bool createconnect();
    bool opendb();
    int getSessionHoursToday(int cin);
};

#endif // CONNECTION_H
