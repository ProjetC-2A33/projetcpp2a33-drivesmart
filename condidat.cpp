#include "condidat.h"
#include "ui_condidat.h"
#include <QMessageBox>
#include <QMainWindow>
#include <QStackedWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QTableWidgetItem>
#include <QDate>
#include <memory>
#include <QPushButton>
#include <QHBoxLayout>
#include <QWidget>
#include <QHeaderView>
#include <QStyle>
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

    // Note: slots on_supp_clicked and on_edit_clicked are auto-connected by Qt
    // via connectSlotsByName(this) inside setupUi, due to their naming.
    // No manual connects needed to avoid duplicate signal handling.

    // Initialize table headers (ensure 8 columns: 7 data + 1 actions)
    ui->tab->setColumnCount(8);
    QStringList headers;
    headers << "CIN" << "nom" << "Prénom" << "Sexe" << "date de naissance" << "Télephone" << "type de permis" << "Actions";
    ui->tab->setHorizontalHeaderLabels(headers);

    // Table visual/interaction tuning
    ui->tab->setShowGrid(true);
    ui->tab->setAlternatingRowColors(true);
    ui->tab->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tab->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tab->horizontalHeader()->setVisible(true);
    ui->tab->verticalHeader()->setVisible(true); // shows row numbers
    ui->tab->horizontalHeader()->setStretchLastSection(true);
    ui->tab->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tab->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // disable bottom scrollbar only
    ui->tab->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Load initial data
    refreshTable();
}

Condidat::~Condidat()
{
    delete ui;
}

void Condidat::on_btn_ajout_clicked()
{
    bool ok = false;
    int cin = ui->cin->toPlainText().trimmed().toInt(&ok);
    if (!ok) { QMessageBox::warning(this, "Erreur", "CIN invalide."); return; }
    QString nom = ui->nom->toPlainText().trimmed();
    QString prenom = ui->prenom->toPlainText().trimmed();
    QString sexe = ui->homme->isChecked() ? "H" : (ui->femme->isChecked() ? "F" : "");
    QDate date = ui->date->date();
    int tel = ui->tel->toPlainText().trimmed().toInt(&ok);
    if (!ok) { QMessageBox::warning(this, "Erreur", "Téléphone invalide."); return; }
    QString type = ui->type->currentText();

    if (isEditMode) {
        int originalCin = currentEditingCin != -1 ? currentEditingCin : selectedCinFromTable();
        if (originalCin == -1) { QMessageBox::warning(this, "Modification", "Sélectionnez une ligne."); return; }
        if (modifier(originalCin, cin, nom, prenom, sexe, date, tel, type)) {
            QMessageBox::information(this, "Modification", "Candidat modifié.");
            refreshTable();
            setEditMode(false);
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de modification.");
        }
        return;
    }

    // Add mode
    this->cin_condidat = cin;
    this->nom = nom;
    this->prenom = prenom;
    this->sexe = sexe;
    this->date_naissance = date;
    this->tel = tel;
    this->type_permis = type;

    if (ajouter()) {
        QMessageBox::information(this, "Succès", "Candidat ajouté avec succès!");
        refreshTable();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec d'ajout du candidat.");
    }
}

void Condidat::on_btn_reset_clicked()
{
    // Add your reset logic here
    ui->cin->clear();
    ui->nom->clear();
    ui->prenom->clear();
    ui->tel->clear();
}

QSqlQueryModel* Condidat::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT cin_condidat, nom, prenom, sexe, date_naissance, tel, type_permis FROM CONDIDAT");
    return model;
}

bool Condidat::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO CONDIDAT (cin_condidat, nom, prenom, sexe, date_naissance, tel, type_permis) "
                  "VALUES (:cin, :nom, :prenom, :sexe, :date_naissance, :tel, :type)");
    query.bindValue(":cin", cin_condidat);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":sexe", sexe);
    query.bindValue(":date_naissance", date_naissance);
    query.bindValue(":tel", tel);
    query.bindValue(":type", type_permis);
    return query.exec();
}

bool Condidat::supprimer(int cin)
{
    QSqlQuery query;
    query.prepare("DELETE FROM CONDIDAT WHERE cin_condidat = :cin");
    query.bindValue(":cin", cin);
    return query.exec();
}

bool Condidat::modifier(int originalCin, int newCin, QString nom, QString prenom, QString sexe, QDate date_naissance, int tel, QString type_permis)
{
    QSqlQuery query;
    query.prepare("UPDATE CONDIDAT SET cin_condidat = :newcin, nom = :nom, prenom = :prenom, sexe = :sexe, date_naissance = :date, tel = :tel, type_permis = :type "
                  "WHERE cin_condidat = :origcin");
    query.bindValue(":newcin", newCin);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":sexe", sexe);
    query.bindValue(":date", date_naissance);
    query.bindValue(":tel", tel);
    query.bindValue(":type", type_permis);
    query.bindValue(":origcin", originalCin);
    return query.exec();
}

void Condidat::refreshTable()
{
    std::unique_ptr<QSqlQueryModel> model(afficher());
    int rows = model->rowCount();
    int cols = model->columnCount();
    ui->tab->setRowCount(rows);
    ui->tab->setColumnCount(cols + 1); // +1 for actions
    QStringList headers;
    headers << "CIN" << "nom" << "Prénom" << "Sexe" << "date de naissance" << "Télephone" << "type de permis" << "Actions";
    ui->tab->setHorizontalHeaderLabels(headers);
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QVariant v = model->data(model->index(r, c));
            ui->tab->setItem(r, c, new QTableWidgetItem(v.toString()));
        }
        // Add actions cell with Edit/Supp buttons
        bool ok = false;
        int cin = model->data(model->index(r, 0)).toInt(&ok);
        ui->tab->setCellWidget(r, cols, createActionsCell(r, ok ? cin : -1));
    }
}

int Condidat::selectedCinFromTable() const
{
    QList<QTableWidgetItem*> items = ui->tab->selectedItems();
    if (items.isEmpty()) return -1;
    int row = items.first()->row();
    QTableWidgetItem *cinItem = ui->tab->item(row, 0);
    bool ok = false;
    int cin = cinItem ? cinItem->text().toInt(&ok) : -1;
    return ok ? cin : -1;
}

void Condidat::setEditMode(bool enabled)
{
    isEditMode = enabled;
    if (enabled) {
        ui->btn_ajout->setText("Edit");
    } else {
        ui->btn_ajout->setText("Ajouter");
        currentEditingCin = -1;
    }
}

void Condidat::on_supp_clicked()
{
    int cin = selectedCinFromTable();
    if (cin == -1) { QMessageBox::warning(this, "Suppression", "Sélectionnez une ligne."); return; }
    if (supprimer(cin)) {
        QMessageBox::information(this, "Suppression", "Candidat supprimé.");
        refreshTable();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de suppression.");
    }
}

void Condidat::on_edit_clicked()
{
    int originalCin = currentEditingCin != -1 ? currentEditingCin : selectedCinFromTable();
    if (originalCin == -1) { QMessageBox::warning(this, "Modification", "Sélectionnez une ligne."); return; }
    bool ok = false;
    int newCin = ui->cin->toPlainText().trimmed().toInt(&ok);
    if (!ok) { QMessageBox::warning(this, "Erreur", "CIN invalide."); return; }
    QString nom = ui->nom->toPlainText().trimmed();
    QString prenom = ui->prenom->toPlainText().trimmed();
    QString sexe = ui->homme->isChecked() ? "H" : (ui->femme->isChecked() ? "F" : "");
    QDate date = ui->date->date();
    int tel = ui->tel->toPlainText().trimmed().toInt(&ok);
    if (!ok) { QMessageBox::warning(this, "Erreur", "Téléphone invalide."); return; }
    QString type = ui->type->currentText();
    if (modifier(originalCin, newCin, nom, prenom, sexe, date, tel, type)) {
        QMessageBox::information(this, "Modification", "Candidat modifié.");
        refreshTable();
        currentEditingCin = -1;
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de modification.");
    }
}

void Condidat::setFormFromRow(int row)
{
    if (row < 0 || row >= ui->tab->rowCount()) return;
    auto getText = [&](int col){ QTableWidgetItem *it = ui->tab->item(row, col); return it ? it->text() : QString(); };
    ui->cin->setText(getText(0));
    ui->nom->setText(getText(1));
    ui->prenom->setText(getText(2));
    QString sexeStr = getText(3).trimmed().toUpper();
    ui->homme->setChecked(sexeStr == "H" || sexeStr == "HOMME");
    ui->femme->setChecked(sexeStr == "F" || sexeStr == "FEMME");
    // Date: try parse ISO or locale
    QDate d = QDate::fromString(getText(4), Qt::ISODate);
    if (!d.isValid()) d = QDate::fromString(getText(4), "dd/MM/yyyy");
    if (d.isValid()) ui->date->setDate(d);
    ui->tel->setText(getText(5));
    int typeIndex = ui->type->findText(getText(6));
    if (typeIndex >= 0) ui->type->setCurrentIndex(typeIndex);
}

QWidget* Condidat::createActionsCell(int row, int cin)
{
    QWidget *container = new QWidget(ui->tab);
    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(6);
    QPushButton *btnEdit = new QPushButton(container);
    QPushButton *btnDel = new QPushButton(container);
    btnEdit->setText("");
    btnDel->setText("");
    btnDel->setIcon(QIcon::fromTheme("edit-delete", style()->standardIcon(QStyle::SP_TrashIcon)));
    btnEdit->setIcon(QIcon::fromTheme("mail-message-new",style()->standardIcon(QStyle::SP_FileDialogNewFolder)));

    btnEdit->setToolTip("Modifier");
    btnDel->setToolTip("Supprimer");
    btnEdit->setFixedSize(28, 24);
    btnDel->setFixedSize(28, 24);
    btnEdit->setIconSize(QSize(18,18));
    btnDel->setIconSize(QSize(18,18));
    layout->addWidget(btnEdit);
    layout->addWidget(btnDel);
    container->setLayout(layout);

    // Delete action
    QObject::connect(btnDel, &QPushButton::clicked, this, [=](){
        if (cin == -1) return;
        if (QMessageBox::question(this, "Suppression", "Supprimer ce candidat ?") == QMessageBox::Yes) {
            if (supprimer(cin)) {
                refreshTable();
            } else {
                QMessageBox::critical(this, "Erreur", "Échec de suppression.");
            }
        }
    });

    // Edit action: toggle edit mode. If clicking again on same row, exit edit mode
    QObject::connect(btnEdit, &QPushButton::clicked, this, [=](){
        if (isEditMode && currentEditingCin == cin) {
            setEditMode(false);
            return;
        }
        setFormFromRow(row);
        currentEditingCin = cin;
        setEditMode(true);
    });

    return container;
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
