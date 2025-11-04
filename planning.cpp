#include "planning.h"
#include "ui_planning.h"
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

Planning::Planning(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Planning)
{
    ui->setupUi(this);

    // Connect navigation buttons
    connect(ui->candidat_3, &QPushButton::clicked, this, &Planning::navigateToCandidat);
    connect(ui->planning_3, &QPushButton::clicked, this, &Planning::navigateToPlanning);
    connect(ui->employee_3, &QPushButton::clicked, this, &Planning::navigateToEmployee);
    connect(ui->vehicule_3, &QPushButton::clicked, this, &Planning::navigateToVehicule);
    connect(ui->examen_3, &QPushButton::clicked, this, &Planning::navigateToExamen);
    connect(ui->equipement_3, &QPushButton::clicked, this, &Planning::navigateToEquipement);

    // Initialize table headers (7 data columns including ID + 1 actions = 8 columns)
    ui->tab_3->setColumnCount(8);
    QStringList headers;
    headers << "ID" << "Date séance" << "condidat" << "type" << "debut" << "fin" << "circuit" << "Actions";
    ui->tab_3->setHorizontalHeaderLabels(headers);

    // Table visual/interaction tuning
    ui->tab_3->setShowGrid(true);
    ui->tab_3->setAlternatingRowColors(true);
    ui->tab_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tab_3->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tab_3->horizontalHeader()->setVisible(true);
    ui->tab_3->verticalHeader()->setVisible(true);
    ui->tab_3->horizontalHeader()->setStretchLastSection(true);
    ui->tab_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tab_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tab_3->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Load initial data
    refreshTable();
}

Planning::~Planning()
{
    delete ui;
}

void Planning::on_btn_ajout_3_clicked()
{
    QDate date = ui->dateEdit->date();
    QString condidat = ui->cin_4->toPlainText().trimmed();
    QString type = ui->homme_3->isChecked() ? "conduit" : (ui->femme_3->isChecked() ? "code" : "");
    QTime debut = ui->timeEdit->time();
    QTime fin = ui->timeEdit_2->time();
    QString circuit = ui->tel_3->toPlainText().trimmed();

    if (condidat.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Nom candidat requis.");
        return;
    }
    if (type.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Type de séance requis.");
        return;
    }
    if (circuit.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Adresse circuit requise.");
        return;
    }

    if (isEditMode) {
        int originalId = currentEditingId != -1 ? currentEditingId : selectedIdFromTable();
        if (originalId == -1) {
            QMessageBox::warning(this, "Modification", "Sélectionnez une ligne.");
            return;
        }
        if (modifier(originalId, date, condidat, type, debut, fin, circuit)) {
            QMessageBox::information(this, "Modification", "Planning modifié.");
            refreshTable();
            setEditMode(false);
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de modification.");
        }
        return;
    }

    // Add mode
    this->date_seance = date;
    this->condidat = condidat;
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
    ui->cin_4->clear();
    ui->tel_3->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->timeEdit->setTime(QTime(9, 0));
    ui->timeEdit_2->setTime(QTime(10, 0));
    ui->homme_3->setChecked(false);
    ui->femme_3->setChecked(false);
    ui->type_3->setCurrentIndex(0);
    setEditMode(false);
}

QSqlQueryModel* Planning::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT \"ID_SÉANCE\", \"DATE_SEANCE\", \"NOM_CANDIDAT\", \"TYPE_SÉANCE\", \"HEURE_DEBUT\", \"HEURE_FIN\", \"ADRESSE_CIRCUIT\" FROM PLANNING");
    return model;
}

bool Planning::ajouter()
{
    QSqlQuery query;
    
    // Obtenir le prochain ID - Essayer plusieurs méthodes
    int nextId = -1;
    
    // Tentative 1: Utiliser une séquence avec nom standard
    query.prepare("SELECT SEQ_PLANNING.NEXTVAL FROM DUAL");
    if (query.exec() && query.next()) {
        nextId = query.value(0).toInt();
    } else {
        // Tentative 2: Séquence avec nom alternatif
        query.prepare("SELECT SEQ_PLANNING_ID.NEXTVAL FROM DUAL");
        if (query.exec() && query.next()) {
            nextId = query.value(0).toInt();
        } else {
            // Tentative 3: Calculer MAX + 1 (méthode de secours)
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
    
    // HEURE_DEBUT est de type DATE dans Oracle, on doit créer un QDateTime
    QDateTime dateTimeDebut(date_seance, heure_debut);
    // HEURE_FIN est VARCHAR2, on stocke comme chaîne
    QString heureFinStr = heure_fin.toString("HH:mm");
    
    query.prepare("INSERT INTO PLANNING (\"ID_SÉANCE\", \"DATE_SEANCE\", \"NOM_CANDIDAT\", \"TYPE_SÉANCE\", \"HEURE_DEBUT\", \"HEURE_FIN\", \"ADRESSE_CIRCUIT\") "
                  "VALUES (:id_seance, :date_seance, :nom_candidat, :type_seance, :heure_debut, :heure_fin, :adresse_circuit)");
    query.bindValue(":id_seance", nextId);
    query.bindValue(":date_seance", date_seance);
    query.bindValue(":nom_candidat", condidat);
    query.bindValue(":type_seance", type_seance);
    query.bindValue(":heure_debut", dateTimeDebut);
    query.bindValue(":heure_fin", heureFinStr);
    query.bindValue(":adresse_circuit", circuit);
    
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur SQL: " + query.lastError().text());
        return false;
    }
    return true;
}

bool Planning::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM PLANNING WHERE \"ID_SÉANCE\" = :id");
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur SQL: " + query.lastError().text());
        return false;
    }
    return true;
}

bool Planning::modifier(int id, QDate date_seance, QString condidat, QString type, QTime debut, QTime fin, QString circuit)
{
    QSqlQuery query;
    QDateTime dateTimeDebut(date_seance, debut);
    QString heureFinStr = fin.toString("HH:mm");
    
    query.prepare("UPDATE PLANNING SET \"DATE_SEANCE\" = :date_seance, \"NOM_CANDIDAT\" = :nom_candidat, \"TYPE_SÉANCE\" = :type_seance, "
                  "\"HEURE_DEBUT\" = :debut, \"HEURE_FIN\" = :fin, \"ADRESSE_CIRCUIT\" = :adresse_circuit "
                  "WHERE \"ID_SÉANCE\" = :id");
    query.bindValue(":date_seance", date_seance);
    query.bindValue(":nom_candidat", condidat);
    query.bindValue(":type_seance", type);
    query.bindValue(":debut", dateTimeDebut);
    query.bindValue(":fin", heureFinStr);
    query.bindValue(":adresse_circuit", circuit);
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur SQL: " + query.lastError().text());
        return false;
    }
    return true;
}

void Planning::refreshTable()
{
    std::unique_ptr<QSqlQueryModel> model(afficher());
    int rows = model->rowCount();
    int cols = model->columnCount();
    ui->tab_3->setRowCount(rows);
    ui->tab_3->setColumnCount(cols + 1); // +1 for actions
    QStringList headers;
    headers << "ID" << "Date séance" << "condidat" << "type" << "debut" << "fin" << "circuit" << "Actions";
    ui->tab_3->setHorizontalHeaderLabels(headers);
    
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QVariant v = model->data(model->index(r, c));
            if (c == 0) { // ID column (ID_SÉANCE)
                ui->tab_3->setItem(r, c, new QTableWidgetItem(v.toString()));
            } else if (c == 1) { // Date column (DATE_SEANCE)
                QDate date = v.toDate();
                if (date.isValid()) {
                    ui->tab_3->setItem(r, c, new QTableWidgetItem(date.toString("dd/MM/yyyy")));
                } else {
                    ui->tab_3->setItem(r, c, new QTableWidgetItem(v.toString()));
                }
            } else if (c == 4) { // HEURE_DEBUT (DATE type, extract time)
                QDateTime datetime = v.toDateTime();
                if (datetime.isValid()) {
                    ui->tab_3->setItem(r, c, new QTableWidgetItem(datetime.time().toString("HH:mm")));
                } else {
                    ui->tab_3->setItem(r, c, new QTableWidgetItem(v.toString()));
                }
            } else if (c == 5) { // HEURE_FIN (VARCHAR2, display as is)
                ui->tab_3->setItem(r, c, new QTableWidgetItem(v.toString()));
            } else {
                ui->tab_3->setItem(r, c, new QTableWidgetItem(v.toString()));
            }
        }
        // Add actions cell with Edit/Supp buttons
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
    bool ok = false;
    int id = idItem ? idItem->text().toInt(&ok) : -1;
    return ok ? id : -1;
}

void Planning::setFormFromRow(int row)
{
    if (row < 0 || row >= ui->tab_3->rowCount()) return;
    auto getText = [&](int col){ QTableWidgetItem *it = ui->tab_3->item(row, col); return it ? it->text() : QString(); };
    
    // ID is in column 0, but we don't need to set it in the form
    QDate date = QDate::fromString(getText(1), "dd/MM/yyyy");
    if (date.isValid()) ui->dateEdit->setDate(date);
    
    ui->cin_4->setText(getText(2)); // NOM_CANDIDAT (condidat)
    
    QString typeStr = getText(3).trimmed().toLower();
    ui->homme_3->setChecked(typeStr == "conduit");
    ui->femme_3->setChecked(typeStr == "code");
    
    QTime debut = QTime::fromString(getText(4), "HH:mm");
    if (debut.isValid()) ui->timeEdit->setTime(debut);
    
    QTime fin = QTime::fromString(getText(5), "HH:mm");
    if (fin.isValid()) ui->timeEdit_2->setTime(fin);
    
    ui->tel_3->setText(getText(6)); // ADRESSE_CIRCUIT (circuit)
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

    // Edit action: toggle edit mode
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
    QString condidat = ui->cin_4->toPlainText().trimmed();
    QString type = ui->homme_3->isChecked() ? "conduit" : (ui->femme_3->isChecked() ? "code" : "");
    QTime debut = ui->timeEdit->time();
    QTime fin = ui->timeEdit_2->time();
    QString circuit = ui->tel_3->toPlainText().trimmed();
    
    if (modifier(originalId, date, condidat, type, debut, fin, circuit)) {
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
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(1);
    }
}

void Planning::navigateToPlanning()
{
    // Already in planning view
}

void Planning::navigateToEmployee()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(0);
    }
}

void Planning::navigateToVehicule()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(2);
    }
}

void Planning::navigateToExamen()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(3);
    }
}

void Planning::navigateToEquipement()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(5);
    }
}
