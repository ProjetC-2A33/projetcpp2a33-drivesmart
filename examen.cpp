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
#include <QComboBox>
#include <QLineEdit>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QTextEdit>
#include <utility>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QMap>
#include <QColor>
#include <QtMath>
#include <QDateTime>

// utilitaire local : remplit le comboBox avec "CIN - Nom" (stocke le CIN en userData)
static void populateCandidateCINs(QComboBox *combo)
{
    if (!combo) return;
    combo->clear();
    QSqlQuery q;
    // essayer quelques variantes courantes de table/colonnes (CIN + NOM)
    const QStringList attempts = {
        "SELECT CIN_CONDIDAT, NOM FROM CANDIDAT",
        "SELECT CIN_CONDIDAT, NOM_CONDIDAT FROM CANDIDAT",
        "SELECT CIN_CONDIDAT, NOM FROM CONDIDAT",
        "SELECT CIN_CONDIDAT, NOM_CONDIDAT FROM CONDIDAT",
        "SELECT CIN, NOM FROM CANDIDAT",
        "SELECT CIN, NOM_CONDIDAT FROM CANDIDAT"
    };

    bool anyOk = false;
    for (const QString &sql : attempts) {
        if (q.exec(sql)) {
            anyOk = true;
            break;
        }
    }
    if (!anyOk) return; // aucune requête réussie

    while (q.next()) {
        QString cin = q.value(0).toString().trimmed();
        QString nom = q.value(1).toString().trimmed();
        if (cin.isEmpty()) continue;
        QString display = cin;
        if (!nom.isEmpty()) display = cin + " - " + nom;
        // éviter doublons : vérifie si un item avec la même data (cin) existe déjà
        if (combo->findData(cin) == -1) {
            combo->addItem(display, QVariant(cin));
        }
    }
}

Examen::Examen(QWidget *parent) : QWidget(parent),
    ui(new Ui::Examen)
{
    ui->setupUi(this);

    // ensure label_3 is ready to display the generated pixmap
    if (ui->label_3) {
        ui->label_3->setAlignment(Qt::AlignCenter);
        ui->label_3->setText(""); // clear initial text
    }

    // Initialiser l'historique au démarrage
    afficher_historique();

    // remplir le comboBox des CIN avec les CIN issus de la table candidat/condidat
    populateCandidateCINs(ui->comboBox_5);

    // Connect navigation buttons
    connect(ui->condidat_5, &QPushButton::clicked, this, &Examen::navigateToCandidat);
    connect(ui->planning_5, &QPushButton::clicked, this, &Examen::navigateToPlanning);
    connect(ui->employee_5, &QPushButton::clicked, this, &Examen::navigateToEmployee);
    connect(ui->vehicule_5, &QPushButton::clicked, this, &Examen::navigateToVehicule);
    connect(ui->examen_5, &QPushButton::clicked, this, &Examen::navigateToExamen);
    connect(ui->equipment_5, &QPushButton::clicked, this, &Examen::navigateToEquipement);

    // Initialize table headers (ensure 6 columns: 5 data + 1 actions)
    ui->tab_5->setColumnCount(6);
    QStringList headers;
    headers << "ID_EXAMEN" << "CIN_CONDIDAT" << "TYPE_EXAMEN" << "TYPE_PERMIS" << "DATE_EXAMEN" << "Actions";
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

    // Connecter la recherche en temps réel (supposant ui->textEdit_16 existe et est un QTextEdit)
    if (ui->textEdit_16) {
        // QTextEdit::textChanged() n'envoie pas le texte, on lit toPlainText() dans le slot lambda
        connect(ui->textEdit_16, &QTextEdit::textChanged, this, [this]() {
            QString text = ui->textEdit_16->toPlainText().trimmed();
            if (text.isEmpty()) refreshTable();
            else rechercher(text);
        });
    }

    // Connecter le comboBox de tri (supposant ui->comboBox_4 existe)
    if (ui->comboBox_4) {
        ui->comboBox_4->addItem("Sans tri", 0);
        ui->comboBox_4->addItem("Date ↑ (croissant)", 1);
        ui->comboBox_4->addItem("Date ↓ (décroissant)", 2);
        connect(ui->comboBox_4, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &Examen::onTriComboChanged);
    }

    // --- NOUVEAU : comboBox_6 pour statistiques par type d'examen ---
    if (ui->comboBox_6) {
        ui->comboBox_6->clear();
        ui->comboBox_6->addItem("Tous");
        QSqlQuery q;
        // essayer de récupérer les types existants dynamiquement
        if (q.exec("SELECT DISTINCT TYPE_EXAMEN FROM EXAMEN")) {
            while (q.next()) {
                QString t = q.value(0).toString().trimmed();
                if (!t.isEmpty() && ui->comboBox_6->findText(t) == -1)
                    ui->comboBox_6->addItem(t);
            }
        } else {
            // fallback si la requête échoue
            ui->comboBox_6->addItem("code");
            ui->comboBox_6->addItem("conduit");
        }
        connect(ui->comboBox_6, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &Examen::onComboBox6Changed);
        // afficher stats initiales
        onComboBox6Changed(ui->comboBox_6->currentIndex());
    }

    // Connecter le bouton export PDF (supposant ui->pushButton_exportPDF existe)
    if (ui->pushButton_8) {
        connect(ui->pushButton_8, &QPushButton::clicked, this, &Examen::exportPDF);
    }

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

    // NEW: read and validate CIN (on utilise la data stockée)
    QString cinCand = ui->comboBox_5->currentData().toString().trimmed();
    if (cinCand.isEmpty() || ui->comboBox_5->currentIndex() == -1)
    {
        QMessageBox::warning(this, "Erreur", "CIN du candidat est requis.");
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
        // NEW: clear CIN selection
        ui->comboBox_5->setCurrentIndex(-1);
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

    // NEW: validate CIN for modification as well (use data)
    QString cinCand = ui->comboBox_5->currentData().toString().trimmed();
    if (cinCand.isEmpty() || ui->comboBox_5->currentIndex() == -1)
    {
        QMessageBox::warning(this, "Erreur", "CIN du candidat est requis.");
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
    // NEW: include CIN_CANDIDAT in select (order: ID, CIN, TYPE_EXAMEN, TYPE_PERMIS, DATE)
    query.prepare("SELECT ID_EXAMEN, CIN_CONDIDAT, TYPE_EXAMEN, TYPE_PERMIS, DATE_EXAMEN FROM EXAMEN");
    if (!query.exec())
    {
        QSqlError error = query.lastError();
        QMessageBox::critical(nullptr, "Erreur SQL",
                              QString("Erreur lors de l'affichage:\n%1")
                                  .arg(error.text()));
        return model;
    }
    // utiliser move pour éviter l'avertissement de dépréciation (Qt accepte QSqlQuery&&)
    model->setQuery(std::move(query));
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
    query.prepare("INSERT INTO EXAMEN (ID_EXAMEN, CIN_CONDIDAT, TYPE_EXAMEN, TYPE_PERMIS, DATE_EXAMEN) "
                  "VALUES (:id, :cin, :type_examen, :type_permis, :date_examen)");
    query.bindValue(":id", id_examen);
    query.bindValue(":cin", ui->comboBox_5->currentData().toString().trimmed());
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

    // Enregistrer dans l'historique
    QString details = QString("ID:%1 | CIN:%2 | Type:%3 | Permis:%4 | Date:%5")
                     .arg(id_examen)
                     .arg(ui->comboBox_5->currentData().toString().trimmed())
                     .arg(type_examen)
                     .arg(type_permis)
                     .arg(date_examen.toString("yyyy-MM-dd"));
    enregistrer_historique("AJOUT", details);
    afficher_historique();

    // Effacer le champ ID_EXAMEN après ajout
    if (ui->textEdit_6) ui->textEdit_6->clear();

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

    // Enregistrer dans l'historique
    enregistrer_historique("SUPPRESSION", QString("ID:%1").arg(id_examen));
    afficher_historique();

    // Effacer le champ ID_EXAMEN après suppression
    if (ui->textEdit_6) ui->textEdit_6->clear();

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
    query.prepare("UPDATE EXAMEN SET ID_EXAMEN = :newid, CIN_CONDIDAT = :cin, TYPE_EXAMEN = :type_examen, TYPE_PERMIS = :type_permis, DATE_EXAMEN = :date_examen "
                  "WHERE ID_EXAMEN = :origid");
    query.bindValue(":newid", newId);
    query.bindValue(":cin", ui->comboBox_5->currentData().toString().trimmed());
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

    // Enregistrer dans l'historique
    QString details = QString("Ancien ID:%1 → Nouveau ID:%2 | Type:%3 | Permis:%4 | Date:%5")
                     .arg(originalId)
                     .arg(newId)
                     .arg(type_examen)
                     .arg(type_permis)
                     .arg(date_examen.toString("yyyy-MM-dd"));
    enregistrer_historique("MODIFICATION", details);
    afficher_historique();

    // Effacer le champ ID_EXAMEN après modification
    if (ui->textEdit_6) ui->textEdit_6->clear();

    return true;
}

// Enregistrer une action dans l'historique
void Examen::enregistrer_historique(const QString &action, const QString &details)
{
    QSqlQuery query;
    query.prepare("INSERT INTO EXAMEN_HISTORIQUE (ACTION, DETAILS, DATE_ACTION) "
                  "VALUES (:action, :details, :date_action)");
    query.bindValue(":action", action);
    query.bindValue(":details", details);
    query.bindValue(":date_action", QDateTime::currentDateTime());

    if (!query.exec()) {
        qWarning() << "Erreur enregistrement historique:" << query.lastError().text();
    }
}

// Afficher l'historique dans textEdit_7 (fusion EXAMEN_HISTORIQUE + fallback EXAMEN)
void Examen::afficher_historique()
{
    // Cherche textEdit_7 dans l'UI
    QTextEdit *histTextEdit = nullptr;
    if (ui->textEdit_7) histTextEdit = ui->textEdit_7;
    else histTextEdit = findChild<QTextEdit *>("textEdit_7");

    if (!histTextEdit) {
        qWarning() << "Aucun QTextEdit trouvé pour afficher l'historique (nom attendu : textEdit_7)";
        return;
    }

    const int maxRows = 100;
    int count = 0;
    QString histText;
    histText.reserve(8192);
    histText += "<html><body style='font-family: Arial; margin: 10px;'>";
    histText += "<h2 style='color:#333; text-align:center;'>HISTORIQUE DES OPÉRATIONS</h2>";
    histText += "<hr style='border: 1px solid #ccc;'>";

    // Structure pour stocker les entrées d'historique (action, details, date)
    struct HistEntry {
        QString action, details, dateStr;
        QDateTime dateTime;
    };
    QList<HistEntry> entries;

    // Étape 1 : Lire EXAMEN_HISTORIQUE (vraies opérations)
    QSqlQuery q;
    if (q.exec("SELECT ACTION, DETAILS, DATE_ACTION FROM EXAMEN_HISTORIQUE ORDER BY DATE_ACTION DESC")) {
        while (q.next()) {
            HistEntry e;
            e.action = q.value(0).toString();
            e.details = q.value(1).toString();
            e.dateTime = q.value(2).toDateTime();
            e.dateStr = e.dateTime.isValid() ? e.dateTime.toString("dd/MM/yyyy hh:mm:ss") : q.value(2).toString();
            entries.append(e);
        }
    } else {
        qWarning() << "Lecture EXAMEN_HISTORIQUE échouée:" << q.lastError().text();
    }

    // Étape 2 : Fallback - Lire tous les examens existants et ajouter comme "AJOUT" s'ils ne sont pas dans l'historique
    QSqlQuery qe;
    if (qe.exec("SELECT ID_EXAMEN, CIN_CONDIDAT, TYPE_EXAMEN, TYPE_PERMIS, DATE_EXAMEN FROM EXAMEN ORDER BY DATE_EXAMEN DESC")) {
        while (qe.next()) {
            QString id = qe.value(0).toString();
            QString cin = qe.value(1).toString();
            QString type = qe.value(2).toString();
            QString permis = qe.value(3).toString();
            QVariant dv = qe.value(4);
            QString dateStr;
            if (dv.canConvert<QDate>()) dateStr = dv.toDate().toString("dd/MM/yyyy");
            else if (dv.canConvert<QDateTime>()) dateStr = dv.toDateTime().toString("dd/MM/yyyy hh:mm:ss");
            else dateStr = dv.toString();

            // Vérifier si cet ID existe déjà dans l'historique
            bool found = false;
            for (const HistEntry &e : entries) {
                if (e.details.contains("ID:" + id)) {
                    found = true;
                    break;
                }
            }

            // Si pas trouvé, ajouter comme "AJOUT" (fallback pour les 6 ajoutes initiaux)
            if (!found) {
                HistEntry e;
                e.action = "AJOUT";
                e.details = QString("ID:%1 | CIN:%2 | Type:%3 | Permis:%4 | Date:%5")
                                .arg(id).arg(cin).arg(type).arg(permis).arg(dateStr);
                e.dateStr = dateStr;
                e.dateTime = QDateTime(); // pas de dateTime pour fallback
                entries.append(e);
            }
        }
    }

    // Étape 3 : Trier par date décroissante et afficher
    std::sort(entries.begin(), entries.end(), [](const HistEntry &a, const HistEntry &b) {
        // Si les deux ont une dateTime valide, les comparer
        if (a.dateTime.isValid() && b.dateTime.isValid()) {
            return a.dateTime > b.dateTime;
        }
        // Sinon, garder l'ordre de lecture (celui de la requête)
        return false;
    });

    for (const HistEntry &e : entries) {
        if (count >= maxRows) break;

        QString actionColor;
        if (e.action == "AJOUT") actionColor = "#4CAF50";
        else if (e.action == "MODIFICATION") actionColor = "#FFC107";
        else if (e.action == "SUPPRESSION") actionColor = "#F44336";
        else actionColor = "#333333";

        histText += QString(
            "<div style='margin-bottom: 12px; padding: 8px; background-color: #f9f9f9; border-left: 4px solid %1;'>"
            "<b style='color:%1; font-size: 14px;'>%2</b>"
            "<div style='color: #666; font-size: 12px; margin-top: 4px;'>%3</div>"
            "<div style='color: #999; font-size: 11px; margin-top: 4px;'>%4</div>"
            "</div>")
            .arg(actionColor)
            .arg(e.action.toHtmlEscaped())
            .arg(e.dateStr.toHtmlEscaped())
            .arg(e.details.toHtmlEscaped());

        ++count;
    }

    histText += QString(
        "<hr style='border: 1px solid #ccc;'>"
        "<p style='text-align: center; color: #999; font-size: 12px;'>"
        "<i>Affichage : %1 action(s) (max %2)</i></p>")
        .arg(count)
        .arg(maxRows);

    histText += "</body></html>";

    histTextEdit->setHtml(histText);
    histTextEdit->setReadOnly(true);
}

void Examen::refreshTable()
{
    std::unique_ptr<QSqlQueryModel> model(afficher());
    // actualiser aussi la liste des CIN disponibles (en cas d'ajout/suppression de candidats)
    populateCandidateCINs(ui->comboBox_5);
    int rows = model->rowCount();
    int cols = model->columnCount();
    ui->tab_5->setRowCount(rows);
    ui->tab_5->setColumnCount(cols + 1); // +1 for actions
    // NEW: headers include CIN_CANDIDAT
    QStringList headers;
    headers << "ID_EXAMEN" << "CIN_CONDIDAT" << "TYPE_EXAMEN" << "TYPE_PERMIS" << "DATE_EXAMEN" << "Actions";
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

    // Rafraîchir l'historique après chaque mise à jour de la table
    afficher_historique();
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
    // NEW: CIN at column 1 -> sélectionner l'item du combo en fonction de la data (cin)
    {
        QString cin = getText(1).trimmed();
        int idx = ui->comboBox_5->findData(cin);
        if (idx != -1) ui->comboBox_5->setCurrentIndex(idx);
        else ui->comboBox_5->setCurrentText(cin); // si pas trouvé, affiche quand même
    }
    QString typeExamenStr = getText(2).trimmed().toLower();
    ui->radioButton_3->setChecked(typeExamenStr == "code");
    ui->radioButton_4->setChecked(typeExamenStr == "conduit");
    ui->textEdit_12->setText(getText(3)); // TYPE_PERMIS
    // Date: now at column 4
    QDate d = QDate::fromString(getText(4), Qt::ISODate);
    if (!d.isValid())
        d = QDate::fromString(getText(4), "dd/MM/yyyy");
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
void Examen::onTriComboChanged(int index)
{
    if (index == 0) {
        // Sans tri : rafraîchir simplement le tableau
        refreshTable();
        return;
    }

    bool ascending = (index == 1); // index 1 = croissant, index 2 = décroissant
    tri(ascending);
}

void Examen::tri(bool ascending)
{
    QSqlQuery query;
    QString order = ascending ? "ASC" : "DESC";
    query.prepare(QString("SELECT ID_EXAMEN, CIN_CONDIDAT, TYPE_EXAMEN, TYPE_PERMIS, DATE_EXAMEN FROM EXAMEN ORDER BY DATE_EXAMEN %1").arg(order));

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors du tri : " + query.lastError().text());
        return;
    }

    ui->tab_5->setRowCount(0);
    int row = 0;

    while (query.next()) {
        ui->tab_5->insertRow(row);

        int id = query.value(0).toInt();
        QString cin = query.value(1).toString();
        QString type_examen = query.value(2).toString();
        QString type_permis = query.value(3).toString();
        QDate date = query.value(4).toDate();

        ui->tab_5->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
        ui->tab_5->setItem(row, 1, new QTableWidgetItem(cin));
        ui->tab_5->setItem(row, 2, new QTableWidgetItem(type_examen));
        ui->tab_5->setItem(row, 3, new QTableWidgetItem(type_permis));
        ui->tab_5->setItem(row, 4, new QTableWidgetItem(date.toString("yyyy-MM-dd")));
        ui->tab_5->setCellWidget(row, 5, createActionsCell(row, QString::number(id)));

        row++;
    }

    // Rafraîchir l'historique après tri
    afficher_historique();
}

void Examen::exportPDF()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize::A4);
    // Ne pas appeler setPageMargins avec l'enum non supporté — garder marges par défaut
    // Si vous voulez définir les marges, utilisez QPagedPaintDevice::PageMargins (selon version Qt).

    QPainter painter;
    if (!painter.begin(&writer)) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le fichier PDF.");
        return;
    }

    // Titre
    QFont titleFont = painter.font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(50, 50, "Liste des Examens");

    // Entête du tableau
    int y = 100;
    int colWidth = 100;
    int rowHeight = 20;
    QStringList headers = {"ID", "CIN", "Type", "Permis", "Date"};
    QFont headerFont = painter.font();
    headerFont.setBold(true);
    headerFont.setPointSize(10);
    painter.setFont(headerFont);

    for (int col = 0; col < headers.count(); ++col) {
        painter.drawText(50 + col * colWidth, y, colWidth, rowHeight, Qt::AlignCenter, headers[col]);
    }

    // Ligne de séparation
    painter.drawLine(50, y + rowHeight, 50 + headers.count() * colWidth, y + rowHeight);
    y += rowHeight + 5;

    // Données du tableau
    QFont dataFont = painter.font();
    dataFont.setBold(false);
    dataFont.setPointSize(9);
    painter.setFont(dataFont);

    for (int row = 0; row < ui->tab_5->rowCount(); ++row) {
        if (y > 280) { // nouvelle page si nécessaire
            writer.newPage();
            y = 50;
        }
        for (int col = 0; col < 5; ++col) {
            QTableWidgetItem *item = ui->tab_5->item(row, col);
            QString text = item ? item->text() : "";
            painter.drawText(50 + col * colWidth, y, colWidth, rowHeight, Qt::AlignLeft, text);
        }
        y += rowHeight;
    }

    painter.end();
    QMessageBox::information(this, "Succès", "PDF exporté avec succès : " + fileName);
}

void Examen::rechercher(const QString &critere)
{
    QSqlQuery query;
    // Compatible Oracle: utiliser TO_CHAR pour les conversions (id/date) et rechercher dans plusieurs colonnes
    query.prepare(
        "SELECT ID_EXAMEN, CIN_CONDIDAT, TYPE_EXAMEN, TYPE_PERMIS, DATE_EXAMEN "
        "FROM EXAMEN "
        "WHERE UPPER(TYPE_EXAMEN) LIKE :critere "
        "OR UPPER(TYPE_PERMIS) LIKE :critere "
        "OR UPPER(CIN_CONDIDAT) LIKE :critere "
        "OR UPPER(TO_CHAR(ID_EXAMEN)) LIKE :critere "
        "OR UPPER(TO_CHAR(DATE_EXAMEN, 'YYYY-MM-DD')) LIKE :critere "
        "ORDER BY ID_EXAMEN ASC");

    QString searchPattern = "%" + critere.toUpper() + "%";
    query.bindValue(":critere", searchPattern);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Erreur",
                              "Erreur lors de la recherche : " + query.lastError().text());
        return;
    }

    ui->tab_5->setRowCount(0);
    int row = 0;

    while (query.next())
    {
        ui->tab_5->insertRow(row);

        int id = query.value(0).toInt();
        QString cinCand = query.value(1).toString();
        QString typeExamen = query.value(2).toString();
        QString typePermis = query.value(3).toString();
        QDate dateExamen = query.value(4).toDate();

        ui->tab_5->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
        ui->tab_5->setItem(row, 1, new QTableWidgetItem(cinCand));
        ui->tab_5->setItem(row, 2, new QTableWidgetItem(typeExamen));
        ui->tab_5->setItem(row, 3, new QTableWidgetItem(typePermis));
        ui->tab_5->setItem(row, 4, new QTableWidgetItem(dateExamen.toString("yyyy-MM-dd")));

        row++;
    }

    // Rafraîchir l'historique après recherche
    afficher_historique();
}

// slot appelé quand comboBox_6 change
void Examen::onComboBox6Changed(int index)
{
    Q_UNUSED(index);
    if (!ui->comboBox_6) return;
    QString sel = ui->comboBox_6->currentText().trimmed();
    if (sel.isEmpty()) sel = "Tous";
    computeStats(sel);
}

// calcule et affiche des statistiques simples par type d'examen DANS label_3
void Examen::computeStats(const QString &/*filter*/)
{
    // Nouveau comportement : toujours montrer une seule statistique contenant "code" et "conduit"
    QMap<QString,int> counts;
    int total = 0;

    QSqlQuery q;
    // récupérer uniquement les deux types 'code' et 'conduit'
    if (!q.exec("SELECT TYPE_EXAMEN, COUNT(*) FROM EXAMEN WHERE UPPER(TYPE_EXAMEN) IN ('CODE','CONDUIT') GROUP BY TYPE_EXAMEN")) {
        if (ui->label_3) ui->label_3->setText("Erreur statistiques: " + q.lastError().text());
        return;
    }

    // initialiser à zéro pour garantir présence des deux clés
    counts["code"] = 0;
    counts["conduit"] = 0;

    while (q.next()) {
        QString t = q.value(0).toString().trimmed().toLower();
        int c = q.value(1).toInt();
        if (t == "code") counts["code"] = c;
        else if (t == "conduit") counts["conduit"] = c;
        total += c;
    }

    // si aucun examen des deux types
    if (total == 0) {
        if (ui->label_3) ui->label_3->setText("Aucun examen de type 'code' ou 'conduit' trouvé.");
        return;
    }

    // Dimensions du pixmap (taille du label)
    QSize targetSize = (ui->label_3 && !ui->label_3->size().isEmpty()) ? ui->label_3->size() : QSize(600, 360);
    int W = targetSize.width();
    int H = targetSize.height();
    QPixmap pix(W, H);
    pix.fill(Qt::white);
    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing);

    // Marges et zones
    int margin = 12;
    QRect titleRect(margin, margin, W - 2*margin, 28);
    int pieSize = qMin(W/2 - 2*margin, H - 120);
    QRect pieRect(margin, titleRect.bottom() + 10, pieSize, pieSize);
    QRect legendRect(pieRect.right() + margin, pieRect.top(), W - pieRect.right() - 2*margin, pieRect.height());

    // Titre
    QFont titleFont = painter.font(); titleFont.setPointSize(12); titleFont.setBold(true);
    painter.setFont(titleFont);
    QString titleText = QString("Répartition : code vs conduit (Total %1)").arg(total);
    painter.drawText(titleRect, Qt::AlignCenter, titleText);

    // Palette simple (code = bleu, conduit = vert)
    QVector<QColor> palette = { QColor("#1E88E5"), QColor("#43A047") };

    // Ordre fixe pour affichage cohérent
    QStringList keys = {"code", "conduit"};

    // Dessiner camembert et labels centrés sur segments
    double startAngleDeg = 0.0;
    QPoint pieCenter = pieRect.center();
    int radius = pieRect.width() / 2;
    for (int i = 0; i < keys.size(); ++i) {
        QString k = keys[i];
        int val = counts.value(k, 0);
        if (val <= 0) continue;
        double fraction = double(val) / double(total);
        double spanDeg = 360.0 * fraction;

        // dessin du segment
        QColor color = palette[i % palette.size()];
        painter.setBrush(color);
        painter.setPen(Qt::NoPen);
        int start16 = int(startAngleDeg * 16.0);
        int span16 = int(spanDeg * 16.0);
        painter.drawPie(pieRect, start16, span16);

        // calcul position label au centre du segment
        double midAngleDeg = startAngleDeg + spanDeg / 2.0;
        double midAngleRad = qDegreesToRadians(midAngleDeg);
        double labelRadius = radius * 0.6; // distance du centre
        int lx = pieCenter.x() + int(labelRadius * cos(midAngleRad));
        int ly = pieCenter.y() - int(labelRadius * sin(midAngleRad)); // y inverse
        QString labelText = QString("%1\n%2%").arg(val).arg(QString::number(fraction * 100.0, 'f', 1));

        // choisir couleur de texte selon luminosité
        double lum = 0.299*color.redF() + 0.587*color.greenF() + 0.114*color.blueF();
        QColor textColor = (lum < 0.6) ? Qt::white : Qt::black;
        QFont f = painter.font(); f.setPointSize(10); // size fixe
        painter.setFont(f);
        painter.setPen(textColor);
        QRectF textRect(lx - 30, ly - 12, 60, 24);
        painter.drawText(textRect, Qt::AlignCenter, labelText);

        startAngleDeg += spanDeg;
    }

    // dessiner contour du camembert
    painter.setPen(QPen(Qt::black, 1));
    painter.drawEllipse(pieRect);

    // Légende (simple, deux lignes) à droite
    QFont legendFont = painter.font(); legendFont.setPointSize(10);
    painter.setFont(legendFont);
    int legendX = legendRect.left();
    int legendY = legendRect.top() + 10;
    int lineHeight = 26;
    for (int i = 0; i < keys.size(); ++i) {
        QString k = keys[i];
        int val = counts.value(k, 0);
        double perc = (val > 0) ? (double(val) * 100.0 / double(total)) : 0.0;
        QColor color = palette[i % palette.size()];

        QRect colorBoxRect(legendX, legendY + i*lineHeight, 16, 16);
        painter.fillRect(colorBoxRect, color);
        painter.setPen(Qt::black);
        QString text = QString("%1 : %2 (%3%)").arg(k).arg(val).arg(QString::number(perc, 'f', 1));
        painter.drawText(legendX + 24, legendY + i*lineHeight + 12, text);
    }

    // Footer total
    QString footer = QString("Total (code+conduit) : %1").arg(total);
    painter.drawText(W - margin - 220, H - margin - 6, footer);

    painter.end();

    // Mettre le pixmap dans label_3
    if (ui->label_3) {
        ui->label_3->setPixmap(pix.scaled(ui->label_3->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        ui->label_3->setAlignment(Qt::AlignCenter);
    }
}
