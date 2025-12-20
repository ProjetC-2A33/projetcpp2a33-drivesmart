#include <QApplication>
#include "logindialog.h"
#include "mainwindow.h"
#include "connection.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("DriveSmart");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("DriveSmart Inc.");

    Connection c;
    bool test = c.createconnect();
    if (!test)
    {
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                              QObject::tr("connection failed.\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
        return 1;
    }

    QMessageBox::information(nullptr, QObject::tr("database is open"),
                             QObject::tr("connection successful.\n"
                                         "Click OK to continue."),
                             QMessageBox::Ok);


    LoginDialog loginDialog;


    if (loginDialog.exec() == QDialog::Accepted)
    {

        MainWindow *mainWindow = new MainWindow();


        mainWindow->show();
        mainWindow->setAttribute(Qt::WA_DeleteOnClose);

        return app.exec();
    }
    else
    {

        return 0;
    }
}
