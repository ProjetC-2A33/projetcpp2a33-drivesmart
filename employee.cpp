#include "employee.h"
#include "ui_employee.h"
#include "connection.h"
#include "navigation_constants.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QStackedWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDebug>
#include <QHBoxLayout>
#include <QStyle>
#include <QFileDialog>
#include <QTextStream>

Employee::Employee(QWidget *parent) : QWidget(parent),
                                      ui(new Ui::Employee),
                                      selectedRow(-1),
                                      isEditMode(false),
                                      currentEditingCin("")
{
    ui->setupUi(this);

    db = QSqlDatabase::database();
    if (!db.isOpen())
    {
        Connection c;
        c.createconnect();
        db = QSqlDatabase::database();
    }

    // Connect navigation buttons
    connect(ui->candidat, &QPushButton::clicked, this, &Employee::navigateToCandidat);
    connect(ui->planning, &QPushButton::clicked, this, &Employee::navigateToPlanning);
    connect(ui->employee, &QPushButton::clicked, this, &Employee::navigateToEmployee);
    connect(ui->vehicule, &QPushButton::clicked, this, &Employee::navigateToVehicule);
    connect(ui->examen, &QPushButton::clicked, this, &Employee::navigateToExamen);
    connect(ui->equipement, &QPushButton::clicked, this, &Employee::navigateToEquipement);

    // Connect table click
    connect(ui->tableWidget, &QTableWidget::itemClicked, this, &Employee::on_tableWidget_itemClicked);

    // Connect search field for live search
    connect(ui->lineEdit_recherche, &QLineEdit::textChanged, this, [this](const QString &text)
            {
        if (text.trimmed().isEmpty()) {
            afficher();
        } else {
            rechercher(text.trimmed());
        } });

    // Setup table
    ui->tableWidget->setColumnCount(10); // Added one more for Actions
    QStringList headers;
    headers << "CIN" << "Nom" << "Prénom" << "Disponibilité" << "Salaire"
            << "Téléphone" << "Email" << "Poste" << "Mot de passe" << "Actions";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(false);
    ui->tableWidget->setColumnWidth(9, 80); // Actions column
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // DEBUG: Check current database user and table structure
    QSqlQuery debugQuery;
    if (debugQuery.exec("SELECT USER FROM DUAL"))
    {
        if (debugQuery.next())
        {
            qDebug() << "Employee module - Connected as Oracle user:" << debugQuery.value(0).toString();
        }
    }

    // DEBUG: Check if EMPLOYEE table exists and has CIN_EMPLOYEE column
    if (debugQuery.exec("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME = 'EMPLOYEE' AND COLUMN_NAME = 'CIN_EMPLOYEE'"))
    {
        if (debugQuery.next())
        {
            qDebug() << "Employee module - CIN_EMPLOYEE column found in EMPLOYEE table";
        }
        else
        {
            qDebug() << "Employee module - WARNING: CIN_EMPLOYEE column NOT found in EMPLOYEE table!";
            qDebug() << "Employee module - Checking what columns exist...";
            QSqlQuery colQuery;
            if (colQuery.exec("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME = 'EMPLOYEE' ORDER BY COLUMN_ID"))
            {
                qDebug() << "Employee module - Available columns:";
                while (colQuery.next())
                {
                    qDebug() << "  -" << colQuery.value(0).toString();
                }
            }
        }
    }

    // Load initial data
    setEditMode(false);
    afficher();
}

Employee::~Employee()
{
    delete ui;
}

// Navigation methods
void Employee::navigateToCandidat()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_CANDIDAT);
    }
}

void Employee::navigateToPlanning()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_PLANNING);
    }
}

void Employee::navigateToEmployee()
{
    // Already on employee page
}

void Employee::navigateToVehicule()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_VEHICULE);
    }
}

void Employee::navigateToExamen()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_EXAMEN);
    }
}

void Employee::navigateToEquipement()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_EQUIPEMENT);
    }
}

// CRUD Operation Slots
void Employee::on_pushButton_ajouter_clicked()
{
    QString cin = ui->lineEdit_cin->text().trimmed();
    QString nom = ui->lineEdit_nom->text().trimmed();
    QString prenom = ui->lineEdit_prenom->text().trimmed();
    QString disponibilite = getDisponibilite();
    QString salaireStr = ui->lineEdit_salaire->text().trimmed();
    QString telephone = ui->lineEdit_telephone->text().trimmed();
    QString email = ui->lineEdit_email->text().trimmed();
    QString poste = ui->comboBox_poste->currentText().trimmed();
    QString mdp = ui->lineEdit_mdp->text().trimmed();

    // Validation
    if (cin.isEmpty() || nom.isEmpty() || prenom.isEmpty() || disponibilite.isEmpty() ||
        salaireStr.isEmpty() || telephone.isEmpty() || email.isEmpty() ||
        poste.isEmpty() || mdp.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    bool ok;
    double salaire = salaireStr.toDouble(&ok);
    if (!ok || salaire < 0)
    {
        QMessageBox::warning(this, "Erreur", "Le salaire doit être un nombre positif.");
        return;
    }

    // Check if we are in edit mode
    if (isEditMode && !currentEditingCin.isEmpty())
    {
        // Modification mode
        if (modifier(cin, nom, prenom, disponibilite, salaire, telephone, email, poste, mdp))
        {
            QMessageBox::information(this, "Succès", "Employé modifié avec succès!");
            viderChamps();
            setEditMode(false);
            currentEditingCin = "";
            selectedRow = -1;
            afficher();
        }
    }
    else
    {
        // Addition mode
        if (ajouter(cin, nom, prenom, disponibilite, salaire, telephone, email, poste, mdp))
        {
            QMessageBox::information(this, "Succès", "Employé ajouté avec succès!");
            viderChamps();
            afficher();
        }
    }
}

void Employee::on_pushButton_annuler_clicked()
{
    viderChamps();
    setEditMode(false);
    currentEditingCin = "";
    selectedRow = -1;
}

void Employee::on_tri_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0: // Par défaut
        afficher();
        break;
    case 1: // Nom A-Z
        afficher("NOM ASC");
        break;
    case 2: // Nom Z-A
        afficher("NOM DESC");
        break;
    case 3: // Poste
        afficher("POSTE ASC");
        break;
    case 4: // Salaire croissant
        afficher("SALAIRE ASC");
        break;
    case 5: // Salaire décroissant
        afficher("SALAIRE DESC");
        break;
    }
}

void Employee::on_exporter_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter les employés", "", "CSV Files (*.csv)");
    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier pour l'écriture.");
        return;
    }

    QTextStream out(&file);

    // Write headers
    out << "CIN,Nom,Prénom,Disponibilité,Salaire,Téléphone,Email,Poste,Mot de passe\n";

    // Write data
    for (int row = 0; row < ui->tableWidget->rowCount(); row++)
    {
        for (int col = 0; col < 9; col++)
        { // 9 columns (excluding Actions)
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            if (item)
            {
                out << item->text();
            }
            if (col < 8)
            {
                out << ",";
            }
        }
        out << "\n";
    }

    file.close();
    QMessageBox::information(this, "Succès", "Données exportées avec succès!");
}

void Employee::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    int row = item->row();
    selectedRow = row;
}

// CRUD Methods
bool Employee::ajouter(const QString &cin, const QString &nom, const QString &prenom,
                       const QString &disponibilite, double salaire, const QString &telephone,
                       const QString &email, const QString &poste, const QString &mdp)
{
    // Check if CIN already exists
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM EMPLOYEE WHERE CIN_EMPLOYEE = :cin");
    checkQuery.bindValue(":cin", cin);

    if (checkQuery.exec() && checkQuery.next())
    {
        if (checkQuery.value(0).toInt() > 0)
        {
            QMessageBox::warning(this, "Erreur", "Ce CIN existe déjà dans la base de données.");
            return false;
        }
    }

    QSqlQuery query;
    query.prepare("INSERT INTO EMPLOYEE (CIN_EMPLOYEE, NOM, PRENOM, DISPONIBILITE, SALAIRE, "
                  "TELEPHONE, EMAIL, POSTE, MDP) "
                  "VALUES (:cin, :nom, :prenom, :disponibilite, :salaire, :telephone, "
                  ":email, :poste, :mdp)");

    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":disponibilite", disponibilite);
    query.bindValue(":salaire", salaire);
    query.bindValue(":telephone", telephone);
    query.bindValue(":email", email);
    query.bindValue(":poste", poste);
    query.bindValue(":mdp", mdp);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout: " + query.lastError().text());
        return false;
    }

    return true;
}

bool Employee::modifier(const QString &cin, const QString &nom, const QString &prenom,
                        const QString &disponibilite, double salaire, const QString &telephone,
                        const QString &email, const QString &poste, const QString &mdp)
{
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYEE SET NOM = :nom, PRENOM = :prenom, "
                  "DISPONIBILITE = :disponibilite, SALAIRE = :salaire, TELEPHONE = :telephone, "
                  "EMAIL = :email, POSTE = :poste, MDP = :mdp "
                  "WHERE CIN_EMPLOYEE = :cin");

    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":disponibilite", disponibilite);
    query.bindValue(":salaire", salaire);
    query.bindValue(":telephone", telephone);
    query.bindValue(":email", email);
    query.bindValue(":poste", poste);
    query.bindValue(":mdp", mdp);
    query.bindValue(":cin", cin);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification: " + query.lastError().text());
        return false;
    }

    return true;
}

bool Employee::supprimer(const QString &cin)
{
    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYEE WHERE CIN_EMPLOYEE = :cin");
    query.bindValue(":cin", cin);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression: " + query.lastError().text());
        return false;
    }

    return true;
}

void Employee::afficher(QString orderBy)
{
    QSqlQuery query;
    QString baseQuery = "SELECT CIN_EMPLOYEE, NOM, PRENOM, DISPONIBILITE, SALAIRE, TELEPHONE, "
                        "EMAIL, POSTE, MDP FROM EMPLOYEE";

    if (orderBy.isEmpty())
    {
        baseQuery += " ORDER BY NOM, PRENOM";
    }
    else
    {
        baseQuery += " ORDER BY " + orderBy;
    }

    query.prepare(baseQuery);

    if (!query.exec())
    {
        QString errorMsg = query.lastError().text();
        qDebug() << "Employee::afficher() - Query failed:" << errorMsg;
        qDebug() << "Employee::afficher() - Query was:" << query.lastQuery();

        // Don't show error dialog if table is just empty or doesn't exist yet
        if (!errorMsg.contains("ORA-00904"))
        {
            QMessageBox::critical(this, "Erreur", "Erreur lors de l'affichage: " + errorMsg);
        }
        return;
    }

    ui->tableWidget->setRowCount(0);
    int row = 0;

    while (query.next())
    {
        ui->tableWidget->insertRow(row);
        QString cin = query.value(0).toString();
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(cin));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(query.value(4).toDouble(), 'f', 2)));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(query.value(6).toString()));
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(query.value(7).toString()));
        ui->tableWidget->setItem(row, 8, new QTableWidgetItem(query.value(8).toString()));

        // Add action buttons
        ui->tableWidget->setCellWidget(row, 9, createActionsCell(row, cin));

        row++;
    }
}

void Employee::rechercher(const QString &critere)
{
    QSqlQuery query;
    query.prepare("SELECT CIN_EMPLOYEE, NOM, PRENOM, DISPONIBILITE, SALAIRE, TELEPHONE, "
                  "EMAIL, POSTE, MDP FROM EMPLOYEE "
                  "WHERE UPPER(CIN_EMPLOYEE) LIKE :critere "
                  "OR UPPER(NOM) LIKE :critere "
                  "OR UPPER(PRENOM) LIKE :critere "
                  "OR UPPER(POSTE) LIKE :critere "
                  "ORDER BY NOM, PRENOM");

    QString searchPattern = "%" + critere.toUpper() + "%";
    query.bindValue(":critere", searchPattern);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la recherche: " + query.lastError().text());
        return;
    }

    ui->tableWidget->setRowCount(0);
    int row = 0;

    while (query.next())
    {
        ui->tableWidget->insertRow(row);
        QString cin = query.value(0).toString();
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(cin));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(query.value(4).toDouble(), 'f', 2)));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(query.value(6).toString()));
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(query.value(7).toString()));
        ui->tableWidget->setItem(row, 8, new QTableWidgetItem(query.value(8).toString()));

        // Add action buttons
        ui->tableWidget->setCellWidget(row, 9, createActionsCell(row, cin));

        row++;
    }
}

// Helper Methods
void Employee::viderChamps()
{
    ui->lineEdit_cin->clear();
    ui->lineEdit_nom->clear();
    ui->lineEdit_prenom->clear();
    ui->lineEdit_salaire->clear();
    ui->lineEdit_telephone->clear();
    ui->lineEdit_email->clear();
    ui->comboBox_poste->setCurrentIndex(0);
    ui->lineEdit_mdp->clear();
    ui->radioButton_disponible->setChecked(false);
    ui->radioButton_non_disponible->setChecked(false);
    ui->lineEdit_recherche->clear();
}

void Employee::remplirChamps(int row)
{
    ui->lineEdit_cin->setText(ui->tableWidget->item(row, 0)->text());
    ui->lineEdit_nom->setText(ui->tableWidget->item(row, 1)->text());
    ui->lineEdit_prenom->setText(ui->tableWidget->item(row, 2)->text());

    QString dispo = ui->tableWidget->item(row, 3)->text();
    if (dispo.toLower() == "disponible" || dispo == "1")
    {
        ui->radioButton_disponible->setChecked(true);
    }
    else
    {
        ui->radioButton_non_disponible->setChecked(true);
    }

    ui->lineEdit_salaire->setText(ui->tableWidget->item(row, 4)->text());
    ui->lineEdit_telephone->setText(ui->tableWidget->item(row, 5)->text());
    ui->lineEdit_email->setText(ui->tableWidget->item(row, 6)->text());

    QString poste = ui->tableWidget->item(row, 7)->text();
    int posteIndex = ui->comboBox_poste->findText(poste);
    if (posteIndex >= 0)
    {
        ui->comboBox_poste->setCurrentIndex(posteIndex);
    }

    ui->lineEdit_mdp->setText(ui->tableWidget->item(row, 8)->text());
}

QString Employee::getDisponibilite()
{
    if (ui->radioButton_disponible->isChecked())
    {
        return "Disponible";
    }
    else if (ui->radioButton_non_disponible->isChecked())
    {
        return "Non disponible";
    }
    return "";
}

void Employee::rafraichirTable()
{
    afficher();
}

QWidget *Employee::createActionsCell(int row, const QString &cin)
{
    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(4, 2, 4, 2);
    layout->setSpacing(4);

    // Edit button
    QPushButton *editBtn = new QPushButton();
    editBtn->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    editBtn->setFixedSize(30, 30);
    editBtn->setStyleSheet("QPushButton { background-color: #ffce00; border: none; border-radius: 4px; }"
                           "QPushButton:hover { background-color: #e6b800; }");
    editBtn->setCursor(Qt::PointingHandCursor);

    connect(editBtn, &QPushButton::clicked, [this, row, cin]()
            {
        setFormFromRow(row);
        setEditMode(true);
        currentEditingCin = cin; });

    // Delete button
    QPushButton *deleteBtn = new QPushButton();
    deleteBtn->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
    deleteBtn->setFixedSize(30, 30);
    deleteBtn->setStyleSheet("QPushButton { background-color: #ff4444; border: none; border-radius: 4px; }"
                             "QPushButton:hover { background-color: #cc0000; }");
    deleteBtn->setCursor(Qt::PointingHandCursor);

    connect(deleteBtn, &QPushButton::clicked, [this, cin]()
            {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmation",
                                     "Êtes-vous sûr de vouloir supprimer cet employé ?",
                                     QMessageBox::Yes | QMessageBox::No);
        
        if (reply == QMessageBox::Yes) {
            if (supprimer(cin)) {
                QMessageBox::information(this, "Succès", "Employé supprimé avec succès!");
                viderChamps();
                setEditMode(false);
                currentEditingCin = "";
                selectedRow = -1;
                afficher();
            }
        } });

    layout->addWidget(editBtn);
    layout->addWidget(deleteBtn);
    layout->setAlignment(Qt::AlignCenter);

    widget->setLayout(layout);
    return widget;
}

void Employee::setEditMode(bool enabled)
{
    isEditMode = enabled;
    if (enabled)
    {
        ui->pushButton_ajouter->setText("Modifier");
    }
    else
    {
        ui->pushButton_ajouter->setText("Ajouter");
    }
}

void Employee::setFormFromRow(int row)
{
    ui->lineEdit_cin->setText(ui->tableWidget->item(row, 0)->text());
    ui->lineEdit_nom->setText(ui->tableWidget->item(row, 1)->text());
    ui->lineEdit_prenom->setText(ui->tableWidget->item(row, 2)->text());

    QString dispo = ui->tableWidget->item(row, 3)->text();
    if (dispo.toLower() == "disponible" || dispo == "1")
    {
        ui->radioButton_disponible->setChecked(true);
    }
    else
    {
        ui->radioButton_non_disponible->setChecked(true);
    }

    ui->lineEdit_salaire->setText(ui->tableWidget->item(row, 4)->text());
    ui->lineEdit_telephone->setText(ui->tableWidget->item(row, 5)->text());
    ui->lineEdit_email->setText(ui->tableWidget->item(row, 6)->text());

    QString poste = ui->tableWidget->item(row, 7)->text();
    int posteIndex = ui->comboBox_poste->findText(poste);
    if (posteIndex >= 0)
    {
        ui->comboBox_poste->setCurrentIndex(posteIndex);
    }

    ui->lineEdit_mdp->setText(ui->tableWidget->item(row, 8)->text());
}
