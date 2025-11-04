#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "condidat.h"
#include "vehicule.h"
#include "examen.h"
#include "planning.h"
#include "gestion_equipement.h"
#include <QStackedWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    condidatView(nullptr),
    vehiculeView(nullptr),
    examenView(nullptr),
    planningView(nullptr),
    equipementView(nullptr),
    employeeView(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("DriveSmart - Main Application");

    // Create stacked widget for navigation
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Create employee view from mainwindow.ui
    employeeView = ui->centralwidget;
    stackedWidget->addWidget(employeeView);

    // Create other views
    condidatView = new Condidat();
    vehiculeView = new Vehicule();
    examenView = new Examen();
    planningView = new Planning();
    equipementView = new Gestion_Equipement();

    // Add all views to stacked widget
    stackedWidget->addWidget(condidatView);    // Index 1 - Candidat
    stackedWidget->addWidget(vehiculeView);    // Index 2 - Vehicule
    stackedWidget->addWidget(examenView);      // Index 3 - Examen
    stackedWidget->addWidget(planningView);    // Index 4 - Planning
    stackedWidget->addWidget(equipementView);  // Index 5 - Equipement

    // Set initial view to Employee
    stackedWidget->setCurrentIndex(0);

    // Connect navigation buttons from employee view
    connect(ui->candidat_7, &QPushButton::clicked, this, &MainWindow::showCandidat);
    connect(ui->planning_7, &QPushButton::clicked, this, &MainWindow::showPlanning);
    connect(ui->employee_7, &QPushButton::clicked, this, &MainWindow::showEmployee);
    connect(ui->vehicule_7, &QPushButton::clicked, this, &MainWindow::showVehicule);
    connect(ui->examen_7, &QPushButton::clicked, this, &MainWindow::showExamen);
    connect(ui->equipement_7, &QPushButton::clicked, this, &MainWindow::showEquipement);
}

MainWindow::~MainWindow()
{
    delete ui;
    // Qt will automatically delete the views since they're parented to stackedWidget
}

void MainWindow::showCandidat()
{
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::showVehicule()
{
    stackedWidget->setCurrentIndex(2);
}

void MainWindow::showExamen()
{
    stackedWidget->setCurrentIndex(3);
}

void MainWindow::showPlanning()
{
    stackedWidget->setCurrentIndex(4);
}

void MainWindow::showEmployee()
{
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::showEquipement()
{
    stackedWidget->setCurrentIndex(5);
}
