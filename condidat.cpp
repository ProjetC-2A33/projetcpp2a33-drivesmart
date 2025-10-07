#include "condidat.h"
#include "ui_condidat.h"

condidat::condidat(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::condidat)
{
    ui->setupUi(this);
    ui->tab->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents); // 0 = 1ère colonne


}

condidat::~condidat()
{
    delete ui;
}
