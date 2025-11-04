#ifndef VEHICULE_H
#define VEHICULE_H

#include <QWidget>

namespace Ui {
class Vehicule;
}

class Vehicule : public QWidget
{
    Q_OBJECT

public:
    explicit Vehicule(QWidget *parent = nullptr);
    ~Vehicule();

private slots:
    void on_AjouterButton_2_clicked();
    void on_AnnulerButton_2_clicked();
    void navigateToCandidat();
    void navigateToPlanning();
    void navigateToEmployee();
    void navigateToVehicule();
    void navigateToExamen();
    void navigateToEquipement();

private:
    Ui::Vehicule *ui;
};

#endif // VEHICULE_H