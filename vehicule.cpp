#include "vehicule.h"
#include "ui_vehicule.h"
#include "connection.h"
#include "navigation_constants.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QStackedWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidgetItem>
#include <QDebug>
#include <QDate>

Vehicule::Vehicule(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Vehicule),
    selectedRow(-1)
{
    ui->setupUi(this);
    
    db = QSqlDatabase::database();
    if (!db.isOpen()) {
        Connection c;
        c.createconnect();
        db = QSqlDatabase::database();
    }
    connect(ui->candidat_4, &QPushButton::clicked, this, &Vehicule::navigateToCandidat);
    connect(ui->planning_4, &QPushButton::clicked, this, &Vehicule::navigateToPlanning);
    connect(ui->employee_4, &QPushButton::clicked, this, &Vehicule::navigateToEmployee);
    connect(ui->vehicule_4, &QPushButton::clicked, this, &Vehicule::navigateToVehicule);
    connect(ui->examen_4, &QPushButton::clicked, this, &Vehicule::navigateToExamen);
    connect(ui->equipement_4, &QPushButton::clicked, this, &Vehicule::navigateToEquipement);
    
    connect(ui->tableWidget, &QTableWidget::itemClicked, this, &Vehicule::on_tableWidget_itemClicked);
    
    afficherVehicules();
    selectedRow = -1;
}

Vehicule::~Vehicule()
{
    delete ui;
}

void Vehicule::on_AjouterButton_clicked()
{
    if (ajouterVehicule()) {
        QMessageBox::information(this, "Succès", "Véhicule ajouté avec succès!");
        viderChamps();
        afficherVehicules();
    }
}

void Vehicule::on_Annuler_Button_clicked()
{
    viderChamps();
    selectedRow = -1;
}

void Vehicule::on_supprimerButton_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un véhicule à supprimer.");
        return;
    }
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer ce véhicule ?",
                                  QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (supprimerVehicule()) {
            QMessageBox::information(this, "Succès", "Véhicule supprimé avec succès!");
            viderChamps();
            afficherVehicules();
            selectedRow = -1;
        }
    }
}

void Vehicule::on_modifierButton_clicked()
{
    if (selectedRow < 0) {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un véhicule à modifier.");
        return;
    }
    
    if (modifierVehicule()) {
        QMessageBox::information(this, "Succès", "Véhicule modifié avec succès!");
        viderChamps();
        afficherVehicules();
        selectedRow = -1;
    }
}

void Vehicule::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    int row = item->row();
    remplirChamps(row);
    selectedRow = row;
}

void Vehicule::navigateToCandidat()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(Navigation::PAGE_CANDIDAT);
    }
}

void Vehicule::navigateToPlanning()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(Navigation::PAGE_PLANNING);
    }
}

void Vehicule::navigateToEmployee()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(Navigation::PAGE_EMPLOYES);
    }
}

void Vehicule::navigateToVehicule()
{
}

void Vehicule::navigateToExamen()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(Navigation::PAGE_EXAMEN);
    }
}

void Vehicule::navigateToEquipement()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(Navigation::PAGE_EQUIPEMENT);
    }
}

bool Vehicule::ajouterVehicule()
{

    QString modele = ui->ModeleLineEdit_2->toPlainText().trimmed();
    QString matricule = ui->MatriculeLineEdit_2->toPlainText().trimmed();
    QString kilometrageStr = ui->KilomLineEdit_2->toPlainText().trimmed();
    QString disponibilite = getDisponibilite();
    QString typeEnergie = ui->type_energie_comboBox->currentText();
    QString etatMaintenance = ui->etatmaintLineEdit->toPlainText().trimmed();
    QDate dateMaintenance = ui->dat_maintenance->date();
    
    //controles de saisie
    if (modele.isEmpty() || matricule.isEmpty() || kilometrageStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires.");
        return false;
    }
    if (typeEnergie.trimmed().isEmpty() || typeEnergie == "--------------") {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires.");
        return false;
    }
    
    bool ok;
    int kilometrage = kilometrageStr.toInt(&ok);
    if (!ok || kilometrage < 0) {
        QMessageBox::warning(this, "Erreur", "Le kilométrage doit être un nombre positif.");
        return false;
    }
    
    if (disponibilite.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un état de disponibilité.");
        return false;
    }
    // contrôle de saisie: état de maintenance doit être "assuré" ou "non assuré"
    {
        QString etatNorm = etatMaintenance.trimmed().toLower();
        if (etatNorm != "assuré" && etatNorm != "non assuré") {
            QMessageBox::warning(this, "Erreur", "L'état de maintenance doit être 'assuré' ou 'non assuré'.");
            return false;
        }
    }
    

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM VEHICULE WHERE MATRICULE = :matricule");
    checkQuery.bindValue(":matricule", matricule);
    if (checkQuery.exec() && checkQuery.next()) {
        if (checkQuery.value(0).toInt() > 0) {
            QMessageBox::warning(this, "Erreur", "Ce matricule existe déjà dans la base de données.");
            return false;
        }
    }
    
    QSqlQuery query;
    query.prepare("INSERT INTO VEHICULE (MODELE, MATRICULE, KILOMETRAGE, DISPONIBILITE, TYPE_ENERGIE, ETAT_MAINTENANCE, DATE_MAINTENANCE) "
                  "VALUES (:modele, :matricule, :kilometrage, :disponibilite, :typeEnergie, :etatMaintenance, :dateMaintenance)");
    
    query.bindValue(":modele", modele);
    query.bindValue(":matricule", matricule);
    query.bindValue(":kilometrage", kilometrage);
    query.bindValue(":disponibilite", disponibilite);
    query.bindValue(":typeEnergie", typeEnergie);
    query.bindValue(":etatMaintenance", etatMaintenance);
    query.bindValue(":dateMaintenance", dateMaintenance);
    
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout du véhicule:\n" + query.lastError().text());
        qDebug() << "Erreur SQL: " << query.lastError().text();
        return false;
    }
    
    return true;
}

void Vehicule::afficherVehicules()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(7);
    
    QStringList headers;
    headers << "Modele" << "Matricule" << "Kilometrage" << "disponibilité" 
            << "type d'energie" << "Etat de maintenance" << "date de maintenance";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    
    QSqlQuery query;
    query.prepare("SELECT MODELE, MATRICULE, KILOMETRAGE, DISPONIBILITE, TYPE_ENERGIE, ETAT_MAINTENANCE, DATE_MAINTENANCE FROM VEHICULE ORDER BY MATRICULE");
    
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des véhicules:\n" + query.lastError().text());
        qDebug() << "Erreur SQL: " << query.lastError().text();
        return;
    }
      // Remplissage tableau 
    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString())); // Modele
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString())); // Matricule
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString())); // Kilometrage
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toString())); // Disponibilite
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value(4).toString())); // Type d'energie
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value(5).toString())); // Etat de maintenance
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(query.value(6).toDate().toString("dd/MM/yyyy"))); // Date maintenance
        
        row++;
    }
    
    ui->tableWidget->resizeColumnsToContents();
}

bool Vehicule::modifierVehicule()
{
    if (selectedRow < 0) {
        return false;
    }
    
    QTableWidgetItem *matriculeItem = ui->tableWidget->item(selectedRow, 1);
    if (!matriculeItem) {
        QMessageBox::warning(this, "Erreur", "Impossible de récupérer le matricule du véhicule sélectionné.");
        return false;
    }
    QString matricule = matriculeItem->text();
    
    QString modele = ui->ModeleLineEdit_2->toPlainText().trimmed();
    QString nouveauMatricule = ui->MatriculeLineEdit_2->toPlainText().trimmed();
    QString kilometrageStr = ui->KilomLineEdit_2->toPlainText().trimmed();
    QString disponibilite = getDisponibilite();
    QString typeEnergie = ui->type_energie_comboBox->currentText();
    QString etatMaintenance = ui->etatmaintLineEdit->toPlainText().trimmed();
    QDate dateMaintenance = ui->dat_maintenance->date();
    if (modele.isEmpty() || nouveauMatricule.isEmpty() || kilometrageStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires.");
        return false;
    }
    // Interdire la modification du matricule
    if (nouveauMatricule != matricule) {
        QMessageBox::warning(this, "Erreur", "erreur , la matricule ne peut pas etre modifié");
        return false;
    }
    if (typeEnergie.trimmed().isEmpty() || typeEnergie == "--------------") {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires.");
        return false;
    }
    
    bool ok;
    int kilometrage = kilometrageStr.toInt(&ok);
    if (!ok || kilometrage < 0) {
        QMessageBox::warning(this, "Erreur", "Le kilométrage doit être un nombre positif.");
        return false;
    }
    
    if (disponibilite.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un état de disponibilité.");
        return false;
    }
    // contrôle de saisie: état de maintenance doit être "assuré" ou "non assuré"
    {
        QString etatNorm = etatMaintenance.trimmed().toLower();
        if (etatNorm != "assuré" && etatNorm != "non assuré") {
            QMessageBox::warning(this, "Erreur", "L'état de maintenance doit être 'assuré' ou 'non assuré'.");
            return false;
        }
    }
    
    
    QSqlQuery query;
    query.prepare("UPDATE VEHICULE SET MODELE = :modele, "
                  "KILOMETRAGE = :kilometrage, DISPONIBILITE = :disponibilite, "
                  "TYPE_ENERGIE = :typeEnergie, ETAT_MAINTENANCE = :etatMaintenance, "
                  "DATE_MAINTENANCE = :dateMaintenance "
                  "WHERE MATRICULE = :ancienMatricule");
    
    query.bindValue(":modele", modele);
    query.bindValue(":kilometrage", kilometrage);
    query.bindValue(":disponibilite", disponibilite);
    query.bindValue(":typeEnergie", typeEnergie);
    query.bindValue(":etatMaintenance", etatMaintenance);
    query.bindValue(":dateMaintenance", dateMaintenance);
    query.bindValue(":ancienMatricule", matricule);
    
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification du véhicule:\n" + query.lastError().text());
        qDebug() << "Erreur SQL: " << query.lastError().text();
        return false;
    }
    
    return true;
}

bool Vehicule::supprimerVehicule()
{
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        return false;
    }
    
    QTableWidgetItem *matriculeItem = ui->tableWidget->item(row, 1);
    if (!matriculeItem) {
        QMessageBox::warning(this, "Erreur", "Impossible de récupérer le matricule du véhicule sélectionné.");
        return false;
    }
    QString matricule = matriculeItem->text();
    
    QSqlQuery query;
    query.prepare("DELETE FROM VEHICULE WHERE MATRICULE = :matricule");
    query.bindValue(":matricule", matricule);
    
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression du véhicule:\n" + query.lastError().text());
        qDebug() << "Erreur SQL: " << query.lastError().text();
        return false;
    }
    
    return true;
}

void Vehicule::viderChamps()
{
    ui->ModeleLineEdit_2->clear();
    ui->MatriculeLineEdit_2->clear();
    ui->KilomLineEdit_2->clear();
    ui->etatmaintLineEdit->clear();
    
    ui->libre->setChecked(false);
    ui->reserve->setChecked(false);
    ui->enservice->setChecked(false);
    
    ui->dat_maintenance->setDate(QDate::currentDate());
    
    ui->type_energie_comboBox->setCurrentIndex(0);
}

void Vehicule::remplirChamps(int row)
{
    if (row < 0 || row >= ui->tableWidget->rowCount()) {
        return;
    }
       // Récupérer les données de la ligne sélectionnée 
    ui->ModeleLineEdit_2->setPlainText(ui->tableWidget->item(row, 0)->text());
    ui->MatriculeLineEdit_2->setPlainText(ui->tableWidget->item(row, 1)->text());
    ui->KilomLineEdit_2->setPlainText(ui->tableWidget->item(row, 2)->text());
    
    QString disponibilite = ui->tableWidget->item(row, 3)->text();
    ui->libre->setChecked(disponibilite == "Libre");
    ui->reserve->setChecked(disponibilite == "Réservé");
    ui->enservice->setChecked(disponibilite == "En service");
    
    QString typeEnergie = ui->tableWidget->item(row, 4)->text();
    int index = ui->type_energie_comboBox->findText(typeEnergie);
    if (index >= 0) {
        ui->type_energie_comboBox->setCurrentIndex(index);
    }
    
    // Récupérer l'état de maintenance
    ui->etatmaintLineEdit->setPlainText(ui->tableWidget->item(row, 5)->text());
    
    // Récupérer la date de maintenance
    QString dateStr = ui->tableWidget->item(row, 6)->text();
    QDate date = QDate::fromString(dateStr, "dd/MM/yyyy");
    if (date.isValid()) {
        ui->dat_maintenance->setDate(date);
    }
}

QString Vehicule::getDisponibilite()
{
    if (ui->libre->isChecked()) {
        return "Libre";
    } else if (ui->reserve->isChecked()) {
        return "Réservé";
    } else if (ui->enservice->isChecked()) {
        return "En service";
    }
    return "";
}
