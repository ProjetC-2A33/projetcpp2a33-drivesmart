#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class Condidat;
class Vehicule;
class Examen;
class Planning;
class Gestion_Equipement;
class QStackedWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    

private slots:
    void showCandidat();
    void showVehicule();
    void showExamen();
    void showPlanning();
    void showEmployee();
    void showEquipement();
    void on_btn_ajout_E_clicked();
    void on_btn_reset_E_clicked();

private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;
    Condidat *condidatView;
    Vehicule *vehiculeView;
    Examen *examenView;
    Planning *planningView;
    Gestion_Equipement *equipementView;
    QWidget *employeeView;
};

#endif // MAINWINDOW_H
