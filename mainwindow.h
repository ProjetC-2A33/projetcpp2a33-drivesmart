#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QTimer>
#include "arduino-integ.h"  // AJOUTEZ CE INCLUDE

QT_BEGIN_NAMESPACE
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
QT_END_NAMESPACE

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

    // Ajoutez ces slots pour l'Arduino
    void readArduino();  // Renommez readArduino en slot
    void onMatriculeReceived(const QString &matricule);
    void sendVehicleInfoToArduino(const QString &model, const QString &plate, const QString &status);

private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;
    Condidat *condidatView;
    pageemploye *employesView;
    Vehicule *vehiculeView;
    Examen *examenView;
    Planning *planningView;
    Gestion_Equipement *equipementView;
    QSerialPort *arduino;

    // Méthodes privées
    void connectArduino();
    void setupDatabase();

    // Variables pour la gestion Arduino
    QString currentMatricule;
};

#endif // MAINWINDOW_H
