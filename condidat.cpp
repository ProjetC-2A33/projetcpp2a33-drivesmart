#include "condidat.h"
#include "ui_condidat.h"
#include "navigation_constants.h"
#include "connection.h"
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
#include <QRegularExpression>
#include <QFileDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QTextStream>
#include <QDesktopServices>
#include <QUrl>
#include <QDateTime>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QStringConverter>
#endif
#include <QPixmap>
#include <QPainter>
#include <QMap>
#include <QApplication>
#include <QFont>
#include <QFontMetrics>
#include <QBrush>
#include <QPen>
#include <cmath>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QAudioSource>
#include <QAudioDevice>
#include <QMediaDevices>
#else
#include <QAudioInput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#endif
#include <QIODevice>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QBuffer>
#include <QRegularExpression>
#include <QDebug>
#include <QInputDialog>
#include <QDialog>
#include <QHttpMultiPart>
#include <QProcessEnvironment>
#ifdef USE_VOSK
#include <vosk_api.h>
#endif
#include <QDir>
#include <QKeyEvent>

Condidat::Condidat(QWidget *parent) : QWidget(parent),
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                      audioSource(nullptr),
#else
                                      audioInput(nullptr),
#endif
                                      audioBuffer(nullptr),
                                      networkManager(nullptr),
                                      recordingTimer(nullptr),
                                      isRecording(false),
                                      ui(new Ui::Condidat)
{
    ui->setupUi(this);
    
    // Initialize network manager for potential future use
    networkManager = new QNetworkAccessManager(this);
    
    // Initialize recording timer
    recordingTimer = new QTimer(this);
    recordingTimer->setSingleShot(true);
    connect(recordingTimer, &QTimer::timeout, this, &Condidat::stopVoiceRecording);
    
    // Resolve voice button even if not present in generated UI header
    voiceButton = ui->btn_vocal ? ui->btn_vocal : this->findChild<QPushButton*>("btn_vocal");
    if (!voiceButton)
    {
        voiceButton = new QPushButton(this);
        voiceButton->setObjectName("btn_vocal");
        voiceButton->setText("🎤 Remplir par voix");
        voiceButton->setStyleSheet("background-color:#002157;color:#ffce00;border-radius:10px;");
        voiceButton->setGeometry(QRect(50, 710, 271, 31));
        voiceButton->raise();
        voiceButton->show();
    }
    voiceButton->setText("🎤 Remplir par voix");
    voiceButton->setStyleSheet("background-color:#002157;color:#ffce00;border-radius:10px;");
    voiceButton->setToolTip("Cliquez pour parler");
    if (this->findChild<QLabel*>("label_10")) this->findChild<QLabel*>("label_10")->show();

    // Connect navigation buttons
    connect(ui->candidat, &QPushButton::clicked, this, &Condidat::navigateToCandidat);
    connect(ui->planning, &QPushButton::clicked, this, &Condidat::navigateToPlanning);
    connect(ui->employee, &QPushButton::clicked, this, &Condidat::navigateToEmployee);
    connect(ui->vehicule, &QPushButton::clicked, this, &Condidat::navigateToVehicule);
    connect(ui->examen, &QPushButton::clicked, this, &Condidat::navigateToExamen);
    connect(ui->equipement, &QPushButton::clicked, this, &Condidat::navigateToEquipement);
    
    // Connect tri combo box
    connect(ui->tri, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Condidat::on_tri_currentIndexChanged);
    
    // Connect recherche text changed
    connect(ui->recherche, &QTextEdit::textChanged, this, &Condidat::on_recherche_textChanged);
    
    // Connect exporter button
    connect(ui->exporter, &QPushButton::clicked, this, &Condidat::on_exporter_clicked);
    
    // Connect statistics combo box
    connect(ui->tri_2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Condidat::on_tri_2_currentIndexChanged);
    
    // Connect voice button
    connect(voiceButton, &QPushButton::clicked, this, &Condidat::on_btn_vocal_clicked);
    
    // Connect candidate combo box for prediction
    connect(ui->cin_condidat, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Condidat::on_cin_condidat_currentIndexChanged);
    
    // Populate candidate combo box from database
    populateCandidateComboBox();
    
    // Initialize sort and filter variables
    currentFilter = QString();
    currentSortColumn = QString();
    currentSortOrder = Qt::AscendingOrder;

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
    
    // Initialize with empty chart
    updateStatisticsChart(0);

    #ifdef USE_VOSK
    // Initialize Vosk lazily (only when needed) to avoid DLL conflicts at startup
    // initVosk(); // Will be called when voice button is clicked
    #endif
    qApp->installEventFilter(this);
}

Condidat::~Condidat()
{
#ifdef USE_VOSK
    freeVosk();
#endif
    qApp->removeEventFilter(this);
    delete ui;
}

void Condidat::on_btn_ajout_clicked()
{
    bool ok = false;
    const QString cinStr = ui->cin->toPlainText().trimmed();
    const QRegularExpression digitsOnly("^\\d{8}$");
    if (!digitsOnly.match(cinStr).hasMatch())
    {
        QMessageBox::warning(this, "Erreur", "CIN doit contenir exactement 8 chiffres.");
        return;
    }
    int cin = cinStr.toInt(&ok);
    if (!ok)
    {
        QMessageBox::warning(this, "Erreur", "CIN invalide.");
        return;
    }
    QString nom = ui->nom->toPlainText().trimmed();
    QString prenom = ui->prenom->toPlainText().trimmed();
    const QRegularExpression namePattern("^[A-Za-zÀ-ÖØ-öø-ÿ\n\r\t '’-]+$");
    if (nom.isEmpty() || !namePattern.match(nom).hasMatch() || nom.contains(QRegularExpression("\\d")))
    {
        QMessageBox::warning(this, "Erreur", "Nom ne doit pas contenir de chiffres et ne peut pas être vide.");
        return;
    }
    if (prenom.isEmpty() || !namePattern.match(prenom).hasMatch() || prenom.contains(QRegularExpression("\\d")))
    {
        QMessageBox::warning(this, "Erreur", "Prénom ne doit pas contenir de chiffres et ne peut pas être vide.");
        return;
    }
    QString sexe = ui->homme->isChecked() ? "H" : (ui->femme->isChecked() ? "F" : "");
    if (sexe.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner le sexe.");
        return;
    }
    QDate date = ui->date->date();
    if (date.addYears(18) > QDate::currentDate())
    {
        QMessageBox::warning(this, "Erreur", "L'âge doit être au minimum 18 ans.");
        return;
    }
    const QString telStr = ui->tel->toPlainText().trimmed();
    if (!digitsOnly.match(telStr).hasMatch())
    {
        QMessageBox::warning(this, "Erreur", "Téléphone doit contenir exactement 8 chiffres.");
        return;
    }
    int tel = telStr.toInt(&ok);
    if (!ok)
    {
        QMessageBox::warning(this, "Erreur", "Téléphone invalide.");
        return;
    }
    QString type = ui->type->currentText();
    if (type.trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner le type de permis.");
        return;
    }

    if (isEditMode)
    {
        int originalCin = currentEditingCin != -1 ? currentEditingCin : selectedCinFromTable();
        if (originalCin == -1)
        {
            QMessageBox::warning(this, "Modification", "Sélectionnez une ligne.");
            return;
        }
        if (modifier(originalCin, cin, nom, prenom, sexe, date, tel, type))
        {
            QMessageBox::information(this, "Modification", "Candidat modifié.");
            refreshTable();
            populateCandidateComboBox(); // Refresh combo box with updated candidate
            setEditMode(false);
        }
        else
        {
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

    if (ajouter())
    {
        QMessageBox::information(this, "Succès", "Candidat ajouté avec succès!");
        refreshTable();
        populateCandidateComboBox(); // Refresh combo box with new candidate
        on_btn_reset_clicked(); // Réinitialiser le formulaire après ajout réussi
    }
    // L'erreur est déjà affichée dans la fonction ajouter()
}

void Condidat::on_btn_reset_clicked()
{
    // Réinitialiser tous les champs du formulaire
    ui->cin->clear();
    ui->nom->clear();
    ui->prenom->clear();
    ui->tel->clear();

    // Réinitialiser les boutons radio (sexe)
    ui->homme->setChecked(false);
    ui->femme->setChecked(false);

    // Réinitialiser la date à une date par défaut (par exemple, il y a 20 ans)
    ui->date->setDate(QDate::currentDate().addYears(-20));

    // Réinitialiser le type de permis (remettre à l'index 0 ou vide)
    if (ui->type->count() > 0)
    {
        ui->type->setCurrentIndex(0);
    }

    // Réinitialiser le mode édition
    setEditMode(false);
}

QSqlQueryModel *Condidat::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT CIN_CONDIDAT, NOM, PRENOM, SEXE, DATE_NAISSANCE, TEL, TYPE_PERMIS_VISE FROM CONDIDAT");

    if (model->lastError().isValid())
    {
        QString errorMsg = model->lastError().text();
        if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist"))
        {
            QMessageBox::critical(this, "Erreur de base de données",
                                  "La table CONDIDAT n'existe pas dans la base de données.\n\n"
                                  "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
        }
    }

    return model;
}

bool Condidat::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO CONDIDAT (CIN_CONDIDAT, NOM, PRENOM, SEXE, DATE_NAISSANCE, TEL, TYPE_PERMIS_VISE) "
                  "VALUES (:cin, :nom, :prenom, :sexe, :date, :tel, :type)");
    query.bindValue(":cin", cin_condidat);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":sexe", sexe);
    query.bindValue(":date", date_naissance);
    query.bindValue(":tel", tel);
    query.bindValue(":type", type_permis);

    if (!query.exec())
    {
        QString errorMsg = query.lastError().text();
        // Vérifier différents types d'erreurs
        if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist"))
        {
            QMessageBox::critical(this, "Erreur de base de données",
                                  "La table CONDIDAT n'existe pas dans la base de données.\n\n"
                                  "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.\n\n"
                                  "Erreur technique: " +
                                      errorMsg);
        }
        else if (errorMsg.contains("ORA-00001") || errorMsg.contains("unique constraint"))
        {
            QMessageBox::warning(this, "Erreur de duplication",
                                 "Un candidat avec ce CIN existe déjà dans la base de données.\n\n"
                                 "Veuillez utiliser un CIN différent.");
        }
        else if (errorMsg.contains("ORA-02290") || errorMsg.contains("check constraint"))
        {
            QMessageBox::warning(this, "Erreur de validation",
                                 "Les données saisies ne respectent pas les contraintes de la base de données.\n\n"
                                 "Vérifiez que:\n"
                                 "- Le sexe est 'H' ou 'F'\n"
                                 "- Tous les champs obligatoires sont remplis\n\n"
                                 "Erreur technique: " +
                                     errorMsg);
        }
        else if (errorMsg.contains("ORA-01400") || errorMsg.contains("cannot insert NULL"))
        {
            QMessageBox::warning(this, "Erreur de saisie",
                                 "Certains champs obligatoires sont vides.\n\n"
                                 "Veuillez remplir tous les champs requis.");
        }
        else
        {
            QMessageBox::critical(this, "Erreur SQL",
                                  "Erreur lors de l'ajout du candidat:\n\n" + errorMsg);
        }
        return false;
    }
    return true;
}

bool Condidat::supprimer(int cin)
{
    QSqlQuery query;
    query.prepare("DELETE FROM CONDIDAT WHERE CIN_CONDIDAT = :cin");
    query.bindValue(":cin", cin);

    if (!query.exec())
    {
        QString errorMsg = query.lastError().text();
        if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist"))
        {
            QMessageBox::critical(this, "Erreur de base de données",
                                  "La table CONDIDAT n'existe pas dans la base de données.\n\n"
                                  "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
        }
        else
        {
            QMessageBox::critical(this, "Erreur SQL",
                                  "Erreur lors de la suppression:\n\n" + errorMsg);
        }
        return false;
    }
    return true;
}

bool Condidat::modifier(int originalCin, int newCin, QString nom, QString prenom, QString sexe, QDate date_naissance, int tel, QString type_permis)
{
    QSqlQuery query;
    query.prepare("UPDATE CONDIDAT SET CIN_CONDIDAT = :newcin, NOM = :nom, PRENOM = :prenom, SEXE = :sexe, DATE_NAISSANCE = :date, TEL = :tel, TYPE_PERMIS_VISE = :type "
                  "WHERE CIN_CONDIDAT = :origcin");
    query.bindValue(":newcin", newCin);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":sexe", sexe);
    query.bindValue(":date", date_naissance);
    query.bindValue(":tel", tel);
    query.bindValue(":type", type_permis);
    query.bindValue(":origcin", originalCin);

    if (!query.exec())
    {
        QString errorMsg = query.lastError().text();
        if (errorMsg.contains("ORA-00942") || errorMsg.contains("table or view does not exist"))
        {
            QMessageBox::critical(this, "Erreur de base de données",
                                  "La table CONDIDAT n'existe pas dans la base de données.\n\n"
                                  "Veuillez exécuter le script SQL 'create_tables_safe.sql' pour créer toutes les tables nécessaires.");
        }
        else if (errorMsg.contains("ORA-00001") || errorMsg.contains("unique constraint"))
        {
            QMessageBox::warning(this, "Erreur de duplication",
                                 "Un candidat avec ce CIN existe déjà dans la base de données.");
        }
        else
        {
            QMessageBox::critical(this, "Erreur SQL",
                                  "Erreur lors de la modification:\n\n" + errorMsg);
        }
        return false;
    }
    return true;
}

void Condidat::refreshTable()
{
    refreshTableWithFilter(currentFilter);
}

void Condidat::refreshTableWithFilter(const QString &filter)
{
    QString queryStr = "SELECT CIN_CONDIDAT, NOM, PRENOM, SEXE, DATE_NAISSANCE, TEL, TYPE_PERMIS_VISE FROM CONDIDAT";
    
    // Add filter if exists
    if (!filter.isEmpty())
    {
        queryStr += " WHERE UPPER(NOM) LIKE UPPER(:filter) OR UPPER(PRENOM) LIKE UPPER(:filter) OR TO_CHAR(CIN_CONDIDAT) LIKE :filter";
    }
    
    // Add sorting if exists
    if (!currentSortColumn.isEmpty())
    {
        queryStr += " ORDER BY " + currentSortColumn;
        if (currentSortOrder == Qt::DescendingOrder)
        {
            queryStr += " DESC";
        }
        else
        {
            queryStr += " ASC";
        }
    }
    
    QSqlQuery query;
    query.prepare(queryStr);
    
    if (!filter.isEmpty())
    {
        QString filterPattern = "%" + filter + "%";
        query.bindValue(":filter", filterPattern);
    }
    
    if (!query.exec())
    {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'affichage des données: " + query.lastError().text());
        return;
    }
    
    // Fill the table
    int rows = 0;
    int cols = 7; // CIN, NOM, PRENOM, SEXE, DATE_NAISSANCE, TEL, TYPE_PERMIS_VISE
    ui->tab->setRowCount(0);
    ui->tab->setColumnCount(cols + 1); // +1 for actions
    QStringList headers;
    headers << "CIN" << "nom" << "Prénom" << "Sexe" << "date de naissance" << "Télephone" << "type de permis" << "Actions";
    ui->tab->setHorizontalHeaderLabels(headers);
    
    while (query.next())
    {
        ui->tab->insertRow(rows);
        for (int c = 0; c < cols; ++c)
        {
            QVariant v = query.value(c);
            ui->tab->setItem(rows, c, new QTableWidgetItem(v.toString()));
        }
        // Add actions cell with Edit/Supp buttons
        bool ok = false;
        int cin = query.value(0).toInt(&ok);
        ui->tab->setCellWidget(rows, cols, createActionsCell(rows, ok ? cin : -1));
        rows++;
    }
}

void Condidat::refreshTableWithSort(const QString &sortColumn, Qt::SortOrder order)
{
    currentSortColumn = sortColumn;
    currentSortOrder = order;
    refreshTableWithFilter(currentFilter);
}

int Condidat::selectedCinFromTable() const
{
    QList<QTableWidgetItem *> items = ui->tab->selectedItems();
    if (items.isEmpty())
        return -1;
    int row = items.first()->row();
    QTableWidgetItem *cinItem = ui->tab->item(row, 0);
    bool ok = false;
    int cin = cinItem ? cinItem->text().toInt(&ok) : -1;
    return ok ? cin : -1;
}

void Condidat::setEditMode(bool enabled)
{
    isEditMode = enabled;
    if (enabled)
    {
        ui->btn_ajout->setText("Edit");
        ui->cin->setReadOnly(true);
    }
    else
    {
        ui->btn_ajout->setText("Ajouter");
        currentEditingCin = -1;
        ui->cin->setReadOnly(false);
    }
}

void Condidat::on_supp_clicked()
{
    int cin = selectedCinFromTable();
    if (cin == -1)
    {
        QMessageBox::warning(this, "Suppression", "Sélectionnez une ligne.");
        return;
    }
    if (supprimer(cin))
    {
        QMessageBox::information(this, "Suppression", "Candidat supprimé.");
        refreshTable();
        populateCandidateComboBox(); // Refresh combo box after deletion
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Échec de suppression.");
    }
}

void Condidat::on_edit_clicked()
{
    int originalCin = currentEditingCin != -1 ? currentEditingCin : selectedCinFromTable();
    if (originalCin == -1)
    {
        QMessageBox::warning(this, "Modification", "Sélectionnez une ligne.");
        return;
    }
    bool ok = false;
    const QString cinStr = ui->cin->toPlainText().trimmed();
    const QRegularExpression digitsOnly("^\\d{8}$");
    if (!digitsOnly.match(cinStr).hasMatch())
    {
        QMessageBox::warning(this, "Erreur", "CIN doit contenir exactement 8 chiffres.");
        return;
    }
    int newCin = cinStr.toInt(&ok);
    if (!ok)
    {
        QMessageBox::warning(this, "Erreur", "CIN invalide.");
        return;
    }
    QString nom = ui->nom->toPlainText().trimmed();
    QString prenom = ui->prenom->toPlainText().trimmed();
    const QRegularExpression namePattern("^[A-Za-zÀ-ÖØ-öø-ÿ\n\r\t '’-]+$");
    if (nom.isEmpty() || !namePattern.match(nom).hasMatch() || nom.contains(QRegularExpression("\\d")))
    {
        QMessageBox::warning(this, "Erreur", "Nom ne doit pas contenir de chiffres et ne peut pas être vide.");
        return;
    }
    if (prenom.isEmpty() || !namePattern.match(prenom).hasMatch() || prenom.contains(QRegularExpression("\\d")))
    {
        QMessageBox::warning(this, "Erreur", "Prénom ne doit pas contenir de chiffres et ne peut pas être vide.");
        return;
    }
    QString sexe = ui->homme->isChecked() ? "H" : (ui->femme->isChecked() ? "F" : "");
    if (sexe.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner le sexe.");
        return;
    }
    QDate date = ui->date->date();
    if (date.addYears(18) > QDate::currentDate())
    {
        QMessageBox::warning(this, "Erreur", "L'âge doit être au minimum 18 ans.");
        return;
    }
    const QString telStr = ui->tel->toPlainText().trimmed();
    if (!digitsOnly.match(telStr).hasMatch())
    {
        QMessageBox::warning(this, "Erreur", "Téléphone doit contenir exactement 8 chiffres.");
        return;
    }
    int tel = telStr.toInt(&ok);
    if (!ok)
    {
        QMessageBox::warning(this, "Erreur", "Téléphone invalide.");
        return;
    }
    QString type = ui->type->currentText();
    if (type.trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner le type de permis.");
        return;
    }
    if (modifier(originalCin, newCin, nom, prenom, sexe, date, tel, type))
    {
        QMessageBox::information(this, "Modification", "Candidat modifié.");
        refreshTable();
        currentEditingCin = -1;
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Échec de modification.");
    }
}

void Condidat::setFormFromRow(int row)
{
    if (row < 0 || row >= ui->tab->rowCount())
        return;
    auto getText = [&](int col)
    { QTableWidgetItem *it = ui->tab->item(row, col); return it ? it->text() : QString(); };
    ui->cin->setText(getText(0));
    ui->nom->setText(getText(1));
    ui->prenom->setText(getText(2));
    QString sexeStr = getText(3).trimmed().toUpper();
    ui->homme->setChecked(sexeStr == "H" || sexeStr == "HOMME");
    ui->femme->setChecked(sexeStr == "F" || sexeStr == "FEMME");
    // Date: try parse ISO or locale
    QDate d = QDate::fromString(getText(4), Qt::ISODate);
    if (!d.isValid())
        d = QDate::fromString(getText(4), "dd/MM/yyyy");
    if (d.isValid())
        ui->date->setDate(d);
    ui->tel->setText(getText(5));
    int typeIndex = ui->type->findText(getText(6));
    if (typeIndex >= 0)
        ui->type->setCurrentIndex(typeIndex);
}

QWidget *Condidat::createActionsCell(int row, int cin)
{
    QWidget *container = new QWidget(ui->tab);
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
        if (cin == -1) return;
        if (QMessageBox::question(this, "Suppression", "Supprimer ce candidat ?") == QMessageBox::Yes) {
            if (supprimer(cin)) {
                QMessageBox::information(this, "Suppression", "Condidat supprimé.");
                refreshTable();
                populateCandidateComboBox(); // Refresh combo box after deletion
            } else {
                QMessageBox::critical(this, "Erreur", "Échec de suppression.");
            }
        } });

    // Edit action: toggle edit mode. If clicking again on same row, exit edit mode
    QObject::connect(btnEdit, &QPushButton::clicked, this, [=]()
                     {
        if (isEditMode && currentEditingCin == cin) {
            setEditMode(false);
            return;
        }
        setFormFromRow(row);
        currentEditingCin = cin;
        setEditMode(true); });

    return container;
}

void Condidat::navigateToCandidat()
{
    // Already in candidat view
}

void Condidat::navigateToPlanning()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_PLANNING);
    }
}

void Condidat::navigateToEmployee()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_EMPLOYES);
    }
}

void Condidat::navigateToVehicule()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_VEHICULE);
    }
}

void Condidat::navigateToExamen()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_EXAMEN);
    }
}

void Condidat::navigateToEquipement()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_EQUIPEMENT);
    }
}

void Condidat::on_tri_currentIndexChanged(int index)
{
    switch(index)
    {
        case 0: // No sort
            currentSortColumn = QString();
            currentSortOrder = Qt::AscendingOrder;
            break;
        case 1: // nom A à Z
            refreshTableWithSort("NOM", Qt::AscendingOrder);
            return;
        case 2: // nom Z à A
            refreshTableWithSort("NOM", Qt::DescendingOrder);
            return;
        case 3: // age croissant (plus jeune en premier)
            refreshTableWithSort("DATE_NAISSANCE", Qt::DescendingOrder);
            return;
        case 4: // age décroissant (plus vieux en premier)
            refreshTableWithSort("DATE_NAISSANCE", Qt::AscendingOrder);
            return;
        default:
            currentSortColumn = QString();
            currentSortOrder = Qt::AscendingOrder;
            break;
    }
    refreshTableWithFilter(currentFilter);
}

void Condidat::on_recherche_textChanged()
{
    QString searchText = ui->recherche->toPlainText().trimmed();
    currentFilter = searchText;
    refreshTableWithFilter(currentFilter);
}

void Condidat::on_exporter_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Exporter les candidats");
    msgBox.setText("Choisissez le format d'export:");
    msgBox.setIcon(QMessageBox::Question);
    
    QPushButton *pdfButton = msgBox.addButton("PDF", QMessageBox::ActionRole);
    QPushButton *excelButton = msgBox.addButton("Excel (CSV)", QMessageBox::ActionRole);
    msgBox.addButton("Annuler", QMessageBox::RejectRole);
    
    msgBox.exec();
    
    if (msgBox.clickedButton() == pdfButton)
    {
        exportToPDF();
    }
    else if (msgBox.clickedButton() == excelButton)
    {
        exportToExcel();
    }
}

void Condidat::exportToPDF()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive))
        fileName += ".pdf";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize::A4);
    printer.setPageMargins(QMarginsF(20, 20, 20, 20), QPageLayout::Millimeter);

    QTextDocument document;
    QString html = "<html><head><style>"
                   "table { border-collapse: collapse; width: 100%; margin: 20px 0; }"
                   "th { background-color: #002157; color: #ffce00; padding: 10px; text-align: left; border: 1px solid #ddd; }"
                   "td { padding: 8px; border: 1px solid #ddd; }"
                   "tr:nth-child(even) { background-color: #f2f2f2; }"
                   "h1 { color: #002157; text-align: center; }"
                   ".header { margin-bottom: 20px; }"
                   "</style></head><body>";
    html += "<div class='header'>";
    html += "<h1>Liste des Candidats</h1>";
    html += "<p><strong>Date d'export:</strong> " + QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") + "</p>";
    html += "</div>";
    html += "<table>";
    html += "<tr><th>CIN</th><th>Nom</th><th>Prénom</th><th>Sexe</th><th>Date de naissance</th><th>Âge</th><th>Téléphone</th><th>Type de permis</th></tr>";

    // Get ALL data directly from database (ignore filters and search)
    QSqlQuery query;
    QString queryStr = "SELECT CIN_CONDIDAT, NOM, PRENOM, SEXE, DATE_NAISSANCE, TEL, TYPE_PERMIS_VISE FROM CONDIDAT ORDER BY NOM, PRENOM";

    query.prepare(queryStr);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des données: " + query.lastError().text());
        return;
    }

    int totalCandidates = 0;
    while (query.next())
    {
        html += "<tr>";

        // CIN
        QString cin = query.value(0).toString();
        html += "<td>" + cin.toHtmlEscaped() + "</td>";

        // Nom
        QString nom = query.value(1).toString();
        html += "<td>" + nom.toHtmlEscaped() + "</td>";

        // Prénom
        QString prenom = query.value(2).toString();
        html += "<td>" + prenom.toHtmlEscaped() + "</td>";

        // Sexe
        QString sexe = query.value(3).toString();
        html += "<td>" + sexe.toHtmlEscaped() + "</td>";

        // Date de naissance
        QDate birthDate = query.value(4).toDate();
        QString birthDateStr = birthDate.toString("dd/MM/yyyy");
        html += "<td>" + birthDateStr.toHtmlEscaped() + "</td>";

        // Âge calculé
        int age = calculateAge(birthDate);
        html += "<td>" + QString::number(age) + " ans</td>";

        // Téléphone
        QString tel = query.value(5).toString();
        html += "<td>" + tel.toHtmlEscaped() + "</td>";

        // Type de permis
        QString typePermis = query.value(6).toString();
        html += "<td>" + typePermis.toHtmlEscaped() + "</td>";

        html += "</tr>";
        totalCandidates++;
    }

    html += "</table>";

    // Add summary
    html += "<div style='margin-top: 20px; padding: 10px; background-color: #f8f9fa; border-radius: 5px;'>";
    html += "<p><strong>Total des candidats:</strong> " + QString::number(totalCandidates) + "</p>";

    // Add statistics if available
    if (totalCandidates > 0)
    {
        // Count by sex
        QSqlQuery sexQuery;
        sexQuery.prepare("SELECT SEXE, COUNT(*) FROM CONDIDAT GROUP BY SEXE");
        if (sexQuery.exec())
        {
            html += "<p><strong>Répartition par sexe:</strong> ";
            bool first = true;
            while (sexQuery.next())
            {
                if (!first) html += ", ";
                QString sexe = sexQuery.value(0).toString();
                int count = sexQuery.value(1).toInt();
                double percentage = (count * 100.0) / totalCandidates;
                html += sexe + ": " + QString::number(count) + " (" + QString::number(percentage, 'f', 1) + "%)";
                first = false;
            }
            html += "</p>";
        }

        // Count by license type
        QSqlQuery typeQuery;
        typeQuery.prepare("SELECT TYPE_PERMIS_VISE, COUNT(*) FROM CONDIDAT GROUP BY TYPE_PERMIS_VISE");
        if (typeQuery.exec())
        {
            html += "<p><strong>Répartition par type de permis:</strong> ";
            bool first = true;
            while (typeQuery.next())
            {
                if (!first) html += ", ";
                QString type = typeQuery.value(0).toString();
                int count = typeQuery.value(1).toInt();
                double percentage = (count * 100.0) / totalCandidates;
                html += type + ": " + QString::number(count) + " (" + QString::number(percentage, 'f', 1) + "%)";
                first = false;
            }
            html += "</p>";
        }
    }

    html += "</div>";
    html += "</body></html>";

    document.setHtml(html);
    document.print(&printer);

    QMessageBox::information(this, "Export PDF",
                             "Les candidats ont été exportés avec succès en PDF:\n" + fileName +
                                 "\n\nTotal: " + QString::number(totalCandidates) + " candidats");
}

void Condidat::exportToExcel()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en Excel", "", "Fichiers CSV (*.csv)");
    if (fileName.isEmpty())
        return;
    
    if (!fileName.endsWith(".csv", Qt::CaseInsensitive))
        fileName += ".csv";
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le fichier: " + fileName);
        return;
    }
    
    QTextStream out(&file);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    out.setEncoding(QStringConverter::Utf8);
#else
    out.setCodec("UTF-8");
#endif
    
    // Write BOM for UTF-8 to ensure Excel recognizes it correctly
    out << "\xEF\xBB\xBF";
    
    // Write header
    out << "CIN,Nom,Prénom,Sexe,Date de naissance,Âge,Téléphone,Type de permis\n";
    
    // Get ALL data directly from database (ignore filters and search)
    QSqlQuery query;
    QString queryStr = "SELECT CIN_CONDIDAT, NOM, PRENOM, SEXE, DATE_NAISSANCE, TEL, TYPE_PERMIS_VISE FROM CONDIDAT ORDER BY NOM, PRENOM";
    
    query.prepare(queryStr);
    
    if (!query.exec())
    {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des données: " + query.lastError().text());
        file.close();
        return;
    }
    
    // Write data from database
    int totalCandidates = 0;
    while (query.next())
    {
        // CIN
        QString cin = query.value(0).toString();
        out << "\"" << cin << "\",";
        
        // Nom
        QString nom = query.value(1).toString();
        out << "\"" << nom << "\",";
        
        // Prénom
        QString prenom = query.value(2).toString();
        out << "\"" << prenom << "\",";
        
        // Sexe
        QString sexe = query.value(3).toString();
        out << "\"" << sexe << "\",";
        
        // Date de naissance
        QDate birthDate = query.value(4).toDate();
        QString birthDateStr = birthDate.toString("dd/MM/yyyy");
        out << "\"" << birthDateStr << "\",";
        
        // Âge calculé
        int age = calculateAge(birthDate);
        out << "\"" << QString::number(age) << " ans\",";
        
        // Téléphone
        QString tel = query.value(5).toString();
        out << "\"" << tel << "\",";
        
        // Type de permis
        QString typePermis = query.value(6).toString();
        out << "\"" << typePermis << "\"";
        
        out << "\n";
        totalCandidates++;
    }
    
    file.close();
    
    QMessageBox::information(this, "Export CSV",
                             "Les candidats ont été exportés avec succès en CSV:\n" + fileName +
                                 "\n\nTotal: " + QString::number(totalCandidates) + " candidats\n\nCe fichier peut être ouvert dans Microsoft Excel.");
}

int Condidat::calculateAge(const QDate &birthDate) const
{
    if (!birthDate.isValid())
        return 0;
    
    QDate today = QDate::currentDate();
    int age = today.year() - birthDate.year();
    
    // Check if birthday hasn't occurred this year
    if (today.month() < birthDate.month() || 
        (today.month() == birthDate.month() && today.day() < birthDate.day()))
    {
        age--;
    }
    
    return age;
}

void Condidat::on_tri_2_currentIndexChanged(int index)
{
    updateStatisticsChart(index);
}

void Condidat::updateStatisticsChart(int index)
{
    if (index == 1) // Sexe
    {
        createSexeChart();
    }
    else if (index == 2) // Type de permis
    {
        createLicenseTypeChart();
    }
    else
    {
        // Clear the label for default selection
        ui->statistique->clear();
        ui->statistique->setText("Sélectionnez une statistique");
    }
}

QPixmap Condidat::createPieChartPixmap(const QMap<QString, int> &data, const QString &title, const QSize &size)
{
    QPixmap pixmap(size);
    pixmap.fill(Qt::white);
    
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Calculate total
    int total = 0;
    for (auto it = data.begin(); it != data.end(); ++it)
    {
        total += it.value();
    }
    
    if (total == 0)
    {
        painter.setFont(QFont("Arial", 12));
        painter.drawText(pixmap.rect(), Qt::AlignCenter, "Aucune donnée disponible");
        painter.end();
        return pixmap;
    }
    
    // Define colors
    QList<QColor> colors = {
        QColor("#ffce00"),  // Yellow
        QColor("#002157"),  // Dark blue
        QColor("#4CAF50"),  // Green
        QColor("#FF5722"),  // Orange
        QColor("#9C27B0")   // Purple
    };
    
    // Draw title
    QFont titleFont("Arial", 11, QFont::Bold);
    painter.setFont(titleFont);
    QRect titleRect(10, 10, size.width() - 20, 25);
    painter.drawText(titleRect, Qt::AlignCenter, title);
    
    // Calculate pie chart area (leave space for title and legend)
    int margin = 20;
    int legendHeight = data.size() * 25 + 10;
    int chartSize = qMin(size.width() - 2 * margin, size.height() - 80 - legendHeight);
    int chartX = (size.width() - chartSize) / 2;
    int chartY = 50;
    QRectF chartRect(chartX, chartY, chartSize, chartSize);
    
    // Draw pie slices
    double startAngle = 90 * 16; // Start at top (90 degrees in 1/16th of a degree units)
    int colorIndex = 0;
    
    for (auto it = data.begin(); it != data.end(); ++it)
    {
        if (it.value() > 0)
        {
            double percentage = (it.value() * 360.0 * 16) / total;
            double spanAngle = percentage;
            
            QColor color = colors[colorIndex % colors.size()];
            painter.setBrush(QBrush(color));
            painter.setPen(QPen(Qt::white, 2));
            
            painter.drawPie(chartRect, startAngle, spanAngle);
            
            startAngle += spanAngle;
            colorIndex++;
        }
    }
    
    // Draw legend
    int legendY = chartY + chartSize + 20;
    int legendX = 20;
    colorIndex = 0;
    painter.setFont(QFont("Arial", 9));
    
    for (auto it = data.begin(); it != data.end(); ++it)
    {
        if (it.value() > 0)
        {
            QColor color = colors[colorIndex % colors.size()];
            double percentage = (it.value() * 100.0) / total;
            QString label = QString("%1: %2 (%3%)")
                           .arg(it.key())
                           .arg(it.value())
                           .arg(QString::number(percentage, 'f', 1));
            
            // Draw color box
            QRect colorRect(legendX, legendY, 15, 15);
            painter.setBrush(QBrush(color));
            painter.setPen(QPen(Qt::black, 1));
            painter.drawRect(colorRect);
            
            // Draw label
            painter.setPen(QPen(Qt::black));
            painter.drawText(legendX + 20, legendY + 12, label);
            
            legendY += 25;
            colorIndex++;
        }
    }
    
    painter.end();
    return pixmap;
}

void Condidat::createSexeChart()
{
    QSqlQuery query;
    query.prepare("SELECT SEXE FROM CONDIDAT");
    
    if (!query.exec())
    {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des données: " + query.lastError().text());
        return;
    }
    
    // Count by sexe
    QMap<QString, int> sexeCounts;
    sexeCounts["Homme"] = 0;
    sexeCounts["Femme"] = 0;
    
    int total = 0;
    while (query.next())
    {
        QString sexe = query.value(0).toString().trimmed().toUpper();
        if (!sexe.isEmpty())
        {
            total++;
            if (sexe == "H" || sexe == "HOMME" || sexe == "M")
                sexeCounts["Homme"]++;
            else if (sexe == "F" || sexe == "FEMME")
                sexeCounts["Femme"]++;
        }
    }
    
    if (total == 0)
    {
        ui->statistique->setText("Aucune donnée disponible");
        return;
    }
    
    // Create and display chart
    QSize labelSize = ui->statistique->size();
    QString title = QString("Répartition par sexe - Total: %1 candidats").arg(total);
    QPixmap pixmap = createPieChartPixmap(sexeCounts, title, labelSize);
    ui->statistique->setPixmap(pixmap);
    ui->statistique->setScaledContents(true);
}

void Condidat::createLicenseTypeChart()
{
    QSqlQuery query;
    query.prepare("SELECT TYPE_PERMIS_VISE FROM CONDIDAT");
    
    if (!query.exec())
    {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des données: " + query.lastError().text());
        return;
    }
    
    // Count by license type
    QMap<QString, int> licenseCounts;
    int total = 0;
    
    while (query.next())
    {
        QString type = query.value(0).toString().trimmed().toLower();
        if (!type.isEmpty())
        {
            total++;
            // Normalize the type (capitalize first letter)
            if (type == "voiture")
                licenseCounts["Voiture"]++;
            else if (type == "moto")
                licenseCounts["Moto"]++;
            else if (type == "poids lourd" || type == "poids_lourd")
                licenseCounts["Poids lourd"]++;
            else
                licenseCounts[type]++; // Keep original if not recognized
        }
    }
    
    if (total == 0)
    {
        ui->statistique->setText("Aucune donnée disponible");
        return;
    }
    
    // Create and display chart
    QSize labelSize = ui->statistique->size();
    QString title = QString("Répartition par type de permis - Total: %1 candidats").arg(total);
    QPixmap pixmap = createPieChartPixmap(licenseCounts, title, labelSize);
    ui->statistique->setPixmap(pixmap);
    ui->statistique->setScaledContents(true);
}

void Condidat::on_btn_vocal_clicked()
{
    if (isRecording)
    {
        stopVoiceRecording();
        return;
    }
    
#ifdef USE_VOSK
    // Check if Vosk is ready before starting
    if (!voskReady)
    {
        QMessageBox::warning(this, "Reconnaissance vocale", 
                            "Le modèle Vosk n'est pas chargé.\n\n"
                            "Vérifiez que:\n"
                            "1. Le modèle 'vosk-model-small-fr-0.22' est présent dans le répertoire du projet\n"
                            "2. Les fichiers du modèle sont complets (am/, graph/, conf/, ivector/)\n"
                            "3. La bibliothèque Vosk est correctement configurée");
        return;
    }
#endif
    
    if (!ui->cin->toPlainText().isEmpty() || 
        !ui->nom->toPlainText().isEmpty() || 
        !ui->prenom->toPlainText().isEmpty())
    {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, 
            "Formulaire déjà rempli", 
            "Le formulaire contient déjà des données. Voulez-vous les remplacer par la saisie vocale?",
            QMessageBox::Yes | QMessageBox::No
        );
        if (reply == QMessageBox::No)
            return;
    }
    startVoiceRecording();
}

void Condidat::startVoiceRecording()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QMediaDevices devices;
    QAudioDevice device = devices.defaultAudioInput();
    if (device.isNull())
    {
        QMessageBox::warning(this, "Erreur", "Aucun microphone détecté.");
        return;
    }
    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);
    if (!device.isFormatSupported(format))
        format = device.preferredFormat();
    audioSource = new QAudioSource(device, format, this);
#else
    QAudioDeviceInfo deviceInfo = QAudioDeviceInfo::defaultInputDevice();
    if (deviceInfo.isNull())
    {
        QMessageBox::warning(this, "Erreur", "Aucun microphone détecté.");
        return;
    }
    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
    if (!deviceInfo.isFormatSupported(format))
        format = deviceInfo.nearestFormat(format);
    audioInput = new QAudioInput(format, this);
#endif

    if (audioBuffer)
    {
        audioBuffer->close();
        audioBuffer->deleteLater();
    }
    audioBuffer = new QBuffer(this);
    audioBuffer->open(QIODevice::ReadWrite);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    audioSource->start(audioBuffer);
#else
    audioInput->start(audioBuffer);
#endif

#ifdef USE_VOSK
    // Prepare Vosk recognizer for streaming
    if (voskReady)
    {
        // free any previous recognizer
        if (voskRecognizer) {
            vosk_recognizer_free(voskRecognizer);
            voskRecognizer = nullptr;
            voskProcessedBytes = 0;
        }
        VoskModel *model = reinterpret_cast<VoskModel*>(this->property("_vosk_model_ptr").toULongLong());
        if (model) {
            voskRecognizer = vosk_recognizer_new(model, 16000.0);
            if (voskPollTimer) {
                voskPollTimer->stop();
                voskPollTimer->deleteLater();
                voskPollTimer = nullptr;
            }
            voskPollTimer = new QTimer(this);
            voskPollTimer->setInterval(100);
            connect(voskPollTimer, &QTimer::timeout, this, [this]() {
                if (!audioBuffer || !voskRecognizer) return;
                qint64 available = audioBuffer->size() - voskProcessedBytes;
                if (available <= 0) return;
                audioBuffer->seek(voskProcessedBytes);
                QByteArray chunk = audioBuffer->read(static_cast<int>(available));
                voskProcessedBytes += chunk.size();
                int accepted = vosk_recognizer_accept_waveform(voskRecognizer, chunk.constData(), chunk.size());
                if (accepted) {
                    QByteArray res(vosk_recognizer_result(voskRecognizer));
                    QJsonDocument doc = QJsonDocument::fromJson(res);
                    QString text = doc.isObject() ? doc.object().value("text").toString() : QString();
                    if (!text.isEmpty()) processVoiceText(text);
                } else {
                    const char* pres = vosk_recognizer_partial_result(voskRecognizer);
                    if (pres) {
                        QByteArray partialBytes(pres);
                        QJsonDocument partialDoc = QJsonDocument::fromJson(partialBytes);
                        if (partialDoc.isObject()) {
                            QString partial = partialDoc.object().value("partial").toString();
                            if (!partial.isEmpty()) {
                                qDebug() << "Vosk partial:" << partial;
                                // Optionally update UI with partial result
                            }
                        }
                    }
                }
            });
            voskPollTimer->start();
        }
    }
#endif

    isRecording = true;
    voiceButton->setText("● Enregistrement...");
    voiceButton->setStyleSheet("background-color:#FF5722;color:white;border-radius:10px;font-weight:bold;");
    voiceButton->setEnabled(true);
    recordingTimer->start(10000);
    voiceButton->setToolTip("Enregistrement en cours... Cliquez pour arrêter");
}

void Condidat::stopVoiceRecording()
{
    isRecording = false;
    if (recordingTimer)
        recordingTimer->stop();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if (audioSource)
        audioSource->stop();
#else
    if (audioInput)
        audioInput->stop();
#endif
    QByteArray pcmData;
    if (audioBuffer)
    {
        pcmData = audioBuffer->buffer();
        audioBuffer->close();
    }

#ifdef USE_VOSK
    if (voskPollTimer) {
        voskPollTimer->stop();
        voskPollTimer->deleteLater();
        voskPollTimer = nullptr;
    }
    if (voskRecognizer)
    {
        // Feed remaining bytes if any
        if (!pcmData.isEmpty() && voskProcessedBytes < pcmData.size()) {
            qint64 remaining = pcmData.size() - voskProcessedBytes;
            if (remaining > 0) {
                QByteArray remainingData = pcmData.mid(static_cast<int>(voskProcessedBytes));
                // Remove WAV header if present
                int idx = remainingData.indexOf("data");
                if (idx != -1 && idx + 8 < remainingData.size()) {
                    remainingData = remainingData.mid(idx + 8);
                }
                if (!remainingData.isEmpty()) {
                    vosk_recognizer_accept_waveform(voskRecognizer, remainingData.constData(), remainingData.size());
                }
            }
        }
        
        // Get final result
        const char* finalRes = vosk_recognizer_final_result(voskRecognizer);
        if (finalRes) {
            QByteArray finalBytes(finalRes);
            QJsonDocument doc = QJsonDocument::fromJson(finalBytes);
            if (doc.isObject()) {
                QString text = doc.object().value("text").toString();
                if (!text.isEmpty()) {
                    qDebug() << "Vosk final result:" << text;
                    processVoiceText(text);
                } else {
                    qDebug() << "Vosk returned empty text - no speech detected";
                }
            }
        }
        
        vosk_recognizer_free(voskRecognizer);
        voskRecognizer = nullptr;
        voskProcessedBytes = 0;
    }
#endif

    // Reset button state
    voiceButton->setText("🎤 Remplir par voix");
    voiceButton->setStyleSheet("background-color:#002157;color:#ffce00;border-radius:10px;");
    voiceButton->setEnabled(true);
    voiceButton->setToolTip("Cliquez pour parler");
    
    if (!pcmData.isEmpty())
    {
#ifdef USE_VOSK
        // Vosk processing already handled above
        if (!voskReady)
        {
            QMessageBox::warning(this, "Reconnaissance vocale", 
                                "Le modèle Vosk n'est pas chargé.\n"
                                "Vérifiez que le modèle 'vosk-model-small-fr-0.22' est disponible.");
        }
#else
        QMessageBox::warning(this, "Reconnaissance vocale", 
                            "La reconnaissance vocale n'est pas disponible.\n"
                            "Veuillez compiler avec la bibliothèque Vosk pour activer cette fonctionnalité.");
#endif
    }
}

void Condidat::processVoiceText(const QString &text)
{
    if (text.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Le texte est vide.");
        return;
    }
    
    // Parse and fill form
    parseAndFillForm(text);
    
    // Show summary of what was filled
    QString summary = "✅ Formulaire rempli automatiquement!\n\n";
    summary += "Données extraites:\n";
    
    if (!ui->cin->toPlainText().isEmpty())
        summary += "• CIN: " + ui->cin->toPlainText() + "\n";
    if (!ui->nom->toPlainText().isEmpty())
        summary += "• Nom: " + ui->nom->toPlainText() + "\n";
    if (!ui->prenom->toPlainText().isEmpty())
        summary += "• Prénom: " + ui->prenom->toPlainText() + "\n";
    if (ui->homme->isChecked() || ui->femme->isChecked())
        summary += "• Sexe: " + QString(ui->homme->isChecked() ? "Homme" : "Femme") + "\n";
    if (ui->date->date().isValid())
        summary += "• Date: " + ui->date->date().toString("dd/MM/yyyy") + "\n";
    if (!ui->tel->toPlainText().isEmpty())
        summary += "• Téléphone: " + ui->tel->toPlainText() + "\n";
    if (ui->type->currentIndex() > 0)
        summary += "• Type de permis: " + ui->type->currentText() + "\n";
    
    summary += "\nVérifiez les informations et modifiez si nécessaire.";
    
    QMessageBox::information(this, "Succès", summary);
    voiceButton->setText("🎤 Remplir par voix");
    voiceButton->setStyleSheet("background-color:#002157;color:#ffce00;border-radius:10px;");
    voiceButton->setEnabled(true);
    voiceButton->setToolTip("Cliquez pour parler");
}

void Condidat::parseAndFillForm(const QString &text)
{
    QString lowerText = text.toLower();
    
    // Extract CIN
    QString cin = extractCIN(lowerText);
    if (!cin.isEmpty())
    {
        ui->cin->setText(cin);
    }
    
    // Extract Nom
    QString nom = extractName(lowerText, false);
    if (!nom.isEmpty())
    {
        ui->nom->setText(nom);
    }
    
    // Extract Prénom
    QString prenom = extractName(lowerText, true);
    if (!prenom.isEmpty())
    {
        ui->prenom->setText(prenom);
    }
    
    // Extract Sexe
    QString sexe = extractSexe(lowerText);
    if (!sexe.isEmpty())
    {
        if (sexe == "H" || sexe == "homme")
            ui->homme->setChecked(true);
        else if (sexe == "F" || sexe == "femme")
            ui->femme->setChecked(true);
    }
    
    // Extract Date
    QDate date = extractDate(lowerText);
    if (date.isValid())
    {
        ui->date->setDate(date);
    }
    
    // Extract Téléphone
    QString tel = extractPhone(lowerText);
    if (!tel.isEmpty())
    {
        ui->tel->setText(tel);
    }
    
    // Extract Type de permis
    QString typePermis = extractLicenseType(lowerText);
    if (!typePermis.isEmpty())
    {
        int index = ui->type->findText(typePermis, Qt::MatchFixedString);
        if (index >= 0)
        {
            ui->type->setCurrentIndex(index);
        }
    }
}

QString Condidat::extractCIN(const QString &text)
{
    // Look for "cin" followed by 8 digits
    QRegularExpression re("cin\\s*(?:est|:)?\\s*(\\d{8})", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = re.match(text);
    if (match.hasMatch())
    {
        return match.captured(1);
    }
    
    // Alternative: look for 8 consecutive digits
    QRegularExpression re2("(\\d{8})");
    QRegularExpressionMatch match2 = re2.match(text);
    if (match2.hasMatch())
    {
        return match2.captured(1);
    }
    
    return QString();
}

QString Condidat::extractName(const QString &text, bool isFirstName)
{
    QString keyword = isFirstName ? "prénom" : "nom";

    // Use raw-string literals for complex regex to avoid escape-sequence warnings
    QRegularExpression re(QString(R"(%1\s*(?:est|:)?\s*([a-zA-ZÀ-ÖØ-öø-ÿ'’\-\s]+))").arg(keyword), QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = re.match(text);
    if (match.hasMatch())
    {
        QString name = match.captured(1).trimmed();
        // Stop at likely next keyword
        name = name.split(QRegularExpression(R"(\s+(?:sexe|date|t[ée]l[ée]phone|tel|type|cin))" , QRegularExpression::CaseInsensitiveOption))[0].trimmed();
        if (!name.isEmpty() && name.length() > 1)
        {
            // Capitalize each word while preserving accents
            QStringList parts = name.split(QRegularExpression(R"([\s\-]+)"), Qt::SkipEmptyParts);
            for (int i = 0; i < parts.size(); ++i)
            {
                QString p = parts.at(i).toLower();
                QString first = p.left(1).toUpper();
                QString rest = p.mid(1);
                parts[i] = first + rest;
            }
            return parts.join(" ");
        }
    }

    return QString();
}

QString Condidat::extractSexe(const QString &text)
{
    QRegularExpression reH(R"(\b(homme|masculin|m)\b)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression reF(R"(\b(femme|f[eé]minin|f)\b)", QRegularExpression::CaseInsensitiveOption);

    if (reH.match(text).hasMatch())
        return "H";
    if (reF.match(text).hasMatch())
        return "F";
    return QString();
}

QString Condidat::extractPhone(const QString &text)
{
    QRegularExpression re(R"((?:téléphone|tel|phone)\s*(?:est|:)?\s*(\d{8}))", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = re.match(text);
    if (match.hasMatch())
    {
        return match.captured(1);
    }

    QRegularExpression re2(R"((?:0|\+216)?\s*(\d{8}))");
    QRegularExpressionMatch match2 = re2.match(text);
    if (match2.hasMatch())
    {
        return match2.captured(1);
    }

    return QString();
}

QString Condidat::extractLicenseType(const QString &text)
{
    QRegularExpression rVoiture(R"(\b(voiture|auto|permis\s*b|b)\b)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression rMoto(R"(\b(moto|permis\s*a|a)\b)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression rPL(R"(\b(poids\s*lourd|poids_lourd|camion|permis\s*c|c)\b)", QRegularExpression::CaseInsensitiveOption);

    if (rVoiture.match(text).hasMatch())
        return "voiture";
    if (rMoto.match(text).hasMatch())
        return "moto";
    if (rPL.match(text).hasMatch())
        return "poids lourd";

    if (text.contains("voiture") || text.contains("auto"))
        return "voiture";
    if (text.contains("moto"))
        return "moto";
    if (text.contains("poids lourd") || text.contains("camion"))
        return "poids lourd";

    return QString();
}

QDate Condidat::extractDate(const QString &text)
{
    QRegularExpression re(R"((\d{1,2})\s*(?:/|-|\s)\s*(\d{1,2}|janvier|février|mars|avril|mai|juin|juillet|août|septembre|octobre|novembre|décembre)\s*(?:/|-|\s)\s*(\d{4}))", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = re.match(text);

    if (match.hasMatch())
    {
        int day = match.captured(1).toInt();
        QString monthStr = match.captured(2).toLower();
        int year = match.captured(3).toInt();

        int month = 1;
        if (monthStr.contains("janvier")) month = 1;
        else if (monthStr.contains("février")) month = 2;
        else if (monthStr.contains("mars")) month = 3;
        else if (monthStr.contains("avril")) month = 4;
        else if (monthStr.contains("mai")) month = 5;
        else if (monthStr.contains("juin")) month = 6;
        else if (monthStr.contains("juillet")) month = 7;
        else if (monthStr.contains("août")) month = 8;
        else if (monthStr.contains("septembre")) month = 9;
        else if (monthStr.contains("octobre")) month = 10;
        else if (monthStr.contains("novembre")) month = 11;
        else if (monthStr.contains("décembre")) month = 12;
        else
            month = monthStr.toInt();

        QDate date(year, month, day);
        if (date.isValid())
        {
            return date;
        }
    }

    QRegularExpression re2(R"(date\s*(?:de\s*naissance)?\s*(?:est|:)?\s*(\d{1,2})[\s/\-](\d{1,2})[\s/\-](\d{4}))", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match2 = re2.match(text);
    if (match2.hasMatch())
    {
        int day = match2.captured(1).toInt();
        int month = match2.captured(2).toInt();
        int year = match2.captured(3).toInt();
        QDate date(year, month, day);
        if (date.isValid())
        {
            return date;
        }
    }

    return QDate();
}

QByteArray Condidat::createWavFromPcm(const QByteArray &pcm, int sampleRate, int channels, int sampleSizeBits)
{
    QByteArray wav;
    // WAV header fields
    quint32 dataSize = static_cast<quint32>(pcm.size());
    quint32 fileSize = 36 + dataSize;
    quint16 audioFormat = 1; // PCM
    quint16 numChannels = static_cast<quint16>(channels);
    quint32 sampleRateU = static_cast<quint32>(sampleRate);
    quint32 byteRate = sampleRateU * numChannels * (sampleSizeBits / 8);
    quint16 blockAlign = static_cast<quint16>(numChannels * (sampleSizeBits / 8));
    quint16 bitsPerSample = static_cast<quint16>(sampleSizeBits);

    // RIFF header
    wav.append("RIFF");
    wav.append(reinterpret_cast<const char *>(&fileSize), 4);
    wav.append("WAVE");

    // fmt chunk
    wav.append("fmt ");
    quint32 fmtChunkSize = 16;
    wav.append(reinterpret_cast<const char *>(&fmtChunkSize), 4);
    wav.append(reinterpret_cast<const char *>(&audioFormat), 2);
    wav.append(reinterpret_cast<const char *>(&numChannels), 2);
    wav.append(reinterpret_cast<const char *>(&sampleRateU), 4);
    wav.append(reinterpret_cast<const char *>(&byteRate), 4);
    wav.append(reinterpret_cast<const char *>(&blockAlign), 2);
    wav.append(reinterpret_cast<const char *>(&bitsPerSample), 2);

    // data chunk
    wav.append("data");
    wav.append(reinterpret_cast<const char *>(&dataSize), 4);
    wav.append(pcm);

    return wav;
}

#ifdef USE_VOSK
QString Condidat::locateVoskModelPath() const
{
    // Check environment variable first
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString path = env.value("VOSK_MODEL_PATH").trimmed();
    if (!path.isEmpty() && QDir(path).exists())
    {
        qDebug() << "Vosk model found in VOSK_MODEL_PATH:" << path;
        return path;
    }
    
    // Check relative to application directory
    QString alt = QCoreApplication::applicationDirPath() + "/models/fr";
    if (QDir(alt).exists())
    {
        qDebug() << "Vosk model found in application directory:" << alt;
        return alt;
    }
    
    // Check in project root directory (for development)
    QString projectPath = QDir::currentPath() + "/vosk-model-small-fr-0.22";
    if (QDir(projectPath).exists())
    {
        qDebug() << "Vosk model found in project directory:" << projectPath;
        return projectPath;
    }
    
    // Check common installation paths
    QStringList commonPaths = {
        QDir::homePath() + "/vosk-model-small-fr-0.22",
        QDir::homePath() + "/models/vosk-model-small-fr-0.22",
        "C:/vosk-model-small-fr-0.22",
        "C:/models/vosk-model-small-fr-0.22"
    };
    
    for (const QString &commonPath : commonPaths)
    {
        if (QDir(commonPath).exists())
        {
            qDebug() << "Vosk model found in common path:" << commonPath;
            return commonPath;
        }
    }
    
    qDebug() << "Vosk model not found in any standard location";
    qDebug() << "Searched in:" << projectPath;
    return QString();
}

void Condidat::initVosk()
{
    voskReady = false;
    
    // Set log level to reduce console output
    vosk_set_log_level(0);
    
    QString modelPath = locateVoskModelPath();
    if (modelPath.isEmpty())
    {
        qDebug() << "Vosk model not found. Voice recognition will be disabled.";
        qDebug() << "Please ensure vosk-model-small-fr-0.22 is available.";
        return;
    }
    
    // Verify model directory structure
    QDir modelDir(modelPath);
    if (!modelDir.exists("am") || !modelDir.exists("graph") || !modelDir.exists("conf"))
    {
        qDebug() << "Invalid Vosk model structure at" << modelPath;
        return;
    }
    
    VoskModel *model = vosk_model_new(modelPath.toUtf8().constData());
    if (!model)
    {
        qDebug() << "Failed to load Vosk model at" << modelPath;
        qDebug() << "Make sure the model files are complete and accessible.";
        return;
    }
    
    this->setProperty("_vosk_model_ptr", quintptr(model));
    voskReady = true;
    qDebug() << "Vosk initialized successfully with model:" << modelPath;
}

void Condidat::freeVosk()
{
    VoskRecognizer *rec = voskRecognizer;
    VoskModel *model = reinterpret_cast<VoskModel*>(this->property("_vosk_model_ptr").toULongLong());
    if (rec)
    {
        vosk_recognizer_free(rec);
        voskRecognizer = nullptr;
    }
    if (model)
    {
        vosk_model_free(model);
        this->setProperty("_vosk_model_ptr", 0);
    }
    voskReady = false;
}

void Condidat::transcribeWithVosk(const QByteArray &pcm)
{
    VoskModel *model = reinterpret_cast<VoskModel*>(this->property("_vosk_model_ptr").toULongLong());
    if (!model)
    {
        if (voiceButton) {
            voiceButton->setText("🎤 Remplir par voix");
            voiceButton->setStyleSheet("background-color:#002157;color:#ffce00;border-radius:10px;");
            voiceButton->setEnabled(true);
        }
        return;
    }
    VoskRecognizer *rec = vosk_recognizer_new(model, 16000.0);
    if (!rec) {
        qDebug() << "Failed to create Vosk recognizer";
        if (voiceButton) {
            voiceButton->setText("🎤 Remplir par voix");
            voiceButton->setStyleSheet("background-color:#002157;color:#ffce00;border-radius:10px;");
            voiceButton->setEnabled(true);
        }
        return;
    }
    vosk_recognizer_accept_waveform(rec, pcm.constData(), pcm.size());
    QByteArray res(vosk_recognizer_result(rec));
    vosk_recognizer_free(rec);
    QJsonDocument doc = QJsonDocument::fromJson(res);
    QString text = doc.isObject() ? doc.object().value("text").toString() : QString();
    processVoiceText(text);
}
#endif

void Condidat::onVoicePressed()
{
    isPressing = true;
    holdTimer->start(400);
}

void Condidat::onVoiceReleased()
{
    isPressing = false;
    if (holdTimer->isActive())
        holdTimer->stop();
    if (isRecording)
        stopVoiceRecording();
}

bool Condidat::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj);
    if (!this->isVisible()) return false;
    if (event->type() == QEvent::KeyPress)
    {
        auto *ke = static_cast<QKeyEvent*>(event);
        if (ke->key() == Qt::Key_V && !ke->modifiers())
        {
            if (isRecording)
                stopVoiceRecording();
            else
                startVoiceRecording();
            return true;
        }
    }
    return false;
}

void Condidat::populateCandidateComboBox()
{
    ui->cin_condidat->clear();
    ui->cin_condidat->addItem("Sélectionner un candidat", -1);
    
    // Check database connection
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen())
    {
        Connection c;
        if (!c.createconnect())
        {
            qDebug() << "Failed to connect to database when populating candidate combo box";
            return;
        }
    }
    
    QSqlQuery query;
    query.prepare("SELECT CIN_CONDIDAT, NOM, PRENOM FROM CONDIDAT ORDER BY NOM, PRENOM");
    
    if (query.exec())
    {
        int count = 0;
        while (query.next())
        {
            int cin = query.value(0).toInt();
            QString nom = query.value(1).toString();
            QString prenom = query.value(2).toString();
            // Display only name and first name (without CIN)
            QString displayText = QString("%1 %2").arg(nom).arg(prenom).trimmed();
            ui->cin_condidat->addItem(displayText, cin);
            count++;
        }
        qDebug() << "Loaded" << count << "candidates into combo box";
    }
    else
    {
        qDebug() << "Error loading candidates:" << query.lastError().text();
    }
}

void Condidat::on_cin_condidat_currentIndexChanged(int index)
{
    if (index <= 0)
    {
        // No selection or "Sélectionner un candidat" selected
        ui->codeLabel->setText("Code : Sélectionnez un candidat");
        ui->conduiteLabel->setText("Conduite : Sélectionnez un candidat");
        return;
    }
    
    // Get the CIN from the combo box item data
    int cin = ui->cin_condidat->itemData(index).toInt();
    if (cin == -1)
    {
        return;
    }
    
    // Retrieve candidate data from database
    QSqlQuery query;
    query.prepare("SELECT DATE_NAISSANCE, SEXE, TYPE_PERMIS_VISE FROM CONDIDAT WHERE CIN_CONDIDAT = :cin");
    query.bindValue(":cin", cin);
    
    if (!query.exec() || !query.next())
    {
        ui->codeLabel->setText("Code : Erreur lors de la récupération des données");
        ui->conduiteLabel->setText("Conduite : Erreur lors de la récupération des données");
        return;
    }
    
    QDate birthDate = query.value(0).toDate();
    QString sexe = query.value(1).toString();
    QString type_permis = query.value(2).toString();
    
    // Calculate age
    int age = calculateAge(birthDate);
    
    // Update prediction
    updatePrediction(age, sexe, type_permis);
}

void Condidat::updatePrediction(int age, QString sexe, QString type_permis)
{
    Q_UNUSED(sexe); // Sexe doesn't affect prediction as per requirements
    
    // Base hours for Code
    int codeMin = 10;
    int codeMoy = 15;
    int codeMax = 25;
    
    // Base hours for Conduite
    int conduiteMin = 15;
    int conduiteMoy = 20;
    int conduiteMax = 25;
    
    // Adjust based on age
    if (age < 23)
    {
        // Faster learning - use minimal or average
        codeMin = 10;
        codeMoy = 15;
        codeMax = 20;
        
        conduiteMin = 15;
        conduiteMoy = 20;
        conduiteMax = 23;
    }
    else if (age >= 23 && age <= 35)
    {
        // Average learning
        codeMin = 15;
        codeMoy = 20;
        codeMax = 25;
        
        conduiteMin = 20;
        conduiteMoy = 22;
        conduiteMax = 25;
    }
    else // age > 35
    {
        // Average to high
        codeMin = 20;
        codeMoy = 25;
        codeMax = 30;
        
        conduiteMin = 22;
        conduiteMoy = 25;
        conduiteMax = 28;
    }
    
    // Adjust for license type
    QString typeLower = type_permis.toLower().trimmed();
    if (typeLower == "camion" || typeLower == "poids lourd" || typeLower.contains("camion"))
    {
        // Add 5 hours to all categories for heavy vehicles
        codeMin += 5;
        codeMoy += 5;
        codeMax += 5;
        
        conduiteMin += 5;
        conduiteMoy += 5;
        conduiteMax += 5;
    }
    // For "voiture" or other types, use normal values (already set above)
    
    // Update labels
    QString codeText = QString("Code : Min %1 h | Moy %2 h | Max %3 h")
                       .arg(codeMin).arg(codeMoy).arg(codeMax);
    QString conduiteText = QString("Conduite : Min %1 h | Moy %2 h | Max %3 h")
                           .arg(conduiteMin).arg(conduiteMoy).arg(conduiteMax);
    
    ui->codeLabel->setText(codeText);
    ui->conduiteLabel->setText(conduiteText);
}

// Constructeur avec paramètres (nécessaire mais non utilisé dans l'UI actuelle)
Condidat::Condidat(int cin_condidat, QString nom, QString prenom, QString sexe, QDate date_naissance, int tel, QString type_permis)
    : cin_condidat(cin_condidat), tel(tel), nom(nom), prenom(prenom), type_permis(type_permis), sexe(sexe), date_naissance(date_naissance),
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
      audioSource(nullptr),
#else
      audioInput(nullptr),
#endif
      audioBuffer(nullptr), networkManager(nullptr), recordingTimer(nullptr), isRecording(false), ui(new Ui::Condidat)
{
    ui->setupUi(this);
}
