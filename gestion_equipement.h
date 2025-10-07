#ifndef GESTION_EQUIPEMENT_H
#define GESTION_EQUIPEMENT_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Gestion_Equipement; }
QT_END_NAMESPACE

class Gestion_Equipement : public QMainWindow
{
    Q_OBJECT

public:
    Gestion_Equipement(QWidget *parent = nullptr);
    ~Gestion_Equipement();

private:
    Ui::Gestion_Equipement *ui;
};
#endif // GESTION_EQUIPEMENT_H
