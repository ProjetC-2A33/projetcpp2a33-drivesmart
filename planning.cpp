#include "planning.h"
#include "ui_planning.h"
#include "navigation_constants.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QStackedWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QTableWidgetItem>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <memory>
#include <QPushButton>
#include <QHBoxLayout>
#include <QWidget>
#include <QHeaderView>
#include <QStyle>
#include <QIcon>
#include <QRegularExpression>
#include <QComboBox>
#include <QDebug>

Planning::Planning(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Planning)
{
    ui->setupUi(this);

    connect(ui->candidat_3, &QPushButton::clicked, this, &Planning::navigateToCandidat);
    connect(ui->planning_3, &QPushButton::clicked, this, &Planning::navigateToPlanning);
    connect(ui->employee_3, &QPushButton::clicked, this, &Planning::navigateToEmployee);
    connect(ui->vehicule_3, &QPushButton::clicked, this, &Planning::navigateToVehicule);
    connect(ui->examen_3, &QPushButton::clicked, this, &Planning::navigateToExamen);
    connect(ui->equipement_3, &QPushButton::clicked, this, &Planning::navigateToEquipement);

    ui->tab_3->setColumnCount(8);
    QStringList headers;
    headers << "ID" << "Date séance" << "condidat" << "type" << "debut" << "fin" << "circuit" << "Actions";
    ui->tab_3->setHorizontalHeaderLabels(headers);

    ui->tab_3->setShowGrid(true);
    ui->tab_3->setAlternatingRowColors(true);
    ui->tab_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tab_3->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tab_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tab_3->horizontalHeader()->setVisible(true);
    ui->tab_3->verticalHeader()->setVisible(true);
    ui->tab_3->horizontalHeader()->setStretchLastSection(true);
    ui->tab_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tab_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tab_3->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    loadMoniteurs();
    loadVehicules();
    loadCondidats();
    refreshTable();
}

Planning::~Planning()
{
    delete ui;
}

void Planning::on_btn_ajout_3_clicked()
{
    QDate date = ui->dateEdit->date();
    
    // Récupérer le CIN du condidat depuis le QComboBox (comme pour le véhicule)
    QString condidat = "";
    if (ui->cin_6 && ui->cin_6->currentIndex() > 0) {
        // Récupérer le CIN depuis les données du combo box
        QVariant cinData = ui->cin_6->currentData();
        if (cinData.isValid()) {
            condidat = QString::number(cinData.toInt());
        } else {
            // Si pas de data, essayer d'extraire le CIN du texte (format: "CIN - Nom Prénom")
            QString text = ui->cin_6->currentText();
            int spaceIndex = text.indexOf(" - ");
            if (spaceIndex > 0) {
                condidat = text.left(spaceIndex);
            } else {
                condidat = text;
            }
        }
    }
    
    QString type = ui->homme_3->isChecked() ? "conduit" : (ui->femme_3->isChecked() ? "code" : "");
    QTime debut = ui->timeEdit->time();
    QTime fin = ui->timeEdit_2->time();
    QString circuit = ui->tel_3->toPlainText().trimmed();
    QString moniteur = ui->cin_4->currentIndex() > 0 ? ui->cin_4->currentText() : "";
    QString vehicule = ui->cin_5->currentIndex() > 0 ? ui->cin_5->currentData().toString() : "";

    auto validateInputs = [&](int excludeId) -> bool {
        // Condidat
        if (condidat.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un condidat.");
            return false;
        }
        
        // Type
        if (type.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Type de séance requis (conduit/code).");
            return false;
        }

        // Circuit
        if (circuit.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Adresse du circuit requise.");
            return false;
        }
        if (circuit.size() < 3 || circuit.size() > 120) {
            QMessageBox::warning(this, "Erreur", "L'adresse du circuit doit contenir entre 3 et 120 caractères.");
            return false;
        }

        // Date
        if (!date.isValid()) {
            QMessageBox::warning(this, "Erreur", "Date de séance invalide.");
            return false;
        }
        if (date < QDate::currentDate()) {
            QMessageBox::warning(this, "Erreur", "La date de séance ne peut pas être dans le passé.");
            return false;
        }

        // Heures
        if (!debut.isValid() || !fin.isValid()) {
            QMessageBox::warning(this, "Erreur", "Heure de début ou de fin invalide.");
            return false;
        }
        if (fin <= debut) {
            QMessageBox::warning(this, "Erreur", "L'heure de fin doit être après l'heure de début.");
            return false;
        }
        QTime ouverture(8, 0);
        QTime fermeture(20, 0);
        if (debut < ouverture || fin > fermeture) {
            QMessageBox::warning(this, "Erreur", "Les séances doivent être entre 08:00 et 20:00.");
            return false;
        }
        int minutes = debut.secsTo(fin) / 60;
        if (minutes < 15) {
            QMessageBox::warning(this, "Erreur", "La durée minimale d'une séance est de 15 minutes.");
            return false;
        }
        if (minutes > 240) {
            QMessageBox::warning(this, "Erreur", "La durée maximale d'une séance est de 4 heures.");
            return false;
        }

        // Chevauchements pour le même candidat et la même date
        QSqlQuery q;
        // Convertir le CIN en nombre pour la comparaison
        bool cinOk;
        int cinValue = condidat.toInt(&cinOk);
        if (!cinOk) {
            QMessageBox::warning(this, "Erreur", "Le CIN du candidat doit être un nombre valide.");
            return false;
        }
        // Utiliser CIN_CANDIDAT (colonne NUMBER pour stocker le CIN)
        q.prepare("SELECT \"ID_SÉANCE\", \"HEURE_DEBUT\", \"HEURE_FIN\" FROM PLANNING WHERE \"DATE_SEANCE\" = :date AND CIN_CANDIDAT = :cin");
        q.bindValue(":date", date);
        q.bindValue(":cin", cinValue);
        if (!q.exec()) {
            QString errorMsg = q.lastError().text();
            // Vérifier si l'erreur est due à une colonne manquante
            if (errorMsg.contains("ORA-00904") && (errorMsg.contains("CIN_CANDIDAT") || errorMsg.contains("invalid identifier"))) {
                QMessageBox::critical(this, "Erreur de base de données", 
                    "La colonne CIN_CANDIDAT n'existe pas dans la table PLANNING.\n\n"
                    "Veuillez exécuter le script SQL 'add_cin_candidat_to_planning.sql' pour ajouter cette colonne.\n\n"
                    "Erreur: " + errorMsg);
            } else if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist")) {
                QMessageBox::critical(this, "Erreur de base de données", 
                    "La table PLANNING n'existe pas dans la base de données.\n\n"
                    "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
            } else {
                QMessageBox::critical(this, "Erreur SQL", 
                    "Erreur lors du contrôle de chevauchement:\n" + errorMsg);
            }
            return false;
        }
        QDateTime newStart(date, debut);
        QDateTime newEnd(date, fin);
        while (q.next()) {
            int existingId = q.value(0).toInt();
            if (excludeId != -1 && existingId == excludeId) continue;
            QDateTime existingStart = q.value(1).toDateTime();
            QTime existingEndTime = QTime::fromString(q.value(2).toString(), "HH:mm");
            if (!existingStart.isValid() || !existingEndTime.isValid()) continue;
            QDateTime existingEnd(existingStart.date(), existingEndTime);
            bool overlap = newStart < existingEnd && newEnd > existingStart;
            if (overlap) {
                QMessageBox::warning(this, "Conflit", "Ce condidat a déjà une séance qui chevauche ce créneau.");
                return false;
            }
        }
        return true;
    };

    int excludeId = -1;
    if (isEditMode) {
        excludeId = (currentEditingId != -1 ? currentEditingId : selectedIdFromTable());
        if (excludeId == -1) {
            QMessageBox::warning(this, "Modification", "Sélectionnez une ligne.");
            return;
        }
    }
    if (!validateInputs(excludeId)) {
        return;
    }

    // Validation moniteur et véhicule
    if (moniteur.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un moniteur.");
        return;
    }
    if (vehicule.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un véhicule.");
        return;
    }

    if (isEditMode) {
        int originalId = excludeId;
        if (modifier(originalId, date, condidat, type, debut, fin, circuit, moniteur, vehicule)) {
            QMessageBox::information(this, "Modification", "Planning modifié.");
            refreshTable();
            setEditMode(false);
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de modification.");
        }
        return;
    }

    this->date_seance = date;
    this->condidat = condidat;
    this->nom_moniteur = moniteur;
    this->vehicule = vehicule;
    this->type_seance = type;
    this->heure_debut = debut;
    this->heure_fin = fin;
    this->circuit = circuit;

    if (ajouter()) {
        QMessageBox::information(this, "Succès", "Planning ajouté avec succès!");
        refreshTable();
        on_btn_reset_3_clicked();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec d'ajout du planning.");
    }
}

void Planning::on_btn_reset_3_clicked()
{
    ui->cin_4->setCurrentIndex(0);
    ui->cin_5->setCurrentIndex(0);
    ui->tel_3->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->timeEdit->setTime(QTime(9, 0));
    ui->timeEdit_2->setTime(QTime(10, 0));
    ui->homme_3->setChecked(false);
    ui->femme_3->setChecked(false);
    
    // Réinitialiser le champ condidat (comme pour le véhicule)
    if (ui->cin_6) {
        ui->cin_6->setCurrentIndex(0);
    }
    
    setEditMode(false);
}

QSqlQueryModel* Planning::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    // Essayer d'inclure les colonnes moniteur et véhicule
    // Vérifier d'abord MATRICULE_VEHICULE (nom réel dans la base), puis VEHICULE
    QSqlQuery testQuery;
    testQuery.prepare("SELECT CIN_EMPLOYEE, MATRICULE_VEHICULE FROM PLANNING WHERE ROWNUM = 1");
    bool hasMoniteurVehicule = testQuery.exec();
    
    if (!hasMoniteurVehicule) {
        // Si MATRICULE_VEHICULE n'existe pas, essayer VEHICULE
        testQuery.prepare("SELECT CIN_EMPLOYEE, \"VEHICULE\" FROM PLANNING WHERE ROWNUM = 1");
        hasMoniteurVehicule = testQuery.exec();
    }
    
    if (!hasMoniteurVehicule) {
        QString errorMsg = testQuery.lastError().text();
        if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist")) {
            QMessageBox::critical(this, "Erreur de base de données", 
                "La table PLANNING n'existe pas dans la base de données.\n\n"
                "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
            return model; // Retourner un modèle vide
        }
    }
    
    // Déterminer le nom de la colonne véhicule (MATRICULE_VEHICULE ou VEHICULE)
    QString vehiculeColumn = "MATRICULE_VEHICULE";
    QSqlQuery testVehicule;
    testVehicule.prepare("SELECT MATRICULE_VEHICULE FROM PLANNING WHERE ROWNUM = 1");
    if (!testVehicule.exec()) {
        // Si MATRICULE_VEHICULE n'existe pas, utiliser VEHICULE
        vehiculeColumn = "\"VEHICULE\"";
    }
    
    // Afficher le nom complet du condidat avec une jointure sur CIN_CANDIDAT
    // CIN_CANDIDAT est un NUMBER, donc on fait la jointure directement
    QString condidatSelect = "NVL(c.nom || ' ' || c.prenom, TO_CHAR(p.CIN_CANDIDAT)) AS NOM_CANDIDAT";
    // Afficher le nom complet de l'employé avec une jointure sur CIN_EMPLOYEE
    QString employeSelect = "NVL(e.nom || ' ' || e.prenom, TO_CHAR(p.CIN_EMPLOYEE)) AS NOM_EMPLOYEE";
    QSqlQuery joinTest;
    // Tester si les jointures fonctionnent
    // Tester la jointure avec EMPLOYEE - utiliser CIN (colonne dans EMPLOYEE) ou CIN_EMPLOYEE
    joinTest.prepare("SELECT p.\"ID_SÉANCE\", c.nom || ' ' || c.prenom, e.nom || ' ' || e.prenom FROM PLANNING p LEFT JOIN CONDIDAT c ON p.CIN_CANDIDAT = c.cin_condidat LEFT JOIN EMPLOYEE e ON p.CIN_EMPLOYEE = e.CIN WHERE ROWNUM = 1");
    bool joinWorks = joinTest.exec();
    
    if (hasMoniteurVehicule) {
        if (joinWorks) {
            // Utiliser les jointures avec noms complets
            model->setQuery("SELECT p.\"ID_SÉANCE\", p.\"DATE_SEANCE\", " + condidatSelect + 
                          ", p.\"TYPE_SÉANCE\", p.\"HEURE_DEBUT\", p.\"HEURE_FIN\", p.\"ADRESSE_CIRCUIT\", "
                          + employeSelect + ", p." + vehiculeColumn + " AS VEHICULE "
                          "FROM PLANNING p LEFT JOIN CONDIDAT c ON p.CIN_CANDIDAT = c.cin_condidat LEFT JOIN EMPLOYEE e ON p.CIN_EMPLOYEE = e.CIN");
        } else {
            // Si la jointure échoue, afficher juste les CINs
            model->setQuery("SELECT \"ID_SÉANCE\", \"DATE_SEANCE\", TO_CHAR(CIN_CANDIDAT) AS NOM_CANDIDAT, \"TYPE_SÉANCE\", \"HEURE_DEBUT\", \"HEURE_FIN\", \"ADRESSE_CIRCUIT\", TO_CHAR(CIN_EMPLOYEE) AS NOM_EMPLOYEE, " + vehiculeColumn + " AS VEHICULE FROM PLANNING");
        }
    } else {
        if (joinWorks) {
            model->setQuery("SELECT p.\"ID_SÉANCE\", p.\"DATE_SEANCE\", " + condidatSelect + 
                          ", p.\"TYPE_SÉANCE\", p.\"HEURE_DEBUT\", p.\"HEURE_FIN\", p.\"ADRESSE_CIRCUIT\" "
                          "FROM PLANNING p LEFT JOIN CONDIDAT c ON p.CIN_CANDIDAT = c.cin_condidat");
        } else {
            model->setQuery("SELECT \"ID_SÉANCE\", \"DATE_SEANCE\", TO_CHAR(CIN_CANDIDAT) AS NOM_CANDIDAT, \"TYPE_SÉANCE\", \"HEURE_DEBUT\", \"HEURE_FIN\", \"ADRESSE_CIRCUIT\" FROM PLANNING");
        }
    }
    
    if (model->lastError().isValid()) {
        QString errorMsg = model->lastError().text();
        // Vérifier si l'erreur est due à une colonne manquante
        if (errorMsg.contains("ORA-00904") && (errorMsg.contains("CIN_CANDIDAT") || errorMsg.contains("invalid identifier"))) {
            QMessageBox::critical(this, "Erreur de base de données", 
                "La colonne CIN_CANDIDAT n'existe pas dans la table PLANNING.\n\n"
                "Veuillez exécuter le script SQL 'add_cin_candidat_to_planning.sql' pour ajouter cette colonne.\n\n"
                "Erreur: " + errorMsg);
        } else if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist")) {
            QMessageBox::critical(this, "Erreur de base de données", 
                "La table PLANNING n'existe pas dans la base de données.\n\n"
                "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
        }
    }
    
    return model;
}

bool Planning::ajouter()
{
    QSqlQuery query;

    int nextId = -1;
    

    query.prepare("SELECT SEQ_PLANNING.NEXTVAL FROM DUAL");
    if (query.exec() && query.next()) {
        nextId = query.value(0).toInt();
    } else {

        query.prepare("SELECT SEQ_PLANNING_ID.NEXTVAL FROM DUAL");
        if (query.exec() && query.next()) {
            nextId = query.value(0).toInt();
        } else {

            query.prepare("SELECT NVL(MAX(\"ID_SÉANCE\"), 0) + 1 FROM PLANNING");
            if (query.exec() && query.next()) {
                nextId = query.value(0).toInt();
            } else {
                QMessageBox::critical(this, "Erreur", "Impossible de générer l'ID: " + query.lastError().text());
                return false;
            }
        }
    }
    
    if (nextId <= 0) {
        QMessageBox::critical(this, "Erreur", "ID généré invalide.");
        return false;
    }
    

    QDateTime dateTimeDebut(date_seance, heure_debut);

    QString heureFinStr = heure_fin.toString("HH:mm");
    
    // Convertir le CIN en nombre
    bool cinOk;
    int cinValue = condidat.toInt(&cinOk);
    if (!cinOk) {
        QMessageBox::critical(this, "Erreur", "Le CIN du candidat doit être un nombre valide.");
        return false;
    }
    
    // Déterminer le nom de la colonne véhicule (MATRICULE_VEHICULE ou VEHICULE)
    QString vehiculeColumn = "MATRICULE_VEHICULE";
    QSqlQuery testVehiculeCol;
    testVehiculeCol.prepare("SELECT MATRICULE_VEHICULE FROM PLANNING WHERE ROWNUM = 1");
    if (!testVehiculeCol.exec()) {
        // Si MATRICULE_VEHICULE n'existe pas, utiliser VEHICULE
        vehiculeColumn = "\"VEHICULE\"";
    }
    
    // Convertir le CIN de l'employé en nombre
    bool employeCinOk;
    int employeCinValue = 0;
    if (!nom_moniteur.isEmpty()) {
        employeCinValue = nom_moniteur.toInt(&employeCinOk);
        if (!employeCinOk) {
            QMessageBox::critical(this, "Erreur", "Le CIN de l'employé doit être un nombre valide.");
            return false;
        }
    }
    
    // Essayer d'insérer avec les colonnes moniteur et véhicule
    // Utiliser CIN_CANDIDAT (NUMBER) et CIN_EMPLOYEE (NUMBER) pour stocker uniquement les CINs
    query.prepare("INSERT INTO PLANNING (\"ID_SÉANCE\", \"DATE_SEANCE\", CIN_CANDIDAT, \"TYPE_SÉANCE\", \"HEURE_DEBUT\", \"HEURE_FIN\", \"ADRESSE_CIRCUIT\", CIN_EMPLOYEE, " + vehiculeColumn + ") "
                  "VALUES (:id_seance, :date_seance, :cin_candidat, :type_seance, :heure_debut, :heure_fin, :adresse_circuit, :cin_employe, :vehicule)");
    query.bindValue(":id_seance", nextId);
    query.bindValue(":date_seance", date_seance);
    query.bindValue(":cin_candidat", cinValue);
    query.bindValue(":type_seance", type_seance);
    query.bindValue(":heure_debut", dateTimeDebut);
    query.bindValue(":heure_fin", heureFinStr);
    query.bindValue(":adresse_circuit", circuit);
    query.bindValue(":cin_employe", employeCinValue > 0 ? employeCinValue : QVariant(QVariant::Int));
    query.bindValue(":vehicule", vehicule);
    
    if (!query.exec()) {
        QString errorMsg = query.lastError().text();
        // Vérifier si l'erreur est due à une colonne manquante
        if (errorMsg.contains("ORA-00904") && (errorMsg.contains("CIN_CANDIDAT") || errorMsg.contains("invalid identifier"))) {
            QMessageBox::critical(this, "Erreur de base de données", 
                "La colonne CIN_CANDIDAT n'existe pas dans la table PLANNING.\n\n"
                "Veuillez exécuter le script SQL 'add_cin_candidat_to_planning.sql' pour ajouter cette colonne.\n\n"
                "Erreur: " + errorMsg);
            return false;
        }
        // Si l'erreur est due à une table manquante, ne pas essayer la deuxième requête
        if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist")) {
            QMessageBox::critical(this, "Erreur de base de données", 
                "La table PLANNING n'existe pas dans la base de données.\n\n"
                "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
            return false;
        }
        // Si l'insertion échoue (colonnes n'existent pas), essayer sans ces colonnes
        query.prepare("INSERT INTO PLANNING (\"ID_SÉANCE\", \"DATE_SEANCE\", CIN_CANDIDAT, \"TYPE_SÉANCE\", \"HEURE_DEBUT\", \"HEURE_FIN\", \"ADRESSE_CIRCUIT\") "
                      "VALUES (:id_seance, :date_seance, :cin_candidat, :type_seance, :heure_debut, :heure_fin, :adresse_circuit)");
        query.bindValue(":id_seance", nextId);
        query.bindValue(":date_seance", date_seance);
        query.bindValue(":cin_candidat", cinValue);
        query.bindValue(":type_seance", type_seance);
        query.bindValue(":heure_debut", dateTimeDebut);
        query.bindValue(":heure_fin", heureFinStr);
        query.bindValue(":adresse_circuit", circuit);
        
        if (!query.exec()) {
            errorMsg = query.lastError().text();
            if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist")) {
                QMessageBox::critical(this, "Erreur de base de données", 
                    "La table PLANNING n'existe pas dans la base de données.\n\n"
                    "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
            } else {
                QMessageBox::critical(this, "Erreur SQL", "Erreur lors de l'ajout:\n" + errorMsg);
            }
            return false;
        }
    }
    return true;
}

bool Planning::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM PLANNING WHERE \"ID_SÉANCE\" = :id");
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        QString errorMsg = query.lastError().text();
        if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist")) {
            QMessageBox::critical(this, "Erreur de base de données", 
                "La table PLANNING n'existe pas dans la base de données.\n\n"
                "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
        } else {
            QMessageBox::critical(this, "Erreur SQL", "Erreur lors de la suppression:\n" + errorMsg);
        }
        return false;
    }
    return true;
}

bool Planning::modifier(int id, QDate date_seance, QString condidat, QString type, QTime debut, QTime fin, QString circuit, QString moniteur, QString vehicule)
{
    QSqlQuery query;
    QDateTime dateTimeDebut(date_seance, debut);
    QString heureFinStr = fin.toString("HH:mm");
    
    // Convertir le CIN en nombre
    bool cinOk;
    int cinValue = condidat.toInt(&cinOk);
    if (!cinOk) {
        QMessageBox::critical(this, "Erreur", "Le CIN du candidat doit être un nombre valide.");
        return false;
    }
    
    // Déterminer le nom de la colonne véhicule (MATRICULE_VEHICULE ou VEHICULE)
    QString vehiculeColumn = "MATRICULE_VEHICULE";
    QSqlQuery testVehiculeCol;
    testVehiculeCol.prepare("SELECT MATRICULE_VEHICULE FROM PLANNING WHERE ROWNUM = 1");
    if (!testVehiculeCol.exec()) {
        // Si MATRICULE_VEHICULE n'existe pas, utiliser VEHICULE
        vehiculeColumn = "\"VEHICULE\"";
    }
    
    // Convertir le CIN de l'employé en nombre
    bool employeCinOk;
    int employeCinValue = 0;
    if (!moniteur.isEmpty()) {
        employeCinValue = moniteur.toInt(&employeCinOk);
        if (!employeCinOk) {
            QMessageBox::critical(this, "Erreur", "Le CIN de l'employé doit être un nombre valide.");
            return false;
        }
    }
    
    // Essayer de mettre à jour avec les colonnes moniteur et véhicule
    query.prepare("UPDATE PLANNING SET \"DATE_SEANCE\" = :date_seance, CIN_CANDIDAT = :cin_candidat, \"TYPE_SÉANCE\" = :type_seance, "
                  "\"HEURE_DEBUT\" = :debut, \"HEURE_FIN\" = :fin, \"ADRESSE_CIRCUIT\" = :adresse_circuit, "
                  "CIN_EMPLOYEE = :cin_employe, " + vehiculeColumn + " = :vehicule "
                  "WHERE \"ID_SÉANCE\" = :id");
    query.bindValue(":date_seance", date_seance);
    query.bindValue(":cin_candidat", cinValue);
    query.bindValue(":type_seance", type);
    query.bindValue(":debut", dateTimeDebut);
    query.bindValue(":fin", heureFinStr);
    query.bindValue(":adresse_circuit", circuit);
    query.bindValue(":cin_employe", employeCinValue > 0 ? employeCinValue : QVariant(QVariant::Int));
    query.bindValue(":vehicule", vehicule);
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        QString errorMsg = query.lastError().text();
        // Si la mise à jour échoue (colonnes n'existent pas), essayer sans ces colonnes
        if (!errorMsg.contains("ORA-00942") && !errorMsg.contains("table or view does not exist")) {
            query.prepare("UPDATE PLANNING SET \"DATE_SEANCE\" = :date_seance, CIN_CANDIDAT = :cin_candidat, \"TYPE_SÉANCE\" = :type_seance, "
                          "\"HEURE_DEBUT\" = :debut, \"HEURE_FIN\" = :fin, \"ADRESSE_CIRCUIT\" = :adresse_circuit "
                          "WHERE \"ID_SÉANCE\" = :id");
            query.bindValue(":date_seance", date_seance);
            query.bindValue(":cin_candidat", cinValue);
            query.bindValue(":type_seance", type);
            query.bindValue(":debut", dateTimeDebut);
            query.bindValue(":fin", heureFinStr);
            query.bindValue(":adresse_circuit", circuit);
            query.bindValue(":id", id);
            
            if (!query.exec()) {
                errorMsg = query.lastError().text();
                if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist")) {
                    QMessageBox::critical(this, "Erreur de base de données", 
                        "La table PLANNING n'existe pas dans la base de données.\n\n"
                        "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
                } else {
                    QMessageBox::critical(this, "Erreur SQL", "Erreur lors de la modification:\n" + errorMsg);
                }
                return false;
            }
        } else {
            QMessageBox::critical(this, "Erreur de base de données", 
                "La table PLANNING n'existe pas dans la base de données.\n\n"
                "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
            return false;
        }
    }
    return true;
}

void Planning::refreshTable()
{
    std::unique_ptr<QSqlQueryModel> model(afficher());
    int rows = model->rowCount();
    int cols = model->columnCount();
    ui->tab_3->setRowCount(rows);
    ui->tab_3->setColumnCount(cols + 1);
    QStringList headers;
    if (cols > 7) {
        headers << "ID" << "Date séance" << "condidat" << "type" << "debut" << "fin" << "circuit" << "moniteur" << "véhicule" << "Actions";
    } else {
        headers << "ID" << "Date séance" << "condidat" << "type" << "debut" << "fin" << "circuit" << "Actions";
    }
    ui->tab_3->setHorizontalHeaderLabels(headers);
    
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QVariant v = model->data(model->index(r, c));
            if (c == 0) {
                ui->tab_3->setItem(r, c, new QTableWidgetItem(v.toString()));
            } else if (c == 1) {
                QDate date = v.toDate();
                if (date.isValid()) {
                    ui->tab_3->setItem(r, c, new QTableWidgetItem(date.toString("dd/MM/yyyy")));
                } else {
                    ui->tab_3->setItem(r, c, new QTableWidgetItem(v.toString()));
                }
            } else if (c == 4) {
                QDateTime datetime = v.toDateTime();
                if (datetime.isValid()) {
                    ui->tab_3->setItem(r, c, new QTableWidgetItem(datetime.time().toString("HH:mm")));
                } else {
                    ui->tab_3->setItem(r, c, new QTableWidgetItem(v.toString()));
                }
            } else if (c == 5) {
                ui->tab_3->setItem(r, c, new QTableWidgetItem(v.toString()));
            } else if (c == 7 && cols > 7) {
                // Colonne moniteur
                ui->tab_3->setItem(r, c, new QTableWidgetItem(v.toString()));
            } else if (c == 8 && cols > 8) {
                // Colonne véhicule
                ui->tab_3->setItem(r, c, new QTableWidgetItem(v.toString()));
            } else {
                ui->tab_3->setItem(r, c, new QTableWidgetItem(v.toString()));
            }
        }
        bool ok = false;
        int id = model->data(model->index(r, 0)).toInt(&ok);
        ui->tab_3->setCellWidget(r, cols, createActionsCell(r, ok ? id : -1));
    }
}

int Planning::selectedIdFromTable() const
{
    QList<QTableWidgetItem*> items = ui->tab_3->selectedItems();
    if (items.isEmpty()) return -1;
    int row = items.first()->row();
    QTableWidgetItem *idItem = ui->tab_3->item(row, 0);
    if (!idItem) return -1;
    QString idText = idItem->text().trimmed();
    // N'accepter que des chiffres
    QRegularExpression digitsOnly("^\\d+$");
    if (!digitsOnly.match(idText).hasMatch()) return -1;
    bool ok = false;
    int id = idText.toInt(&ok);
    return ok ? id : -1;
}

void Planning::setFormFromRow(int row)
{
    if (row < 0 || row >= ui->tab_3->rowCount()) return;
    auto getText = [&](int col){ QTableWidgetItem *it = ui->tab_3->item(row, col); return it ? it->text() : QString(); };
    
    QDate date = QDate::fromString(getText(1), "dd/MM/yyyy");
    if (date.isValid()) ui->dateEdit->setDate(date);
    
    // Charger le CIN du condidat depuis la base de données (comme pour le véhicule)
    QString condidatText = getText(2); // Colonne "condidat" dans le tableau
    if (ui->cin_6 && !condidatText.isEmpty()) {
        // Essayer de trouver le CIN dans le texte (format peut être "CIN" ou "CIN - Nom Prénom")
        QString cinStr = condidatText;
        int spaceIndex = condidatText.indexOf(" - ");
        if (spaceIndex > 0) {
            cinStr = condidatText.left(spaceIndex);
        }
        
        // Essayer de trouver par CIN dans les données (comme pour le véhicule)
        bool found = false;
        for (int i = 0; i < ui->cin_6->count(); ++i) {
            QVariant data = ui->cin_6->itemData(i);
            if (data.isValid() && QString::number(data.toInt()) == cinStr) {
                ui->cin_6->setCurrentIndex(i);
                found = true;
                break;
            }
        }
        
        // Si pas trouvé par data, essayer par texte
        if (!found) {
            int index = ui->cin_6->findText(cinStr, Qt::MatchStartsWith);
            if (index >= 0) {
                ui->cin_6->setCurrentIndex(index);
            }
        }
    }
    
    QString typeStr = getText(3).trimmed().toLower();
    ui->homme_3->setChecked(typeStr == "conduit");
    ui->femme_3->setChecked(typeStr == "code");
    
    QTime debut = QTime::fromString(getText(4), "HH:mm");
    if (debut.isValid()) ui->timeEdit->setTime(debut);
    
    QTime fin = QTime::fromString(getText(5), "HH:mm");
    if (fin.isValid()) ui->timeEdit_2->setTime(fin);
    
    ui->tel_3->setText(getText(6));
    
    // Charger le CIN de l'employé depuis la base de données si disponible
    if (ui->tab_3->columnCount() > 7) {
        QString employeText = getText(7); // Colonne "employé" dans le tableau
        if (ui->cin_4 && !employeText.isEmpty()) {
            // Essayer de trouver le CIN dans le texte (format peut être "CIN" ou "CIN - Nom Prénom")
            QString cinStr = employeText;
            int spaceIndex = employeText.indexOf(" - ");
            if (spaceIndex > 0) {
                cinStr = employeText.left(spaceIndex);
            }
            
            // Essayer de trouver par CIN dans les données (comme pour le condidat)
            bool found = false;
            for (int i = 0; i < ui->cin_4->count(); ++i) {
                QVariant data = ui->cin_4->itemData(i);
                if (data.isValid() && QString::number(data.toInt()) == cinStr) {
                    ui->cin_4->setCurrentIndex(i);
                    found = true;
                    break;
                }
            }
            
            // Si pas trouvé par data, essayer par texte
            if (!found) {
                int index = ui->cin_4->findText(cinStr, Qt::MatchStartsWith);
                if (index >= 0) {
                    ui->cin_4->setCurrentIndex(index);
                }
            }
        }
    }
    
    // Charger le véhicule depuis la base de données si disponible
    if (ui->tab_3->columnCount() > 8) {
        QString vehicule = getText(8);
        if (!vehicule.isEmpty()) {
            int index = ui->cin_5->findData(vehicule);
            if (index >= 0) {
                ui->cin_5->setCurrentIndex(index);
            } else {
                // Essayer de trouver par texte
                index = ui->cin_5->findText(vehicule);
                if (index >= 0) {
                    ui->cin_5->setCurrentIndex(index);
                }
            }
        }
    }
}

QWidget* Planning::createActionsCell(int row, int id)
{
    QWidget *container = new QWidget(ui->tab_3);
    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(6);
    QPushButton *btnEdit = new QPushButton(container);
    QPushButton *btnDel = new QPushButton(container);
    btnEdit->setText("");
    btnDel->setText("");
    btnDel->setIcon(QIcon::fromTheme("edit-delete", style()->standardIcon(QStyle::SP_TrashIcon)));
    btnEdit->setIcon(QIcon::fromTheme("mail-message-new", style()->standardIcon(QStyle::SP_FileDialogNewFolder)));

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
        if (id == -1) return;
        if (QMessageBox::question(this, "Suppression", "Supprimer ce planning ?") == QMessageBox::Yes) {
            if (supprimer(id)) {
                refreshTable();
            } else {
                QMessageBox::critical(this, "Erreur", "Échec de suppression.");
            }
        }
    });

    QObject::connect(btnEdit, &QPushButton::clicked, this, [=](){
        if (isEditMode && currentEditingId == id) {
            setEditMode(false);
            return;
        }
        setFormFromRow(row);
        currentEditingId = id;
        setEditMode(true);
    });

    return container;
}

void Planning::setEditMode(bool enabled)
{
    isEditMode = enabled;
    if (enabled) {
        ui->btn_ajout_3->setText("Modifier");
    } else {
        ui->btn_ajout_3->setText("Ajouter");
        currentEditingId = -1;
    }
}

void Planning::on_supp_clicked()
{
    int id = selectedIdFromTable();
    if (id == -1) {
        QMessageBox::warning(this, "Suppression", "Sélectionnez une ligne.");
        return;
    }
    if (QMessageBox::question(this, "Suppression", "Supprimer ce planning ?") == QMessageBox::Yes) {
        if (supprimer(id)) {
            QMessageBox::information(this, "Suppression", "Planning supprimé.");
            refreshTable();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de suppression.");
        }
    }
}

void Planning::on_edit_clicked()
{
    int originalId = currentEditingId != -1 ? currentEditingId : selectedIdFromTable();
    if (originalId == -1) {
        QMessageBox::warning(this, "Modification", "Sélectionnez une ligne.");
        return;
    }
    
    QDate date = ui->dateEdit->date();
    
    // Récupérer le CIN du condidat depuis le QComboBox (comme pour le véhicule)
    QString condidat = "";
    if (ui->cin_6 && ui->cin_6->currentIndex() > 0) {
        // Récupérer le CIN depuis les données du combo box
        QVariant cinData = ui->cin_6->currentData();
        if (cinData.isValid()) {
            condidat = QString::number(cinData.toInt());
        } else {
            // Si pas de data, essayer d'extraire le CIN du texte (format: "CIN - Nom Prénom")
            QString text = ui->cin_6->currentText();
            int spaceIndex = text.indexOf(" - ");
            if (spaceIndex > 0) {
                condidat = text.left(spaceIndex);
            } else {
                condidat = text;
            }
        }
    }
    
    QString type = ui->homme_3->isChecked() ? "conduit" : (ui->femme_3->isChecked() ? "code" : "");
    QTime debut = ui->timeEdit->time();
    QTime fin = ui->timeEdit_2->time();
    QString circuit = ui->tel_3->toPlainText().trimmed();
    
    // Réutiliser la même validation que pour l'ajout
    auto validateInputs = [&](int excludeId) -> bool {
        if (condidat.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un candidat.");
            return false;
        }
        
        if (type.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Type de séance requis (conduit/code).");
            return false;
        }
        if (circuit.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Adresse du circuit requise.");
            return false;
        }
        if (circuit.size() < 3 || circuit.size() > 120) {
            QMessageBox::warning(this, "Erreur", "L'adresse du circuit doit contenir entre 3 et 120 caractères.");
            return false;
        }
        if (!date.isValid()) {
            QMessageBox::warning(this, "Erreur", "Date de séance invalide.");
            return false;
        }
        if (date < QDate::currentDate()) {
            QMessageBox::warning(this, "Erreur", "La date de séance ne peut pas être dans le passé.");
            return false;
        }
        if (!debut.isValid() || !fin.isValid()) {
            QMessageBox::warning(this, "Erreur", "Heure de début ou de fin invalide.");
            return false;
        }
        if (fin <= debut) {
            QMessageBox::warning(this, "Erreur", "L'heure de fin doit être après l'heure de début.");
            return false;
        }
        QTime ouverture(8, 0);
        QTime fermeture(20, 0);
        if (debut < ouverture || fin > fermeture) {
            QMessageBox::warning(this, "Erreur", "Les séances doivent être entre 08:00 et 20:00.");
            return false;
        }
        int minutes = debut.secsTo(fin) / 60;
        if (minutes < 15) {
            QMessageBox::warning(this, "Erreur", "La durée minimale d'une séance est de 15 minutes.");
            return false;
        }
        if (minutes > 240) {
            QMessageBox::warning(this, "Erreur", "La durée maximale d'une séance est de 4 heures.");
            return false;
        }
        QSqlQuery q;
        // Convertir le CIN en nombre pour la comparaison
        bool cinOk;
        int cinValue = condidat.toInt(&cinOk);
        if (!cinOk) {
            return false; // Le CIN n'est pas valide
        }
        // Utiliser CIN_CANDIDAT (colonne NUMBER pour stocker le CIN)
        q.prepare("SELECT \"ID_SÉANCE\", \"HEURE_DEBUT\", \"HEURE_FIN\" FROM PLANNING WHERE \"DATE_SEANCE\" = :date AND CIN_CANDIDAT = :cin");
        q.bindValue(":date", date);
        q.bindValue(":cin", cinValue);
        if (!q.exec()) {
            QString errorMsg = q.lastError().text();
            // Vérifier si l'erreur est due à une colonne manquante
            if (errorMsg.contains("ORA-00904") && (errorMsg.contains("CIN_CANDIDAT") || errorMsg.contains("invalid identifier"))) {
                QMessageBox::critical(this, "Erreur de base de données", 
                    "La colonne CIN_CANDIDAT n'existe pas dans la table PLANNING.\n\n"
                    "Veuillez exécuter le script SQL 'add_cin_candidat_to_planning.sql' pour ajouter cette colonne.\n\n"
                    "Erreur: " + errorMsg);
            } else if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist")) {
                QMessageBox::critical(this, "Erreur de base de données", 
                    "La table PLANNING n'existe pas dans la base de données.\n\n"
                    "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
            } else {
                QMessageBox::critical(this, "Erreur SQL", 
                    "Erreur lors du contrôle de chevauchement:\n" + errorMsg);
            }
            return false;
        }
        QDateTime newStart(date, debut);
        QDateTime newEnd(date, fin);
        while (q.next()) {
            int existingId = q.value(0).toInt();
            if (excludeId != -1 && existingId == excludeId) continue;
            QDateTime existingStart = q.value(1).toDateTime();
            QTime existingEndTime = QTime::fromString(q.value(2).toString(), "HH:mm");
            if (!existingStart.isValid() || !existingEndTime.isValid()) continue;
            QDateTime existingEnd(existingStart.date(), existingEndTime);
            bool overlap = newStart < existingEnd && newEnd > existingStart;
            if (overlap) {
                QMessageBox::warning(this, "Conflit", "Ce condidat a déjà une séance qui chevauche ce créneau.");
                return false;
            }
        }
        return true;
    };
    
    if (!validateInputs(originalId)) {
        return;
    }
    
    QString moniteur = ui->cin_4->currentIndex() > 0 ? ui->cin_4->currentText() : "";
    QString vehicule = ui->cin_5->currentIndex() > 0 ? ui->cin_5->currentData().toString() : "";
    
    if (moniteur.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un moniteur.");
        return;
    }
    if (vehicule.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un véhicule.");
        return;
    }
    
    if (modifier(originalId, date, condidat, type, debut, fin, circuit, moniteur, vehicule)) {
        QMessageBox::information(this, "Modification", "Planning modifié.");
        refreshTable();
        currentEditingId = -1;
        setEditMode(false);
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de modification.");
    }
}

void Planning::navigateToCandidat()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(Navigation::PAGE_CANDIDAT);
    }
}

void Planning::navigateToPlanning()
{
}

void Planning::navigateToEmployee()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(Navigation::PAGE_EMPLOYEE);
    }
}

void Planning::navigateToVehicule()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(Navigation::PAGE_VEHICULE);
    }
}

void Planning::navigateToExamen()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(Navigation::PAGE_EXAMEN);
    }
}

void Planning::navigateToEquipement()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(Navigation::PAGE_EQUIPEMENT);
    }
}

void Planning::loadMoniteurs()
{
    ui->cin_4->clear();
    ui->cin_4->addItem("Choisir un employé", QVariant());
    
    QSqlQuery query;
    // Charger CIN, NOM et PRENOM depuis la table EMPLOYEE
    // La colonne dans EMPLOYEE s'appelle CIN (pas CIN_EMPLOYEE)
    query.prepare("SELECT CIN, NOM, PRENOM FROM EMPLOYEE WHERE CIN IS NOT NULL ORDER BY NOM, PRENOM");
    if (!query.exec()) {
        QMessageBox::warning(this, "Avertissement", "Impossible de charger les employés: " + query.lastError().text());
        return;
    }
    
    while (query.next()) {
        int cinEmploye = query.value(0).toInt();
        QString nom = query.value(1).toString();
        QString prenom = query.value(2).toString();
        QString nomComplet = nom;
        if (!prenom.isEmpty() && query.record().count() > 2) {
            nomComplet = nom + " " + prenom;
        }
        // Afficher "CIN - Nom Prénom" et stocker le CIN dans les données
        QString displayText = QString::number(cinEmploye) + " - " + nomComplet;
        ui->cin_4->addItem(displayText, cinEmploye);
    }
}

void Planning::loadVehicules()
{
    ui->cin_5->clear();
    ui->cin_5->addItem("Choisir un véhicule", QVariant());
    
    QSqlQuery query;
    // Essayer différentes possibilités de noms de table et colonnes
    query.prepare("SELECT DISTINCT MATRICULE, MODELE FROM VEHICULE ORDER BY MATRICULE");
    if (!query.exec()) {
        // Essayer avec un autre nom de table
        query.prepare("SELECT DISTINCT MATRICULE, MODELE FROM VEHICULES ORDER BY MATRICULE");
        if (!query.exec()) {
            // Essayer avec juste MATRICULE
            query.prepare("SELECT DISTINCT MATRICULE FROM VEHICULE ORDER BY MATRICULE");
            if (!query.exec()) {
                query.prepare("SELECT DISTINCT MATRICULE FROM VEHICULES ORDER BY MATRICULE");
                if (!query.exec()) {
                    QMessageBox::warning(this, "Avertissement", "Impossible de charger les véhicules: " + query.lastError().text());
                    return;
                }
            }
        }
    }
    
    while (query.next()) {
        QString matricule = query.value(0).toString();
        QString modele = query.value(1).toString();
        QString vehiculeInfo = matricule;
        if (!modele.isEmpty() && query.record().count() > 1) {
            vehiculeInfo = matricule + " - " + modele;
        }
        ui->cin_5->addItem(vehiculeInfo, matricule);
    }
}

QComboBox* Planning::getCondidatCombo()
{
    // Utiliser directement ui->cin_6 comme pour ui->cin_4 et ui->cin_5
    // Si cin_6 n'existe pas, essayer de trouver le champ par d'autres noms
    if (ui->cin_6) {
        return ui->cin_6;
    }
    
    // Fallback: chercher le champ QComboBox pour le CIN du condidat
    QComboBox *condidatCombo = nullptr;
    
    // Essayer de trouver le champ par différents noms possibles
    condidatCombo = findChild<QComboBox*>("cin_condidat");
    if (!condidatCombo) {
        condidatCombo = findChild<QComboBox*>("condidat_cin");
    }
    if (!condidatCombo) {
        condidatCombo = findChild<QComboBox*>("cin_condidat_3");
    }
    if (!condidatCombo) {
        condidatCombo = findChild<QComboBox*>("cin_6");
    }
    if (!condidatCombo) {
        // Si aucun champ n'est trouvé, essayer de trouver tous les QComboBox et utiliser celui qui n'est pas cin_4 ou cin_5
        QList<QComboBox*> combos = findChildren<QComboBox*>();
        for (QComboBox *combo : combos) {
            QString name = combo->objectName();
            if (name != "cin_4" && name != "cin_5" && (name.contains("condidat", Qt::CaseInsensitive) || name.contains("candidat", Qt::CaseInsensitive) || name == "cin_6")) {
                condidatCombo = combo;
                break;
            }
        }
    }
    
    return condidatCombo;
}

void Planning::loadCondidats()
{
    // Utiliser directement ui->cin_6 comme pour ui->cin_4 et ui->cin_5
    if (!ui->cin_6) {
        qDebug() << "Avertissement: ui->cin_6 n'existe pas. Vérifiez que le champ est bien nommé 'cin_6' dans planning.ui";
        return;
    }
    
    ui->cin_6->clear();
    ui->cin_6->addItem("Choisir un condidat", QVariant());
    
    QSqlQuery query;
    // Charger les CINs avec les noms et prénoms pour affichage
    query.prepare("SELECT cin_condidat, nom, prenom FROM CONDIDAT ORDER BY cin_condidat");
    if (!query.exec()) {
        QString errorMsg = query.lastError().text();
        if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist")) {
            QMessageBox::warning(this, "Avertissement", 
                "La table CONDIDAT n'existe pas dans la base de données.\n\n"
                "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
        } else {
            QMessageBox::warning(this, "Avertissement", "Impossible de charger les condidats: " + errorMsg);
        }
        return;
    }
    
    while (query.next()) {
        int cin = query.value(0).toInt();
        QString nom = query.value(1).toString();
        QString prenom = query.value(2).toString();
        QString displayText = QString::number(cin);
        
        // Ajouter le nom et prénom si disponibles pour un affichage plus lisible
        if (!nom.isEmpty() && !prenom.isEmpty()) {
            displayText = QString::number(cin) + " - " + nom + " " + prenom;
        } else if (!nom.isEmpty()) {
            displayText = QString::number(cin) + " - " + nom;
        }
        
        // Stocker le CIN comme data pour faciliter la récupération
        ui->cin_6->addItem(displayText, cin);
    }
}
