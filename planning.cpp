#include "planning.h"
#include "ui_planning.h"
#include "navigation_constants.h"
#include "openstreetmap_service.h"
#include "local_calendar.h"
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
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextCharFormat>
#include <QCalendarWidget>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>
#include <QTextTableFormat>
#include <QPageSize>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

Planning::Planning(QWidget *parent) : QWidget(parent),
                                      ui(new Ui::Planning),
                                      mapDisplayLabel(nullptr),
                                      pendingPlanningId(-1),
                                      currentLatitude(0.0),
                                      currentLongitude(0.0)
{
    ui->setupUi(this);

    qDebug() << "========================================";
    qDebug() << "PLANNING PAGE CONSTRUCTOR STARTING";
    qDebug() << "Main widget size:" << this->size();
    qDebug() << "========================================";

    // Initialize Google Calendar service
    calendarService = new GoogleCalendarService(this);
    
    // IMPORTANT: Configure your Google Calendar API key below
    // 1. Go to: https://console.cloud.google.com/
    // 2. Create a new project or select existing one
    // 3. Enable Google Calendar API
    // 4. Create credentials -> API Key
    // 5. Replace "YOUR_API_KEY_HERE" with your actual key
    calendarService->setApiKey("YOUR_API_KEY_HERE");
    calendarService->setCalendarId("primary"); // Default calendar
    
    // Initialize OpenStreetMap service (100% FREE - No API key needed!)
    mapService = new OpenStreetMapService(this);
    // No API key required for OpenStreetMap! 🎉

    // Initialize LocalCalendar service (100% FREE - No API key needed!)
    localCalendar = new LocalCalendar(this);
    
    // Connect calendar widget signals
    if (ui->calendarWidget) {
        // When user clicks a date, show events for that date
        connect(ui->calendarWidget, &QCalendarWidget::clicked, this, &Planning::onCalendarDateClicked);
        
        // Set calendar to current date
        ui->calendarWidget->setSelectedDate(QDate::currentDate());
        
        // Enable grid lines
        ui->calendarWidget->setGridVisible(true);
        
        // Set first day of week to Monday
        ui->calendarWidget->setFirstDayOfWeek(Qt::Monday);
        
        qDebug() << "Calendar widget initialized successfully";
    }

    // Connect calendar signals
    connect(calendarService, &GoogleCalendarService::eventCreated, this, [this](const QString &eventId, const QString &summary) {
        qDebug() << "Calendar event created with ID:" << eventId << "for:" << summary;
        // Store the event ID mapping with the pending planning ID
        if (pendingPlanningId > 0 && !eventId.isEmpty()) {
            eventIdMap[pendingPlanningId] = eventId;
            qDebug() << "Mapped planning ID" << pendingPlanningId << "to Calendar event ID" << eventId;
            pendingPlanningId = -1; // Reset
        }
    });
    connect(calendarService, &GoogleCalendarService::eventUpdated, this, []() {
        qDebug() << "Calendar event updated successfully";
    });
    connect(calendarService, &GoogleCalendarService::eventDeleted, this, []() {
        qDebug() << "Calendar event deleted successfully";
    });
    connect(calendarService, &GoogleCalendarService::errorOccurred, this, [](const QString &error) {
        qWarning() << "Calendar API error:" << error;
    });
    
    // Connect OpenStreetMap signals
    connect(mapService, &OpenStreetMapService::geocodeCompleted, this, [this](const QString &address, double lat, double lon, const QString &displayName) {
        qDebug() << "Geocoded:" << address << "to" << lat << "," << lon;
        onGeocodeCompleted(lat, lon, displayName);
    });
    connect(mapService, &OpenStreetMapService::staticMapReady, this, &Planning::onStaticMapReady);
    connect(mapService, &OpenStreetMapService::errorOccurred, this, &Planning::onMapsError);
    
    // Connect map button (will be created after setupUi)
    if (ui->btn_search_location) {
        connect(ui->btn_search_location, &QPushButton::clicked, this, &Planning::onSearchLocationClicked);
    }
    
    // Connect circuit combo box to enable/disable custom input
    if (ui->circuit_combo) {
        connect(ui->circuit_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
            // Enable custom input only when "Autre" is selected (last item)
            bool isCustom = (index == ui->circuit_combo->count() - 1);
            if (ui->custom_circuit_input) {
                ui->custom_circuit_input->setEnabled(isCustom);
                ui->custom_circuit_input->clear();
                if (!isCustom) {
                    ui->custom_circuit_input->setStyleSheet("background-color:white;border-radius:5px;border:2px solid #cccccc;color:black;padding: 5px;");
                } else {
                    ui->custom_circuit_input->setStyleSheet("background-color:white;border-radius:5px;border:2px solid #ffce00;color:black;padding: 5px;");
                }
            }
        });
    }
    
    // Initialize map display labels with visibility and styling
    if (ui->map_info_label) {
        ui->map_info_label->setVisible(true);
        ui->map_info_label->raise();
        ui->map_info_label->setText("📍 Sélectionnez un circuit et cliquez sur 'Voir carte' pour afficher la carte");
        ui->map_info_label->setStyleSheet("background-color:#4CAF50;color:white;border-radius:5px;padding:8px;font-size:11px;font-weight:bold;");
        qDebug() << "map_info_label initialized at position:" << ui->map_info_label->pos() << "size:" << ui->map_info_label->size();
        qDebug() << "map_info_label isVisible:" << ui->map_info_label->isVisible();
    } else {
        qDebug() << "ERROR: map_info_label not found in UI!";
    }
    
    // Set the map display label reference
    if (ui->map_display_label) {
        mapDisplayLabel = ui->map_display_label;
        mapDisplayLabel->setVisible(true);
        mapDisplayLabel->raise();
        mapDisplayLabel->setText("🗺️ La carte s'affichera ici");
        mapDisplayLabel->setStyleSheet("background-color:#f5f5f5;border:2px solid #4CAF50;border-radius:8px;padding:10px;color:#666;font-size:14px;");
        qDebug() << "map_display_label initialized at position:" << mapDisplayLabel->pos() << "size:" << mapDisplayLabel->size();
        qDebug() << "map_display_label isVisible:" << mapDisplayLabel->isVisible();
        
        // Force update
        mapDisplayLabel->update();
        mapDisplayLabel->repaint();
    } else {
        qDebug() << "ERROR: map_display_label not found in UI!";
    }
    
    qDebug() << "========================================";

    connect(ui->candidat_3, &QPushButton::clicked, this, &Planning::navigateToCandidat);
    connect(ui->planning_3, &QPushButton::clicked, this, &Planning::navigateToPlanning);
    connect(ui->employee_3, &QPushButton::clicked, this, &Planning::navigateToEmployee);
    connect(ui->vehicule_3, &QPushButton::clicked, this, &Planning::navigateToVehicule);
    connect(ui->examen_3, &QPushButton::clicked, this, &Planning::navigateToExamen);
    connect(ui->equipement_3, &QPushButton::clicked, this, &Planning::navigateToEquipement);

    // Connect search and sort functionality
    connect(ui->recherche_3, &QTextEdit::textChanged, this, &Planning::on_recherche_3_textChanged);
    connect(ui->tri_3, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Planning::on_tri_3_currentIndexChanged);
    // Note: pushButton_8 uses Qt's auto-connect, no manual connection needed
    
    // Initialize search/sort state
    currentSearchText = "";
    currentSortIndex = 0;

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
    QString nomCandidat = "";
    if (ui->cin_6 && ui->cin_6->currentIndex() > 0)
    {
        // Récupérer le CIN depuis les données du combo box
        QVariant cinData = ui->cin_6->currentData();
        if (cinData.isValid())
        {
            condidat = cinData.toString();
        }
        else
        {
            // Si pas de data, essayer d'extraire le CIN du texte (format: "CIN - Nom Prénom")
            QString text = ui->cin_6->currentText();
            int spaceIndex = text.indexOf(" - ");
            if (spaceIndex > 0)
            {
                condidat = text.left(spaceIndex);
            }
            else
            {
                condidat = text;
            }
        }

        // Extraire le nom du candidat depuis le texte affiché (format: "CIN - Nom Prénom")
        QString text = ui->cin_6->currentText();
        int spaceIndex = text.indexOf(" - ");
        if (spaceIndex > 0)
        {
            nomCandidat = text.mid(spaceIndex + 3).trimmed(); // +3 pour sauter " - "
        }
    }

    QString type = ui->homme_3->isChecked() ? "conduit" : (ui->femme_3->isChecked() ? "code" : "");
    QTime debut = ui->timeEdit->time();
    QTime fin = ui->timeEdit_2->time();
    
    // Get circuit address from new UI
    QString circuit;
    if (ui->circuit_combo && ui->circuit_combo->currentIndex() > 0) {
        if (ui->circuit_combo->currentIndex() == ui->circuit_combo->count() - 1) {
            // Custom address
            circuit = ui->custom_circuit_input ? ui->custom_circuit_input->text().trimmed() : "";
        } else {
            // Predefined circuit - store the full name
            circuit = ui->circuit_combo->currentText();
        }
    }
    
    QString moniteur = ui->cin_4->currentIndex() > 0 ? ui->cin_4->currentText() : "";
    QString vehicule = ui->cin_5->currentIndex() > 0 ? ui->cin_5->currentData().toString() : "";

    auto validateInputs = [&](int excludeId) -> bool
    {
        // Condidat
        if (condidat.isEmpty())
        {
            QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un condidat.");
            return false;
        }

        // Type
        if (type.isEmpty())
        {
            QMessageBox::warning(this, "Erreur", "Type de séance requis (conduit/code).");
            return false;
        }

        // Circuit
        if (circuit.isEmpty())
        {
            QMessageBox::warning(this, "Erreur", "Adresse du circuit requise.");
            return false;
        }
        if (circuit.size() < 3 || circuit.size() > 120)
        {
            QMessageBox::warning(this, "Erreur", "L'adresse du circuit doit contenir entre 3 et 120 caractères.");
            return false;
        }

        // Date
        if (!date.isValid())
        {
            QMessageBox::warning(this, "Erreur", "Date de séance invalide.");
            return false;
        }
        if (date < QDate::currentDate())
        {
            QMessageBox::warning(this, "Erreur", "La date de séance ne peut pas être dans le passé.");
            return false;
        }

        // Heures
        if (!debut.isValid() || !fin.isValid())
        {
            QMessageBox::warning(this, "Erreur", "Heure de début ou de fin invalide.");
            return false;
        }
        if (fin <= debut)
        {
            QMessageBox::warning(this, "Erreur", "L'heure de fin doit être après l'heure de début.");
            return false;
        }
        QTime ouverture(8, 0);
        QTime fermeture(20, 0);
        if (debut < ouverture || fin > fermeture)
        {
            QMessageBox::warning(this, "Erreur", "Les séances doivent être entre 08:00 et 20:00.");
            return false;
        }
        int minutes = debut.secsTo(fin) / 60;
        if (minutes < 15)
        {
            QMessageBox::warning(this, "Erreur", "La durée minimale d'une séance est de 15 minutes.");
            return false;
        }
        if (minutes > 240)
        {
            QMessageBox::warning(this, "Erreur", "La durée maximale d'une séance est de 4 heures.");
            return false;
        }

        // Chevauchements pour le même candidat et la même date
        QSqlQuery q;
        // CIN_CONDIDAT est VARCHAR2, utiliser directement la chaîne
        // Convertir la date pour la comparaison
        QString dateStr = date.toString("yyyy-MM-dd");
        q.prepare("SELECT ID_SEANCE, HEURE_DEBUT, HEURE_FIN FROM PLANNING WHERE DATE_SEANCE = TO_DATE(:date, 'YYYY-MM-DD') AND CIN_CONDIDAT = :cin");
        q.bindValue(":date", dateStr);
        q.bindValue(":cin", condidat);
        if (!q.exec())
        {
            QString errorMsg = q.lastError().text();
            // Vérifier si l'erreur est due à une colonne manquante
            if (errorMsg.contains("ORA-00904") && (errorMsg.contains("CIN_CONDIDAT") || errorMsg.contains("invalid identifier")))
            {
                QMessageBox::critical(this, "Erreur de base de données",
                                      "La colonne CIN_CONDIDAT n'existe pas dans la table PLANNING.\n\n"
                                      "Veuillez exécuter le script SQL 'add_CIN_CONDIDAT_to_planning.sql' pour ajouter cette colonne.\n\n"
                                      "Erreur: " +
                                          errorMsg);
            }
            else if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist"))
            {
                QMessageBox::critical(this, "Erreur de base de données",
                                      "La table PLANNING n'existe pas dans la base de données.\n\n"
                                      "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
            }
            else
            {
                QMessageBox::critical(this, "Erreur SQL",
                                      "Erreur lors du contrôle de chevauchement:\n" + errorMsg);
            }
            return false;
        }
        QDateTime newStart(date, debut);
        QDateTime newEnd(date, fin);
        while (q.next())
        {
            int existingId = q.value(0).toInt();
            if (excludeId != -1 && existingId == excludeId)
                continue;
            QDateTime existingStart = q.value(1).toDateTime();
            QTime existingEndTime = QTime::fromString(q.value(2).toString(), "HH:mm");
            if (!existingStart.isValid() || !existingEndTime.isValid())
                continue;
            QDateTime existingEnd(existingStart.date(), existingEndTime);
            bool overlap = newStart < existingEnd && newEnd > existingStart;
            if (overlap)
            {
                QMessageBox::warning(this, "Conflit", "Ce condidat a déjà une séance qui chevauche ce créneau.");
                return false;
            }
        }
        return true;
    };

    int excludeId = -1;
    if (isEditMode)
    {
        excludeId = (currentEditingId != -1 ? currentEditingId : selectedIdFromTable());
        if (excludeId == -1)
        {
            QMessageBox::warning(this, "Modification", "Sélectionnez une ligne.");
            return;
        }
    }
    if (!validateInputs(excludeId))
    {
        return;
    }

    // Validation moniteur et véhicule
    if (moniteur.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un moniteur.");
        return;
    }
    if (vehicule.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un véhicule.");
        return;
    }

    if (isEditMode)
    {
        int originalId = excludeId;
        if (modifier(originalId, date, condidat, type, debut, fin, circuit, moniteur, vehicule))
        {
            QMessageBox::information(this, "Modification", "Planning modifié.");
            refreshTable();
            setEditMode(false);
        }
        else
        {
            QMessageBox::critical(this, "Erreur", "Échec de modification.");
        }
        return;
    }

    this->date_seance = date;
    this->condidat = condidat;
    this->nom_candidat = nomCandidat;
    this->nom_moniteur = moniteur;
    this->vehicule = vehicule;
    this->type_seance = type;
    this->heure_debut = debut;
    this->heure_fin = fin;
    this->circuit = circuit;

    if (ajouter())
    {
        QMessageBox::information(this, "Succès", "Planning ajouté avec succès!");
        refreshTable();
        on_btn_reset_3_clicked();
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Échec d'ajout du planning.");
    }
}

void Planning::on_btn_reset_3_clicked()
{
    ui->cin_4->setCurrentIndex(0);
    ui->cin_5->setCurrentIndex(0);
    ui->dateEdit->setDate(QDate::currentDate());
    ui->timeEdit->setTime(QTime(9, 0));
    ui->timeEdit_2->setTime(QTime(10, 0));
    ui->homme_3->setChecked(false);
    ui->femme_3->setChecked(false);

    // Réinitialiser le champ condidat
    if (ui->cin_6)
    {
        ui->cin_6->setCurrentIndex(0);
    }
    
    // Réinitialiser les nouveaux champs de circuit
    if (ui->circuit_combo) {
        ui->circuit_combo->setCurrentIndex(0);
    }
    if (ui->custom_circuit_input) {
        ui->custom_circuit_input->clear();
        ui->custom_circuit_input->setEnabled(false);
    }
    
    // Clear map display
    if (mapDisplayLabel) {
        mapDisplayLabel->clear();
        mapDisplayLabel->setText("🗺️ La carte du circuit s'affichera ici après sélection");
    }
    
    // Reset coordinates
    currentLatitude = 0.0;
    currentLongitude = 0.0;
    currentFormattedAddress.clear();

    setEditMode(false);
}

QSqlQueryModel *Planning::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    // Select both display values and actual IDs for preselection during edit
    model->setQuery("SELECT P.ID_SEANCE, "
                    "TO_CHAR(P.DATE_SEANCE, 'DD/MM/YYYY') AS DATE_SEANCE, "
                    "P.NOM_CANDIDAT, "
                    "P.TYPE_SEANCE, "
                    "TO_CHAR(P.HEURE_DEBUT, 'HH24:MI') AS DEBUT, "
                    "TO_CHAR(P.HEURE_FIN, 'HH24:MI') AS FIN, "
                    "P.ADRESSE_CIRCUIT, "
                    "P.MATRICULE, "
                    "P.CIN_CONDIDAT, "
                    "P.CIN_EMPLOYEE "
                    "FROM PLANNING P "
                    "ORDER BY P.DATE_SEANCE DESC, P.HEURE_DEBUT DESC");

    if (model->lastError().isValid())
    {
        QString errorMsg = model->lastError().text();
        // Check if error is due to missing table
        if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist"))
        {
            QMessageBox::critical(this, "Erreur de base de données",
                                  "La table PLANNING n'existe pas dans la base de données.\n\n"
                                  "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
            return model;
        }
        // Check if error is due to missing CIN columns
        if (errorMsg.contains("ORA-00904") && (errorMsg.contains("CIN_CONDIDAT") || errorMsg.contains("CIN_EMPLOYEE") || errorMsg.contains("invalid identifier")))
        {
            QMessageBox::critical(this, "Erreur de base de données",
                                  "La colonne CIN_CONDIDAT n'existe pas dans la table PLANNING.\n\n"
                                  "Veuillez exécuter le script SQL 'add_CIN_CONDIDAT_to_planning.sql' pour ajouter cette colonne.\n\n"
                                  "Erreur: " +
                                      errorMsg);
        }
        else if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist"))
        {
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
    if (query.exec() && query.next())
    {
        nextId = query.value(0).toInt();
    }
    else
    {

        query.prepare("SELECT SEQ_PLANNING_ID.NEXTVAL FROM DUAL");
        if (query.exec() && query.next())
        {
            nextId = query.value(0).toInt();
        }
        else
        {

            query.prepare("SELECT NVL(MAX(ID_SEANCE), 0) + 1 FROM PLANNING");
            if (query.exec() && query.next())
            {
                nextId = query.value(0).toInt();
            }
            else
            {
                QMessageBox::critical(this, "Erreur", "Impossible de générer l'ID: " + query.lastError().text());
                return false;
            }
        }
    }

    if (nextId <= 0)
    {
        QMessageBox::critical(this, "Erreur", "ID généré invalide.");
        return false;
    }

    QDateTime dateTimeDebut(date_seance, heure_debut);
    QDateTime dateTimeFin(date_seance, heure_fin);

    // Extract CIN from "CIN - Name" format (take text before the dash)
    QString employeCin = "";
    if (!nom_moniteur.isEmpty())
    {
        if (nom_moniteur.contains(" - "))
        {
            employeCin = nom_moniteur.split(" - ").first().trimmed();
        }
        else
        {
            employeCin = nom_moniteur.trimmed();
        }
    }

    // Utiliser CIN_CONDIDAT (VARCHAR2) et CIN_EMPLOYEE (VARCHAR2) pour stocker les CINs
    // Utiliser MATRICULE pour stocker le matricule du véhicule
    // DATE_SEANCE est maintenant de type DATE
    // Convertir QDate en string format ISO (yyyy-MM-dd) pour Oracle
    QString dateStr = date_seance.toString("yyyy-MM-dd");

    query.prepare("INSERT INTO PLANNING (ID_SEANCE, DATE_SEANCE, CIN_CONDIDAT, NOM_CANDIDAT, TYPE_SEANCE, HEURE_DEBUT, HEURE_FIN, ADRESSE_CIRCUIT, CIN_EMPLOYEE, MATRICULE) "
                  "VALUES (:id_seance, TO_DATE(:date_seance, 'YYYY-MM-DD'), :cin_condidat, :nom_candidat, :type_seance, :heure_debut, :heure_fin, :adresse_circuit, :cin_employe, :vehicule)");
    query.bindValue(":id_seance", nextId);
    query.bindValue(":date_seance", dateStr);
    qDebug() << "Binding date_seance:" << dateStr;
    query.bindValue(":cin_condidat", condidat);
    qDebug() << "Binding cin_condidat:" << condidat;
    query.bindValue(":nom_candidat", nom_candidat);
    qDebug() << "Binding nom_candidat:" << nom_candidat;
    query.bindValue(":type_seance", type_seance);
    query.bindValue(":heure_debut", dateTimeDebut);
    query.bindValue(":heure_fin", dateTimeFin);
    query.bindValue(":adresse_circuit", circuit);
    query.bindValue(":cin_employe", !employeCin.isEmpty() ? employeCin : QVariant());
    query.bindValue(":vehicule", vehicule);

    qDebug() << "Executing query:" << query.lastQuery();
    if (!query.exec())
    {
        qDebug() << "Query failed:" << query.lastError().text();
        QString errorMsg = query.lastError().text();
        // Vérifier si l'erreur est due à une colonne manquante
        if (errorMsg.contains("ORA-00904") && (errorMsg.contains("CIN_CONDIDAT") || errorMsg.contains("invalid identifier")))
        {
            QMessageBox::critical(this, "Erreur de base de données",
                                  "La colonne CIN_CONDIDAT n'existe pas dans la table PLANNING.\n\n"
                                  "Veuillez exécuter le script SQL 'add_CIN_CONDIDAT_to_planning.sql' pour ajouter cette colonne.\n\n"
                                  "Erreur: " +
                                      errorMsg);
            return false;
        }
        // Si l'erreur est due à une table manquante, ne pas essayer la deuxième requête
        if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist"))
        {
            QMessageBox::critical(this, "Erreur de base de données",
                                  "La table PLANNING n'existe pas dans la base de données.\n\n"
                                  "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
            return false;
        }
        // Si l'insertion échoue (colonnes n'existent pas), essayer sans ces colonnes
        query.prepare("INSERT INTO PLANNING (ID_SEANCE, DATE_SEANCE, CIN_CONDIDAT, TYPE_SEANCE, HEURE_DEBUT, HEURE_FIN, ADRESSE_CIRCUIT) "
                      "VALUES (:id_seance, TO_DATE(:date_seance, 'YYYY-MM-DD'), :cin_condidat, :type_seance, :heure_debut, :heure_fin, :adresse_circuit)");
        query.bindValue(":id_seance", nextId);
        query.bindValue(":date_seance", dateStr);
        query.bindValue(":cin_condidat", condidat);
        query.bindValue(":type_seance", type_seance);
        query.bindValue(":heure_debut", dateTimeDebut);
        query.bindValue(":heure_fin", dateTimeFin);
        query.bindValue(":adresse_circuit", circuit);

        if (!query.exec())
        {
            errorMsg = query.lastError().text();
            if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist"))
            {
                QMessageBox::critical(this, "Erreur de base de données",
                                      "La table PLANNING n'existe pas dans la base de données.\n\n"
                                      "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
            }
            else
            {
                QMessageBox::critical(this, "Erreur SQL", "Erreur lors de l'ajout:\n" + errorMsg);
            }
            return false;
        }
    }

    // TODO: Reactivate when OAuth2 is implemented (API keys don't support write operations)
    // Create Google Calendar event after successful database insert
    /*
    QString summary = QString("Séance de conduite - %1").arg(nom_candidat);
    QString description = QString("Type: %1\nMoniteur: %2\nVéhicule: %3")
                              .arg(type_seance)
                              .arg(nom_moniteur)
                              .arg(vehicule);
    QDateTime startDateTime(date_seance, heure_debut);
    QDateTime endDateTime(date_seance, heure_fin);

    // Store the planning ID so we can map it when the Calendar event is created (async)
    pendingPlanningId = nextId;
    qDebug() << "Creating Calendar event for planning ID:" << nextId;
    calendarService->createEvent(summary, description, startDateTime, endDateTime, circuit, "");
    */

    return true;
}

bool Planning::supprimer(int id)
{
    // TODO: Reactivate when OAuth2 is implemented
    /*
    // Delete from Google Calendar first if event ID exists
    if (eventIdMap.contains(id)) {
        QString eventId = eventIdMap[id];
        calendarService->deleteEvent(eventId);
        eventIdMap.remove(id);
    }
    */

    QSqlQuery query;
    query.prepare("DELETE FROM PLANNING WHERE ID_SEANCE = :id");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        QString errorMsg = query.lastError().text();
        if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist"))
        {
            QMessageBox::critical(this, "Erreur de base de données",
                                  "La table PLANNING n'existe pas dans la base de données.\n\n"
                                  "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
        }
        else
        {
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
    QDateTime dateTimeFin(date_seance, fin);

    // Extract CIN from "CIN - Name" format (take text before the dash)
    QString employeCin = "";
    QString nomCandidat = "";
    if (!moniteur.isEmpty())
    {
        if (moniteur.contains(" - "))
        {
            employeCin = moniteur.split(" - ").first().trimmed();
        }
        else
        {
            employeCin = moniteur.trimmed();
        }
    }

    // Get candidate name for calendar event
    QSqlQuery nameQuery;
    nameQuery.prepare("SELECT NOM, PRENOM FROM CONDIDAT WHERE CIN_CONDIDAT = :cin");
    nameQuery.bindValue(":cin", condidat);
    if (nameQuery.exec() && nameQuery.next()) {
        QString nom = nameQuery.value(0).toString();
        QString prenom = nameQuery.value(1).toString();
        nomCandidat = prenom + " " + nom;
    }

    // Utiliser MATRICULE pour stocker le matricule du véhicule
    // DATE_SEANCE est maintenant de type DATE
    // Convertir QDate en string format ISO pour Oracle
    QString dateStr = date_seance.toString("yyyy-MM-dd");

    query.prepare("UPDATE PLANNING SET DATE_SEANCE = TO_DATE(:date_seance, 'YYYY-MM-DD'), CIN_CONDIDAT = :CIN_CONDIDAT, TYPE_SEANCE = :type_seance, "
                  "HEURE_DEBUT = :debut, HEURE_FIN = :fin, ADRESSE_CIRCUIT = :adresse_circuit, "
                  "CIN_EMPLOYEE = :cin_employe, MATRICULE = :vehicule "
                  "WHERE ID_SEANCE = :id");
    query.bindValue(":date_seance", dateStr);
    query.bindValue(":CIN_CONDIDAT", condidat);
    query.bindValue(":type_seance", type);
    query.bindValue(":debut", dateTimeDebut);
    query.bindValue(":fin", dateTimeFin);
    query.bindValue(":adresse_circuit", circuit);
    query.bindValue(":cin_employe", !employeCin.isEmpty() ? employeCin : QVariant());
    query.bindValue(":vehicule", vehicule);
    query.bindValue(":id", id);

    if (!query.exec())
    {
        QString errorMsg = query.lastError().text();
        // Si la mise à jour échoue (colonnes n'existent pas), essayer sans ces colonnes
        if (!errorMsg.contains("ORA-00942") && !errorMsg.contains("table or view does not exist"))
        {
            query.prepare("UPDATE PLANNING SET DATE_SEANCE = TO_DATE(:date_seance, 'YYYY-MM-DD'), CIN_CONDIDAT = :CIN_CONDIDAT, TYPE_SEANCE = :type_seance, "
                          "HEURE_DEBUT = :debut, HEURE_FIN = :fin, ADRESSE_CIRCUIT = :adresse_circuit "
                          "WHERE ID_SEANCE = :id");
            query.bindValue(":date_seance", dateStr);
            query.bindValue(":CIN_CONDIDAT", condidat);
            query.bindValue(":type_seance", type);
            query.bindValue(":debut", dateTimeDebut);
            query.bindValue(":fin", dateTimeFin);
            query.bindValue(":adresse_circuit", circuit);
            query.bindValue(":id", id);

            if (!query.exec())
            {
                errorMsg = query.lastError().text();
                if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist"))
                {
                    QMessageBox::critical(this, "Erreur de base de données",
                                          "La table PLANNING n'existe pas dans la base de données.\n\n"
                                          "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
                }
                else
                {
                    QMessageBox::critical(this, "Erreur SQL", "Erreur lors de la modification:\n" + errorMsg);
                }
                return false;
            }
        }
        else
        {
            QMessageBox::critical(this, "Erreur de base de données",
                                  "La table PLANNING n'existe pas dans la base de données.\n\n"
                                  "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
            return false;
        }
    }

    // TODO: Reactivate when OAuth2 is implemented
    /*
    // Update Google Calendar event if it exists
    if (eventIdMap.contains(id)) {
        QString eventId = eventIdMap[id];
        QString summary = QString("Séance de conduite - %1").arg(nomCandidat);
        QString description = QString("Type: %1\nMoniteur: %2\nVéhicule: %3")
                                  .arg(type)
                                  .arg(moniteur)
                                  .arg(vehicule);
        QDateTime startDateTime(date_seance, debut);
        QDateTime endDateTime(date_seance, fin);

        calendarService->updateEvent(eventId, summary, description, circuit, startDateTime, endDateTime);
    }
    */

    return true;
}

void Planning::refreshTable()
{
    std::unique_ptr<QSqlQueryModel> model(afficher());
    int rows = model->rowCount();
    int cols = model->columnCount(); // This will be 10 (includes hidden CIN columns)

    // Set table to have visible columns + Actions column
    // Columns: ID, Date, Nom Candidat, Type, Debut, Fin, Circuit, Matricule, CIN_CONDIDAT(hidden), CIN_EMPLOYEE(hidden), Actions
    ui->tab_3->setRowCount(rows);
    ui->tab_3->setColumnCount(cols + 1); // +1 for Actions

    QStringList headers;
    headers << "ID" << "Date séance" << "condidat" << "type" << "debut" << "fin" << "circuit" << "véhicule" << "CIN_CAND" << "CIN_EMP" << "Actions";
    ui->tab_3->setHorizontalHeaderLabels(headers);

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            QVariant v = model->data(model->index(r, c));
            if (c == 0)
            {
                ui->tab_3->setItem(r, c, new QTableWidgetItem(v.toString()));
            }
            else if (c == 1)
            {
                QDate date = v.toDate();
                if (date.isValid())
                {
                    ui->tab_3->setItem(r, c, new QTableWidgetItem(date.toString("dd/MM/yyyy")));
                }
                else
                {
                    ui->tab_3->setItem(r, c, new QTableWidgetItem(v.toString()));
                }
            }
            else if (c == 4)
            {
                QDateTime datetime = v.toDateTime();
                if (datetime.isValid())
                {
                    ui->tab_3->setItem(r, c, new QTableWidgetItem(datetime.time().toString("HH:mm")));
                }
                else
                {
                    ui->tab_3->setItem(r, c, new QTableWidgetItem(v.toString()));
                }
            }
            else
            {
                ui->tab_3->setItem(r, c, new QTableWidgetItem(v.toString()));
            }
        }
        bool ok = false;
        int id = model->data(model->index(r, 0)).toInt(&ok);
        ui->tab_3->setCellWidget(r, cols, createActionsCell(r, ok ? id : -1));
    }

    // Hide the CIN columns (they're only for preselection during edit)
    ui->tab_3->setColumnHidden(8, true); // CIN_CONDIDAT
    ui->tab_3->setColumnHidden(9, true); // CIN_EMPLOYEE
    
    // Refresh calendar view after table refresh
    refreshCalendarView();
}

int Planning::selectedIdFromTable() const
{
    QList<QTableWidgetItem *> items = ui->tab_3->selectedItems();
    if (items.isEmpty())
        return -1;
    int row = items.first()->row();
    QTableWidgetItem *idItem = ui->tab_3->item(row, 0);
    if (!idItem)
        return -1;
    QString idText = idItem->text().trimmed();
    // N'accepter que des chiffres
    QRegularExpression digitsOnly("^\\d+$");
    if (!digitsOnly.match(idText).hasMatch())
        return -1;
    bool ok = false;
    int id = idText.toInt(&ok);
    return ok ? id : -1;
}

void Planning::setFormFromRow(int row)
{
    if (row < 0 || row >= ui->tab_3->rowCount())
        return;
    auto getText = [&](int col)
    { QTableWidgetItem *it = ui->tab_3->item(row, col); return it ? it->text() : QString(); };

    QDate date = QDate::fromString(getText(1), "dd/MM/yyyy");
    if (date.isValid())
        ui->dateEdit->setDate(date);

    // Get actual CIN_CONDIDAT from column 8 (hidden column)
    QString cinCondidat = getText(8);
    if (ui->cin_6 && !cinCondidat.isEmpty())
    {
        // Find by CIN value stored in combo data
        bool found = false;
        for (int i = 0; i < ui->cin_6->count(); ++i)
        {
            QVariant data = ui->cin_6->itemData(i);
            if (data.isValid() && data.toString() == cinCondidat)
            {
                ui->cin_6->setCurrentIndex(i);
                found = true;
                break;
            }
        }

        // If not found by data, try by text prefix
        if (!found)
        {
            int index = ui->cin_6->findText(cinCondidat, Qt::MatchStartsWith);
            if (index >= 0)
            {
                ui->cin_6->setCurrentIndex(index);
            }
        }
    }

    QString typeStr = getText(3).trimmed().toLower();
    ui->homme_3->setChecked(typeStr == "conduit");
    ui->femme_3->setChecked(typeStr == "code");

    QTime debut = QTime::fromString(getText(4), "HH:mm");
    if (debut.isValid())
        ui->timeEdit->setTime(debut);

    QTime fin = QTime::fromString(getText(5), "HH:mm");
    if (fin.isValid())
        ui->timeEdit_2->setTime(fin);

    // Set circuit from column 6
    QString circuitText = getText(6);
    if (ui->circuit_combo && !circuitText.isEmpty()) {
        // Try to find in predefined circuits
        int index = ui->circuit_combo->findText(circuitText);
        if (index >= 0) {
            ui->circuit_combo->setCurrentIndex(index);
        } else {
            // Set as custom circuit
            ui->circuit_combo->setCurrentIndex(ui->circuit_combo->count() - 1); // "Autre"
            if (ui->custom_circuit_input) {
                ui->custom_circuit_input->setText(circuitText);
            }
        }
    }

    // Get actual MATRICULE from column 7
    QString vehicule = getText(7);
    if (ui->cin_5 && !vehicule.isEmpty())
    {
        int index = ui->cin_5->findData(vehicule);
        if (index >= 0)
        {
            ui->cin_5->setCurrentIndex(index);
        }
        else
        {
            // Try to find by text
            index = ui->cin_5->findText(vehicule, Qt::MatchStartsWith);
            if (index >= 0)
            {
                ui->cin_5->setCurrentIndex(index);
            }
        }
    }

    // Get actual CIN_EMPLOYEE from column 9 (hidden column)
    QString cinEmployee = getText(9);
    if (ui->cin_4 && !cinEmployee.isEmpty())
    {
        // Find by CIN in combo text (format: "CIN - Name")
        bool found = false;
        for (int i = 0; i < ui->cin_4->count(); ++i)
        {
            QString itemText = ui->cin_4->itemText(i);
            if (itemText.startsWith(cinEmployee + " - ") || itemText == cinEmployee)
            {
                ui->cin_4->setCurrentIndex(i);
                found = true;
                break;
            }
        }

        // If not found, try by text prefix
        if (!found)
        {
            int index = ui->cin_4->findText(cinEmployee, Qt::MatchStartsWith);
            if (index >= 0)
            {
                ui->cin_4->setCurrentIndex(index);
            }
        }
    }
}

QWidget *Planning::createActionsCell(int row, int id)
{
    QWidget *container = new QWidget(ui->tab_3);
    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
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
    btnEdit->setIconSize(QSize(18, 18));
    btnDel->setIconSize(QSize(18, 18));
    layout->addWidget(btnEdit);
    layout->addWidget(btnDel);
    container->setLayout(layout);

    // Delete action
    QObject::connect(btnDel, &QPushButton::clicked, this, [=]()
                     {
        if (id == -1) return;
        if (QMessageBox::question(this, "Suppression", "Supprimer ce planning ?") == QMessageBox::Yes) {
            if (supprimer(id)) {
                refreshTable();
            } else {
                QMessageBox::critical(this, "Erreur", "Échec de suppression.");
            }
        } });

    QObject::connect(btnEdit, &QPushButton::clicked, this, [=]()
                     {
        if (isEditMode && currentEditingId == id) {
            setEditMode(false);
            return;
        }
        setFormFromRow(row);
        currentEditingId = id;
        setEditMode(true); });

    return container;
}

void Planning::setEditMode(bool enabled)
{
    isEditMode = enabled;
    if (enabled)
    {
        ui->btn_ajout_3->setText("Modifier");
    }
    else
    {
        ui->btn_ajout_3->setText("Ajouter");
        currentEditingId = -1;
    }
}

void Planning::on_supp_clicked()
{
    int id = selectedIdFromTable();
    if (id == -1)
    {
        QMessageBox::warning(this, "Suppression", "Sélectionnez une ligne.");
        return;
    }
    if (QMessageBox::question(this, "Suppression", "Supprimer ce planning ?") == QMessageBox::Yes)
    {
        if (supprimer(id))
        {
            QMessageBox::information(this, "Suppression", "Planning supprimé.");
            refreshTable();
        }
        else
        {
            QMessageBox::critical(this, "Erreur", "Échec de suppression.");
        }
    }
}

void Planning::on_edit_clicked()
{
    int originalId = currentEditingId != -1 ? currentEditingId : selectedIdFromTable();
    if (originalId == -1)
    {
        QMessageBox::warning(this, "Modification", "Sélectionnez une ligne.");
        return;
    }

    QDate date = ui->dateEdit->date();

    // Récupérer le CIN du condidat depuis le QComboBox (comme pour le véhicule)
    QString condidat = "";
    if (ui->cin_6 && ui->cin_6->currentIndex() > 0)
    {
        // Récupérer le CIN depuis les données du combo box
        QVariant cinData = ui->cin_6->currentData();
        if (cinData.isValid())
        {
            condidat = QString::number(cinData.toInt());
        }
        else
        {
            // Si pas de data, essayer d'extraire le CIN du texte (format: "CIN - Nom Prénom")
            QString text = ui->cin_6->currentText();
            int spaceIndex = text.indexOf(" - ");
            if (spaceIndex > 0)
            {
                condidat = text.left(spaceIndex);
            }
            else
            {
                condidat = text;
            }
        }
    }

    QString type = ui->homme_3->isChecked() ? "conduit" : (ui->femme_3->isChecked() ? "code" : "");
    QTime debut = ui->timeEdit->time();
    QTime fin = ui->timeEdit_2->time();
    
    // Get circuit from new UI
    QString circuit;
    if (ui->circuit_combo && ui->circuit_combo->currentIndex() > 0) {
        if (ui->circuit_combo->currentIndex() == ui->circuit_combo->count() - 1) {
            // Custom address
            circuit = ui->custom_circuit_input ? ui->custom_circuit_input->text().trimmed() : "";
        } else {
            // Predefined circuit
            circuit = ui->circuit_combo->currentText();
        }
    }

    // Réutiliser la même validation que pour l'ajout
    auto validateInputs = [&](int excludeId) -> bool
    {
        if (condidat.isEmpty())
        {
            QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un candidat.");
            return false;
        }

        if (type.isEmpty())
        {
            QMessageBox::warning(this, "Erreur", "Type de séance requis (conduit/code).");
            return false;
        }
        if (circuit.isEmpty())
        {
            QMessageBox::warning(this, "Erreur", "Adresse du circuit requise.");
            return false;
        }
        if (circuit.size() < 3 || circuit.size() > 120)
        {
            QMessageBox::warning(this, "Erreur", "L'adresse du circuit doit contenir entre 3 et 120 caractères.");
            return false;
        }
        if (!date.isValid())
        {
            QMessageBox::warning(this, "Erreur", "Date de séance invalide.");
            return false;
        }
        if (date < QDate::currentDate())
        {
            QMessageBox::warning(this, "Erreur", "La date de séance ne peut pas être dans le passé.");
            return false;
        }
        if (!debut.isValid() || !fin.isValid())
        {
            QMessageBox::warning(this, "Erreur", "Heure de début ou de fin invalide.");
            return false;
        }
        if (fin <= debut)
        {
            QMessageBox::warning(this, "Erreur", "L'heure de fin doit être après l'heure de début.");
            return false;
        }
        QTime ouverture(8, 0);
        QTime fermeture(20, 0);
        if (debut < ouverture || fin > fermeture)
        {
            QMessageBox::warning(this, "Erreur", "Les séances doivent être entre 08:00 et 20:00.");
            return false;
        }
        int minutes = debut.secsTo(fin) / 60;
        if (minutes < 15)
        {
            QMessageBox::warning(this, "Erreur", "La durée minimale d'une séance est de 15 minutes.");
            return false;
        }
        if (minutes > 240)
        {
            QMessageBox::warning(this, "Erreur", "La durée maximale d'une séance est de 4 heures.");
            return false;
        }
        QSqlQuery q;
        // CIN_CONDIDAT est VARCHAR2, pas besoin de convertir
        // Utiliser CIN_CONDIDAT (colonne VARCHAR2 pour stocker le CIN)
        QString dateStr = date.toString("yyyy-MM-dd");
        q.prepare("SELECT ID_SEANCE, HEURE_DEBUT, HEURE_FIN FROM PLANNING WHERE DATE_SEANCE = TO_DATE(:date, 'YYYY-MM-DD') AND CIN_CONDIDAT = :cin");
        q.bindValue(":date", dateStr);
        q.bindValue(":cin", condidat);
        if (!q.exec())
        {
            QString errorMsg = q.lastError().text();
            // Vérifier si l'erreur est due à une colonne manquante
            if (errorMsg.contains("ORA-00904") && (errorMsg.contains("CIN_CONDIDAT") || errorMsg.contains("invalid identifier")))
            {
                QMessageBox::critical(this, "Erreur de base de données",
                                      "La colonne CIN_CONDIDAT n'existe pas dans la table PLANNING.\n\n"
                                      "Veuillez exécuter le script SQL 'add_CIN_CONDIDAT_to_planning.sql' pour ajouter cette colonne.\n\n"
                                      "Erreur: " +
                                          errorMsg);
            }
            else if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist"))
            {
                QMessageBox::critical(this, "Erreur de base de données",
                                      "La table PLANNING n'existe pas dans la base de données.\n\n"
                                      "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
            }
            else
            {
                QMessageBox::critical(this, "Erreur SQL",
                                      "Erreur lors du contrôle de chevauchement:\n" + errorMsg);
            }
            return false;
        }
        QDateTime newStart(date, debut);
        QDateTime newEnd(date, fin);
        while (q.next())
        {
            int existingId = q.value(0).toInt();
            if (excludeId != -1 && existingId == excludeId)
                continue;
            QDateTime existingStart = q.value(1).toDateTime();
            QTime existingEndTime = QTime::fromString(q.value(2).toString(), "HH:mm");
            if (!existingStart.isValid() || !existingEndTime.isValid())
                continue;
            QDateTime existingEnd(existingStart.date(), existingEndTime);
            bool overlap = newStart < existingEnd && newEnd > existingStart;
            if (overlap)
            {
                QMessageBox::warning(this, "Conflit", "Ce condidat a déjà une séance qui chevauche ce créneau.");
                return false;
            }
        }
        return true;
    };

    if (!validateInputs(originalId))
    {
        return;
    }

    QString moniteur = ui->cin_4->currentIndex() > 0 ? ui->cin_4->currentText() : "";
    QString vehicule = ui->cin_5->currentIndex() > 0 ? ui->cin_5->currentData().toString() : "";

    if (moniteur.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un moniteur.");
        return;
    }
    if (vehicule.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un véhicule.");
        return;
    }

    if (modifier(originalId, date, condidat, type, debut, fin, circuit, moniteur, vehicule))
    {
        QMessageBox::information(this, "Modification", "Planning modifié.");
        refreshTable();
        currentEditingId = -1;
        setEditMode(false);
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Échec de modification.");
    }
}

void Planning::navigateToCandidat()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_CANDIDAT);
    }
}

void Planning::navigateToPlanning()
{
}

void Planning::navigateToEmployee()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_EMPLOYES);
    }
}

void Planning::navigateToVehicule()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_VEHICULE);
    }
}

void Planning::navigateToExamen()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_EXAMEN);
    }
}

void Planning::navigateToEquipement()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_EQUIPEMENT);
    }
}

void Planning::loadMoniteurs()
{
    ui->cin_4->clear();
    ui->cin_4->addItem("Choisir un employé", QVariant());

    QSqlQuery query;
    // Charger CIN_EMPLOYEE, NOM et PRENOM depuis la table EMPLOYEE
    query.prepare("SELECT CIN_EMPLOYEE, NOM, PRENOM FROM EMPLOYEE WHERE CIN_EMPLOYEE IS NOT NULL ORDER BY NOM, PRENOM");
    if (!query.exec())
    {
        QMessageBox::warning(this, "Avertissement", "Impossible de charger les employés: " + query.lastError().text());
        return;
    }

    while (query.next())
    {
        QString cinEmploye = query.value(0).toString();
        QString nom = query.value(1).toString();
        QString prenom = query.value(2).toString();
        QString nomComplet = nom;
        if (!prenom.isEmpty() && query.record().count() > 2)
        {
            nomComplet = nom + " " + prenom;
        }
        // Afficher "CIN - Nom Prénom" et stocker le CIN dans les données
        QString displayText = cinEmploye + " - " + nomComplet;
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
    if (!query.exec())
    {
        // Essayer avec un autre nom de table
        query.prepare("SELECT DISTINCT MATRICULE, MODELE FROM VEHICULES ORDER BY MATRICULE");
        if (!query.exec())
        {
            // Essayer avec juste MATRICULE
            query.prepare("SELECT DISTINCT MATRICULE FROM VEHICULE ORDER BY MATRICULE");
            if (!query.exec())
            {
                query.prepare("SELECT DISTINCT MATRICULE FROM VEHICULES ORDER BY MATRICULE");
                if (!query.exec())
                {
                    QMessageBox::warning(this, "Avertissement", "Impossible de charger les véhicules: " + query.lastError().text());
                    return;
                }
            }
        }
    }

    while (query.next())
    {
        QString matricule = query.value(0).toString();
        QString modele = query.value(1).toString();
        QString vehiculeInfo = matricule;
        if (!modele.isEmpty() && query.record().count() > 1)
        {
            vehiculeInfo = matricule + " - " + modele;
        }
        ui->cin_5->addItem(vehiculeInfo, matricule);
    }
}

QComboBox *Planning::getCondidatCombo()
{
    // Utiliser directement ui->cin_6 comme pour ui->cin_4 et ui->cin_5
    // Si cin_6 n'existe pas, essayer de trouver le champ par d'autres noms
    if (ui->cin_6)
    {
        return ui->cin_6;
    }

    // Fallback: chercher le champ QComboBox pour le CIN du condidat
    QComboBox *condidatCombo = nullptr;

    // Essayer de trouver le champ par différents noms possibles
    condidatCombo = findChild<QComboBox *>("cin_condidat");
    if (!condidatCombo)
    {
        condidatCombo = findChild<QComboBox *>("condidat_cin");
    }
    if (!condidatCombo)
    {
        condidatCombo = findChild<QComboBox *>("cin_condidat_3");
    }
    if (!condidatCombo)
    {
        condidatCombo = findChild<QComboBox *>("cin_6");
    }
    if (!condidatCombo)
    {
        // Si aucun champ n'est trouvé, essayer de trouver tous les QComboBox et utiliser celui qui n'est pas cin_4 ou cin_5
        QList<QComboBox *> combos = findChildren<QComboBox *>();
        for (QComboBox *combo : combos)
        {
            QString name = combo->objectName();
            if (name != "cin_4" && name != "cin_5" && (name.contains("condidat", Qt::CaseInsensitive) || name.contains("candidat", Qt::CaseInsensitive) || name == "cin_6"))
            {
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
    if (!ui->cin_6)
    {
        qDebug() << "Avertissement: ui->cin_6 n'existe pas. Vérifiez que le champ est bien nommé 'cin_6' dans planning.ui";
        return;
    }

    ui->cin_6->clear();
    ui->cin_6->addItem("Choisir un condidat", QVariant());

    QSqlQuery query;
    // Charger les CINs avec les noms et prénoms pour affichage
    query.prepare("SELECT cin_condidat, nom, prenom FROM CONDIDAT ORDER BY cin_condidat");
    if (!query.exec())
    {
        QString errorMsg = query.lastError().text();
        if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist"))
        {
            QMessageBox::warning(this, "Avertissement",
                                 "La table CONDIDAT n'existe pas dans la base de données.\n\n"
                                 "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
        }
        else
        {
            QMessageBox::warning(this, "Avertissement", "Impossible de charger les condidats: " + errorMsg);
        }
        return;
    }

    while (query.next())
    {
        QString cin = query.value(0).toString();
        QString nom = query.value(1).toString();
        QString prenom = query.value(2).toString();
        QString displayText = cin;

        // Ajouter le nom et prénom si disponibles pour un affichage plus lisible
        if (!nom.isEmpty() && !prenom.isEmpty())
        {
            displayText = cin + " - " + nom + " " + prenom;
        }
        else if (!nom.isEmpty())
        {
            displayText = cin + " - " + nom;
        }

        // Stocker le CIN comme data pour faciliter la récupération
        ui->cin_6->addItem(displayText, cin);
    }
}

// ==================== Google Maps Integration ====================

QString Planning::getCircuitAddress(const QString &circuitName)
{
    // Map predefined circuit names to actual addresses
    static QMap<QString, QString> circuitAddresses = {
        {"Circuit Urbain - Centre Ville Tunis", "Avenue de la Liberté, Tunis, Tunisia"},
        {"Circuit Avenue Habib Bourguiba", "Avenue Habib Bourguiba, Tunis 1000, Tunisia"},
        {"Circuit La Marsa - Gammarth", "Avenue Habib Bourguiba, La Marsa, Tunisia"},
        {"Circuit Lac 2 - Les Berges du Lac", "Les Berges du Lac 2, Tunis, Tunisia"},
        {"Circuit Périphérique - Ariana", "Route de la Marsa, Ariana, Tunisia"},
        {"Circuit Autoroute - Sousse", "Autoroute A1, Sousse, Tunisia"},
        {"Circuit Montagne - Zaghouan", "Route de Zaghouan, Zaghouan, Tunisia"}
    };
    
    return circuitAddresses.value(circuitName, circuitName);
}

void Planning::onSearchLocationClicked()
{
    qDebug() << "=== Search Location Button Clicked ===";
    
    QString address;
    
    // Check if custom circuit is selected
    if (ui->circuit_combo && ui->circuit_combo->currentIndex() == ui->circuit_combo->count() - 1) {
        // Custom address
        if (ui->custom_circuit_input) {
            address = ui->custom_circuit_input->text().trimmed();
        }
    } else {
        // Predefined circuit
        if (ui->circuit_combo && ui->circuit_combo->currentIndex() > 0) {
            QString circuitName = ui->circuit_combo->currentText();
            address = getCircuitAddress(circuitName);
            qDebug() << "Circuit selected:" << circuitName;
            qDebug() << "Mapped to address:" << address;
        }
    }
    
    if (address.isEmpty()) {
        qDebug() << "ERROR: Address is empty!";
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un circuit ou entrer une adresse.");
        return;
    }
    
    // Update info label to show loading state
    if (ui->map_info_label) {
        ui->map_info_label->setText(QString("🔄 Recherche en cours: %1...").arg(address));
        ui->map_info_label->setStyleSheet("background-color:#FF9800;color:white;border-radius:5px;padding:5px;");
        qDebug() << "Info label updated with loading message";
    } else {
        qDebug() << "WARNING: map_info_label is NULL!";
    }
    
    // Check if mapService is valid
    if (!mapService) {
        qDebug() << "ERROR: mapService is NULL!";
        QMessageBox::critical(this, "Erreur", "Service de carte non initialisé!");
        return;
    }
    
    qDebug() << "Calling geocodeAddress with:" << address;
    // Geocode the address to get coordinates using OpenStreetMap (FREE!)
    mapService->geocodeAddress(address);
}

void Planning::onGeocodeCompleted(double latitude, double longitude, const QString &formattedAddress)
{
    qDebug() << "=== Geocode Completed ===";
    qDebug() << "Latitude:" << latitude << "Longitude:" << longitude;
    qDebug() << "Formatted Address:" << formattedAddress;
    
    currentLatitude = latitude;
    currentLongitude = longitude;
    currentFormattedAddress = formattedAddress;
    
    // Update the circuit field if using custom address
    if (ui->circuit_combo && ui->circuit_combo->currentIndex() == ui->circuit_combo->count() - 1) {
        if (ui->custom_circuit_input) {
            ui->custom_circuit_input->setText(formattedAddress);
        }
    }
    
    // Update the map info label with location details
    if (ui->map_info_label) {
        ui->map_info_label->setText(QString("📍 Circuit localisé: %1 | Coordonnées: %2, %3")
            .arg(formattedAddress)
            .arg(latitude, 0, 'f', 5)
            .arg(longitude, 0, 'f', 5));
        ui->map_info_label->setStyleSheet("background-color:#4CAF50;color:white;border-radius:5px;padding:5px;");
        qDebug() << "Info label updated with location details";
    } else {
        qDebug() << "WARNING: map_info_label is NULL in onGeocodeCompleted!";
    }
    
    // Request static map image with marker (OpenStreetMap)
    qDebug() << "Requesting static map...";
    mapService->getStaticMapWithMarker(latitude, longitude, 15, 1021, 350);
}

void Planning::onStaticMapReady(const QPixmap &mapImage)
{
    qDebug() << "=== Static Map Ready ===";
    qDebug() << "Map image size:" << mapImage.size();
    qDebug() << "Map image isNull:" << mapImage.isNull();
    
    // Create a dialog to show the map
    QDialog *mapDialog = new QDialog(this);
    mapDialog->setWindowTitle("Carte du Circuit");
    mapDialog->setModal(false);
    mapDialog->resize(900, 700);
    
    QVBoxLayout *layout = new QVBoxLayout(mapDialog);
    
    // Info label
    QLabel *infoLabel = new QLabel(mapDialog);
    infoLabel->setText(QString("📍 %1\n📌 Coordonnées: %2, %3")
        .arg(currentFormattedAddress)
        .arg(currentLatitude, 0, 'f', 5)
        .arg(currentLongitude, 0, 'f', 5));
    infoLabel->setStyleSheet("background-color:#4CAF50;color:white;border-radius:5px;padding:10px;font-size:12px;");
    infoLabel->setWordWrap(true);
    layout->addWidget(infoLabel);
    
    // Map label
    QLabel *mapLabel = new QLabel(mapDialog);
    mapLabel->setPixmap(mapImage.scaled(850, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    mapLabel->setStyleSheet("border:2px solid #4CAF50;background-color:white;");
    mapLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(mapLabel);
    
    // Close button
    QPushButton *closeBtn = new QPushButton("Fermer", mapDialog);
    closeBtn->setStyleSheet("background-color:#f44336;color:white;padding:8px;font-size:12px;border-radius:5px;");
    connect(closeBtn, &QPushButton::clicked, mapDialog, &QDialog::close);
    layout->addWidget(closeBtn);
    
    mapDialog->show();
    
    qDebug() << "Map dialog created and shown!";
    
    // Also try to display in the embedded label if it exists
    if (mapDisplayLabel) {
        qDebug() << "Also updating embedded label...";
        mapDisplayLabel->clear();
        mapDisplayLabel->setPixmap(mapImage.scaled(mapDisplayLabel->size(), 
                                                    Qt::KeepAspectRatio, 
                                                    Qt::SmoothTransformation));
        mapDisplayLabel->setStyleSheet("background-color:#ffffff;border:3px solid #4CAF50;border-radius:10px;padding:5px;");
        mapDisplayLabel->setVisible(true);
        mapDisplayLabel->update();
        mapDisplayLabel->repaint();
    }
    
    // Update info label
    if (ui->map_info_label) {
        ui->map_info_label->setText(QString("✅ Carte affichée! | %1")
            .arg(currentFormattedAddress));
        ui->map_info_label->setStyleSheet("background-color:#2196F3;color:white;border-radius:5px;padding:8px;font-size:11px;font-weight:bold;");
        ui->map_info_label->update();
    }
}

void Planning::onMapsError(const QString &error)
{
    qDebug() << "=== Maps Error ===";
    qWarning() << "Maps API error:" << error;
    
    // Update info label with error message
    if (ui->map_info_label) {
        ui->map_info_label->setText(QString("❌ Erreur: %1").arg(error));
        ui->map_info_label->setStyleSheet("background-color:#f44336;color:white;border-radius:5px;padding:5px;");
        qDebug() << "Info label updated with error message";
    } else {
        qDebug() << "WARNING: map_info_label is NULL in onMapsError!";
    }
    
    // Also show message box for critical errors
    QMessageBox::warning(this, "Erreur Maps", 
                        QString("Erreur lors de la récupération de la carte:\n%1").arg(error));
}

void Planning::onMapLocationSelected(double latitude, double longitude)
{
    currentLatitude = latitude;
    currentLongitude = longitude;
    
    // Reverse geocode to get the address (OpenStreetMap)
    mapService->reverseGeocode(latitude, longitude);
}
// Calendar functionality methods for Planning class
// Append these methods to planning.cpp

void Planning::onCalendarDateClicked(const QDate &date)
{
    qDebug() << "Calendar date clicked:" << date.toString("dd/MM/yyyy");
    
    // Get events for this date from database using the actual PLANNING table structure
    QSqlQuery query;
    query.prepare("SELECT ID_SEANCE, "
                  "TO_CHAR(DATE_SEANCE, 'DD/MM/YYYY') as DATE_STR, "
                  "NOM_CANDIDAT, "
                  "TYPE_SEANCE, "
                  "TO_CHAR(HEURE_DEBUT, 'HH24:MI') as DEBUT, "
                  "TO_CHAR(HEURE_FIN, 'HH24:MI') as FIN, "
                  "ADRESSE_CIRCUIT, "
                  "MATRICULE "
                  "FROM PLANNING "
                  "WHERE DATE_SEANCE = TO_DATE(:date, 'YYYY-MM-DD') "
                  "ORDER BY HEURE_DEBUT");
    
    // Convert QDate to string format
    QString dateStr = date.toString("yyyy-MM-dd");
    query.bindValue(":date", dateStr);
    
    if (!query.exec()) {
        qWarning() << "Error fetching events for date:" << query.lastError().text();
        QMessageBox::warning(this, "Erreur", "Impossible de charger les événements pour cette date.");
        return;
    }
    
    // Build message with all events for this date
    QString message = QString("📅 <b>Événements pour %1</b><br><br>")
        .arg(date.toString("dddd dd MMMM yyyy"));
    
    int eventCount = 0;
    while (query.next()) {
        eventCount++;
        QString candidat = query.value("NOM_CANDIDAT").toString();
        QString type = query.value("TYPE_SEANCE").toString();
        QString debut = query.value("DEBUT").toString();
        QString fin = query.value("FIN").toString();
        QString circuit = query.value("ADRESSE_CIRCUIT").toString();
        QString vehicule = query.value("MATRICULE").toString();
        
        message += QString("<div style='background-color:#f0f0f0; padding:10px; margin:5px 0; border-left:4px solid #4CAF50; border-radius:5px;'>"
                          "<b>🕐 %1 - %2</b><br>"
                          "👤 Candidat: <b>%3</b><br>"
                          "📝 Type: <b>%4</b><br>"
                          "📍 Circuit: %5<br>")
            .arg(debut)
            .arg(fin)
            .arg(candidat)
            .arg(type)
            .arg(circuit);
        
        if (!vehicule.isEmpty()) {
            message += QString("🚗 Véhicule: %1<br>").arg(vehicule);
        }
        
        message += "</div>";
    }
    
    if (eventCount == 0) {
        message += "<p style='color:#666; text-align:center;'>Aucune séance planifiée pour cette date.</p>";
    } else {
        message += QString("<br><p style='text-align:center; color:#4CAF50;'><b>Total: %1 séance(s)</b></p>").arg(eventCount);
    }
    
    // Show message box with events
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Calendrier des Séances");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void Planning::highlightDatesWithEvents()
{
    if (!ui->calendarWidget) {
        return;
    }
    
    qDebug() << "Highlighting dates with events...";
    
    // Clear all date formats first
    ui->calendarWidget->setDateTextFormat(QDate(), QTextCharFormat());
    
    // Get all dates with events from database
    QSqlQuery query;
    query.prepare("SELECT DISTINCT DATE_SEANCE, COUNT(*) as EVENT_COUNT "
                  "FROM PLANNING "
                  "WHERE DATE_SEANCE IS NOT NULL "
                  "GROUP BY DATE_SEANCE");
    
    if (!query.exec()) {
        qWarning() << "Error fetching dates with events:" << query.lastError().text();
        return;
    }
    
    int totalHighlighted = 0;
    while (query.next()) {
        QDate eventDate = query.value("DATE_SEANCE").toDate();
        int eventCount = query.value("EVENT_COUNT").toInt();
        
        if (eventDate.isValid()) {
            QTextCharFormat format;
            
            // Different colors based on number of events
            if (eventCount >= 3) {
                // Many events - dark green
                format.setBackground(QColor(56, 142, 60));
                format.setForeground(Qt::white);
            } else if (eventCount >= 2) {
                // Multiple events - medium green
                format.setBackground(QColor(76, 175, 80));
                format.setForeground(Qt::white);
            } else {
                // Single event - light green
                format.setBackground(QColor(129, 199, 132));
                format.setForeground(Qt::black);
            }
            
            format.setFontWeight(QFont::Bold);
            ui->calendarWidget->setDateTextFormat(eventDate, format);
            totalHighlighted++;
        }
    }
    
    qDebug() << "Highlighted" << totalHighlighted << "dates with events";
    
    // Highlight today's date with special color
    QTextCharFormat todayFormat;
    todayFormat.setBackground(QColor(255, 206, 0, 100)); // Light yellow
    todayFormat.setForeground(QColor(0, 33, 87)); // Dark blue
    todayFormat.setFontWeight(QFont::Bold);
    ui->calendarWidget->setDateTextFormat(QDate::currentDate(), todayFormat);
}

void Planning::refreshCalendarView()
{
    if (!ui->calendarWidget) {
        return;
    }
    
    qDebug() << "Refreshing calendar view...";
    
    // Re-highlight dates with events
    highlightDatesWithEvents();
    
    // Update calendar widget
    ui->calendarWidget->update();
    ui->calendarWidget->repaint();
}
// Search, Sort, and Export functionality for Planning
// Add these methods to planning.cpp

void Planning::on_recherche_3_textChanged()
{
    currentSearchText = ui->recherche_3->toPlainText().trimmed();
    qDebug() << "Search text changed:" << currentSearchText;
    refreshTableWithFilters();
}

void Planning::on_tri_3_currentIndexChanged(int index)
{
    currentSortIndex = index;
    qDebug() << "Sort index changed:" << index;
    refreshTableWithFilters();
}

void Planning::refreshTableWithFilters()
{
    // Build query with search and sort
    QString queryStr = "SELECT P.ID_SEANCE, "
                      "TO_CHAR(P.DATE_SEANCE, 'DD/MM/YYYY') AS DATE_SEANCE, "
                      "P.NOM_CANDIDAT, "
                      "P.TYPE_SEANCE, "
                      "TO_CHAR(P.HEURE_DEBUT, 'HH24:MI') AS DEBUT, "
                      "TO_CHAR(P.HEURE_FIN, 'HH24:MI') AS FIN, "
                      "P.ADRESSE_CIRCUIT, "
                      "P.MATRICULE, "
                      "P.CIN_CONDIDAT, "
                      "P.CIN_EMPLOYEE "
                      "FROM PLANNING P ";
    
    // Add search filter
    if (!currentSearchText.isEmpty()) {
        queryStr += "WHERE (UPPER(P.NOM_CANDIDAT) LIKE :search "
                   "OR UPPER(P.TYPE_SEANCE) LIKE :search "
                   "OR UPPER(P.ADRESSE_CIRCUIT) LIKE :search "
                   "OR UPPER(P.MATRICULE) LIKE :search) ";
    }
    
    // Add sorting
    switch (currentSortIndex) {
        case 1: // Date (Plus récent)
            queryStr += "ORDER BY P.DATE_SEANCE DESC, P.HEURE_DEBUT DESC";
            break;
        case 2: // Date (Plus ancien)
            queryStr += "ORDER BY P.DATE_SEANCE ASC, P.HEURE_DEBUT ASC";
            break;
        case 3: // Candidat (A-Z)
            queryStr += "ORDER BY P.NOM_CANDIDAT ASC";
            break;
        case 4: // Candidat (Z-A)
            queryStr += "ORDER BY P.NOM_CANDIDAT DESC";
            break;
        case 5: // Type (Code/Conduit)
            queryStr += "ORDER BY P.TYPE_SEANCE ASC";
            break;
        case 6: // Heure (Plus tôt)
            queryStr += "ORDER BY P.HEURE_DEBUT ASC";
            break;
        default: // Default sorting
            queryStr += "ORDER BY P.DATE_SEANCE DESC, P.HEURE_DEBUT DESC";
            break;
    }
    
    QSqlQueryModel *model = new QSqlQueryModel();
    
    QSqlQuery query;
    query.prepare(queryStr);
    
    if (!currentSearchText.isEmpty()) {
        QString searchPattern = "%" + currentSearchText.toUpper() + "%";
        query.bindValue(":search", searchPattern);
    }
    
    if (!query.exec()) {
        qWarning() << "Error executing filtered query:" << query.lastError().text();
        QMessageBox::warning(this, "Erreur", "Erreur lors de la recherche: " + query.lastError().text());
        delete model;
        return;
    }
    
    model->setQuery(std::move(query));
    
    int rows = model->rowCount();
    int cols = model->columnCount();
    
    ui->tab_3->setRowCount(rows);
    ui->tab_3->setColumnCount(cols + 1);
    
    QStringList headers;
    headers << "ID" << "Date séance" << "condidat" << "type" << "debut" << "fin" << "circuit" << "véhicule" << "CIN_CAND" << "CIN_EMP" << "Actions";
    ui->tab_3->setHorizontalHeaderLabels(headers);
    
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QVariant v = model->data(model->index(r, c));
            ui->tab_3->setItem(r, c, new QTableWidgetItem(v.toString()));
        }
        bool ok = false;
        int id = model->data(model->index(r, 0)).toInt(&ok);
        ui->tab_3->setCellWidget(r, cols, createActionsCell(r, ok ? id : -1));
    }
    
    ui->tab_3->setColumnHidden(8, true);
    ui->tab_3->setColumnHidden(9, true);
    
    delete model;
    
    // Refresh calendar view
    refreshCalendarView();
    
    qDebug() << "Table refreshed with" << rows << "rows (search:" << currentSearchText << ", sort:" << currentSortIndex << ")";
}

void Planning::on_pushButton_8_clicked()
{
    qDebug() << "Export to PDF button clicked";
    
    // Ask user for save location
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Exporter Planning en PDF",
                                                    QDir::homePath() + "/planning_" + QDate::currentDate().toString("yyyy-MM-dd") + ".pdf",
                                                    "PDF Files (*.pdf)");
    
    if (fileName.isEmpty()) {
        qDebug() << "Export cancelled by user";
        return;
    }
    
    // Ensure .pdf extension
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }
    
    // Create printer
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Landscape); // Landscape for wide table
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);
    
    // Create HTML document for PDF
    QString html = "<html><head><style>"
                  "body { font-family: Arial, sans-serif; }"
                  "h1 { color: #002157; text-align: center; margin-bottom: 20px; }"
                  "table { width: 100%; border-collapse: collapse; margin-top: 20px; }"
                  "th { background-color: #ffce00; color: #002157; padding: 10px; text-align: left; border: 1px solid #ddd; font-weight: bold; }"
                  "td { padding: 8px; border: 1px solid #ddd; }"
                  "tr:nth-child(even) { background-color: #f9f9f9; }"
                  ".footer { margin-top: 30px; text-align: center; font-size: 10px; color: #666; }"
                  "</style></head><body>";
    
    html += "<h1>📋 Planning des Séances de Conduite - DriveSmart</h1>";
    html += "<p style='text-align:center;'><strong>Date d'export:</strong> " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm") + "</p>";
    
    // Get current table data
    QSqlQuery query;
    QString queryStr = "SELECT P.ID_SEANCE, "
                      "TO_CHAR(P.DATE_SEANCE, 'DD/MM/YYYY') AS DATE_SEANCE, "
                      "P.NOM_CANDIDAT, "
                      "P.TYPE_SEANCE, "
                      "TO_CHAR(P.HEURE_DEBUT, 'HH24:MI') AS DEBUT, "
                      "TO_CHAR(P.HEURE_FIN, 'HH24:MI') AS FIN, "
                      "P.ADRESSE_CIRCUIT, "
                      "P.MATRICULE "
                      "FROM PLANNING P ";
    
    // Apply current filters
    if (!currentSearchText.isEmpty()) {
        queryStr += "WHERE (UPPER(P.NOM_CANDIDAT) LIKE :search "
                   "OR UPPER(P.TYPE_SEANCE) LIKE :search "
                   "OR UPPER(P.ADRESSE_CIRCUIT) LIKE :search "
                   "OR UPPER(P.MATRICULE) LIKE :search) ";
    }
    
    // Apply current sorting
    switch (currentSortIndex) {
        case 1: queryStr += "ORDER BY P.DATE_SEANCE DESC, P.HEURE_DEBUT DESC"; break;
        case 2: queryStr += "ORDER BY P.DATE_SEANCE ASC, P.HEURE_DEBUT ASC"; break;
        case 3: queryStr += "ORDER BY P.NOM_CANDIDAT ASC"; break;
        case 4: queryStr += "ORDER BY P.NOM_CANDIDAT DESC"; break;
        case 5: queryStr += "ORDER BY P.TYPE_SEANCE ASC"; break;
        case 6: queryStr += "ORDER BY P.HEURE_DEBUT ASC"; break;
        default: queryStr += "ORDER BY P.DATE_SEANCE DESC, P.HEURE_DEBUT DESC"; break;
    }
    
    query.prepare(queryStr);
    
    if (!currentSearchText.isEmpty()) {
        QString searchPattern = "%" + currentSearchText.toUpper() + "%";
        query.bindValue(":search", searchPattern);
    }
    
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'export: " + query.lastError().text());
        return;
    }
    
    // Build table
    html += "<table>";
    html += "<tr><th>ID</th><th>Date</th><th>Candidat</th><th>Type</th><th>Début</th><th>Fin</th><th>Circuit</th><th>Véhicule</th></tr>";
    
    int rowCount = 0;
    while (query.next()) {
        rowCount++;
        html += "<tr>";
        html += "<td>" + query.value(0).toString() + "</td>";
        html += "<td>" + query.value(1).toString() + "</td>";
        html += "<td>" + query.value(2).toString() + "</td>";
        html += "<td>" + query.value(3).toString() + "</td>";
        html += "<td>" + query.value(4).toString() + "</td>";
        html += "<td>" + query.value(5).toString() + "</td>";
        html += "<td>" + query.value(6).toString() + "</td>";
        html += "<td>" + query.value(7).toString() + "</td>";
        html += "</tr>";
    }
    
    html += "</table>";
    html += "<div class='footer'>";
    html += "<p><strong>Total: " + QString::number(rowCount) + " séance(s)</strong></p>";
    html += "<p>Généré par DriveSmart - Système de Gestion d'Auto-École</p>";
    html += "</div>";
    html += "</body></html>";
    
    // Print to PDF
    QTextDocument document;
    document.setHtml(html);
    document.print(&printer);
    
    qDebug() << "PDF exported successfully to:" << fileName;
    QMessageBox::information(this, "Succès", 
                            QString("Planning exporté avec succès!\n\nFichier: %1\nNombre de séances: %2")
                            .arg(fileName)
                            .arg(rowCount));
}

// ==================== Statistics Functionality ====================

void Planning::on_pushButton_10_clicked()
{
    qDebug() << "Bouton statistiques cliqué";
    afficherStatistiquesPlanning();
}

void Planning::afficherStatistiquesPlanning()
{
    qDebug() << "Génération des statistiques de planning...";

    // Créer une boîte de dialogue pour afficher les statistiques
    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("📊 Statistiques des Séances de Planning");
    statsDialog->setMinimumSize(900, 600);
    statsDialog->setStyleSheet("QDialog { background-color: #f5f5f5; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(statsDialog);

    // Titre
    QLabel *title = new QLabel("📊 Statistiques des Séances");
    title->setStyleSheet("font-size: 18px; font-weight: bold; color: #2c3e50; margin: 10px;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // Récupérer les statistiques depuis la base de données
    QSqlQuery query;
    
    // 1. Statistiques par type de séance (Conduit vs Code)
    query.prepare("SELECT TYPE_SEANCE, COUNT(*) FROM PLANNING GROUP BY TYPE_SEANCE");
    
    int totalSeances = 0;
    int seancesConduit = 0;
    int seancesCode = 0;
    
    if (query.exec()) {
        while (query.next()) {
            QString type = query.value(0).toString();
            int count = query.value(1).toInt();
            
            if (type.toLower() == "conduit") {
                seancesConduit = count;
            } else if (type.toLower() == "code") {
                seancesCode = count;
            }
            totalSeances += count;
        }
    } else {
        qDebug() << "Erreur requête statistiques par type:" << query.lastError().text();
    }

    // Vérifier s'il y a des données
    if (totalSeances == 0) {
        QMessageBox::information(this, "Statistiques", "Aucune séance trouvée dans la base de données.");
        delete statsDialog;
        return;
    }

    // === GRAPHIQUE 1: Répartition par type de séance (Pie Chart) ===
    QPieSeries *typeSeries = new QPieSeries();
    
    if (seancesConduit > 0) {
        QPieSlice *sliceConduit = typeSeries->append("Conduit", seancesConduit);
        sliceConduit->setColor(QColor(76, 175, 80)); // Vert
        sliceConduit->setExploded(true);
        sliceConduit->setLabelVisible(true);
        double percentConduit = (seancesConduit * 100.0) / totalSeances;
        sliceConduit->setLabel(QString("Conduit\n%1 (%2%)")
                                .arg(seancesConduit)
                                .arg(QString::number(percentConduit, 'f', 1)));
    }
    
    if (seancesCode > 0) {
        QPieSlice *sliceCode = typeSeries->append("Code", seancesCode);
        sliceCode->setColor(QColor(33, 150, 243)); // Bleu
        sliceCode->setLabelVisible(true);
        double percentCode = (seancesCode * 100.0) / totalSeances;
        sliceCode->setLabel(QString("Code\n%1 (%2%)")
                             .arg(seancesCode)
                             .arg(QString::number(percentCode, 'f', 1)));
    }

    QChart *typeChart = new QChart();
    typeChart->addSeries(typeSeries);
    typeChart->setTitle("Répartition par Type de Séance");
    typeChart->setTitleFont(QFont("Arial", 12, QFont::Bold));
    typeChart->setAnimationOptions(QChart::AllAnimations);
    typeChart->legend()->setVisible(true);
    typeChart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *typeChartView = new QChartView(typeChart);
    typeChartView->setRenderHint(QPainter::Antialiasing);
    typeChartView->setMinimumSize(400, 300);

    // === GRAPHIQUE 2: Top 5 moniteurs (Bar Chart) ===
    query.prepare("SELECT E.NOM || ' ' || E.PRENOM AS MONITEUR, COUNT(*) AS NB_SEANCES "
                  "FROM PLANNING P "
                  "LEFT JOIN EMPLOYEE E ON P.CIN_EMPLOYEE = E.CIN_EMPLOYEE "
                  "WHERE E.NOM IS NOT NULL "
                  "GROUP BY E.NOM, E.PRENOM "
                  "ORDER BY NB_SEANCES DESC "
                  "FETCH FIRST 5 ROWS ONLY");
    
    QBarSet *moniteurSet = new QBarSet("Séances");
    moniteurSet->setColor(QColor(255, 152, 0)); // Orange
    QStringList moniteurNames;
    
    if (query.exec()) {
        while (query.next()) {
            QString moniteur = query.value(0).toString();
            int count = query.value(1).toInt();
            *moniteurSet << count;
            moniteurNames << moniteur;
        }
    }

    QBarSeries *moniteurSeries = new QBarSeries();
    moniteurSeries->append(moniteurSet);

    QChart *moniteurChart = new QChart();
    moniteurChart->addSeries(moniteurSeries);
    moniteurChart->setTitle("Top 5 Moniteurs (par nombre de séances)");
    moniteurChart->setTitleFont(QFont("Arial", 12, QFont::Bold));
    moniteurChart->setAnimationOptions(QChart::AllAnimations);
    moniteurChart->legend()->setVisible(false);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(moniteurNames);
    axisX->setTitleText("Moniteurs");
    moniteurChart->addAxis(axisX, Qt::AlignBottom);
    moniteurSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre de séances");
    axisY->setLabelFormat("%d");
    axisY->setTickCount(6);
    moniteurChart->addAxis(axisY, Qt::AlignLeft);
    moniteurSeries->attachAxis(axisY);

    QChartView *moniteurChartView = new QChartView(moniteurChart);
    moniteurChartView->setRenderHint(QPainter::Antialiasing);
    moniteurChartView->setMinimumSize(400, 300);

    // === GRAPHIQUE 3: Séances par mois (Bar Chart) ===
    query.prepare("SELECT TO_CHAR(DATE_SEANCE, 'YYYY-MM') AS MOIS, COUNT(*) AS NB_SEANCES "
                  "FROM PLANNING "
                  "WHERE DATE_SEANCE >= ADD_MONTHS(TRUNC(SYSDATE, 'MM'), -5) "
                  "GROUP BY TO_CHAR(DATE_SEANCE, 'YYYY-MM') "
                  "ORDER BY MOIS");
    
    QBarSet *monthSet = new QBarSet("Séances");
    monthSet->setColor(QColor(156, 39, 176)); // Violet
    QStringList monthNames;
    
    if (query.exec()) {
        while (query.next()) {
            QString mois = query.value(0).toString();
            int count = query.value(1).toInt();
            *monthSet << count;
            
            // Format: 2024-12 -> Dec 2024
            QDate date = QDate::fromString(mois + "-01", "yyyy-MM-dd");
            monthNames << date.toString("MMM yyyy");
        }
    }

    QBarSeries *monthSeries = new QBarSeries();
    monthSeries->append(monthSet);

    QChart *monthChart = new QChart();
    monthChart->addSeries(monthSeries);
    monthChart->setTitle("Séances des 6 derniers mois");
    monthChart->setTitleFont(QFont("Arial", 12, QFont::Bold));
    monthChart->setAnimationOptions(QChart::AllAnimations);
    monthChart->legend()->setVisible(false);

    QBarCategoryAxis *monthAxisX = new QBarCategoryAxis();
    monthAxisX->append(monthNames);
    monthAxisX->setTitleText("Mois");
    monthChart->addAxis(monthAxisX, Qt::AlignBottom);
    monthSeries->attachAxis(monthAxisX);

    QValueAxis *monthAxisY = new QValueAxis();
    monthAxisY->setTitleText("Nombre de séances");
    monthAxisY->setLabelFormat("%d");
    monthAxisY->setTickCount(6);
    monthChart->addAxis(monthAxisY, Qt::AlignLeft);
    monthSeries->attachAxis(monthAxisY);

    QChartView *monthChartView = new QChartView(monthChart);
    monthChartView->setRenderHint(QPainter::Antialiasing);
    monthChartView->setMinimumSize(400, 300);

    // === Statistiques textuelles supplémentaires ===
    QLabel *statsText = new QLabel();
    QString statsHtml = "<div style='background: white; padding: 15px; border-radius: 8px;'>";
    statsHtml += "<h3 style='color: #2c3e50; margin-bottom: 10px;'>📈 Résumé Statistique</h3>";
    statsHtml += QString("<p><b>Total des séances:</b> %1</p>").arg(totalSeances);
    statsHtml += QString("<p><b>Séances de conduit:</b> %1 (%2%)</p>")
                  .arg(seancesConduit)
                  .arg(QString::number((seancesConduit * 100.0) / totalSeances, 'f', 1));
    statsHtml += QString("<p><b>Séances de code:</b> %1 (%2%)</p>")
                  .arg(seancesCode)
                  .arg(QString::number((seancesCode * 100.0) / totalSeances, 'f', 1));
    
    // Nombre de candidats uniques
    QSqlQuery candQuery;
    candQuery.prepare("SELECT COUNT(DISTINCT CIN_CONDIDAT) FROM PLANNING WHERE CIN_CONDIDAT IS NOT NULL");
    if (candQuery.exec() && candQuery.next()) {
        int nbCandidats = candQuery.value(0).toInt();
        statsHtml += QString("<p><b>Candidats actifs:</b> %1</p>").arg(nbCandidats);
    }
    
    // Nombre de moniteurs actifs
    QSqlQuery monQuery;
    monQuery.prepare("SELECT COUNT(DISTINCT CIN_EMPLOYEE) FROM PLANNING WHERE CIN_EMPLOYEE IS NOT NULL");
    if (monQuery.exec() && monQuery.next()) {
        int nbMoniteurs = monQuery.value(0).toInt();
        statsHtml += QString("<p><b>Moniteurs actifs:</b> %1</p>").arg(nbMoniteurs);
    }
    
    statsHtml += "</div>";
    statsText->setText(statsHtml);
    statsText->setWordWrap(true);

    // === Layout: Organiser les graphiques ===
    QHBoxLayout *topChartsLayout = new QHBoxLayout();
    topChartsLayout->addWidget(typeChartView);
    topChartsLayout->addWidget(moniteurChartView);

    QHBoxLayout *bottomChartsLayout = new QHBoxLayout();
    bottomChartsLayout->addWidget(monthChartView);
    bottomChartsLayout->addWidget(statsText);

    mainLayout->addLayout(topChartsLayout);
    mainLayout->addLayout(bottomChartsLayout);

    // Bouton Fermer
    QPushButton *closeButton = new QPushButton("Fermer");
    closeButton->setStyleSheet("QPushButton { "
                               "background-color: #3498db; "
                               "color: white; "
                               "border: none; "
                               "padding: 10px 30px; "
                               "border-radius: 5px; "
                               "font-weight: bold;"
                               "}"
                               "QPushButton:hover { "
                               "background-color: #2980b9;"
                               "}");
    connect(closeButton, &QPushButton::clicked, statsDialog, &QDialog::accept);
    mainLayout->addWidget(closeButton, 0, Qt::AlignCenter);

    statsDialog->exec();
    
    qDebug() << "Statistiques affichées avec succès";
}