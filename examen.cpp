#include "examen.h"
#include "ui_examen.h"
#include "navigation_constants.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QStackedWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QTableWidgetItem>
#include <QDate>
#include <memory>
#include <QPushButton>
#include <QHBoxLayout>
#include <QWidget>
#include <QHeaderView>
#include <QStyle>
#include <QIcon>

Examen::Examen(QWidget *parent) : QWidget(parent),
                                  ui(new Ui::Examen)
{
    ui->setupUi(this);

    // Connect navigation buttons
    connect(ui->condidat_5, &QPushButton::clicked, this, &Examen::navigateToCandidat);
    connect(ui->planning_5, &QPushButton::clicked, this, &Examen::navigateToPlanning);
    connect(ui->employee_5, &QPushButton::clicked, this, &Examen::navigateToEmployee);
    connect(ui->vehicule_5, &QPushButton::clicked, this, &Examen::navigateToVehicule);
    connect(ui->examen_5, &QPushButton::clicked, this, &Examen::navigateToExamen);
    connect(ui->equipment_5, &QPushButton::clicked, this, &Examen::navigateToEquipement);

    // Initialize table headers (ensure 5 columns: 4 data + 1 actions)
    ui->tab_5->setColumnCount(5);
    QStringList headers;
    headers << "ID_EXAMEN" << "TYPE_EXAMEN" << "TYPE_PERMIS" << "DATE_EXAMEN" << "Actions";
    ui->tab_5->setHorizontalHeaderLabels(headers);

    // Table visual/interaction tuning
    ui->tab_5->setShowGrid(true);
    ui->tab_5->setAlternatingRowColors(true);
    ui->tab_5->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tab_5->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tab_5->horizontalHeader()->setVisible(true);
    ui->tab_5->verticalHeader()->setVisible(true);
    ui->tab_5->horizontalHeader()->setStretchLastSection(true);
    ui->tab_5->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tab_5->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tab_5->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Load initial data
    refreshTable();
}

Examen::~Examen()
{
    delete ui;
}

void Examen::on_pushButton_4_clicked()
{
    QString id = ui->textEdit_6->toPlainText().trimmed();
    if (id.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "ID_EXAMEN est requis.");
        return;
    }

    QString type_examen = ui->radioButton_3->isChecked() ? "code" : (ui->radioButton_4->isChecked() ? "conduit" : "");
    if (type_examen.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Type d'examen est requis.");
        return;
    }

    QDate date = ui->dateEdit_5->date();
    QString type_permis = ui->textEdit_12->toPlainText().trimmed();
    if (type_permis.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Type de permis est requis.");
        return;
    }

    if (isEditMode)
    {
        QString originalId = !currentEditingId.isEmpty() ? currentEditingId : selectedIdFromTable();
        if (originalId.isEmpty())
        {
            QMessageBox::warning(this, "Modification", "Sélectionnez une ligne.");
            return;
        }
        if (modifier(originalId, id, type_examen, type_permis, date))
        {
            QMessageBox::information(this, "Modification", "Examen modifié.");
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
    this->id_examen = id;
    this->type_examen = type_examen;
    this->type_permis = type_permis;
    this->date_examen = date;

    if (ajouter())
    {
        QMessageBox::information(this, "Succès", "Examen ajouté avec succès!");
        refreshTable();
        // Clear form
        ui->textEdit_6->clear();
        ui->textEdit_12->clear();
        ui->radioButton_3->setChecked(false);
        ui->radioButton_4->setChecked(false);
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Échec d'ajout de l'examen.");
    }
}

void Examen::on_pushButton_5_clicked()
{
    QString originalId = !currentEditingId.isEmpty() ? currentEditingId : selectedIdFromTable();
    if (originalId.isEmpty())
    {
        QMessageBox::warning(this, "Modification", "Sélectionnez une ligne.");
        return;
    }

    QString id = ui->textEdit_6->toPlainText().trimmed();
    if (id.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "ID_EXAMEN est requis.");
        return;
    }

    QString type_examen = ui->radioButton_3->isChecked() ? "code" : (ui->radioButton_4->isChecked() ? "conduit" : "");
    if (type_examen.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Type d'examen est requis.");
        return;
    }

    QDate date = ui->dateEdit_5->date();
    QString type_permis = ui->textEdit_12->toPlainText().trimmed();
    if (type_permis.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Type de permis est requis.");
        return;
    }

    if (modifier(originalId, id, type_examen, type_permis, date))
    {
        QMessageBox::information(this, "Modification", "Examen modifié.");
        refreshTable();
        currentEditingId = "";
        setEditMode(false);
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Échec de modification.");
    }
}

void Examen::on_pushButton_7_clicked()
{
    ui->textEdit_6->clear();
    ui->textEdit_12->clear();
    ui->radioButton_3->setChecked(false);
    ui->radioButton_4->setChecked(false);
    setEditMode(false);
    currentEditingId = "";
}

QSqlQueryModel *Examen::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT ID_EXAMEN, TYPE_EXAMEN, TYPE_PERMIS, DATE_EXAMEN FROM EXAMEN");
    if (!query.exec())
    {
        QSqlError error = query.lastError();
        QMessageBox::critical(nullptr, "Erreur SQL",
                              QString("Erreur lors de l'affichage:\n%1")
                                  .arg(error.text()));
        return model;
    }
    model->setQuery(query);
    return model;
}

bool Examen::ajouter()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen())
    {
        QMessageBox::critical(nullptr, "Erreur", "La connexion à la base de données n'est pas ouverte.");
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO EXAMEN (ID_EXAMEN, TYPE_EXAMEN, TYPE_PERMIS, DATE_EXAMEN) "
                  "VALUES (:id, :type_examen, :type_permis, :date_examen)");
    query.bindValue(":id", id_examen);
    query.bindValue(":type_examen", type_examen);
    query.bindValue(":type_permis", type_permis);
    query.bindValue(":date_examen", date_examen);

    if (!query.exec())
    {
        QSqlError error = query.lastError();
        QMessageBox::critical(nullptr, "Erreur SQL",
                              QString("Erreur lors de l'ajout:\n%1\n\nRequête: %2")
                                  .arg(error.text())
                                  .arg(query.lastQuery()));
        return false;
    }
    return true;
}

bool Examen::supprimer(QString id_examen)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen())
    {
        QMessageBox::critical(nullptr, "Erreur", "La connexion à la base de données n'est pas ouverte.");
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM EXAMEN WHERE ID_EXAMEN = :id");
    query.bindValue(":id", id_examen);

    if (!query.exec())
    {
        QSqlError error = query.lastError();
        QMessageBox::critical(nullptr, "Erreur SQL",
                              QString("Erreur lors de la suppression:\n%1")
                                  .arg(error.text()));
        return false;
    }
    return true;
}

bool Examen::modifier(QString originalId, QString newId, QString type_examen, QString type_permis, QDate date_examen)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen())
    {
        QMessageBox::critical(nullptr, "Erreur", "La connexion à la base de données n'est pas ouverte.");
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE EXAMEN SET ID_EXAMEN = :newid, TYPE_EXAMEN = :type_examen, TYPE_PERMIS = :type_permis, DATE_EXAMEN = :date_examen "
                  "WHERE ID_EXAMEN = :origid");
    query.bindValue(":newid", newId);
    query.bindValue(":type_examen", type_examen);
    query.bindValue(":type_permis", type_permis);
    query.bindValue(":date_examen", date_examen);
    query.bindValue(":origid", originalId);

    if (!query.exec())
    {
        QSqlError error = query.lastError();
        QMessageBox::critical(nullptr, "Erreur SQL",
                              QString("Erreur lors de la modification:\n%1")
                                  .arg(error.text()));
        return false;
    }
    return true;
}

void Examen::refreshTable()
{
    std::unique_ptr<QSqlQueryModel> model(afficher());
    int rows = model->rowCount();
    int cols = model->columnCount();
    ui->tab_5->setRowCount(rows);
    ui->tab_5->setColumnCount(cols + 1); // +1 for actions
    QStringList headers;
    headers << "ID_EXAMEN" << "TYPE_EXAMEN" << "TYPE_PERMIS" << "DATE_EXAMEN" << "Actions";
    ui->tab_5->setHorizontalHeaderLabels(headers);
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            QVariant v = model->data(model->index(r, c));
            ui->tab_5->setItem(r, c, new QTableWidgetItem(v.toString()));
        }
        // Add actions cell with Edit/Supp buttons
        QString id = model->data(model->index(r, 0)).toString();
        ui->tab_5->setCellWidget(r, cols, createActionsCell(r, id));
    }
}

QString Examen::selectedIdFromTable() const
{
    QList<QTableWidgetItem *> items = ui->tab_5->selectedItems();
    if (items.isEmpty())
        return "";
    int row = items.first()->row();
    QTableWidgetItem *idItem = ui->tab_5->item(row, 0);
    return idItem ? idItem->text() : "";
}

void Examen::setFormFromRow(int row)
{
    if (row < 0 || row >= ui->tab_5->rowCount())
        return;
    auto getText = [&](int col)
    { QTableWidgetItem *it = ui->tab_5->item(row, col); return it ? it->text() : QString(); };
    ui->textEdit_6->setText(getText(0)); // ID_EXAMEN
    QString typeExamenStr = getText(1).trimmed().toLower();
    ui->radioButton_3->setChecked(typeExamenStr == "code");
    ui->radioButton_4->setChecked(typeExamenStr == "conduit");
    ui->textEdit_12->setText(getText(2)); // TYPE_PERMIS
    // Date: try parse ISO or locale
    QDate d = QDate::fromString(getText(3), Qt::ISODate);
    if (!d.isValid())
        d = QDate::fromString(getText(3), "dd/MM/yyyy");
    if (d.isValid())
        ui->dateEdit_5->setDate(d);
}

QWidget *Examen::createActionsCell(int row, QString id)
{
    QWidget *container = new QWidget(ui->tab_5);
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

    // Set gray color for action buttons
    QString grayStyle = "background-color: #808080; border-radius: 3px; border: none;";
    btnEdit->setStyleSheet(grayStyle);
    btnDel->setStyleSheet(grayStyle);
    layout->addWidget(btnEdit);
    layout->addWidget(btnDel);
    container->setLayout(layout);

    // Delete action
    QObject::connect(btnDel, &QPushButton::clicked, this, [=]()
                     {
        if (id.isEmpty()) return;
        if (QMessageBox::question(this, "Suppression", "Supprimer cet examen ?") == QMessageBox::Yes) {
            if (supprimer(id)) {
                refreshTable();
            } else {
                QMessageBox::critical(this, "Erreur", "Échec de suppression.");
            }
        } });

    // Edit action: toggle edit mode. If clicking again on same row, exit edit mode
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

void Examen::setEditMode(bool enabled)
{
    isEditMode = enabled;
    if (enabled)
    {
        ui->pushButton_4->setText("Modifier");
    }
    else
    {
        ui->pushButton_4->setText("Ajouter");
        currentEditingId = "";
    }
}

void Examen::navigateToCandidat()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_CANDIDAT);
    }
}

void Examen::navigateToPlanning()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_PLANNING);
    }
}

void Examen::navigateToEmployee()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_EMPLOYEE);
    }
}

void Examen::navigateToVehicule()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_VEHICULE);
    }
}

void Examen::navigateToExamen()
{
    // Already in examen view
}

void Examen::navigateToEquipement()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_EQUIPEMENT);
    }
}
