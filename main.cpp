<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> a68cd6328b1cf20c24f116f3d2fd75bcc01ca6ca
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
<<<<<<< HEAD
}
=======
}
=======
#include "condidat/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.showMaximized();

    return a.exec();
}
>>>>>>> e695879ad76debb070bc454bc4b9a8500213ef04
>>>>>>> a68cd6328b1cf20c24f116f3d2fd75bcc01ca6ca
