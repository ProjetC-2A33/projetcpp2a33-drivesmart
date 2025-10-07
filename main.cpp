<<<<<<< HEAD
#include "gestion_equipement.h"

=======
<<<<<<< HEAD
#include <QApplication>
#include "Login.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("DriveSmart");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("DriveSmart Inc.");

    Login loginWindow;
    loginWindow.show();

    return app.exec();
}
=======
#include "condidat/mainwindow.h"
>>>>>>> 533b51eb6d32de7776b0ce234550d15971b62dbb
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
<<<<<<< HEAD
    Gestion_Equipement w;
    w.show();
    return a.exec();
}
=======
    MainWindow w;
    w.show();
    w.showMaximized();

    return a.exec();
}
>>>>>>> e695879ad76debb070bc454bc4b9a8500213ef04
>>>>>>> 533b51eb6d32de7776b0ce234550d15971b62dbb
