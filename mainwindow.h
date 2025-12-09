#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pageemploye.h"
#include "cin_access_control.h"
#include "arduino.h"
namespace Ui
{
    class MainWindow;
}

class Condidat;
class pageemploye;
class Vehicule;
class Examen;
class Planning;
class Gestion_Equipement;
class QStackedWidget;
class CINAccessControl;
class ArduinoReader;

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
    void showemployes();
    void showEquipement();
    void on_btn_ajout_E_clicked();
    void on_btn_reset_E_clicked();
    
    // Access control slots
    void startAccessControl();
    void stopAccessControl();
    void onAccessGranted(QString cin, QString nom, QString prenom);
    void onAccessDenied(QString reason);
    void onAccessSystemStatus(QString status);



private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;
    Condidat *condidatView;
    pageemploye *employesView;
    Vehicule *vehiculeView;
    Examen *examenView;
    Planning *planningView;
    Gestion_Equipement *equipementView;
    CINAccessControl *accessControl;
    ArduinoReader *arduinoReader;
};

#endif // MAINWINDOW_H
