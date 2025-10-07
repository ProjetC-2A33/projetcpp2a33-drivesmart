#include "gestion_equipement.h"
#include "ui_gestion_equipement.h"

Gestion_Equipement::Gestion_Equipement(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Gestion_Equipement)
{
    ui->setupUi(this);
}

Gestion_Equipement::~Gestion_Equipement()
{
    delete ui;
}

