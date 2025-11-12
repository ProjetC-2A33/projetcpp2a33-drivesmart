#include "examen.h"
#include "ui_examen.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QStackedWidget>
Examen::Examen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Examen)
{
    ui->setupUi(this);

    // Connect navigation buttons
    connect(ui->condidat_5, &QPushButton::clicked, this, &Examen::navigateToCandidat);
    connect(ui->planning_5, &QPushButton::clicked, this, &Examen::navigateToPlanning);
    connect(ui->employee_5, &QPushButton::clicked, this, &Examen::navigateToEmployee);
    connect(ui->vehicule_5, &QPushButton::clicked, this, &Examen::navigateToVehicule);
    connect(ui->examen_5, &QPushButton::clicked, this, &Examen::navigateToExamen);
    connect(ui->equipment_5, &QPushButton::clicked, this, &Examen::navigateToEquipement);
}

Examen::~Examen()
{
    delete ui;
}

void Examen::on_pushButton_4_clicked()
{
    // Add exam logic here
    QMessageBox::information(this, "Success", "Examen ajouté avec succès!");
}

void Examen::on_pushButton_5_clicked()
{
    // Modify exam logic here
    QMessageBox::information(this, "Success", "Examen modifié avec succès!");
}

void Examen::on_pushButton_7_clicked()
{
    // Cancel logic here
    ui->textEdit_6->clear();
    ui->textEdit_11->clear();
    ui->textEdit_13->clear();
    ui->textEdit_12->clear();
}

void Examen::navigateToCandidat()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(1);
    }
}

void Examen::navigateToPlanning()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(4);
    }
}

void Examen::navigateToEmployee()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(0);
    }
}

void Examen::navigateToVehicule()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(2);
    }
}

void Examen::navigateToExamen()
{
    // Already in examen view
}

void Examen::navigateToEquipement()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(5);
    }
}
