#include "gestion_equipement.h"
#include "ui_gestion_equipement.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QStackedWidget>
Gestion_Equipement::Gestion_Equipement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Gestion_Equipement)
{
    ui->setupUi(this);

    // Connect navigation buttons
    connect(ui->candidat, &QPushButton::clicked, this, &Gestion_Equipement::navigateToCandidat);
    connect(ui->planning, &QPushButton::clicked, this, &Gestion_Equipement::navigateToPlanning);
    connect(ui->employee, &QPushButton::clicked, this, &Gestion_Equipement::navigateToEmployee);
    connect(ui->vehicule, &QPushButton::clicked, this, &Gestion_Equipement::navigateToVehicule);
    connect(ui->examen, &QPushButton::clicked, this, &Gestion_Equipement::navigateToExamen);
    connect(ui->equipement, &QPushButton::clicked, this, &Gestion_Equipement::navigateToEquipement);
}

Gestion_Equipement::~Gestion_Equipement()
{
    delete ui;
}

void Gestion_Equipement::on_btn_ajout_clicked()
{
    // Add equipment logic here
    QMessageBox::information(this, "Success", "Équipement ajouté avec succès!");
}

void Gestion_Equipement::on_btn_reset_clicked()
{
    // Cancel logic here
    ui->nom_equipement->clear();
    ui->nom_equipement_2->clear();
}

void Gestion_Equipement::on_btnUpdateMaintenance_clicked()
{
    // Maintenance update logic here
    QMessageBox::information(this, "Maintenance", "Statut de maintenance mis à jour!");
}

void Gestion_Equipement::on_btnCheckWeather_clicked()
{
    // Weather check logic here
    QMessageBox::information(this, "Météo", "Vérification de la météo effectuée!");
}

void Gestion_Equipement::navigateToCandidat()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(1);
    }
}

void Gestion_Equipement::navigateToPlanning()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(4);
    }
}

void Gestion_Equipement::navigateToEmployee()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(0);
    }
}

void Gestion_Equipement::navigateToVehicule()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(2);
    }
}

void Gestion_Equipement::navigateToExamen()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(3);
    }
}

void Gestion_Equipement::navigateToEquipement()
{
    // Already in equipement view
}
