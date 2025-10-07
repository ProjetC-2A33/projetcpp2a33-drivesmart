#include "vehicule.h"
#include "ui_vehicule.h"

// Inclure les autres fenêtres
#include "mainwindow.h"        // candidat
#include "employe.h"
#include "plannification.h"
#include "examen.h"

Vehicule::Vehicule(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Vehicule)
{
    ui->setupUi(this);

    // Aller vers Candidat
    connect(ui->btncandidat, &QPushButton::clicked, this, [=](){
        MainWindow *w = new MainWindow(); // fenêtre candidat
        w->show();
        this->close();
    });

    // Aller vers Employé
    connect(ui->btnemployee, &QPushButton::clicked, this, [=](){
        Employe *w = new Employe();
        w->show();
        this->close();
    });

    // Aller vers Planification
    connect(ui->btnplanification, &QPushButton::clicked, this, [=](){
        Plannification *w = new Plannification();
        w->show();
        this->close();
    });

    // Aller vers Examen
    connect(ui->btnexamen, &QPushButton::clicked, this, [=](){
        Examen *w = new Examen();
        w->show();
        this->close();
    });
}

Vehicule::~Vehicule()
{
    delete ui;
}
