#ifndef CONDIDAT_H
#define CONDIDAT_H

#include <QWidget>

namespace Ui {
class Condidat;
}

class Condidat : public QWidget
{
    Q_OBJECT

public:
    explicit Condidat(QWidget *parent = nullptr);
    ~Condidat();

private slots:
    void on_btn_ajout_clicked();
    void on_btn_reset_clicked();
    void navigateToCandidat();
    void navigateToPlanning();
    void navigateToEmployee();
    void navigateToVehicule();
    void navigateToExamen();
    void navigateToEquipement();

private:
    Ui::Condidat *ui;
};

#endif // CONDIDAT_H