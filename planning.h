#ifndef PLANNING_H
#define PLANNING_H

#include <QWidget>

namespace Ui {
class Planning;
}

class Planning : public QWidget
{
    Q_OBJECT

public:
    explicit Planning(QWidget *parent = nullptr);
    ~Planning();

private slots:
    void on_btn_ajout_3_clicked();
    void on_btn_reset_3_clicked();
    void navigateToCandidat();
    void navigateToPlanning();
    void navigateToEmployee();
    void navigateToVehicule();
    void navigateToExamen();
    void navigateToEquipement();

private:
    Ui::Planning *ui;
};

#endif // PLANNING_H