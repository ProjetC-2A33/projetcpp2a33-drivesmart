#include "planning.h"
#include "ui_planning.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QStackedWidget>
Planning::Planning(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Planning)
{
    ui->setupUi(this);

    // Connect navigation buttons
    connect(ui->candidat_3, &QPushButton::clicked, this, &Planning::navigateToCandidat);
    connect(ui->planning_3, &QPushButton::clicked, this, &Planning::navigateToPlanning);
    connect(ui->employee_3, &QPushButton::clicked, this, &Planning::navigateToEmployee);
    connect(ui->vehicule_3, &QPushButton::clicked, this, &Planning::navigateToVehicule);
    connect(ui->examen_3, &QPushButton::clicked, this, &Planning::navigateToExamen);
    connect(ui->equipement_3, &QPushButton::clicked, this, &Planning::navigateToEquipement);
}

Planning::~Planning()
{
    delete ui;
}

void Planning::on_btn_ajout_3_clicked()
{
    // Add planning logic here
    QMessageBox::information(this, "Success", "Planning ajouté avec succès!");
}

void Planning::on_btn_reset_3_clicked()
{
    // Cancel planning logic here
    ui->cin_4->clear();
    ui->tel_3->clear();
}

void Planning::navigateToCandidat()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(1);
    }
}

void Planning::navigateToPlanning()
{
    // Already in planning view
}

void Planning::navigateToEmployee()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(0);
    }
}

void Planning::navigateToVehicule()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(2);
    }
}

void Planning::navigateToExamen()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(3);
    }
}

void Planning::navigateToEquipement()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(5);
    }
}
