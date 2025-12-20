#include "vehicule.h"
#include "ui_vehicule.h"
#include "connection.h"
#include "navigation_constants.h"
#include <QMainWindow>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextDocument>
#include <QFileInfo>
#include <QDir>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDebug>
#include <QDate>
#include <QTimer>
#include <QPainter>
#include <QPrinter>
#include <QFileDialog>
#include <QPrintDialog>
#include <QTextDocument>
#include <QPdfWriter>
#include <QDesktopServices>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QUrl>
#include <QUrlQuery>
#include <QClipboard>
#include <QApplication>
#include <QHBoxLayout>
#include <QTime>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRandomGenerator>
#include <QPixmap>
#include <QStackedWidget>
#include <QTableWidgetItem>
#include <QKeyEvent>
#include <QPageSize>
#include <QPageLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QColor>
#include <QResizeEvent>
#include <QDesktopServices>
#include <QFileInfo>
#include <QStringConverter>
#include "qrcodegen.h"
#include <memory>
#include <QSaveFile>
#include <QVBoxLayout>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QDateTime>
#include <QNetworkAddressEntry>

Vehicule::Vehicule(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Vehicule),
    selectedRow(-1),
    notificationLabel(nullptr),
    notificationTimer(nullptr),
    httpServer(nullptr),
    httpServerAlt(nullptr),
    httpPort(0)
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

    // Connecter le signal returnPressed pour recherche_4
    // Comme recherche_4 est un QTextEdit dans le .ui, on intercepte les événements clavier
    ui->recherche_4->installEventFilter(this);

    // Initialiser le timer pour les notifications
    notificationTimer = new QTimer(this);
    notificationTimer->setSingleShot(true);
    connect(notificationTimer, &QTimer::timeout, this, [this]() {
        if (notificationLabel) {
            notificationLabel->hide();
            notificationLabel->deleteLater();
            notificationLabel = nullptr;
        }
    });

    afficherVehicules();
    selectedRow = -1;
}

Vehicule::~Vehicule()
{
    delete ui;
}

void Vehicule::on_AjouterButton_clicked()
{
    // Récupérer la date de maintenance avant l'ajout pour l'afficher dans le message
    QDate dateMaintenance = ui->dat_maintenance->date();
    QString matricule = ui->MatriculeLineEdit_2->toPlainText().trimmed();

    if (ajouterVehicule()) {
        QMessageBox::information(this, "Succès", "Véhicule ajouté avec succès!");

        // Afficher le message des jours restants pour la maintenance
        afficherJoursRestantsMaintenance(dateMaintenance, matricule);

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

    // Récupérer la date de maintenance et le matricule pour afficher le message
    QTableWidgetItem *dateItem = ui->tableWidget->item(row, 6); // Colonne date de maintenance
    QTableWidgetItem *matriculeItem = ui->tableWidget->item(row, 1); // Colonne matricule

    if (dateItem && matriculeItem) {
        QString dateStr = dateItem->text();
        QString matricule = matriculeItem->text();
        QDate dateMaintenance = QDate::fromString(dateStr, "dd/MM/yyyy");

        if (dateMaintenance.isValid()) {
            afficherJoursRestantsMaintenance(dateMaintenance, matricule);
        }
    }
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
    QString etatMaintenance = ui->etatmainComboBox->currentText().trimmed();
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
    QString etatMaintenance = ui->etatmainComboBox->currentText().trimmed();
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

    // D'abord, supprimer tous les enregistrements de PLANNING qui référencent ce véhicule
    QSqlQuery deletePlanningQuery;
    deletePlanningQuery.prepare("DELETE FROM PLANNING WHERE MATRICULE = :matricule");
    deletePlanningQuery.bindValue(":matricule", matricule);

    if (!deletePlanningQuery.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression des enregistrements de planning associés:\n" + deletePlanningQuery.lastError().text());
        qDebug() << "Erreur SQL (suppression planning): " << deletePlanningQuery.lastError().text();
        return false;
    }

    // Ensuite, supprimer le véhicule
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
    ui->etatmainComboBox->setCurrentIndex(0);

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
    QString etatMaintenance = ui->tableWidget->item(row, 5)->text();
    int indexEtat = ui->etatmainComboBox->findText(etatMaintenance);
    if (indexEtat >= 0) {
        ui->etatmainComboBox->setCurrentIndex(indexEtat);
    }

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

void Vehicule::afficherJoursRestantsMaintenance(const QDate& dateMaintenance, const QString& matricule)
{
    if (!dateMaintenance.isValid()) {
        return; // Pas de date valide, on ne fait rien
    }

    QDate aujourdhui = QDate::currentDate();
    int joursRestants = aujourdhui.daysTo(dateMaintenance);

    QString message;
    QString couleurFond;
    QString couleurTexte;

    if (joursRestants > 0) {
        message = QString("Il vous reste %1 jour%2 pour la maintenance de votre véhicule (Matricule: %3).")
                      .arg(joursRestants)
                      .arg(joursRestants > 1 ? "s" : "")
                      .arg(matricule);
        couleurFond = "#4CAF50"; // Vert pour les jours restants
        couleurTexte = "#FFFFFF";
    } else if (joursRestants == 0) {
        message = QString("La maintenance de votre véhicule (Matricule: %1) est prévue aujourd'hui !")
                      .arg(matricule);
        couleurFond = "#FF9800"; // Orange pour aujourd'hui
        couleurTexte = "#FFFFFF";
    } else {
        // La date est passée
        int joursDepasses = -joursRestants;
        message = QString("La maintenance de votre véhicule (Matricule: %1) est en retard de %2 jour%3 !")
                      .arg(matricule)
                      .arg(joursDepasses)
                      .arg(joursDepasses > 1 ? "s" : "");
        couleurFond = "#F44336"; // Rouge pour retard
        couleurTexte = "#FFFFFF";
    }

    // Supprimer l'ancienne notification si elle existe
    if (notificationLabel) {
        notificationLabel->hide();
        notificationLabel->deleteLater();
        notificationLabel = nullptr;
    }

    // Arrêter le timer précédent s'il est actif
    if (notificationTimer->isActive()) {
        notificationTimer->stop();
    }

    // Créer le label de notification
    notificationLabel = new QLabel(message, this);
    notificationLabel->setWordWrap(true);
    notificationLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // Style de la notification
    notificationLabel->setStyleSheet(
        QString("QLabel {"
                "background-color: %1;"
                "color: %2;"
                "padding: 15px 20px;"
                "border-radius: 8px;"
                "font-size: 14px;"
                "font-weight: bold;"
                "border: 2px solid rgba(0,0,0,0.1);"
                "}")
            .arg(couleurFond)
            .arg(couleurTexte)
        );

    // Positionner en bas à droite
    int margin = 20;
    int labelWidth = 500;
    notificationLabel->setFixedWidth(labelWidth);
    int labelHeight = notificationLabel->sizeHint().height();
    // Hauteur minimale pour le label
    if (labelHeight < 60) {
        labelHeight = 60;
    }
    int x = this->width() - labelWidth - margin;
    int y = this->height() - labelHeight - margin;

    notificationLabel->setGeometry(x, y, labelWidth, labelHeight);
    notificationLabel->show();
    notificationLabel->raise(); // Mettre au premier plan

    // Faire disparaître la notification après 5 secondes
    notificationTimer->start(5000);
}

void Vehicule::on_recherche_4_returnPressed()
{
    QString recherche = ui->recherche_4->toPlainText().trimmed();

    if (recherche.isEmpty()) {
        afficherVehicules();
        return;
    }

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(7);

    QStringList headers;
    headers << "Modele" << "Matricule" << "Kilometrage" << "disponibilité"
            << "type d'energie" << "Etat de maintenance" << "date de maintenance";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    QSqlQuery query;
    // Recherche par matricule ou par modèle (recherche partielle insensible à la casse)
    query.prepare("SELECT MODELE, MATRICULE, KILOMETRAGE, DISPONIBILITE, TYPE_ENERGIE, ETAT_MAINTENANCE, DATE_MAINTENANCE "
                  "FROM VEHICULE "
                  "WHERE UPPER(MATRICULE) LIKE UPPER(:recherche) OR UPPER(MODELE) LIKE UPPER(:recherche)");
    query.bindValue(":recherche", "%" + recherche + "%");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la recherche:\n" + query.lastError().text());
        qDebug() << "Erreur SQL: " << query.lastError().text();
        return;
    }

    bool found = false;
    int row = 0;
    while (query.next()) {
        found = true;
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

    if (!found) {
        QMessageBox::information(this, "Recherche", "Aucun véhicule trouvé");
    }
}

void Vehicule::on_TriComboBox_2_currentIndexChanged(int index)
{
    // Index 0 = "--------------" -> afficher sans tri spécial
    if (index == 0) {
        afficherVehicules();
        return;
    }

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(7);

    QStringList headers;
    headers << "Modele" << "Matricule" << "Kilometrage" << "disponibilité"
            << "type d'energie" << "Etat de maintenance" << "date de maintenance";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    QSqlQuery query;
    QString orderBy;

    // Index 1 = "Date de maintenance" -> tri par date de maintenance croissante
    // Index 2 = "Kilométrage" -> tri par kilométrage croissant
    if (index == 1) {
        // Tri par date de maintenance croissante (les NULL en dernier)
        orderBy = "ORDER BY DATE_MAINTENANCE ASC NULLS LAST";
    } else if (index == 2) {
        // Tri par kilométrage croissant (les NULL en dernier)
        orderBy = "ORDER BY KILOMETRAGE ASC NULLS LAST";
    } else {
        // Par défaut, afficher tous les véhicules
        afficherVehicules();
        return;
    }

    query.prepare("SELECT MODELE, MATRICULE, KILOMETRAGE, DISPONIBILITE, TYPE_ENERGIE, ETAT_MAINTENANCE, DATE_MAINTENANCE FROM VEHICULE " + orderBy);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors du tri:\n" + query.lastError().text());
        qDebug() << "Erreur SQL: " << query.lastError().text();
        return;
    }

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

void Vehicule::onExporterVehicules()
{
    bool ok = false;
    QString choix = QInputDialog::getItem(
        this,
        "Exporter les véhicules",
        "Format d'export",
        {"PDF", "CSV"},
        0,
        false,
        &ok
        );

    if (!ok || choix.isEmpty())
        return;

    if (choix == "PDF")
        exporterVehiculesPDF();
    else
        exporterVehiculesCSV();
}
void Vehicule::exporterVehiculesPDF()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF",
                                                    QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
                                                    "Fichiers PDF (*.pdf)");

    if (fileName.isEmpty())
        return;

    // S'assurer que le fichier a l'extension .pdf
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive))
        fileName += ".pdf";

    // Créer un objet QPrinter configuré pour le format PDF
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Portrait);
    printer.setOutputFileName(fileName);

    // Créer un document pour le contenu
    QTextDocument doc;
    QTextCursor cursor(&doc);

    // Style CSS pour le document
    QString html = "<style>"
                   "body { font-family: Arial, sans-serif; }"
                   "h1 { color: #2c3e50; text-align: center; }"
                   "table { width: 100%; border-collapse: collapse; margin: 20px 0; }"
                   "th { background-color: #3498db; color: white; padding: 10px; text-align: left; }"
                   "td { padding: 8px; border: 1px solid #ddd; }"
                   "tr:nth-child(even) { background-color: #f2f2f2; }"
                   ".footer { margin-top: 20px; text-align: right; font-size: 0.8em; color: #7f8c8d; }"
                   "</style>";

    // En-tête du document
    html += QString("<h1>Liste des véhicules</h1>"
                    "<p><strong>Date d'export :</strong> %1</p>")
                .arg(QDate::currentDate().toString("dd/MM/yyyy"));

    // Début du tableau
    html += "<table>";

    // En-têtes du tableau
    html += "<tr>";
    QStringList headers;
    headers << "Modèle" << "Matricule" << "Kilométrage" << "Disponibilité"
            << "Type énergie" << "État maintenance" << "Date maintenance";

    for (const QString& header : headers) {
        html += "<th>" + header + "</th>";
    }
    html += "</tr>";

    QSqlQuery query(db);
    query.prepare("SELECT MODELE, MATRICULE, KILOMETRAGE, DISPONIBILITE, TYPE_ENERGIE, ETAT_MAINTENANCE, DATE_MAINTENANCE FROM VEHICULE ORDER BY MATRICULE");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les données : " + query.lastError().text());
        return;
    }

    int total = 0;

    while (query.next()) {
        total++;
        html += "<tr>";
        html += "<td>" + query.value(0).toString().toHtmlEscaped() + "</td>";
        html += "<td>" + query.value(1).toString().toHtmlEscaped() + "</td>";
        html += "<td>" + query.value(2).toString().toHtmlEscaped() + "</td>";
        html += "<td>" + query.value(3).toString().toHtmlEscaped() + "</td>";
        html += "<td>" + query.value(4).toString().toHtmlEscaped() + "</td>";
        html += "<td>" + query.value(5).toString().toHtmlEscaped() + "</td>";
        html += "<td>" + query.value(6).toDate().toString("dd/MM/yyyy").toHtmlEscaped() + "</td>";
        html += "</tr>";
    }

    // Fin du tableau
    html += "</table>";

    // Pied de page avec le total
    html += QString("<div class='footer'>Total des véhicules : <strong>%1</strong></div>")
                .arg(total);

    // Définir le contenu HTML du document
    doc.setHtml(html);

    // Ajuster la taille du document à la largeur de la page
    doc.setPageSize(printer.pageRect(QPrinter::Point).size());

    // Imprimer le document dans le fichier PDF
    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::critical(this, "Erreur", "Impossible d'écrire dans le fichier PDF");
        return;
    }

    // Dessiner le contenu sur la page
    doc.drawContents(&painter);
    painter.end();

    // Afficher un message de confirmation
    QMessageBox::information(this, "Export réussi",
                             QString("Le fichier PDF a été généré avec succès.\n"
                                     "%1 véhicules exportés.\n"
                                     "Emplacement : %2")
                                 .arg(total)
                                 .arg(QDir::toNativeSeparators(fileName)));

    // Ouvrir le fichier PDF avec le visualiseur par défaut
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}
void Vehicule::exporterVehiculesCSV()
{
    const QString path = QFileDialog::getSaveFileName(
        this,
        "Exporter CSV",
        QString(),
        "CSV (*.csv)"
        );

    if (path.isEmpty()) return;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    // En-têtes
    QStringList headers;
    headers << "Modèle" << "Matricule" << "Kilométrage" << "Disponibilité"
            << "Type énergie" << "État maintenance" << "Date maintenance";

    out << headers.join(",") << "\n";

    // Récupérer données SQL
    QSqlQuery query;
    query.prepare("SELECT MODELE, MATRICULE, KILOMETRAGE, DISPONIBILITE, "
                  "TYPE_ENERGIE, ETAT_MAINTENANCE, DATE_MAINTENANCE "
                  "FROM VEHICULE ORDER BY MATRICULE");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", query.lastError().text());
        return;
    }

    int total = 0;

    while (query.next()) {
        total++;

        out << query.value(0).toString() << ","
            << query.value(1).toString() << ","
            << query.value(2).toString() << ","
            << query.value(3).toString() << ","
            << query.value(4).toString() << ","
            << query.value(5).toString() << ","
            << query.value(6).toDate().toString("dd/MM/yyyy")
            << "\n";
    }

    file.close();

    QMessageBox::information(this, "Succès",
                             QString("Export CSV réussi (%1 véhicules).\nFichier : %2")
                                 .arg(total)
                                 .arg(path));
}
void Vehicule::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

bool Vehicule::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->recherche_4 && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            on_recherche_4_returnPressed();
            return true; // Consommer l'événement pour éviter l'ajout de nouvelle ligne
        }
    }
    return QWidget::eventFilter(obj, event);
}

// === Statistiques de disponibilité ===
QMap<QString, QVariant> Vehicule::getStatistiquesDisponibilite()
{
    QMap<QString, QVariant> statistiques;
    int libre = 0;
    int reserve = 0;
    int enService = 0;
    int totalVehicules = 0;

    QSqlQuery query;
    query.prepare("SELECT DISPONIBILITE, COUNT(*) FROM VEHICULE GROUP BY DISPONIBILITE");

    if (query.exec()) {
        while (query.next()) {
            QString dispo = query.value(0).toString().trimmed();
            int count = query.value(1).toInt();

            // Comparaison insensible à la casse et aux espaces
            QString dispoLower = dispo.toLower();
            if (dispoLower == "libre") {
                libre = count;
            } else if (dispoLower == "réservé" || dispoLower == "reserve") {
                reserve = count;
            } else if (dispoLower == "en service" || dispoLower == "enservice") {
                enService = count;
            }

            totalVehicules += count;
        }

        // Si aucune donnée trouvée, compter le total depuis la table
        if (totalVehicules == 0) {
            QSqlQuery countQuery;
            countQuery.prepare("SELECT COUNT(*) FROM VEHICULE");
            if (countQuery.exec() && countQuery.next()) {
                totalVehicules = countQuery.value(0).toInt();
            }
        }

        // Calcul des pourcentages
        double pourcentageLibre = totalVehicules > 0 ? (libre * 100.0) / totalVehicules : 0;
        double pourcentageReserve = totalVehicules > 0 ? (reserve * 100.0) / totalVehicules : 0;
        double pourcentageEnService = totalVehicules > 0 ? (enService * 100.0) / totalVehicules : 0;

        // Remplir la map avec les résultats
        statistiques["libre"] = libre;
        statistiques["reserve"] = reserve;
        statistiques["enService"] = enService;
        statistiques["totalVehicules"] = totalVehicules;
        statistiques["pourcentageLibre"] = pourcentageLibre;
        statistiques["pourcentageReserve"] = pourcentageReserve;
        statistiques["pourcentageEnService"] = pourcentageEnService;

        qDebug() << "Statistiques disponibilité - Libre:" << libre
                 << "Réservé:" << reserve
                 << "En service:" << enService
                 << "Total:" << totalVehicules;
    } else {
        qDebug() << "Erreur requête statistiques disponibilité:" << query.lastError().text();
        // Retourner des valeurs par défaut en cas d'erreur
        statistiques["libre"] = 0;
        statistiques["reserve"] = 0;
        statistiques["enService"] = 0;
        statistiques["totalVehicules"] = 0;
        statistiques["pourcentageLibre"] = 0;
        statistiques["pourcentageReserve"] = 0;
        statistiques["pourcentageEnService"] = 0;
    }

    return statistiques;
}

// === Statistiques de type d'énergie ===
QMap<QString, QVariant> Vehicule::getStatistiquesTypeEnergie()
{
    QMap<QString, QVariant> statistiques;
    int gaz = 0;
    int essence = 0;
    int hybride = 0;
    int totalVehicules = 0;

    QSqlQuery query;
    query.prepare("SELECT TYPE_ENERGIE, COUNT(*) FROM VEHICULE GROUP BY TYPE_ENERGIE");

    if (query.exec()) {
        while (query.next()) {
            QString typeEnergie = query.value(0).toString().trimmed();
            int count = query.value(1).toInt();

            // Comparaison insensible à la casse
            QString typeLower = typeEnergie.toLower();
            if (typeLower == "gaz") {
                gaz = count;
            } else if (typeLower == "essence") {
                essence = count;
            } else if (typeLower == "hybride") {
                hybride = count;
            }

            totalVehicules += count;
        }

        // Si aucune donnée trouvée, compter le total depuis la table
        if (totalVehicules == 0) {
            QSqlQuery countQuery;
            countQuery.prepare("SELECT COUNT(*) FROM VEHICULE");
            if (countQuery.exec() && countQuery.next()) {
                totalVehicules = countQuery.value(0).toInt();
            }
        }

        // Calcul des pourcentages
        double pourcentageGaz = totalVehicules > 0 ? (gaz * 100.0) / totalVehicules : 0;
        double pourcentageEssence = totalVehicules > 0 ? (essence * 100.0) / totalVehicules : 0;
        double pourcentageHybride = totalVehicules > 0 ? (hybride * 100.0) / totalVehicules : 0;

        // Remplir la map avec les résultats
        statistiques["gaz"] = gaz;
        statistiques["essence"] = essence;
        statistiques["hybride"] = hybride;
        statistiques["totalVehicules"] = totalVehicules;
        statistiques["pourcentageGaz"] = pourcentageGaz;
        statistiques["pourcentageEssence"] = pourcentageEssence;
        statistiques["pourcentageHybride"] = pourcentageHybride;

        qDebug() << "Statistiques type d'énergie - Gaz:" << gaz
                 << "Essence:" << essence
                 << "Hybride:" << hybride
                 << "Total:" << totalVehicules;
    } else {
        qDebug() << "Erreur requête statistiques type d'énergie:" << query.lastError().text();
        // Retourner des valeurs par défaut en cas d'erreur
        statistiques["gaz"] = 0;
        statistiques["essence"] = 0;
        statistiques["hybride"] = 0;
        statistiques["totalVehicules"] = 0;
        statistiques["pourcentageGaz"] = 0;
        statistiques["pourcentageEssence"] = 0;
        statistiques["pourcentageHybride"] = 0;
    }

    return statistiques;
}

void Vehicule::on_tri_2_currentIndexChanged(int index)
{
    // Index 0 = "--------------" -> ne rien afficher
    if (index == 0) {
        return;
    }

    // Créer une fenêtre de dialogue pour afficher les statistiques
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Statistiques des véhicules");
    dialog->setMinimumSize(600, 500);

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    QPieSeries *series = new QPieSeries();
    QString titreStat = "";
    int total = 0;

    if (index == 1) {
        // Disponibilité
        QMap<QString, QVariant> stats = getStatistiquesDisponibilite();

        int libre = stats["libre"].toInt();
        int reserve = stats["reserve"].toInt();
        int enService = stats["enService"].toInt();
        total = stats["totalVehicules"].toInt();
        double pourcentageLibre = stats["pourcentageLibre"].toDouble();
        double pourcentageReserve = stats["pourcentageReserve"].toDouble();
        double pourcentageEnService = stats["pourcentageEnService"].toDouble();

        titreStat = "Statistiques de Disponibilité des Véhicules";

        if (total == 0) {
            QLabel *label = new QLabel("Aucun véhicule trouvé dans la base de données.", dialog);
            layout->addWidget(label);
            QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, dialog);
            layout->addWidget(buttonBox);
            connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
            dialog->exec();
            return;
        }

        // Ajouter les tranches du camembert avec couleurs
        if (libre > 0) {
            QPieSlice *slice = series->append("Libre " + QString::number(libre) + " (" + QString::number(pourcentageLibre, 'f', 1) + "%)", libre);
            slice->setColor(QColor(34, 139, 34)); // Vert
            slice->setLabelVisible(true);
        }
        if (reserve > 0) {
            QPieSlice *slice = series->append("Réservé " + QString::number(reserve) + " (" + QString::number(pourcentageReserve, 'f', 1) + "%)", reserve);
            slice->setColor(QColor(255, 99, 71)); // Rouge/Orange
            slice->setLabelVisible(true);
        }
        if (enService > 0) {
            QPieSlice *slice = series->append("En service " + QString::number(enService) + " (" + QString::number(pourcentageEnService, 'f', 1) + "%)", enService);
            slice->setColor(QColor(70, 130, 180)); // Bleu
            slice->setLabelVisible(true);
        }

    } else if (index == 2) {
        // Type d'énergie
        QMap<QString, QVariant> stats = getStatistiquesTypeEnergie();

        int gaz = stats["gaz"].toInt();
        int essence = stats["essence"].toInt();
        int hybride = stats["hybride"].toInt();
        total = stats["totalVehicules"].toInt();
        double pourcentageGaz = stats["pourcentageGaz"].toDouble();
        double pourcentageEssence = stats["pourcentageEssence"].toDouble();
        double pourcentageHybride = stats["pourcentageHybride"].toDouble();

        titreStat = "Statistiques de Type d'Énergie des Véhicules";

        if (total == 0) {
            QLabel *label = new QLabel("Aucun véhicule trouvé dans la base de données.", dialog);
            layout->addWidget(label);
            QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, dialog);
            layout->addWidget(buttonBox);
            connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
            dialog->exec();
            return;
        }

        // Ajouter les tranches du camembert avec couleurs
        if (gaz > 0) {
            QPieSlice *slice = series->append("Gaz " + QString::number(gaz) + " (" + QString::number(pourcentageGaz, 'f', 1) + "%)", gaz);
            slice->setColor(QColor(255, 140, 0)); // Orange
            slice->setLabelVisible(true);
        }
        if (essence > 0) {
            QPieSlice *slice = series->append("Essence " + QString::number(essence) + " (" + QString::number(pourcentageEssence, 'f', 1) + "%)", essence);
            slice->setColor(QColor(220, 20, 60)); // Rouge
            slice->setLabelVisible(true);
        }
        if (hybride > 0) {
            QPieSlice *slice = series->append("Hybride " + QString::number(hybride) + " (" + QString::number(pourcentageHybride, 'f', 1) + "%)", hybride);
            slice->setColor(QColor(50, 205, 50)); // Vert clair
            slice->setLabelVisible(true);
        }
    } else {
        delete dialog;
        return;
    }

    // Créer le graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(titreStat);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Créer la vue du graphique
    QChartView *chartView = new QChartView(chart, dialog);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Ajouter le titre avec le total
    QLabel *titleLabel = new QLabel(QString("Statistiques - Total: %1 véhicule(s)").arg(total), dialog);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(titleLabel);
    layout->addWidget(chartView);

    // Ajouter le bouton OK
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, dialog);
    layout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);

    dialog->exec();
    delete dialog;
}

void Vehicule::on_QR_CODE_2_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un véhicule pour générer le QR code.");
        return;
    }

    // Récupérer le matricule du véhicule sélectionné
    QTableWidgetItem *matriculeItem = ui->tableWidget->item(row, 1);
    if (!matriculeItem) {
        QMessageBox::warning(this, "Erreur", "Impossible de récupérer le matricule du véhicule sélectionné.");
        return;
    }

    QString matricule = matriculeItem->text();

    // Générer un QR avec un texte lisible sur smartphone (pas d'URL)
    QString dataText = genererTexteQRCode(matricule);
    if (dataText.startsWith("Erreur:")) {
        QMessageBox::critical(this, "Erreur", dataText);
        return;
    }
    QImage qrImage = ::QRCodeGenerator::generateQRCode(dataText, 360);

    if (qrImage.isNull() || qrImage.width() == 0 || qrImage.height() == 0) {
        QMessageBox::critical(this, "Erreur",
                              "Impossible de générer le QR code.\n\n"
                              "Vérifiez les informations du véhicule et réessayez.");
        return;
    }

    // Créer une boîte de dialogue pour afficher le QR code
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("QR Code - Véhicule " + matricule);
    dialog->setMinimumSize(550, 720);

    // Style pour la boîte de dialogue
    dialog->setStyleSheet(
        "QDialog { "
        "    background-color: #f5f7fa;"
        "    font-family: 'Segoe UI', Arial, sans-serif;"
        "}"
        "QLabel { "
        "    color: #2c3e50; "
        "    margin: 5px; "
        "    font-size: 14px;"
        "}"
        "QPushButton { "
        "    background-color: #ffce00; "
        "    color: #002157; "
        "    border: none; "
        "    padding: 10px 20px; "
        "    border-radius: 6px; "
        "    font-weight: 600;"
        "    min-width: 150px;"
        "    margin: 5px;"
        "}"
        "QPushButton:hover { "
        "    background-color: #e6b800; "
        "    box-shadow: 0 2px 5px rgba(0,0,0,0.2);"
        "}"
        "QPushButton:pressed { "
        "    background-color: #d4a600; "
        "    padding: 9px 19px 11px 21px;"
        "}"
        );

    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
    mainLayout->setSpacing(8);
    mainLayout->setContentsMargins(20, 6, 20, 20);

    // En-tête
    QLabel *headerLabel = new QLabel("FICHE VÉHICULE", dialog);
    headerLabel->setStyleSheet(
        "font-size: 22px; "
        "font-weight: bold; "
        "color: #002157; "
        "margin-bottom: 6px;"
        );
    headerLabel->setAlignment(Qt::AlignCenter);



    // Conteneur pour le QR code
    QFrame *qrFrame = new QFrame(dialog);
    qrFrame->setStyleSheet(
        "QFrame { "
        "    background: white; "
        "    border-radius: 12px; "
        "    border: 2px solid #e1e8ed; "
        "    padding: 4px 8px 8px 8px;"
        "}"
        );

    QVBoxLayout *qrLayout = new QVBoxLayout(qrFrame);
    qrLayout->setContentsMargins(8, 0, 8, 10);

    // Afficher le QR code
    QLabel *qrLabel = new QLabel(dialog);
    qrLabel->setStyleSheet("background-color: #ffffff;");
    qrLabel->setFixedSize(qrImage.size());
    QPixmap qrPixmap = QPixmap::fromImage(qrImage);
    qrLabel->setScaledContents(false);
    if (!qrPixmap.isNull()) {
        qrLabel->setPixmap(qrPixmap);
        qrLabel->setAlignment(Qt::AlignCenter);
    } else {
        qrLabel->setText("Erreur: Impossible de générer le QR code");
        qrLabel->setStyleSheet("color: #e74c3c; font-weight: 500;");
    }

    // Informations supplémentaires (affichées sous le QR, en dehors du cadre)
    QLabel *infoLabel = new QLabel(
        "<div style='text-align: center; margin-top: 8px; color: #7f8c8d; font-size: 12px;'>"
        "Scannez ce QR pour afficher la boîte d’informations sur votre smartphone."
        "</div>",
        dialog
        );
    infoLabel->setTextFormat(Qt::RichText);
    infoLabel->setWordWrap(true);
    infoLabel->setAlignment(Qt::AlignCenter);



    QPushButton *closeButton = new QPushButton("Fermer", dialog);
    closeButton->setIcon(QIcon(":/icons/close.png"));
    connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);

    // Assembler tout le contenu
    qrLayout->addWidget(qrLabel, 0, Qt::AlignTop | Qt::AlignHCenter);

    mainLayout->addWidget(headerLabel);
    mainLayout->addWidget(qrFrame, 0, Qt::AlignTop);
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(closeButton, 0, Qt::AlignHCenter);

    // Afficher la boîte de dialogue
    dialog->exec();
    delete dialog;
}

QString Vehicule::genererTexteQRCode(const QString& matricule)
{
    QSqlQuery query;
    query.prepare("SELECT MODELE, MATRICULE, KILOMETRAGE, DISPONIBILITE, TYPE_ENERGIE, ETAT_MAINTENANCE, DATE_MAINTENANCE FROM VEHICULE WHERE MATRICULE = :matricule");
    query.bindValue(":matricule", matricule);

    if (!query.exec() || !query.next()) {
        return "Erreur: Véhicule non trouvé";
    }

    QString modele = query.value(0).toString();
    QString matriculeValue = query.value(1).toString();
    int kilometrage = query.value(2).toInt();
    QString disponibilite = query.value(3).toString();
    QString typeEnergie = query.value(4).toString();
    QString etatMaintenance = query.value(5).toString();
    QDate dateMaintenance = query.value(6).toDate();

    QString prediction = predirePanne(matriculeValue, kilometrage, dateMaintenance, etatMaintenance, typeEnergie);

    QString predictionShort = prediction;
    if (predictionShort.length() > 220) {
        predictionShort = predictionShort.left(220) + "...";
    }

    QString msg = QString(
                      "DriveSmart\n"
                      "FICHE VÉHICULE\n"
                      "Matricule: %1\n"
                      "Modèle: %2\n"
                      "Kilométrage: %3 km\n"
                      "Disponibilité: %4\n"
                      "Type d'énergie: %5\n"
                      "État maintenance: %6\n"
                      "Date maintenance: %7\n\n"
                      "%8")
                      .arg(matriculeValue)
                      .arg(modele)
                      .arg(kilometrage)
                      .arg(disponibilite)
                      .arg(typeEnergie)
                      .arg(etatMaintenance)
                      .arg(dateMaintenance.toString("dd/MM/yyyy"))
                      .arg(predictionShort);

    return msg;
}

bool Vehicule::ensureHttpServer()
{
    if (httpServer && httpServer->isListening()) {
        return true;
    }

    httpRootDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/qrcode_html";
    if (!QDir(httpRootDir).exists()) {
        QDir().mkpath(httpRootDir);
    }

    if (!httpServer) {
        httpServer = new QTcpServer(this);
        connect(httpServer, &QTcpServer::newConnection, this, &Vehicule::handleHttpConnection);
    }
    if (!httpServerAlt) {
        httpServerAlt = new QTcpServer(this);
        connect(httpServerAlt, &QTcpServer::newConnection, this, &Vehicule::handleHttpConnection);
    }

    QList<quint16> candidates;
    candidates << 8080 << 8787;
    for (quint16 p : candidates) {
        if (httpServer->isListening()) break;
        if (httpServer->listen(QHostAddress::AnyIPv4, p)) {
            httpPort = p;
            break;
        }
    }
    if (!httpServer->isListening()) {
        for (quint16 p = 8000; p < 8050; ++p) {
            if (httpServer->listen(QHostAddress::AnyIPv4, p)) {
                httpPort = p;
                break;
            }
        }
    }
    if (!httpServer->isListening()) {
        return false;
    }

    if (!httpServerAlt->isListening()) {
        httpServerAlt->listen(QHostAddress::AnyIPv4, 80);
    }
    return true;
}

void Vehicule::handleHttpConnection()
{
    while (httpServer->hasPendingConnections()) {
        QTcpSocket *socket = httpServer->nextPendingConnection();
        if (!socket) continue;

        connect(socket, &QTcpSocket::readyRead, this, [this, socket]() {
            QByteArray req = socket->readAll();
            int headerEnd = req.indexOf("\r\n\r\n");
            if (headerEnd < 0) headerEnd = req.indexOf("\n\n");
            QByteArray requestLine;
            if (headerEnd >= 0) {
                int firstLineEnd = req.indexOf("\r\n");
                if (firstLineEnd < 0) firstLineEnd = req.indexOf('\n');
                requestLine = req.left(firstLineEnd);
            } else {
                // Fallback: prendre la première ligne jusqu'au \n
                int firstLineEnd = req.indexOf('\n');
                requestLine = firstLineEnd >= 0 ? req.left(firstLineEnd) : req;
            }

            requestLine = requestLine.trimmed();
            QList<QByteArray> parts = requestLine.split(' ');
            if (parts.size() < 2) {
                return;
            }
            QString path = QString::fromLatin1(parts.at(1)).trimmed();
            if (path.startsWith('/')) path.remove(0,1);
            path.replace("..", "");
            bool serveIndex = path.isEmpty();
            QString fullPath = QDir(httpRootDir).filePath(path);
            QFile file(fullPath);

            auto contentTypeFor = [](const QString &filePath) -> QByteArray {
                if (filePath.endsWith(".html", Qt::CaseInsensitive) || filePath.endsWith(".htm", Qt::CaseInsensitive))
                    return "text/html; charset=UTF-8";
                if (filePath.endsWith(".png", Qt::CaseInsensitive))
                    return "image/png";
                if (filePath.endsWith(".jpg", Qt::CaseInsensitive) || filePath.endsWith(".jpeg", Qt::CaseInsensitive))
                    return "image/jpeg";
                if (filePath.endsWith(".css", Qt::CaseInsensitive))
                    return "text/css";
                if (filePath.endsWith(".js", Qt::CaseInsensitive))
                    return "application/javascript";
                return "application/octet-stream";
            };

            QByteArray body;
            QByteArray statusLine;
            QByteArray headers;
            if (!serveIndex && file.exists() && file.open(QIODevice::ReadOnly)) {
                body = file.readAll();
                statusLine = "HTTP/1.1 200 OK\r\n";
                QByteArray ct = contentTypeFor(fullPath);
                headers = "Content-Type: " + ct + "\r\n";
            } else if (serveIndex) {
                QDir dir(httpRootDir);
                QStringList files = dir.entryList(QStringList() << "vehicle_*.html", QDir::Files, QDir::Name);
                QString indexHtml = "<html><head><meta charset=\"utf-8\"><title>DriveSmart</title></head><body><h3>DriveSmart</h3><ul>";
                for (const QString &f : files) {
                    indexHtml += QString("<li><a href=\"/%1\">%1</a></li>").arg(f);
                }
                indexHtml += "</ul></body></html>";
                body = indexHtml.toUtf8();
                statusLine = "HTTP/1.1 200 OK\r\n";
                headers = "Content-Type: text/html; charset=UTF-8\r\n";
            } else {
                body = QByteArray("<html><head><meta charset=\"utf-8\"></head><body>Not Found</body></html>");
                statusLine = "HTTP/1.1 404 Not Found\r\n";
                headers = "Content-Type: text/html; charset=UTF-8\r\n";
            }
            headers += "Content-Length: " + QByteArray::number(body.size()) + "\r\n";
            headers += "Connection: close\r\n";
            headers += "Cache-Control: no-cache\r\n\r\n";

            socket->write(statusLine);
            socket->write(headers);
            socket->write(body);
            socket->flush();
            socket->disconnectFromHost();
        });
    }
}

QString Vehicule::selectBestLanIp()
{
    auto isPrivateLan = [](const QString &ipStr) -> bool {
        QStringList parts = ipStr.split('.');
        if (parts.size() != 4) return false;
        int a = parts[0].toInt();
        int b = parts[1].toInt();
        if (a == 10) return true;
        if (a == 192 && b == 168) return true;
        if (a == 172 && b >= 16 && b <= 31) return true;
        return false;
    };

    auto looksVirtual = [](const QString &name) -> bool {
        QString n = name.toLower();
        return n.contains("virtual") || n.contains("vmware") || n.contains("vbox") || n.contains("hyper-v") || n.contains("bridge") || n.contains("npcap") || n.contains("tap") || n.contains("hamachi") || n.contains("zerotier");
    };

    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();

    auto pickFrom = [&](QNetworkInterface::InterfaceType preferredType) -> QString {
        for (const QNetworkInterface &iface : ifaces) {
            if (iface.flags().testFlag(QNetworkInterface::IsLoopBack)) continue;
            if (!iface.flags().testFlag(QNetworkInterface::IsUp)) continue;
            if (!iface.flags().testFlag(QNetworkInterface::IsRunning)) continue;
            if (!iface.flags().testFlag(QNetworkInterface::CanBroadcast)) continue;
            if (looksVirtual(iface.humanReadableName()) || looksVirtual(iface.name())) continue;
            if (preferredType != QNetworkInterface::Unknown && iface.type() != preferredType) continue;

            const auto entries = iface.addressEntries();
            for (const QNetworkAddressEntry &entry : entries) {
                QHostAddress addr = entry.ip();
                if (addr.protocol() != QAbstractSocket::IPv4Protocol) continue;
                QString s = addr.toString();
                if (isPrivateLan(s)) return s;
            }
        }
        return QString();
    };

    QString ip;
    ip = pickFrom(QNetworkInterface::Wifi);
    if (ip.isEmpty()) ip = pickFrom(QNetworkInterface::Ethernet);
    if (ip.isEmpty()) ip = pickFrom(QNetworkInterface::Unknown);

    if (!ip.isEmpty()) return ip;

    // Dernier recours: parcourir toutes les adresses IPv4 non loopback
    for (const QHostAddress &addr : QNetworkInterface::allAddresses()) {
        if (addr.protocol() == QAbstractSocket::IPv4Protocol && !addr.isLoopback()) {
            QString s = addr.toString();
            if (isPrivateLan(s)) return s;
        }
    }
    return QString();
}
QString Vehicule::predirePanne(const QString& matricule [[maybe_unused]], int kilometrage, const QDate& dateMaintenance, const QString& etatMaintenance, const QString& typeEnergie)
{
    QDate aujourdhui = QDate::currentDate();
    int joursDepuisMaintenance = dateMaintenance.daysTo(aujourdhui);

    // Facteurs de risque
    double facteurKilometrage = 0.0;
    double facteurMaintenance = 0.0;
    double facteurTemps = 0.0;
    double facteurEnergie = 0.0;

    // Facteur kilométrage (plus le kilométrage est élevé, plus le risque augmente)
    if (kilometrage > 200000) {
        facteurKilometrage = 0.4; // Risque très élevé
    } else if (kilometrage > 150000) {
        facteurKilometrage = 0.3; // Risque élevé
    } else if (kilometrage > 100000) {
        facteurKilometrage = 0.2; // Risque moyen
    } else if (kilometrage > 50000) {
        facteurKilometrage = 0.1; // Risque faible
    }

    // Facteur maintenance
    QString etatNorm = etatMaintenance.trimmed().toLower();
    if (etatNorm == "non assuré") {
        facteurMaintenance = 0.3; // Risque élevé si non assuré
    } else if (etatNorm == "assuré") {
        facteurMaintenance = 0.05; // Risque faible si assuré
    }

    // Facteur temps depuis la dernière maintenance
    if (joursDepuisMaintenance > 365) {
        facteurTemps = 0.3; // Plus d'un an depuis la maintenance
    } else if (joursDepuisMaintenance > 180) {
        facteurTemps = 0.2; // Plus de 6 mois
    } else if (joursDepuisMaintenance > 90) {
        facteurTemps = 0.1; // Plus de 3 mois
    }

    // Facteur type d'énergie
    QString energieNorm = typeEnergie.trimmed().toLower();
    if (energieNorm == "essence") {
        facteurEnergie = 0.05; // Essence nécessite plus de maintenance
    } else if (energieNorm == "gaz") {
        facteurEnergie = 0.08; // Gaz nécessite plus d'attention
    } else if (energieNorm == "hybride") {
        facteurEnergie = 0.03; // Hybride est plus fiable
    }

    // Calcul du score de risque total (0.0 à 1.0)
    double scoreRisque = facteurKilometrage + facteurMaintenance + facteurTemps + facteurEnergie;
    scoreRisque = qMin(scoreRisque, 1.0); // Limiter à 1.0

    // Calculer les jours jusqu'à la prochaine panne prédite
    // Base: 365 jours si risque faible, 30 jours si risque très élevé
    int joursJusquaPanne;
    QString typePanne;

    QRandomGenerator *rng = QRandomGenerator::global();
    if (scoreRisque >= 0.7) {
        joursJusquaPanne = 15 + (rng->bounded(30)); // 15-45 jours
        typePanne = "panne moteur";
    } else if (scoreRisque >= 0.5) {
        joursJusquaPanne = 30 + (rng->bounded(45)); // 30-75 jours
        typePanne = "panne moteur";
    } else if (scoreRisque >= 0.3) {
        joursJusquaPanne = 60 + (rng->bounded(60)); // 60-120 jours
        typePanne = "panne système";
    } else if (scoreRisque >= 0.15) {
        joursJusquaPanne = 120 + (rng->bounded(90)); // 120-210 jours
        typePanne = "panne système";
    } else {
        joursJusquaPanne = 200 + (rng->bounded(100)); // 200-300 jours
        typePanne = "maintenance préventive";
    }

    // Formater le message de prédiction (sans caractères spéciaux pour éviter les problèmes d'encodage)
    QString message = QString(
                          "PREDICTION DU SYSTEME:\n\n"
                          "Base sur l'historique, ce vehicule risque une %1 dans %2 jours.\n\n"
                          "Facteurs analyses:\n"
                          "- Kilometrage: %3 km\n"
                          "- Etat maintenance: %4\n"
                          "- Derniere maintenance: %5 jours\n"
                          "- Type d'energie: %6\n\n"
                          "Recommandation: Planifier une maintenance preventive."
                          )
                          .arg(typePanne)
                          .arg(joursJusquaPanne)
                          .arg(kilometrage)
                          .arg(etatMaintenance)
                          .arg(joursDepuisMaintenance > 0 ? joursDepuisMaintenance : 0)
                          .arg(typeEnergie);

    return message;
}

QString Vehicule::genererFichierHTML(const QString& matricule, const QString& modele, int kilometrage,
                                     const QString& disponibilite, const QString& typeEnergie,
                                     const QString& etatMaintenance, const QDate& dateMaintenance,
                                     const QString& prediction)
{
    // Vérification des paramètres d'entrée
    if (matricule.isEmpty() || modele.isEmpty()) {
        qWarning() << "Erreur: Matricule ou modèle vide";
        return "Erreur: Matricule ou modèle invalide";
    }

    // Extraire le nombre de jours depuis la prédiction
    int joursPanne = 0;
    QRegularExpression rx(R"(dans (\d+) jours)");
    QRegularExpressionMatch match = rx.match(prediction);
    if (match.hasMatch()) {
        bool ok;
        joursPanne = match.captured(1).toInt(&ok);
        if (!ok) joursPanne = 0;
    }

    // Déterminer l'état du véhicule
    QString etat = "En bon état";
    QString couleurEtat = "green";
    if (joursPanne < 30) {
        etat = "À réviser";
        couleurEtat = "orange";
    }
    if (joursPanne < 15) {
        etat = "Urgent";
        couleurEtat = "red";
    }

    // Utiliser QStandardPaths pour un emplacement plus approprié
    QString htmlDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/qrcode_html";
    QDir dir;
    if (!dir.exists(htmlDir)) {
        if (!dir.mkpath(htmlDir)) {
            qCritical() << "Erreur: Impossible de créer le répertoire" << htmlDir;
            return QString("Erreur: Impossible de créer le répertoire de sortie");
        }
    }

    // Nettoyer le nom de fichier pour éviter les injections
    QString safeMatricule = matricule;
    safeMatricule.replace(QRegularExpression("[^a-zA-Z0-9_-]"), "_");
    QString fileName = QString("vehicle_%1.html").arg(safeMatricule);
    QString filePath = QDir::toNativeSeparators(htmlDir + "/" + fileName);

    // Créer le contenu HTML avec une syntaxe plus sécurisée
    QString htmlContent = QString(
                              R"(<!DOCTYPE html>
        <html lang="fr">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Véhicule %1</title>
            <style>
                body { font-family: Arial, sans-serif; max-width: 600px; margin: 20px auto; padding: 20px; background-color: #f5f5f5; }
                .container { background-color: white; padding: 20px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
                h2 { color: #002157; border-bottom: 3px solid #ffce00; padding-bottom: 10px; }
                p { margin: 15px 0; font-size: 16px; line-height: 1.6; }
                .info-label { font-weight: bold; color: #002157; }
                .prediction { background-color: #fff3cd; padding: 15px; border-radius: 5px; border-left: 4px solid #ffc107; margin-top: 20px; }
                .prediction-urgent { background-color: #f8d7da; border-left-color: #dc3545; }
            </style>
        </head>
        <body>
            <div class="container">
                <h2>Véhicule : %2 %1</h2>
                <p><span class="info-label">Kilométrage :</span> %3 km</p>
                <p><span class="info-label">Type d'énergie :</span> %4</p>
                <p><span class="info-label">Disponibilité :</span> %5</p>
                <p><span class="info-label">État de maintenance :</span> %6</p>
                <p><span class="info-label">Date de maintenance :</span> %7</p>
                <p style="color:%8; font-weight:bold;">État : %9</p>
                <div class="prediction%10">
                    <p style="color:%11; font-weight:bold; font-size:18px; margin:0;">🔮 Prochaine panne estimée : %12 jours</p>
                </div>
                <hr style="margin: 20px 0; border: none; border-top: 1px solid #ddd;">
                <p style="color: #666; font-size: 12px; text-align: center;">Généré par DriveSmart</p>
            </div>
        </body>
        </html>)")
                              .arg(matricule.toHtmlEscaped())
                              .arg(modele.toHtmlEscaped())
                              .arg(kilometrage)
                              .arg(typeEnergie.toHtmlEscaped())
                              .arg(disponibilite.toHtmlEscaped())
                              .arg(etatMaintenance.toHtmlEscaped())
                              .arg(dateMaintenance.toString("dd/MM/yyyy"))
                              .arg(couleurEtat)
                              .arg(etat.toHtmlEscaped())
                              .arg(joursPanne < 15 ? " prediction-urgent" : "")
                              .arg(joursPanne < 15 ? "red" : (joursPanne < 30 ? "orange" : "green"))
                              .arg(joursPanne);

    // Utiliser QSaveFile pour une écriture atomique
    QSaveFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out.setEncoding(QStringConverter::Utf8);
        out << htmlContent;

        if (!file.commit()) {
            qCritical() << "Erreur lors de l'écriture du fichier:" << filePath << "-" << file.errorString();
            return QString("Erreur: Impossible d'écrire dans le fichier");
        }

        qDebug() << "Fichier HTML généré avec succès:" << filePath;
        return QUrl::fromLocalFile(filePath).toString();
    } else {
        qCritical() << "Erreur: Impossible d'ouvrir le fichier en écriture:" << filePath << "-" << file.errorString();
        return QString("Erreur: Impossible de créer le fichier HTML");
    }
}

void Vehicule::addInfoRow(QVBoxLayout *layout, const QString &label, const QString &value)
{
    // Créer un conteneur horizontal pour la ligne
    QWidget *rowContainer = new QWidget();
    QHBoxLayout *rowLayout = new QHBoxLayout(rowContainer);
    rowLayout->setContentsMargins(5, 2, 5, 2);
    rowLayout->setSpacing(10);

    // Créer le label avec le texte en gras
    QLabel *labelWidget = new QLabel(label, rowContainer);
    labelWidget->setStyleSheet("font-weight: bold; min-width: 180px;");

    // Créer le label pour la valeur
    QLabel *valueWidget = new QLabel(value, rowContainer);
    valueWidget->setWordWrap(true);
    valueWidget->setStyleSheet("padding: 2px 5px; background-color: #ffffff; border-radius: 3px;");

    // Ajouter les widgets au layout de la ligne
    rowLayout->addWidget(labelWidget);
    rowLayout->addWidget(valueWidget, 1); // Le 1 permet à la valeur de prendre l'espace disponible

    // Ajouter la ligne au layout principal
    layout->addWidget(rowContainer);
}

void Vehicule::on_pushButton_18_clicked()
{
    // Vérifier la connexion à la base de données
    if (!db.isValid() || !db.isOpen()) {
        db = QSqlDatabase::database();
        if (!db.isOpen()) {
            Connection c;
            c.createconnect();
            db = QSqlDatabase::database();

            if (!db.isOpen()) {
                QMessageBox::critical(this, "Erreur de base de données",
                                      "Impossible de se connecter à la base de données.");
                return;
            }
        }
    }

    // Demander à l'utilisateur où sauvegarder le fichier
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "Liste_Vehicules.pdf", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) {
        return; // L'utilisateur a annulé
    }

    // S'assurer que le fichier a l'extension .pdf
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }

    // Vérifier la connexion à la base de données
    if (!db.isValid() || !db.isOpen()) {
        db = QSqlDatabase::database();
        if (!db.isOpen()) {
            Connection c;
            if (!c.createconnect()) {
                QMessageBox::critical(this, "Erreur de connexion",
                                      "Impossible de se connecter à la base de données.\n"
                                      "Veuillez vérifier la configuration de la base de données.");
                return;
            }
            db = QSqlDatabase::database();
        }
    }

    QSqlQuery query(db);
    query.prepare("SELECT MATRICULE, MODELE, KILOMETRAGE, DISPONIBILITE, TYPE_ENERGIE, ETAT_MAINTENANCE, DATE_MAINTENANCE FROM VEHICULE ORDER BY MATRICULE");
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur",
                              "Erreur lors de la récupération des véhicules :\n" +
                                  query.lastError().text() +
                                  "\n\nVérifiez que la table VEHICULE existe et contient des données.");
        return;
    }

    // Créer le contenu HTML
    QString html = "<html><head>"
                   "<style>"
                   "body { font-family: Arial, sans-serif; }"
                   "h1 { color: #2c3e50; text-align: center; }"
                   ".info { margin: 20px 0; }"
                   "table { width: 100%; border-collapse: collapse; margin: 20px 0; }"
                   "th { background-color: #3498db; color: white; padding: 10px; text-align: left; }"
                   "td { padding: 8px; border: 1px solid #ddd; }"
                   "tr:nth-child(even) { background-color: #f2f2f2; }"
                   ".disponible { color: green; font-weight: bold; }"
                   ".indisponible { color: red; font-weight: bold; }"
                   ".footer { margin-top: 30px; font-style: italic; text-align: right; }"
                   "</style>"
                   "</head><body>";

    // En-tête du document
    html += "<h1>Liste des Véhicules</h1>";
    html += "<div class='info'>";
    html += "<p><strong>Date d'export :</strong> " + QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") + "</p>";
    html += "</div>";

    // Début du tableau
    html += "<table>";

    // En-têtes du tableau
    html += "<tr>";
    html += "<th>Modèle</th>";
    html += "<th>Matricule</th>";
    html += "<th>Kilométrage</th>";
    html += "<th>Disponibilité</th>";
    html += "<th>Type d'énergie</th>";
    html += "<th>État maintenance</th>";
    html += "<th>Date maintenance</th>";
    html += "</tr>";

    // Compter les véhicules
    int totalVehicules = 0;
    int vehiculesDisponibles = 0;

    // Remplir le tableau avec les données
    while (query.next()) {
        QString matricule = query.value(0).toString();
        QString modele = query.value(1).toString();
        int kilometrage = query.value(2).toInt();
        QString disponibilite = query.value(3).toString();
        QString typeEnergie = query.value(4).toString();
        QString etatMaintenance = query.value(5).toString();
        QDate dateMaintenance = query.value(6).toDate();

        // Compter les véhicules disponibles
        if (disponibilite.compare("Disponible", Qt::CaseInsensitive) == 0) {
            vehiculesDisponibles++;
        }

        html += "<tr>";
        html += "<td>" + modele + "</td>";
        html += "<td>" + matricule + "</td>";
        html += "<td>" + QString::number(kilometrage) + " km</td>";

        // Mettre en forme la disponibilité
        QString dispoClass = (disponibilite.compare("Disponible", Qt::CaseInsensitive) == 0)
                                 ? "disponible"
                                 : "indisponible";
        html += "<td class='" + dispoClass + "'>" + disponibilite + "</td>";

        html += "<td>" + typeEnergie + "</td>";
        html += "<td>" + etatMaintenance + "</td>";
        html += "<td>" + (dateMaintenance.isValid() ? dateMaintenance.toString("dd/MM/yyyy") : "N/A") + "</td>";
        html += "</tr>";

        totalVehicules++;
    }

    // Fin du tableau
    html += "</table>";

    // Ajouter un résumé
    html += "<div class='info'>";
    html += "<p><strong>Total des véhicules :</strong> " + QString::number(totalVehicules) + "</p>";

    if (totalVehicules > 0) {
        // Statistiques par type d'énergie
        QSqlQuery energieQuery("SELECT TYPE_ENERGIE, COUNT(*) FROM VEHICULE GROUP BY TYPE_ENERGIE");
        if (energieQuery.exec() && energieQuery.next()) {
            html += "<p><strong>Répartition par type d'énergie :</strong> ";
            bool first = true;
            do {
                if (!first) html += ", ";
                QString type = energieQuery.value(0).toString();
                int count = energieQuery.value(1).toInt();
                double percentage = (count * 100.0) / totalVehicules;
                html += type + ": " + QString::number(count) + " (" + QString::number(percentage, 'f', 1) + "%)";
                first = false;
            } while (energieQuery.next());
            html += "</p>";
        }
    }
    html += "</div>";

    // Pied de page
    html += "<div class='footer'>";
    html += "<p>Document généré par DriveSmart - " + QDate::currentDate().toString("yyyy") + "</p>";
    html += "</div>";

    // Fin du document
    html += "</body></html>";

    // Créer et configurer le document PDF
    QTextDocument document;
    document.setHtml(html);

    // Configurer l'imprimante PDF
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize::A4);
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    // Générer le PDF
    document.print(&printer);

    // Vérifier si le fichier a été créé
    QFileInfo fileInfo(fileName);
    if (fileInfo.exists() && fileInfo.size() > 0) {
        QMessageBox::information(this, "Export réussi",
                                 QString("Le fichier PDF a été généré avec succès.\n"
                                         "%1 véhicule(s) exporté(s).\n\n"
                                         "Emplacement : %2")
                                     .arg(totalVehicules)
                                     .arg(QDir::toNativeSeparators(fileName)));
    } else {
        QMessageBox::warning(this, "Erreur",
                             "Le fichier PDF n'a pas pu être créé.\n"
                             "Veuillez vérifier les permissions d'écriture.");
    }
}
