#ifndef GESTION_EQUIPEMENT_H
#define GESTION_EQUIPEMENT_H

#include <QWidget>

namespace Ui {
class Gestion_Equipement;
}

class Gestion_Equipement : public QWidget
{
    Q_OBJECT

public:
    explicit Gestion_Equipement(QWidget *parent = nullptr);
    ~Gestion_Equipement();

private slots:
    void on_btn_ajout_clicked();
    void on_btn_reset_clicked();
    void on_btnUpdateMaintenance_clicked();
    void on_btnCheckWeather_clicked();
    void navigateToCandidat();
    void navigateToPlanning();
    void navigateToEmployee();
    void navigateToVehicule();
    void navigateToExamen();
    void navigateToEquipement();

private:
    Ui::Gestion_Equipement *ui;
};

#endif // GESTION_EQUIPEMENT_H