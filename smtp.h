#ifndef SMTP_H
#define SMTP_H

#include <QtNetwork/QSslSocket>
#include <QtNetwork/QAbstractSocket>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QtWidgets/QMessageBox>
#include <QByteArray>
#include <QFile>
#include <QFileInfo>

class Smtp : public QObject
{
    Q_OBJECT

public:
    Smtp( const QString &user, const QString &pass, const QString &host, int port = 465, int timeout = 30000, QObject *parent = nullptr );
    ~Smtp();

    void sendMail(const QString &from, const QString &to, const QString &subject, const QString &body, QStringList files = QStringList());

signals:
    void status(const QString &status);

private slots:
    void stateChanged(QAbstractSocket::SocketState socketState);
    void errorReceived(QAbstractSocket::SocketError socketError);
    void disconnected();
    void connected();
    void readyRead();

private:
    QString message;
    QTextStream *t;
    QSslSocket *socket;
    QString from;
    QString rcpt;
    QString response;
    QString user;
    QString pass;
    QString host;
    int port;
    int timeout;

    enum states {
        Init,
        HandShake,
        Auth,
        User,
        Pass,
        Mail,
        Rcpt,
        Data,
        Body,
        Quit,
        Close
    };
    states state;
};

#endif // SMTP_H
