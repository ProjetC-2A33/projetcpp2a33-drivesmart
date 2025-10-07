#include "gestion_equipement.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Gestion_Equipement w;
    w.show();
    return a.exec();
}
