#include "condidat.h"
#include "ui_condidat.h"
#include <QMessageBox>
#include <QMainWindow>
#include <QStackedWidget>
Condidat::Condidat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Condidat)
{
    ui->setupUi(this);

    // Connect navigation buttons
    connect(ui->candidat, &QPushButton::clicked, this, &Condidat::navigateToCandidat);
    connect(ui->planning, &QPushButton::clicked, this, &Condidat::navigateToPlanning);
    connect(ui->employee, &QPushButton::clicked, this, &Condidat::navigateToEmployee);
    connect(ui->vehicule, &QPushButton::clicked, this, &Condidat::navigateToVehicule);
    connect(ui->examen, &QPushButton::clicked, this, &Condidat::navigateToExamen);
    connect(ui->equipement, &QPushButton::clicked, this, &Condidat::navigateToEquipement);
}

Condidat::~Condidat()
{
    delete ui;
}

void Condidat::on_btn_ajout_clicked()
{
    // Add your candidate addition logic here
    QMessageBox::information(this, "Success", "Candidat ajouté avec succès!");
}

void Condidat::on_btn_reset_clicked()
{
    // Add your reset logic here
    ui->cin->clear();
    ui->nom->clear();
    ui->prenom->clear();
    ui->tel->clear();
}

void Condidat::navigateToCandidat()
{
    // Already in candidat view
}

void Condidat::navigateToPlanning()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(4);
    }
}

void Condidat::navigateToEmployee()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(0);
    }
}

void Condidat::navigateToVehicule()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(2);
    }
}

void Condidat::navigateToExamen()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(3);
    }
}

void Condidat::navigateToEquipement()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(5);
    }
}
