#include "gestion_equipement.h"
#include "ui_gestion_equipement.h"
#include "connection.h"
#include "navigation_constants.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QStackedWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDebug>
#include <QHBoxLayout>
#include <QStyle>
#include <QFileDialog>
#include <QTextStream>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QPrinter>
#include <QPainter>
#include <QFont>
#include <QPageSize>
#include <QTextDocument>
#include <QTextTable>
#include <QTextTableFormat>
#include <QTextCursor>
#include <QAbstractTextDocumentLayout>

Gestion_Equipement::Gestion_Equipement(QWidget *parent) : QWidget(parent),
                                                          ui(new Ui::Gestion_Equipement),
                                                          selectedRow(-1),
                                                          isEditMode(false),
                                                          currentEditingId(-1)
{
    ui->setupUi(this);

    db = QSqlDatabase::database();
    if (!db.isOpen())
    {
        Connection c;
        c.createconnect();
        db = QSqlDatabase::database();
    }
    
    // Initialize weather API
    weatherManager = new QNetworkAccessManager(this);
    weatherApiKey = "bd5e378503939ddaee76f12ad7a97608"; // Free OpenWeatherMap API key
    ui->lineEdit_city->setText("Tunis"); // Default city

    // Connect navigation buttons
    connect(ui->candidat, &QPushButton::clicked, this, &Gestion_Equipement::navigateToCandidat);
    connect(ui->planning, &QPushButton::clicked, this, &Gestion_Equipement::navigateToPlanning);
    connect(ui->employee, &QPushButton::clicked, this, &Gestion_Equipement::navigateToEmployee);
    connect(ui->vehicule, &QPushButton::clicked, this, &Gestion_Equipement::navigateToVehicule);
    connect(ui->examen, &QPushButton::clicked, this, &Gestion_Equipement::navigateToExamen);
    connect(ui->equipement, &QPushButton::clicked, this, &Gestion_Equipement::navigateToEquipement);

    // Connect table click
    connect(ui->tableWidget, &QTableWidget::itemClicked, this, &Gestion_Equipement::on_tableWidget_itemClicked);

    // Connect search field
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
    headers << "ID" << "Nom" << "Catégorie" << "Quantité Totale" << "Quantité Disponible"
            << "État" << "Fournisseur" << "Date Acquisition" << "CIN Employé" << "Actions";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(false);
    ui->tableWidget->setColumnWidth(9, 80); // Actions column
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Load initial data
    chargerEmployees();
    setEditMode(false);
    afficher();
    
    // Load equipment for lifecycle analysis
    loadEquipmentForLifecycle();
    
    // Initialize statistics dashboard
    updateStatistics();
}

Gestion_Equipement::~Gestion_Equipement()
{
    delete ui;
}

// Navigation methods
void Gestion_Equipement::navigateToCandidat()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_CANDIDAT);
    }
}

void Gestion_Equipement::navigateToPlanning()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_PLANNING);
    }
}

void Gestion_Equipement::navigateToEmployee()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_EMPLOYEE);
    }
}

void Gestion_Equipement::navigateToVehicule()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_VEHICULE);
    }
}

void Gestion_Equipement::navigateToExamen()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_EXAMEN);
    }
}

void Gestion_Equipement::navigateToEquipement()
{
    // Already on equipement page
}

// CRUD Operation Slots
void Gestion_Equipement::on_pushButton_ajouter_clicked()
{
    // Check if we're in edit mode - if so, this is a "Modifier" action
    if (isEditMode && currentEditingId != -1)
    {
        // This is a modification
        QString idStr = ui->lineEdit_id->text().trimmed();
        QString nom = ui->lineEdit_nom->text().trimmed();
        QString categorie = ui->comboBox_categorie->currentText().trimmed();
        QString quantiteTotaleStr = ui->lineEdit_quantite_totale->text().trimmed();
        QString quantiteDisponibleStr = ui->lineEdit_quantite_disponible->text().trimmed();
        QString etat = ui->comboBox_etat->currentText().trimmed();
        QString fournisseur = ui->lineEdit_fournisseur->text().trimmed();
        QDate dateAcquisition = ui->dateEdit_acquisition->date();
        QString cinEmployee = ui->comboBox_cin_employee->currentData().toString();

        if (nom.isEmpty() || categorie.isEmpty() ||
            quantiteTotaleStr.isEmpty() || quantiteDisponibleStr.isEmpty() ||
            etat.isEmpty() || fournisseur.isEmpty())
        {
            QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires.");
            return;
        }

        bool ok;
        int id = idStr.toInt(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "Erreur", "ID invalide.");
            return;
        }

        int quantiteTotale = quantiteTotaleStr.toInt(&ok);
        if (!ok || quantiteTotale < 0)
        {
            QMessageBox::warning(this, "Erreur", "La quantité totale doit être un nombre positif.");
            return;
        }

        int quantiteDisponible = quantiteDisponibleStr.toInt(&ok);
        if (!ok || quantiteDisponible < 0 || quantiteDisponible > quantiteTotale)
        {
            QMessageBox::warning(this, "Erreur", "La quantité disponible doit être un nombre positif et inférieur ou égal à la quantité totale.");
            return;
        }

        if (modifier(id, nom, categorie, quantiteTotale, quantiteDisponible, etat,
                     fournisseur, dateAcquisition, cinEmployee))
        {
            QMessageBox::information(this, "Succès", "Équipement modifié avec succès!");
            setEditMode(false);
            viderChamps();
            afficher();
        }
        return;
    }

    // Normal add operation
    int id = genererNouvelID();
    QString nom = ui->lineEdit_nom->text().trimmed();
    QString categorie = ui->comboBox_categorie->currentText().trimmed();
    QString quantiteTotaleStr = ui->lineEdit_quantite_totale->text().trimmed();
    QString quantiteDisponibleStr = ui->lineEdit_quantite_disponible->text().trimmed();
    QString etat = ui->comboBox_etat->currentText().trimmed();
    QString fournisseur = ui->lineEdit_fournisseur->text().trimmed();
    QDate dateAcquisition = ui->dateEdit_acquisition->date();
    QString cinEmployee = ui->comboBox_cin_employee->currentData().toString();

    // Validation
    if (nom.isEmpty() || categorie.isEmpty() ||
        quantiteTotaleStr.isEmpty() || quantiteDisponibleStr.isEmpty() ||
        etat.isEmpty() || fournisseur.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    bool ok;
    int quantiteTotale = quantiteTotaleStr.toInt(&ok);
    if (!ok || quantiteTotale < 0)
    {
        QMessageBox::warning(this, "Erreur", "La quantité totale doit être un nombre positif.");
        return;
    }

    int quantiteDisponible = quantiteDisponibleStr.toInt(&ok);
    if (!ok || quantiteDisponible < 0 || quantiteDisponible > quantiteTotale)
    {
        QMessageBox::warning(this, "Erreur", "La quantité disponible doit être un nombre positif et inférieur ou égal à la quantité totale.");
        return;
    }

    if (ajouter(id, nom, categorie, quantiteTotale, quantiteDisponible, etat,
                fournisseur, dateAcquisition, cinEmployee))
    {
        QMessageBox::information(this, "Succès", "Équipement ajouté avec succès!");
        viderChamps();
        afficher();
    }
}

void Gestion_Equipement::on_pushButton_annuler_clicked()
{
    setEditMode(false);
    viderChamps();
    selectedRow = -1;
}

void Gestion_Equipement::on_tri_currentIndexChanged(int index)
{
    QString orderBy = "";

    switch (index)
    {
    case 1: // Nom A à Z
        orderBy = "NOM ASC";
        break;
    case 2: // Nom Z à A
        orderBy = "NOM DESC";
        break;
    case 3: // Catégorie
        orderBy = "CATEGORIE ASC, NOM ASC";
        break;
    case 4: // État
        orderBy = "ETAT ASC, NOM ASC";
        break;
    default:
        orderBy = "ID_EQUIPEMENT ASC";
        break;
    }

    afficher(orderBy);
}

void Gestion_Equipement::on_exporter_clicked()
{
    // Ask user which format
    QMessageBox msgBox;
    msgBox.setWindowTitle("Choisir le format d'export");
    msgBox.setText("Sélectionnez le format d'export:");
    QPushButton *csvButton = msgBox.addButton("CSV", QMessageBox::ActionRole);
    QPushButton *pdfButton = msgBox.addButton("PDF", QMessageBox::ActionRole);
    msgBox.addButton("Annuler", QMessageBox::RejectRole);
    msgBox.exec();
    
    if (msgBox.clickedButton() == csvButton)
    {
        // Export to CSV
        QString fileName = QFileDialog::getSaveFileName(this, "Exporter en CSV", "", "CSV (*.csv)");
        if (fileName.isEmpty())
            return;

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::critical(this, "Erreur", "Impossible de créer le fichier.");
            return;
        }

        QTextStream out(&file);

        // Write headers
        out << "ID,Nom,Catégorie,Quantité Totale,Quantité Disponible,État,Fournisseur,Date Acquisition,CIN Employé\n";

        // Write data
        for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
        {
            for (int col = 0; col < 9; ++col)
            { // Skip Actions column
                QTableWidgetItem *item = ui->tableWidget->item(row, col);
                if (item)
                {
                    out << "\"" << item->text() << "\"";
                }
                if (col < 8)
                    out << ",";
            }
            out << "\n";
        }

        file.close();
        QMessageBox::information(this, "Succès", "Export CSV réussi!");
    }
    else if (msgBox.clickedButton() == pdfButton)
    {
        exportToPDF();
    }
}

void Gestion_Equipement::exportToPDF()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "PDF (*.pdf)");
    if (fileName.isEmpty())
        return;

    // Create a text document
    QTextDocument document;
    
    // Build HTML table
    QString html = "<html><head><style>"
                   "body { font-family: Arial; }"
                   "h1 { color: #002157; font-size: 24px; }"
                   "table { border-collapse: collapse; width: 100%; }"
                   "th { background-color: #002157; color: white; padding: 12px; text-align: left; font-weight: bold; }"
                   "td { padding: 10px; border: 1px solid #dee2e6; }"
                   "tr:nth-child(even) { background-color: #f8f9fa; }"
                   ".footer { margin-top: 20px; color: #666; font-size: 10px; }"
                   "</style></head><body>";
    
    // Title
    html += "<h1>Liste des Équipements - DriveSmart</h1>";
    html += "<p style='color: #666;'>Généré le: " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm") + "</p>";
    
    // Table
    html += "<table border='1'>";
    
    // Headers
    html += "<tr>";
    html += "<th>ID</th>";
    html += "<th>Nom</th>";
    html += "<th>Catégorie</th>";
    html += "<th>Qté Tot.</th>";
    html += "<th>Qté Disp.</th>";
    html += "<th>État</th>";
    html += "<th>Fournisseur</th>";
    html += "<th>Date Acq.</th>";
    html += "<th>CIN Emp.</th>";
    html += "</tr>";
    
    // Data rows
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        html += "<tr>";
        for (int col = 0; col < 9; ++col)
        {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            QString text = item ? item->text() : "";
            html += "<td>" + text + "</td>";
        }
        html += "</tr>";
    }
    
    html += "</table>";
    
    // Footer
    html += "<div class='footer'>";
    html += "<p>Total: " + QString::number(ui->tableWidget->rowCount()) + " équipement(s) | DriveSmart © 2025</p>";
    html += "</div>";
    
    html += "</body></html>";
    
    // Set HTML to document
    document.setHtml(html);
    
    // Setup printer
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);
    
    // Print document to PDF
    document.print(&printer);
    
    QMessageBox::information(this, "Succès", "Export PDF réussi!");
}

void Gestion_Equipement::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    // Clicking on table doesn't auto-fill form anymore
    selectedRow = item->row();
}

// CRUD Methods
bool Gestion_Equipement::ajouter(int id, const QString &nom, const QString &categorie,
                                 int quantite_totale, int quantite_disponible, const QString &etat,
                                 const QString &fournisseur, const QDate &date_acquisition,
                                 const QString &cin_employee)
{
    // Check if ID already exists
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM EQUIPEMENT WHERE ID_EQUIPEMENT = :id");
    checkQuery.bindValue(":id", id);

    if (checkQuery.exec() && checkQuery.next())
    {
        if (checkQuery.value(0).toInt() > 0)
        {
            QMessageBox::warning(this, "Erreur", "Cet ID existe déjà dans la base de données.");
            return false;
        }
    }

    QSqlQuery query;
    query.prepare("INSERT INTO EQUIPEMENT (ID_EQUIPEMENT, NOM, CATEGORIE, QUANTITE_TOTALE, "
                  "QUANTITE_DISPONIBLE, ETAT, FOURNISSEUR, DATE_ACQUISITION, CIN_EMPLOYEE) "
                  "VALUES (:id, :nom, :categorie, :quantite_totale, :quantite_disponible, "
                  ":etat, :fournisseur, :date_acquisition, :cin_employee)");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":categorie", categorie);
    query.bindValue(":quantite_totale", quantite_totale);
    query.bindValue(":quantite_disponible", quantite_disponible);
    query.bindValue(":etat", etat);
    query.bindValue(":fournisseur", fournisseur);
    query.bindValue(":date_acquisition", date_acquisition);
    query.bindValue(":cin_employee", cin_employee.isEmpty() ? QVariant() : QVariant(cin_employee));

    if (!query.exec())
    {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout: " + query.lastError().text());
        return false;
    }

    return true;
}

bool Gestion_Equipement::modifier(int id, const QString &nom, const QString &categorie,
                                  int quantite_totale, int quantite_disponible, const QString &etat,
                                  const QString &fournisseur, const QDate &date_acquisition,
                                  const QString &cin_employee)
{
    QSqlQuery query;
    query.prepare("UPDATE EQUIPEMENT SET NOM = :nom, CATEGORIE = :categorie, "
                  "QUANTITE_TOTALE = :quantite_totale, QUANTITE_DISPONIBLE = :quantite_disponible, "
                  "ETAT = :etat, FOURNISSEUR = :fournisseur, DATE_ACQUISITION = :date_acquisition, "
                  "CIN_EMPLOYEE = :cin_employee "
                  "WHERE ID_EQUIPEMENT = :id");

    query.bindValue(":nom", nom);
    query.bindValue(":categorie", categorie);
    query.bindValue(":quantite_totale", quantite_totale);
    query.bindValue(":quantite_disponible", quantite_disponible);
    query.bindValue(":etat", etat);
    query.bindValue(":fournisseur", fournisseur);
    query.bindValue(":date_acquisition", date_acquisition);
    query.bindValue(":cin_employee", cin_employee.isEmpty() ? QVariant() : QVariant(cin_employee));
    query.bindValue(":id", id);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification: " + query.lastError().text());
        return false;
    }

    return true;
}

bool Gestion_Equipement::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM EQUIPEMENT WHERE ID_EQUIPEMENT = :id");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression: " + query.lastError().text());
        return false;
    }

    return true;
}

void Gestion_Equipement::afficher(QString orderBy)
{
    if (orderBy.isEmpty())
    {
        orderBy = "ID_EQUIPEMENT";
    }

    QSqlQuery query;
    QString sql = QString("SELECT ID_EQUIPEMENT, NOM, CATEGORIE, QUANTITE_TOTALE, QUANTITE_DISPONIBLE, "
                          "ETAT, FOURNISSEUR, DATE_ACQUISITION, CIN_EMPLOYEE "
                          "FROM EQUIPEMENT ORDER BY %1")
                      .arg(orderBy);
    query.prepare(sql);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'affichage: " + query.lastError().text());
        return;
    }

    ui->tableWidget->setRowCount(0);
    int row = 0;

    while (query.next())
    {
        ui->tableWidget->insertRow(row);

        int id = query.value(0).toInt();

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(query.value(6).toString()));
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(query.value(7).toDate().toString("yyyy-MM-dd")));
        ui->tableWidget->setItem(row, 8, new QTableWidgetItem(query.value(8).toString()));

        // Add action buttons
        ui->tableWidget->setCellWidget(row, 9, createActionsCell(row, id));

        row++;
    }
    
    // Update statistics dashboard
    updateStatistics();
    
    // Reload lifecycle equipment list to sync with table
    loadEquipmentForLifecycle();
}

void Gestion_Equipement::rechercher(const QString &critere)
{
    QSqlQuery query;
    query.prepare("SELECT ID_EQUIPEMENT, NOM, CATEGORIE, QUANTITE_TOTALE, QUANTITE_DISPONIBLE, "
                  "ETAT, FOURNISSEUR, DATE_ACQUISITION, CIN_EMPLOYEE "
                  "FROM EQUIPEMENT "
                  "WHERE UPPER(NOM) LIKE :critere "
                  "OR UPPER(CATEGORIE) LIKE :critere "
                  "OR UPPER(FOURNISSEUR) LIKE :critere "
                  "OR UPPER(ETAT) LIKE :critere "
                  "ORDER BY ID_EQUIPEMENT");

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

        int id = query.value(0).toInt();

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(query.value(6).toString()));
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(query.value(7).toDate().toString("yyyy-MM-dd")));
        ui->tableWidget->setItem(row, 8, new QTableWidgetItem(query.value(8).toString()));

        // Add action buttons
        ui->tableWidget->setCellWidget(row, 9, createActionsCell(row, id));

        row++;
    }
}

// Helper Methods
void Gestion_Equipement::viderChamps()
{
    ui->lineEdit_id->clear();
    ui->lineEdit_nom->clear();
    ui->comboBox_categorie->setCurrentIndex(-1);
    ui->lineEdit_quantite_totale->clear();
    ui->lineEdit_quantite_disponible->clear();
    ui->comboBox_etat->setCurrentIndex(0);
    ui->lineEdit_fournisseur->clear();
    ui->dateEdit_acquisition->setDate(QDate::currentDate());
    ui->comboBox_cin_employee->setCurrentIndex(-1);
    ui->lineEdit_recherche->clear();
}

void Gestion_Equipement::remplirChamps(int row)
{
    ui->lineEdit_id->setText(ui->tableWidget->item(row, 0)->text());
    ui->lineEdit_nom->setText(ui->tableWidget->item(row, 1)->text());
    ui->comboBox_categorie->setCurrentText(ui->tableWidget->item(row, 2)->text());
    ui->lineEdit_quantite_totale->setText(ui->tableWidget->item(row, 3)->text());
    ui->lineEdit_quantite_disponible->setText(ui->tableWidget->item(row, 4)->text());
    ui->comboBox_etat->setCurrentText(ui->tableWidget->item(row, 5)->text());
    ui->lineEdit_fournisseur->setText(ui->tableWidget->item(row, 6)->text());
    ui->dateEdit_acquisition->setDate(QDate::fromString(ui->tableWidget->item(row, 7)->text(), "yyyy-MM-dd"));

    // Set CIN_EMPLOYEE combobox
    QString cinValue = ui->tableWidget->item(row, 8)->text();
    int index = ui->comboBox_cin_employee->findData(cinValue);
    if (index >= 0)
    {
        ui->comboBox_cin_employee->setCurrentIndex(index);
    }
}

void Gestion_Equipement::rafraichirTable()
{
    afficher();
}

void Gestion_Equipement::chargerEmployees()
{
    ui->comboBox_cin_employee->clear();
    ui->comboBox_cin_employee->addItem("-- Aucun --", "");

    QSqlQuery query;
    query.prepare("SELECT CIN_EMPLOYEE, NOM, PRENOM FROM EMPLOYEE ORDER BY NOM, PRENOM");

    if (query.exec())
    {
        while (query.next())
        {
            QString cin = query.value(0).toString();
            QString nom = query.value(1).toString();
            QString prenom = query.value(2).toString();
            QString displayText = QString("%1 - %2 %3").arg(cin, nom, prenom);
            ui->comboBox_cin_employee->addItem(displayText, cin);
        }
    }
    else
    {
        qDebug() << "Error loading employees:" << query.lastError().text();
    }
}

int Gestion_Equipement::genererNouvelID()
{
    QSqlQuery query;
    query.prepare("SELECT NVL(MAX(ID_EQUIPEMENT), 0) + 1 FROM EQUIPEMENT");

    if (query.exec() && query.next())
    {
        return query.value(0).toInt();
    }

    return 1; // Default to 1 if query fails
}

QWidget *Gestion_Equipement::createActionsCell(int row, int id)
{
    QWidget *container = new QWidget(ui->tableWidget);
    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(6);

    QPushButton *btnEdit = new QPushButton(container);
    QPushButton *btnDel = new QPushButton(container);

    btnEdit->setText("");
    btnDel->setText("");
    btnDel->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
    btnEdit->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));

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
        if (QMessageBox::question(this, "Suppression", "Supprimer cet équipement ?") == QMessageBox::Yes) {
            if (supprimer(id)) {
                QMessageBox::information(this, "Suppression", "Équipement supprimé.");
                afficher();
            } else {
                QMessageBox::critical(this, "Erreur", "Échec de suppression.");
            }
        } });

    // Edit action
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

void Gestion_Equipement::setEditMode(bool enabled)
{
    isEditMode = enabled;
    if (enabled)
    {
        ui->pushButton_ajouter->setText("Modifier");
        ui->lineEdit_id->setEnabled(false);
    }
    else
    {
        ui->pushButton_ajouter->setText("Ajouter");
        ui->lineEdit_id->setEnabled(true);
        currentEditingId = -1;
    }
}

void Gestion_Equipement::setFormFromRow(int row)
{
    ui->lineEdit_id->setText(ui->tableWidget->item(row, 0)->text());
    ui->lineEdit_nom->setText(ui->tableWidget->item(row, 1)->text());
    ui->comboBox_categorie->setCurrentText(ui->tableWidget->item(row, 2)->text());
    ui->lineEdit_quantite_totale->setText(ui->tableWidget->item(row, 3)->text());
    ui->lineEdit_quantite_disponible->setText(ui->tableWidget->item(row, 4)->text());
    ui->comboBox_etat->setCurrentText(ui->tableWidget->item(row, 5)->text());
    ui->lineEdit_fournisseur->setText(ui->tableWidget->item(row, 6)->text());
    ui->dateEdit_acquisition->setDate(QDate::fromString(ui->tableWidget->item(row, 7)->text(), "yyyy-MM-dd"));

    QString cinValue = ui->tableWidget->item(row, 8)->text();
    int index = ui->comboBox_cin_employee->findData(cinValue);
    if (index >= 0)
    {
        ui->comboBox_cin_employee->setCurrentIndex(index);
    }
}

// ===========================
// WEATHER API IMPLEMENTATION
// ===========================

void Gestion_Equipement::on_pushButton_get_weather_clicked()
{
    QString city = ui->lineEdit_city->text().trimmed();
    if (city.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer le nom d'une ville.");
        return;
    }
    
    fetchWeather(city);
    fetchForecast(city);
}

void Gestion_Equipement::fetchWeather(const QString &city)
{
    QString url = QString("https://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric&lang=fr")
                      .arg(city, weatherApiKey);
    
    QNetworkRequest request((QUrl(url)));
    QNetworkReply *reply = weatherManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleWeatherResponse(reply);
    });
}

void Gestion_Equipement::fetchForecast(const QString &city)
{
    QString url = QString("https://api.openweathermap.org/data/2.5/forecast?q=%1&appid=%2&units=metric&lang=fr")
                      .arg(city, weatherApiKey);
    
    QNetworkRequest request((QUrl(url)));
    QNetworkReply *reply = weatherManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleForecastResponse(reply);
    });
}

void Gestion_Equipement::handleWeatherResponse(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonObject jsonObj = jsonDoc.object();
        
        if (jsonObj.contains("cod") && jsonObj["cod"].toInt() == 200)
        {
            updateWeatherDisplay(jsonObj);
        }
        else
        {
            QMessageBox::warning(this, "Erreur", "Ville introuvable. Veuillez vérifier le nom.");
        }
    }
    else
    {
        QMessageBox::critical(this, "Erreur réseau", "Impossible de récupérer les données météo:\n" + reply->errorString());
    }
    
    reply->deleteLater();
}

void Gestion_Equipement::handleForecastResponse(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonObject jsonObj = jsonDoc.object();
        
        if (jsonObj.contains("list"))
        {
            QJsonArray forecastList = jsonObj["list"].toArray();
            updateForecastDisplay(forecastList);
        }
    }
    
    reply->deleteLater();
}

void Gestion_Equipement::updateWeatherDisplay(const QJsonObject &weatherData)
{
    // Extract main weather info
    QJsonObject main = weatherData["main"].toObject();
    QJsonArray weatherArray = weatherData["weather"].toArray();
    QJsonObject wind = weatherData["wind"].toObject();
    QString cityName = weatherData["name"].toString();
    
    double temp = main["temp"].toDouble();
    double feelsLike = main["feels_like"].toDouble();
    int humidity = main["humidity"].toInt();
    int pressure = main["pressure"].toInt();
    double windSpeed = wind["speed"].toDouble();
    
    QString description = "";
    if (!weatherArray.isEmpty())
    {
        QJsonObject weather = weatherArray[0].toObject();
        description = weather["description"].toString();
        // Capitalize first letter
        if (!description.isEmpty())
        {
            description[0] = description[0].toUpper();
        }
    }
    
    // Update UI
    ui->label_current_temp->setText(QString::number(qRound(temp)) + "°C");
    ui->label_weather_desc->setText(description);
    ui->label_city_name->setText(cityName);
    ui->label_humidity_value->setText(QString::number(humidity) + "%");
    ui->label_wind_value->setText(QString::number(qRound(windSpeed * 3.6)) + " km/h"); // Convert m/s to km/h
    ui->label_pressure_value->setText(QString::number(pressure) + " hPa");
    ui->label_feels_like_value->setText(QString::number(qRound(feelsLike)) + "°C");
    
    // Update weather recommendation based on conditions
    QString recommendation;
    QString styleSheet;
    
    // Check for poor weather conditions
    bool isRaining = description.contains("pluie", Qt::CaseInsensitive) || 
                     description.contains("orage", Qt::CaseInsensitive) || 
                     description.contains("bruine", Qt::CaseInsensitive);
    bool isSnowing = description.contains("neige", Qt::CaseInsensitive);
    bool isFoggy = description.contains("brouillard", Qt::CaseInsensitive) || 
                   description.contains("brume", Qt::CaseInsensitive);
    bool isStormy = description.contains("orage", Qt::CaseInsensitive);
    bool extremeTemp = temp > 40 || temp < -5;
    bool highWind = windSpeed > 15; // > 54 km/h
    
    if (isStormy)
    {
        recommendation = "⛔ Danger - Éviter toute utilisation de véhicules et équipements extérieurs";
        styleSheet = "background-color:#f8d7da; border:2px solid #dc3545; color:#721c24; border-radius:8px;";
    }
    else if (isSnowing || extremeTemp)
    {
        recommendation = "⚠️ Conditions difficiles - Limiter l'utilisation aux situations urgentes";
        styleSheet = "background-color:#fff3cd; border:2px solid #ffc107; color:#856404; border-radius:8px;";
    }
    else if (isRaining || isFoggy || highWind)
    {
        recommendation = "⚠️ Prudence requise - Adapter la conduite et l'utilisation des équipements";
        styleSheet = "background-color:#fff3cd; border:2px solid #ffc107; color:#856404; border-radius:8px;";
    }
    else if (temp >= 15 && temp <= 30 && humidity < 80 && windSpeed < 10)
    {
        recommendation = "✅ Conditions idéales pour l'utilisation de tous les équipements";
        styleSheet = "background-color:#d4edda; border:2px solid #28a745; color:#155724; border-radius:8px;";
    }
    else
    {
        recommendation = "✅ Conditions acceptables - Utilisation normale des équipements";
        styleSheet = "background-color:#d1ecf1; border:2px solid #17a2b8; color:#0c5460; border-radius:8px;";
    }
    
    ui->label_weather_recommendation->setText(recommendation);
    ui->frame_weather_recommendation->setStyleSheet(styleSheet);
    
    // Update timestamp
    QDateTime now = QDateTime::currentDateTime();
    ui->label_update_time->setText("Mise a jour: " + now.toString("dd/MM/yyyy hh:mm"));
}

void Gestion_Equipement::updateForecastDisplay(const QJsonArray &forecastList)
{
    // Forecast display removed from UI to save space
    // Weather widget now shows only current conditions
    Q_UNUSED(forecastList);
}

// ==================== EQUIPMENT LIFECYCLE & DEPRECIATION ====================

void Gestion_Equipement::loadEquipmentForLifecycle()
{
    ui->comboBox_lifecycle_equipment->clear();
    ui->comboBox_lifecycle_equipment->addItem("-- Sélectionnez un équipement --", -1);
    
    QSqlQuery query(db);
    query.prepare("SELECT ID_EQUIPEMENT, NOM, CATEGORIE FROM EQUIPEMENT ORDER BY NOM");
    
    if (query.exec())
    {
        while (query.next())
        {
            int id = query.value("ID_EQUIPEMENT").toInt();
            QString nom = query.value("NOM").toString();
            QString categorie = query.value("CATEGORIE").toString();
            QString displayText = QString("%1 (%2)").arg(nom, categorie);
            ui->comboBox_lifecycle_equipment->addItem(displayText, id);
        }
    }
}

void Gestion_Equipement::on_comboBox_lifecycle_equipment_currentIndexChanged(int index)
{
    if (index <= 0)
    {
        // Reset display
        ui->label_current_value->setText("0 TND");
        ui->label_original_cost->setText("0 TND");
        ui->label_depreciation_amount->setText("0 TND");
        ui->label_lifecycle_stage->setText("🟢 Nouveau");
        ui->label_lifecycle_stage->setStyleSheet("color:#28a745");
        ui->label_age_info->setText("Âge: 0 ans, 0 mois");
        ui->label_useful_life->setText("Vie utile: 0 / 5 ans (0%)");
        ui->label_replacement_year->setText("Estimé en: -");
        ui->label_replacement_recommendation->setText("Sélectionnez un équipement pour voir les recommandations");
        ui->progressBar_depreciation->setValue(0);
        ui->label_annual_depreciation->setText("0 TND/an");
        ui->label_monthly_depreciation->setText("0 TND/mois");
        ui->label_residual_value->setText("0 TND");
        return;
    }
    
    int equipmentId = ui->comboBox_lifecycle_equipment->currentData().toInt();
    calculateLifecycle(equipmentId);
}

void Gestion_Equipement::calculateLifecycle(int equipmentId)
{
    QSqlQuery query(db);
    query.prepare("SELECT NOM, CATEGORIE, DATE_ACQUISITION FROM EQUIPEMENT WHERE ID_EQUIPEMENT = :id");
    query.bindValue(":id", equipmentId);
    
    if (!query.exec() || !query.next())
    {
        QMessageBox::warning(this, "Erreur", "Équipement introuvable");
        return;
    }
    
    QString nom = query.value("NOM").toString();
    QString categorie = query.value("CATEGORIE").toString();
    QDate acquisitionDate = query.value("DATE_ACQUISITION").toDate();
    
    if (!acquisitionDate.isValid())
    {
        QMessageBox::warning(this, "Erreur", "Date d'acquisition invalide pour cet équipement");
        return;
    }
    
    // Estimate cost based on equipment type
    double estimatedCost = estimateEquipmentCost(nom, categorie);
    
    // Calculate lifecycle data
    LifecycleData data = computeDepreciation(acquisitionDate, categorie, estimatedCost);
    
    // Update UI
    ui->label_current_value->setText(QString::number(data.currentValue, 'f', 2) + " TND");
    ui->label_original_cost->setText(QString::number(data.originalCost, 'f', 2) + " TND");
    ui->label_depreciation_amount->setText(QString::number(data.depreciationAmount, 'f', 2) + " TND");
    
    // Lifecycle stage with color
    ui->label_lifecycle_stage->setText(data.lifecycleStage);
    if (data.lifecycleStage.contains("Nouveau"))
        ui->label_lifecycle_stage->setStyleSheet("color:#28a745");
    else if (data.lifecycleStage.contains("Bon"))
        ui->label_lifecycle_stage->setStyleSheet("color:#17a2b8");
    else if (data.lifecycleStage.contains("Vieillissant"))
        ui->label_lifecycle_stage->setStyleSheet("color:#ffc107");
    else
        ui->label_lifecycle_stage->setStyleSheet("color:#dc3545");
    
    // Age info
    QString ageText = QString("Âge: %1 an%2, %3 mois")
                          .arg(data.ageYears)
                          .arg(data.ageYears > 1 ? "s" : "")
                          .arg(data.ageMonths);
    ui->label_age_info->setText(ageText);
    
    // Useful life
    QString usefulLifeText = QString("Vie utile: %1 / %2 ans (%3%)")
                                 .arg(data.ageYears)
                                 .arg(data.usefulLifeYears)
                                 .arg(QString::number(data.usefulLifePercent, 'f', 0));
    ui->label_useful_life->setText(usefulLifeText);
    
    // Replacement year
    if (data.replacementYear > 0)
        ui->label_replacement_year->setText("Estimé en: " + QString::number(data.replacementYear));
    else
        ui->label_replacement_year->setText("Remplacer maintenant");
    
    // Recommendation
    ui->label_replacement_recommendation->setText(data.recommendation);
    
    // Progress bar
    ui->progressBar_depreciation->setValue(static_cast<int>(data.depreciationPercent));
    
    // Financial summary
    ui->label_annual_depreciation->setText(QString::number(data.annualDepreciation, 'f', 2) + " TND/an");
    ui->label_monthly_depreciation->setText(QString::number(data.monthlyDepreciation, 'f', 2) + " TND/mois");
    ui->label_residual_value->setText(QString::number(data.residualValue, 'f', 2) + " TND");
}

Gestion_Equipement::LifecycleData Gestion_Equipement::computeDepreciation(
    const QDate &acquisitionDate, const QString &category, double estimatedCost)
{
    LifecycleData data;
    data.originalCost = estimatedCost;
    
    QDate currentDate = QDate::currentDate();
    int totalDays = acquisitionDate.daysTo(currentDate);
    
    // Calculate age
    data.ageYears = totalDays / 365;
    data.ageMonths = (totalDays % 365) / 30;
    
    // Get useful life based on category
    data.usefulLifeYears = getUsefulLife(category);
    
    // Calculate depreciation using straight-line method
    double totalMonths = data.ageYears * 12 + data.ageMonths;
    double usefulLifeMonths = data.usefulLifeYears * 12;
    
    // Residual value is 10% of original cost
    data.residualValue = estimatedCost * 0.10;
    double depreciableAmount = estimatedCost - data.residualValue;
    
    // Annual and monthly depreciation
    data.annualDepreciation = depreciableAmount / data.usefulLifeYears;
    data.monthlyDepreciation = depreciableAmount / usefulLifeMonths;
    
    // Calculate current depreciation
    if (totalMonths >= usefulLifeMonths)
    {
        // Fully depreciated
        data.depreciationAmount = depreciableAmount;
        data.currentValue = data.residualValue;
        data.depreciationPercent = 100.0;
    }
    else
    {
        // Partial depreciation
        data.depreciationAmount = data.monthlyDepreciation * totalMonths;
        data.currentValue = estimatedCost - data.depreciationAmount;
        data.depreciationPercent = (data.depreciationAmount / depreciableAmount) * 100.0;
    }
    
    // Calculate useful life percentage
    data.usefulLifePercent = (static_cast<double>(data.ageYears) / data.usefulLifeYears) * 100.0;
    if (data.usefulLifePercent > 100.0)
        data.usefulLifePercent = 100.0;
    
    // Determine lifecycle stage
    data.lifecycleStage = getLifecycleStage(data.usefulLifePercent);
    
    // Calculate replacement year
    int yearsRemaining = data.usefulLifeYears - data.ageYears;
    if (yearsRemaining > 0)
        data.replacementYear = currentDate.year() + yearsRemaining;
    else
        data.replacementYear = 0; // Should replace now
    
    // Generate recommendation
    if (data.usefulLifePercent >= 100.0)
    {
        data.recommendation = "⚠️ Équipement en fin de vie. Remplacement urgent recommandé pour maintenir la qualité du service.";
    }
    else if (data.usefulLifePercent >= 75.0)
    {
        data.recommendation = QString("📅 Planifier le remplacement dans les ") + 
                             QString::number(yearsRemaining) + 
                             QString(" année(s) à venir. Budgétiser environ ") +
                             QString::number(estimatedCost, 'f', 0) + QString(" TND.");
    }
    else if (data.usefulLifePercent >= 50.0)
    {
        data.recommendation = QString("✅ Équipement en bon état. Continuer la maintenance régulière. ") +
                             QString("Remplacement prévu en ") + QString::number(data.replacementYear) + QString(".");
    }
    else
    {
        data.recommendation = QString("🌟 Équipement récent en excellente condition. ") +
                             QString("Valeur actuelle: ") + QString::number(data.currentValue, 'f', 0) + QString(" TND.");
    }
    
    return data;
}

int Gestion_Equipement::getUsefulLife(const QString &category)
{
    // Define useful life years by equipment category
    if (category.contains("Véhicule", Qt::CaseInsensitive) || 
        category.contains("Voiture", Qt::CaseInsensitive))
    {
        return 8; // Vehicles: 8 years
    }
    else if (category.contains("Informatique", Qt::CaseInsensitive) ||
             category.contains("Électronique", Qt::CaseInsensitive))
    {
        return 5; // Electronics: 5 years
    }
    else if (category.contains("Mobilier", Qt::CaseInsensitive) ||
             category.contains("Bureautique", Qt::CaseInsensitive))
    {
        return 10; // Furniture: 10 years
    }
    else if (category.contains("Sécurité", Qt::CaseInsensitive) ||
             category.contains("Signalisation", Qt::CaseInsensitive))
    {
        return 7; // Safety equipment: 7 years
    }
    else if (category.contains("Pédagogique", Qt::CaseInsensitive) ||
             category.contains("Formation", Qt::CaseInsensitive))
    {
        return 6; // Educational materials: 6 years
    }
    else
    {
        return 5; // Default: 5 years
    }
}

QString Gestion_Equipement::getLifecycleStage(double usefulLifePercent)
{
    if (usefulLifePercent >= 100.0)
        return "🔴 À Remplacer";
    else if (usefulLifePercent >= 75.0)
        return "🟠 Vieillissant";
    else if (usefulLifePercent >= 50.0)
        return "🟡 Bon État";
    else if (usefulLifePercent >= 25.0)
        return "🟢 Récent";
    else
        return "🟢 Nouveau";
}

double Gestion_Equipement::estimateEquipmentCost(const QString &nom, const QString &category)
{
    // Estimate equipment cost based on name and category
    // This is a simplified estimation - in real scenarios, you'd store actual costs in database
    
    QString lowerName = nom.toLower();
    QString lowerCategory = category.toLower();
    
    // Vehicles
    if (lowerCategory.contains("véhicule") || lowerCategory.contains("voiture"))
    {
        if (lowerName.contains("mercedes") || lowerName.contains("bmw") || lowerName.contains("audi"))
            return 80000.0; // Luxury vehicles
        else
            return 45000.0; // Standard vehicles
    }
    
    // Electronics & IT
    if (lowerCategory.contains("informatique") || lowerCategory.contains("électronique"))
    {
        if (lowerName.contains("ordinateur") || lowerName.contains("pc"))
            return 3000.0;
        else if (lowerName.contains("imprimante") || lowerName.contains("printer"))
            return 1500.0;
        else if (lowerName.contains("projecteur") || lowerName.contains("écran"))
            return 2500.0;
        else
            return 1000.0;
    }
    
    // Educational materials
    if (lowerCategory.contains("pédagogique") || lowerCategory.contains("formation"))
    {
        if (lowerName.contains("simulateur"))
            return 8000.0;
        else if (lowerName.contains("panneau") || lowerName.contains("signalisation"))
            return 500.0;
        else
            return 1500.0;
    }
    
    // Safety equipment
    if (lowerCategory.contains("sécurité"))
    {
        if (lowerName.contains("extincteur"))
            return 300.0;
        else if (lowerName.contains("caméra") || lowerName.contains("camera"))
            return 2000.0;
        else
            return 800.0;
    }
    
    // Furniture
    if (lowerCategory.contains("mobilier") || lowerCategory.contains("bureautique"))
    {
        if (lowerName.contains("bureau"))
            return 1200.0;
        else if (lowerName.contains("chaise") || lowerName.contains("siège"))
            return 400.0;
        else if (lowerName.contains("armoire") || lowerName.contains("meuble"))
            return 1800.0;
        else
            return 800.0;
    }
    
    // Default estimation
    return 2000.0;
}

// Statistics Methods
void Gestion_Equipement::updateStatistics()
{
    // Update total equipment count
    int totalEquipment = ui->tableWidget->rowCount();
    ui->label_total_equipment_value->setText(QString::number(totalEquipment));
    
    // Update total value and category breakdown
    calculateTotalValue();
    updateCategoryBreakdown();
}

void Gestion_Equipement::calculateTotalValue()
{
    double totalValue = 0.0;
    
    QSqlQuery query;
    query.prepare("SELECT NOM, CATEGORIE, DATE_ACQUISITION FROM EQUIPEMENT");
    
    if (query.exec())
    {
        while (query.next())
        {
            QString nom = query.value(0).toString();
            QString categorie = query.value(1).toString();
            QDate dateAcquisition = query.value(2).toDate();
            
            // Estimate cost and calculate current value
            double estimatedCost = estimateEquipmentCost(nom, categorie);
            int usefulLifeYears = getUsefulLife(categorie);
            
            // Calculate age in months
            int ageMonths = dateAcquisition.daysTo(QDate::currentDate()) / 30;
            int usefulLifeMonths = usefulLifeYears * 12;
            
            // Calculate depreciation
            double residualValue = estimatedCost * 0.10;
            double depreciableAmount = estimatedCost - residualValue;
            double monthlyDepreciation = depreciableAmount / usefulLifeMonths;
            double totalDepreciation = monthlyDepreciation * ageMonths;
            
            if (totalDepreciation > depreciableAmount)
                totalDepreciation = depreciableAmount;
            
            double currentValue = estimatedCost - totalDepreciation;
            totalValue += currentValue;
        }
    }
    
    ui->label_total_value_amount->setText(QString::number(totalValue, 'f', 0) + " TND");
}

void Gestion_Equipement::updateCategoryBreakdown()
{
    QMap<QString, int> categoryCount;
    
    QSqlQuery query;
    query.prepare("SELECT CATEGORIE FROM EQUIPEMENT");
    
    if (query.exec())
    {
        while (query.next())
        {
            QString categorie = query.value(0).toString();
            categoryCount[categorie]++;
        }
    }
    
    // Build display string
    QString statsText;
    for (auto it = categoryCount.begin(); it != categoryCount.end(); ++it)
    {
        statsText += it.key() + ": " + QString::number(it.value()) + "\n";
    }
    
    if (statsText.isEmpty())
        statsText = "Aucune donnée disponible";
    
    ui->label_category_stats->setText(statsText);
}
