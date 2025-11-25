#include "condidat.h"
#include "ui_condidat.h"
#include "navigation_constants.h"
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
Condidat::Condidat(QWidget *parent) : QWidget(parent),
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
    std::unique_ptr<QSqlQueryModel> model(afficher());
    int rows = model->rowCount();
    int cols = model->columnCount();
    ui->tab->setRowCount(rows);
    ui->tab->setColumnCount(cols + 1); // +1 for actions
    QStringList headers;
    headers << "CIN" << "nom" << "Prénom" << "Sexe" << "date de naissance" << "Télephone" << "type de permis" << "Actions";
    ui->tab->setHorizontalHeaderLabels(headers);
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
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
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_EMPLOYEE);
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
