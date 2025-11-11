#ifndef EXAMEN_H
#define EXAMEN_H

#include <QWidget>

namespace Ui {
class Examen;
}

class Examen : public QWidget
{
    Q_OBJECT

public:
    explicit Examen(QWidget *parent = nullptr);
    ~Examen();

private slots:
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_7_clicked();
    void navigateToCandidat();
    void navigateToPlanning();
    void navigateToEmployee();
    void navigateToVehicule();
    void navigateToExamen();
    void navigateToEquipement();

private:
    Ui::Examen *ui;
};

#endif // EXAMEN_H