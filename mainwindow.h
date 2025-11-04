#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

// Forward declarations
class Condidat;
class Vehicule;
class Examen;
class Planning;
class Gestion_Equipement;
class QWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void showCandidat();
    void showVehicule();
    void showExamen();
    void showPlanning();
    void showEmployee();
    void showEquipement();

private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;
    
    // Pointers to our views
    Condidat *condidatView;
    Vehicule *vehiculeView;
    Examen *examenView;
    Planning *planningView;
    Gestion_Equipement *equipementView;
    QWidget *employeeView;
};

#endif // MAINWINDOW_H