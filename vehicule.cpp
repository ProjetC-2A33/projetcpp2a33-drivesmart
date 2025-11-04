#include "vehicule.h"
#include "ui_vehicule.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QStackedWidget>
Vehicule::Vehicule(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Vehicule)
{
    ui->setupUi(this);

    // Connect navigation buttons
    connect(ui->candidat_4, &QPushButton::clicked, this, &Vehicule::navigateToCandidat);
    connect(ui->planning_4, &QPushButton::clicked, this, &Vehicule::navigateToPlanning);
    connect(ui->employee_4, &QPushButton::clicked, this, &Vehicule::navigateToEmployee);
    connect(ui->vehicule_4, &QPushButton::clicked, this, &Vehicule::navigateToVehicule);
    connect(ui->examen_4, &QPushButton::clicked, this, &Vehicule::navigateToExamen);
    connect(ui->equipement_4, &QPushButton::clicked, this, &Vehicule::navigateToEquipement);
}

Vehicule::~Vehicule()
{
    delete ui;
}

void Vehicule::on_AjouterButton_2_clicked()
{
    // Add your vehicle addition logic here
    QMessageBox::information(this, "Success", "Véhicule ajouté avec succès!");
}

void Vehicule::on_AnnulerButton_2_clicked()
{
    // Add your cancel logic here
    ui->ModeleLineEdit_2->clear();
    ui->MatriculeLineEdit_2->clear();
    ui->KilomLineEdit_2->clear();
}

void Vehicule::navigateToCandidat()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(1);
    }
}

void Vehicule::navigateToPlanning()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(4);
    }
}

void Vehicule::navigateToEmployee()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(0);
    }
}

void Vehicule::navigateToVehicule()
{
    // Already in vehicule view
}

void Vehicule::navigateToExamen()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(3);
    }
}

void Vehicule::navigateToEquipement()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(5);
    }
}
