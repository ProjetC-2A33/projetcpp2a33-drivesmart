#include "pageemploye.h"
#include "ui_pageemploye.h"
#include "employes.h"
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QDesktopServices>
#include <QUrl>
#include <QChart>
#include <QPieSeries>
#include <QtCharts>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>

pageemploye::pageemploye(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pageemploye)
    , networkManager(new QNetworkAccessManager(this))
    , originalCIN("")  // Initialize originalCIN as empty
{
    ui->setupUi(this);
    qDebug() << "pageemploye ouvert";

    // Configure CIN input: 8 digits only
    ui->cin_E->setMaxLength(8);
    ui->cin_E->setPlaceholderText("8 chiffres");
    QRegularExpression cinRegex("[0-9]{0,8}");
    QRegularExpressionValidator *cinValidator = new QRegularExpressionValidator(cinRegex, this);
    ui->cin_E->setValidator(cinValidator);

    afficherEmployes();

    // Connexions
    connect(ui->tab_em, &QTableWidget::cellClicked, this, &pageemploye::on_tab_em_cellClicked);
    connect(ui->btn_ajout_E, &QPushButton::clicked, this, &pageemploye::on_btn_ajout_E_clicked);
    connect(ui->btn_reset_E, &QPushButton::clicked, this, &pageemploye::on_btn_reset_E_clicked);
    connect(ui->modif_E, &QPushButton::clicked, this, &pageemploye::on_modif_E_clicked);
    connect(ui->sup_E, &QPushButton::clicked, this, &pageemploye::on_sup_E_clicked);
    connect(ui->btn_c, &QPushButton::clicked, this, &pageemploye::on_btn_c_clicked);
    connect(ui->trier_c, &QPushButton::clicked, this, &pageemploye::on_trier_c_clicked);
    connect(ui->pdf_c, &QPushButton::clicked, this, &pageemploye::on_pdf_c_clicked);
    connect(ui->stat_c, &QPushButton::clicked, this, &pageemploye::on_stat_c_clicked);
    connect(ui->recherche_c, &QLineEdit::textChanged, this, &pageemploye::on_recherche_c_textChanged);

    updateButtonStates();

    connect(ui->candidat_7, &QPushButton::clicked, this, &pageemploye::navigateToCandidat);
    connect(ui->planning_7, &QPushButton::clicked, this, &pageemploye::navigateToPlanning);
    connect(ui->employee_7, &QPushButton::clicked, this, &pageemploye::navigateToEmployee);
    connect(ui->vehicule_7, &QPushButton::clicked, this, &pageemploye::navigateToVehicule);
    connect(ui->examen_7, &QPushButton::clicked, this, &pageemploye::navigateToExamen);
    connect(ui->equipement_7, &QPushButton::clicked, this, &pageemploye::navigateToEquipement);
}

pageemploye::~pageemploye()
{
    delete ui;
}

void pageemploye::afficherEmployes()
{
    Employes emp;
    QSqlQueryModel *model = emp.afficher();

    if (!model) {
        QMessageBox::critical(this, "Erreur", "Impossible de charger les données depuis la base de données.\nVérifiez que la table EMPLOYEE existe.");
        ui->tab_em->setRowCount(0);
        return;
    }
    
    qDebug() << "Modèle chargé, nombre de lignes:" << model->rowCount();
    qDebug() << "Modèle colonnes:" << model->columnCount();

    ui->tab_em->setRowCount(0);
    ui->tab_em->setColumnCount(8);
    QStringList headers = {"CIN", "Nom", "Prénom", "Disponibilité", "Salaire", "Téléphone", "Email", "Poste"};
    ui->tab_em->setHorizontalHeaderLabels(headers);

    int totalRows = model->rowCount();
    if (totalRows == 0) {
        qDebug() << "Aucune donnée d'employé trouvée dans la base de données.";
    }
    
    for (int row = 0; row < totalRows; ++row) {
        ui->tab_em->insertRow(row);
        for (int col = 0; col < 8; ++col) {
            QVariant data = model->data(model->index(row, col));
            QString text = data.toString();
            QTableWidgetItem *item = new QTableWidgetItem(text);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->tab_em->setItem(row, col, item);
        }
    }

    ui->tab_em->resizeColumnsToContents();
    qDebug() << "Affiché :" << totalRows << "employés";

    delete model;
}








// === Vérifie que le CIN est valide : 8 chiffres, non vide, unique ===
bool pageemploye::validerCin(const QString &cin)
{
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "CIN", "Le CIN ne doit pas être vide.");
        return false;
    }
    if (cin.length() != 8) {
        QMessageBox::warning(this, "CIN", "Le CIN doit contenir exactement 8 chiffres.");
        return false;
    }
    if (!cin.contains(QRegularExpression("^[0-9]{8}$"))) {
        QMessageBox::warning(this, "CIN", "Le CIN doit contenir uniquement des chiffres.");
        return false;
    }
    
    // Use originalCIN for modification context
    QString cinToExclude = originalCIN.isEmpty() ? "" : originalCIN;
    if (Employes::cinExiste(cin, cinToExclude)) {
        QMessageBox::warning(this, "CIN", "Ce CIN est déjà utilisé.");
        return false;
    }
    return true;
}

// === Vérifie nom/prénom : lettres uniquement, max 15 caractères ===
bool pageemploye::validerNomPrenom(const QString &texte, const QString &champ)
{
    if (texte.isEmpty()) {
        QMessageBox::warning(this, champ, champ + " ne doit pas être vide.");
        return false;
    }
    if (texte.length() > 15) {
        QMessageBox::warning(this, champ, champ + " ne doit pas dépasser 15 caractères.");
        return false;
    }
    if (!texte.contains(QRegularExpression("^[a-zA-ZÀ-ÿ\\s'-]+$"))) {
        QMessageBox::warning(this, champ, champ + " doit contenir uniquement des lettres.");
        return false;
    }
    return true;
}

// === Vérifie salaire : > 0 ===
bool pageemploye::validerSalaire(const QString &salaireStr)
{
    bool ok;
    double salaire = salaireStr.toDouble(&ok);
    if (!ok || salaire <= 0) {
        QMessageBox::warning(this, "Salaire", "Le salaire doit être un nombre positif.");
        return false;
    }
    return true;
}

// === Vérifie téléphone : 8 chiffres uniquement ===
bool pageemploye::validerTelephone(const QString &tel)
{
    if (tel.length() != 8) {
        QMessageBox::warning(this, "Téléphone", "Le téléphone doit contenir exactement 8 chiffres.");
        return false;
    }
    if (!tel.contains(QRegularExpression("^[0-9]{8}$"))) {
        QMessageBox::warning(this, "Téléphone", "Le téléphone doit contenir uniquement des chiffres.");
        return false;
    }
    return true;
}

// === Vérifie email : contient @, unique ===
bool pageemploye::validerEmail(const QString &email)
{
    if (!email.contains("@") || email.startsWith("@") || email.endsWith("@")) {
        QMessageBox::warning(this, "Email", "L'email doit contenir un @ valide.");
        return false;
    }

    // Use originalCIN if modifying, otherwise use current CIN
    QString cinToExclude = originalCIN.isEmpty() ? ui->cin_E->text().trimmed() : originalCIN;
    if (Employes::emailExiste(email, cinToExclude)) {
        QMessageBox::warning(this, "Email", "Cet email est déjà utilisé par un autre employé.");
        return false;
    }
    return true;
}

// === Vérifie mot de passe : min 8 caractères ===
bool pageemploye::validerMdp(const QString &mdp)
{
    if (mdp.length() < 8) {
        QMessageBox::warning(this, "Mot de passe", "Le mot de passe doit contenir au moins 8 caractères.");
        return false;
    }
    return true;
}






// === Fonction pour vider tous les champs du formulaire ===
void pageemploye::clearForm()
{
    qDebug() << "Clearing form...";
    
    if (ui->cin_E) ui->cin_E->clear();
    if (ui->nom_E) ui->nom_E->clear();
    if (ui->prenom_E_2) ui->prenom_E_2->clear();
    if (ui->salaire_E) ui->salaire_E->clear();
    if (ui->tel_E) ui->tel_E->clear();
    if (ui->email_E) ui->email_E->clear();
    if (ui->mdp_E) ui->mdp_E->clear();
    if (ui->dispo_E) ui->dispo_E->setCurrentIndex(0);
    if (ui->poste_E) ui->poste_E->setCurrentIndex(0);

    updateButtonStates();
    if (ui->tab_em) ui->tab_em->blockSignals(false);
    
    qDebug() << "Form cleared successfully";
}
// pageemploye.cpp
void pageemploye::on_btn_reset_E_clicked()
{
    ui->cin_E->setEnabled(true);
    originalCIN.clear();  // Clear stored CIN
    clearForm();
}

void pageemploye::on_btn_ajout_E_clicked()
{
    qDebug() << "=== Add button clicked ===";
    
    QString cin = ui->cin_E->text().trimmed();
    QString nom = ui->nom_E->text().trimmed();
    QString prenom = ui->prenom_E_2->text().trimmed();
    QString salaireStr = ui->salaire_E->text().trimmed();
    QString tel = ui->tel_E->text().trimmed();
    QString email = ui->email_E->text().trimmed();
    QString mdp = ui->mdp_E->text();
    QString dispoText = ui->dispo_E->currentText();
    QString poste = ui->poste_E->currentText();
    
    qDebug() << "CIN:" << cin << "Nom:" << nom << "Email:" << email;
    
    // Early validation - check if fields are empty
    if (cin.isEmpty() || nom.isEmpty() || email.isEmpty()) {
        qDebug() << "Empty fields detected - aborting add";
        QMessageBox::warning(this, "Validation", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    // === TOUTES LES VALIDATIONS ===
    if (!validerCin(cin)) return;
    if (!validerNomPrenom(nom, "Nom")) return;
    if (!validerNomPrenom(prenom, "Prénom")) return;
    if (!validerSalaire(salaireStr)) return;
    if (!validerTelephone(tel)) return;
    if (!validerEmail(email)) return;
    if (!validerMdp(mdp)) return;
    if (poste.isEmpty()) {
        QMessageBox::warning(this, "Poste", "Veuillez sélectionner un poste.");
        return;
    }

    qDebug() << "All validations passed";
    
    // Safety checks to prevent crash
    if (dispoText.isEmpty()) {
        QMessageBox::warning(this, "Disponibilité", "Veuillez sélectionner une disponibilité.");
        return;
    }

    // === Création et ajout ===
    double salaire = salaireStr.toDouble();
    bool dispo = (dispoText == "Disponible");

    Employes emp;
    emp.setCinEmployee(cin);
    emp.setNom(nom);
    emp.setPrenom(prenom);
    emp.setDisponibilite(dispo);
    emp.setSalaire(salaire);
    emp.setTelephone(tel);
    emp.setEmail(email);
    emp.setPoste(poste);
    emp.setMdp(mdp);

    qDebug() << "Calling emp.ajouter()...";
    
    if (emp.ajouter()) {
        qDebug() << "Employee added successfully";
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès !");
        clearForm();                    // Vide les champs
        
        qDebug() << "Refreshing table...";
        try {
            afficherEmployes();             // Rafraîchit le tableau
            qDebug() << "Table refreshed successfully";
        } catch (...) {
            qDebug() << "ERROR: Exception during afficherEmployes()";
            QMessageBox::critical(this, "Erreur", "Erreur lors du rafraîchissement du tableau.");
        }
    } else {
        qDebug() << "Failed to add employee";
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout en base.");
    }
}




void pageemploye::on_tab_em_cellClicked(int row, int column)
{
    Q_UNUSED(column);
    ui->tab_em->selectRow(row);

    // Récupérer les données de la ligne
    QString cin       = ui->tab_em->item(row, 0)->text();
    QString nom       = ui->tab_em->item(row, 1)->text();
    QString prenom    = ui->tab_em->item(row, 2)->text();
    QString dispoText = ui->tab_em->item(row, 3)->text();
    QString salaire   = ui->tab_em->item(row, 4)->text();
    QString tel       = ui->tab_em->item(row, 5)->text();
    QString email     = ui->tab_em->item(row, 6)->text();
    QString poste     = ui->tab_em->item(row, 7)->text();

    // Remplir les champs
    ui->cin_E->setText(cin);
    ui->cin_E->setEnabled(true);  // CIN modifiable
    originalCIN = cin;  // Save original CIN for UPDATE WHERE clause

    ui->nom_E->setText(nom);
    ui->prenom_E_2->setText(prenom);
    ui->salaire_E->setText(salaire);
    ui->tel_E->setText(tel);
    ui->email_E->setText(email);
    ui->mdp_E->clear();  // Sécurité : ne pas afficher le MDP

    // ComboBox Disponibilité
    int dispoIndex = (dispoText == "Disponible") ? 0 : 1;
    ui->dispo_E->setCurrentIndex(dispoIndex);

    // ComboBox Poste
    int posteIndex = ui->poste_E->findText(poste);
    if (posteIndex != -1) {
        ui->poste_E->setCurrentIndex(posteIndex);
    }

    updateButtonStates();  // Active les boutons Modif/Suppr
}

void pageemploye::on_modif_E_clicked()
{
    qDebug() << "=== Modify button clicked ===";
    
    QString cin = ui->cin_E->text().trimmed();

    // Vérifier qu'un employé est sélectionné
    if (originalCIN.isEmpty()) {
        QMessageBox::warning(this, "Sélection", "Veuillez sélectionner un employé à modifier depuis le tableau.");
        return;
    }
    
    qDebug() << "Original CIN:" << originalCIN << "New CIN:" << cin;
    
    // Check if CIN is being changed and if equipment exists
    if (!originalCIN.isEmpty() && originalCIN != cin) {
        QSqlQuery checkEquipment;
        checkEquipment.prepare("SELECT COUNT(*) FROM EQUIPEMENT WHERE CIN_EMPLOYEE = :cin");
        checkEquipment.bindValue(":cin", originalCIN);
        
        if (checkEquipment.exec() && checkEquipment.next()) {
            int equipmentCount = checkEquipment.value(0).toInt();
            if (equipmentCount > 0) {
                QMessageBox::warning(this, "Modification impossible",
                    QString("Impossible de modifier le CIN car cet employé a %1 équipement(s) associé(s).\n\n"
                            "Pour modifier le CIN, vous devez d'abord :\n"
                            "1. Supprimer ou réassigner les équipements dans la table EQUIPEMENT\n"
                            "2. Ou garder le CIN actuel (%2)").arg(equipmentCount).arg(originalCIN));
                return;
            }
        }
    }

    // Récupérer les valeurs actuelles
    QString nom        = ui->nom_E->text().trimmed();
    QString prenom     = ui->prenom_E_2->text().trimmed();
    QString salaireStr = ui->salaire_E->text().trimmed();
    QString tel        = ui->tel_E->text().trimmed();
    QString email      = ui->email_E->text().trimmed();
    QString mdp        = ui->mdp_E->text();
    QString dispoText  = ui->dispo_E->currentText();
    QString poste      = ui->poste_E->currentText();

    // === VALIDATIONS ===
    if (!validerCin(cin)) return;  // Validate CIN (uses originalCIN internally)
    if (!validerNomPrenom(nom, "Nom")) return;
    if (!validerNomPrenom(prenom, "Prénom")) return;
    if (!validerSalaire(salaireStr)) return;
    if (!validerTelephone(tel)) return;

    // Vérification Email : use originalCIN for query
    QString ancienEmail;
    QSqlQuery queryEmail;
    queryEmail.prepare("SELECT EMAIL FROM EMPLOYEE WHERE CIN_EMPLOYEE = :cin");
    queryEmail.bindValue(":cin", originalCIN);  // Use original CIN
    if (queryEmail.exec() && queryEmail.next()) {
        ancienEmail = queryEmail.value(0).toString();
    }
    if (email != ancienEmail) {
        if (!validerEmail(email)) return;
    }

    // Mot de passe : si vide, garder l'ancien
    if (!mdp.isEmpty()) {
        if (!validerMdp(mdp)) return;
    } else {
        QSqlQuery queryMdp;
        queryMdp.prepare("SELECT MDP FROM EMPLOYEE WHERE CIN_EMPLOYEE = :cin");
        queryMdp.bindValue(":cin", originalCIN);  // Use original CIN
        if (queryMdp.exec() && queryMdp.next()) {
            mdp = queryMdp.value(0).toString();
        }
    }

    // === Objet Employes ===
    double salaire = salaireStr.toDouble();
    bool dispo = (dispoText == "Disponible");

    // Check if CIN was modified
    QString cinToUpdate = originalCIN.isEmpty() ? cin : originalCIN;
    
    Employes emp;
    emp.setCinEmployee(cin);
    emp.setNom(nom);
    emp.setPrenom(prenom);
    emp.setDisponibilite(dispo);
    emp.setSalaire(salaire);
    emp.setTelephone(tel);
    emp.setEmail(email);
    emp.setPoste(poste);
    emp.setMdp(mdp);

    // === Modification en base ===
    bool success = false;
    if (!originalCIN.isEmpty() && originalCIN != cin) {
        // CIN was changed - use overloaded modifier with original CIN
        qDebug() << "CIN changed, using modifier(originalCIN)";
        success = emp.modifier(originalCIN);
    } else {
        // CIN not changed or new employee - use regular modifier
        qDebug() << "CIN not changed, using modifier()";
        success = emp.modifier();
    }
    
    if (success) {
        QMessageBox::information(this, "Succès", "Employé modifié avec succès !");
        originalCIN.clear();  // Clear the stored original CIN
        clearForm();
        
        qDebug() << "Refreshing table after modification...";
        try {
            afficherEmployes();
            qDebug() << "Table refreshed successfully after modification";
        } catch (...) {
            qDebug() << "ERROR: Exception during afficherEmployes() after modification";
        }
        return;  // ✅ Empêche l'exécution du reste (et donc l'affichage du warning)
    } else {
        QMessageBox::critical(this, "Erreur", 
            "Échec de la modification.\n\n"
            "Si vous essayez de modifier le CIN, assurez-vous qu'aucun équipement "
            "n'est associé à cet employé dans la table EQUIPEMENT.\n\n"
            "Supprimez d'abord les équipements associés ou gardez le même CIN.");
    }
}

void pageemploye::on_sup_E_clicked()
{
    QString cin = ui->cin_E->text().trimmed();

    // Vérifier qu'un employé est sélectionné
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Sélection", "Veuillez sélectionner un employé à supprimer.");
        return;
    }

    // Demander confirmation
    int reponse = QMessageBox::question(
        this,
        "Confirmer la suppression",
        QString("Voulez-vous vraiment supprimer l'employé avec CIN : <b>%1</b> ?").arg(cin),
        QMessageBox::Yes | QMessageBox::No
        );

    if (reponse != QMessageBox::Yes) {
        return;  // Annulation
    }

    // === Suppression en base ===
    if (Employes::supprimer(cin)) {
        QMessageBox::information(this, "Succès", "Employé supprimé avec succès !");
        clearForm();         // Vider les champs
        afficherEmployes();  // Rafraîchir le tableau
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression.");
    }
}






void pageemploye::updateButtonStates()
{
    bool hasSelection = !ui->cin_E->text().trimmed().isEmpty();
    ui->modif_E->setEnabled(hasSelection);
    ui->sup_E->setEnabled(hasSelection);
}


void pageemploye::on_btn_c_clicked()
{
    // Plus besoin du combo ou de dernierTypeRecherche
    QString texte = ui->recherche_c->text().trimmed();
    lancerRecherche(texte);
}

void pageemploye::on_recherche_c_textChanged(const QString &text)
{
    // Recherche automatique dès qu'on tape
    lancerRecherche(text.trimmed());
}
void pageemploye::lancerRecherche(const QString &recherche)
{
    if (recherche.isEmpty()) {
        afficherEmployes();
        return;
    }

    QSqlQueryModel *model = Employes::rechercherMulti(recherche);

    if (!model) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la recherche.");
        return;
    }

    // Effacer le tableau actuel
    ui->tab_em->setRowCount(0);
    ui->tab_em->setColumnCount(8);
    QStringList headers = {"CIN", "Nom", "Prénom", "Disponibilité", "Salaire", "Téléphone", "Email", "Poste"};
    ui->tab_em->setHorizontalHeaderLabels(headers);

    // Remplir le tableau avec les résultats
    for (int row = 0; row < model->rowCount(); ++row) {
        ui->tab_em->insertRow(row);
        for (int col = 0; col < 8; ++col) {
            QString text = model->data(model->index(row, col)).toString();
            QTableWidgetItem *item = new QTableWidgetItem(text);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->tab_em->setItem(row, col, item);
        }
    }

    ui->tab_em->resizeColumnsToContents();
    qDebug() << "Recherche multi effectuée :" << model->rowCount() << "résultats";

    delete model;
}

void pageemploye::on_trier_c_clicked()
{
    // Créer une boîte de dialogue avec ComboBox
    QDialog dialog(this);
    dialog.setWindowTitle("Choisir le type de tri");
    dialog.setFixedSize(300, 150);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QComboBox *comboTri = new QComboBox(&dialog);
    comboTri->addItem("Tri par Nom (A-Z)");
    comboTri->addItem("Tri par Prénom (A-Z)");
    comboTri->addItem("Tri par Salaire");

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);

    layout->addWidget(new QLabel("Sélectionnez le type de tri :"));
    layout->addWidget(comboTri);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        qDebug() << "Tri annulé par l'utilisateur";
        return;
    }

    QSqlQueryModel *model = nullptr;
    QString triType;

    // Créer une instance d'Employes
    Employes employes;

    switch (comboTri->currentIndex()) {
    case 0:
        model = employes.trierParNomCroissant();
        triType = "nom (ordre alphabétique croissant)";
        break;
    case 1:
        model = employes.trierParPrenomAlphabetique();
        triType = "prénom (ordre alphabétique)";
        break;
    case 2:
        model = employes.trierParSalaire();
        triType = "salaire (ordre croissant)";
        break;
    default:
        QMessageBox::warning(this, "Erreur", "Type de tri non valide.");
        return;
    }

    if (!model) {
        QMessageBox::critical(this, "Erreur", "Erreur lors du tri de la base de données.");
        return;
    }

    // Affichage des résultats triés dans le tableau
    ui->tab_em->setRowCount(0);
    ui->tab_em->setColumnCount(8);
    QStringList headers = {"CIN", "Nom", "Prénom", "Disponibilité", "Salaire", "Téléphone", "Email", "Poste"};
    ui->tab_em->setHorizontalHeaderLabels(headers);

    for (int row = 0; row < model->rowCount(); ++row) {
        ui->tab_em->insertRow(row);
        for (int col = 0; col < 8; ++col) {
            QString text = model->data(model->index(row, col)).toString();
            QTableWidgetItem *item = new QTableWidgetItem(text);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->tab_em->setItem(row, col, item);
        }
    }

    ui->tab_em->resizeColumnsToContents();
    qDebug() << "Tri effectué avec succès :" << model->rowCount() << "employés triés par" << triType;

    QMessageBox::information(this, "Tri réussi", QString("Liste triée par %1.\n%2 employés affichés.").arg(triType).arg(model->rowCount()));
    delete model;
}
void pageemploye::on_pdf_c_clicked()
{
    qDebug() << "Export PDF demandé...";

    // Vérifier s'il y a des données à exporter
    if (ui->tab_em->rowCount() == 0) {
        QMessageBox::warning(this, "Export PDF", "Aucune donnée à exporter.");
        return;
    }

    // Demander où sauvegarder le fichier
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Exporter en PDF",
        QDir::homePath() + "/liste_employes.pdf",
        "Fichiers PDF (*.pdf)"
        );

    if (fileName.isEmpty()) {
        return; // Annulation
    }

    // Créer le printer
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Landscape);

    // Créer le painter
    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le fichier PDF.");
        return;
    }

    // Configuration du painter
    painter.setRenderHint(QPainter::Antialiasing);
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    // Dimensions utiles
    const int pageWidth = printer.width();
    const int pageHeight = printer.height();
    const int margin = 40;
    const int usableWidth = pageWidth - 2 * margin;

    // ========== CONFIGURATION DES DIMENSIONS ==========
    const int rowHeight = 450;              // Hauteur des lignes
    const int headerHeight = 250;           // Hauteur de l'en-tête
    const int columnWidthPx = 750;         // ← LARGEUR DES COLONNES (modifiable)
    const int startY = margin;
    int currentY = startY;
    // ==================================================

    // Nombre de colonnes
    int columnCount = ui->tab_em->columnCount();

    // Calcul des largeurs de colonnes - LARGEUR PERSONNALISÉE
    QVector<int> columnWidths(columnCount);
    int totalWidth = 0;

    // Utiliser la largeur personnalisée pour chaque colonne
    for (int col = 0; col < columnCount; ++col) {
        columnWidths[col] = columnWidthPx;
        totalWidth += columnWidthPx;
    }

    // Ajuster si le tableau est trop large
    if (totalWidth > usableWidth) {
        double ratio = (double)usableWidth / totalWidth;
        totalWidth = 0;
        for (int col = 0; col < columnCount; ++col) {
            columnWidths[col] = columnWidths[col] * ratio;
            totalWidth += columnWidths[col];
        }
        qDebug() << "Largeur ajustée - ratio:" << ratio;
    }

    qDebug() << "Largeur utilisable:" << usableWidth;
    qDebug() << "Largeur totale tableau:" << totalWidth;
    qDebug() << "Nombre de colonnes:" << columnCount;
    qDebug() << "Hauteur des lignes:" << rowHeight << "pixels";
    qDebug() << "Largeur des colonnes:" << columnWidthPx << "pixels";

    // Dessiner l'en-tête du tableau
    painter.save();
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(QBrush(QColor(200, 200, 200)));

    // Rectangle d'en-tête
    painter.drawRect(margin, currentY, totalWidth, headerHeight);

    // Texte d'en-tête
    painter.setPen(Qt::black);
    QFont tableHeaderFont = painter.font();
    tableHeaderFont.setPointSize(10);
    tableHeaderFont.setBold(true);
    painter.setFont(tableHeaderFont);

    int xPos = margin;
    for (int col = 0; col < columnCount; ++col) {
        QString header = ui->tab_em->horizontalHeaderItem(col)->text();
        painter.drawText(xPos + 5, currentY + 5, columnWidths[col] - 10, headerHeight - 10,
                         Qt::AlignCenter, header);

        // Bordures entre colonnes
        if (col < columnCount - 1) {
            painter.drawLine(xPos + columnWidths[col], currentY,
                             xPos + columnWidths[col], currentY + headerHeight);
        }

        xPos += columnWidths[col];
    }

    painter.restore();
    currentY += headerHeight;

    // Dessiner les données du tableau
    QFont dataFont = painter.font();
    dataFont.setPointSize(8);
    dataFont.setBold(false);
    painter.setFont(dataFont);

    for (int row = 0; row < ui->tab_em->rowCount(); ++row) {
        // Vérifier si on dépasse la page
        if (currentY + rowHeight > pageHeight - margin) {
            printer.newPage();
            currentY = margin;

            // Redessiner l'en-tête sur la nouvelle page
            painter.save();
            painter.setPen(QPen(Qt::black, 2));
            painter.setBrush(QBrush(QColor(200, 200, 200)));
            painter.drawRect(margin, currentY, totalWidth, headerHeight);

            painter.setPen(Qt::black);
            painter.setFont(tableHeaderFont);

            xPos = margin;
            for (int col = 0; col < columnCount; ++col) {
                QString header = ui->tab_em->horizontalHeaderItem(col)->text();
                painter.drawText(xPos + 5, currentY + 5, columnWidths[col] - 10, headerHeight - 10,
                                 Qt::AlignCenter, header);

                if (col < columnCount - 1) {
                    painter.drawLine(xPos + columnWidths[col], currentY,
                                     xPos + columnWidths[col], currentY + headerHeight);
                }

                xPos += columnWidths[col];
            }
            painter.restore();
            currentY += headerHeight;
        }

        // Fond de la ligne
        painter.save();
        if (row % 2 == 0) {
            painter.setBrush(QBrush(QColor(245, 245, 245)));
        } else {
            painter.setBrush(QBrush(Qt::white));
        }

        painter.setPen(QPen(Qt::black, 1));
        painter.drawRect(margin, currentY, totalWidth, rowHeight);

        // Contenu des cellules
        painter.setPen(Qt::black);
        xPos = margin;

        for (int col = 0; col < columnCount; ++col) {
            QTableWidgetItem *item = ui->tab_em->item(row, col);
            QString text = item ? item->text() : "";

            // Bordures entre colonnes
            if (col < columnCount - 1) {
                painter.drawLine(xPos + columnWidths[col], currentY,
                                 xPos + columnWidths[col], currentY + rowHeight);
            }

            // Texte centré
            painter.drawText(xPos + 3, currentY + 3, columnWidths[col] - 6, rowHeight - 6,
                             Qt::AlignCenter | Qt::TextWordWrap, text);

            xPos += columnWidths[col];
        }

        painter.restore();
        currentY += rowHeight;
    }

    // Bordure finale du tableau
    painter.setPen(QPen(Qt::black, 2));
    painter.drawRect(margin, startY, totalWidth, currentY - startY);

    painter.end();

    // Ouvrir le PDF généré
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));

    QMessageBox::information(this, "Export PDF",
                             QString("Export PDF réussi !\nFichier : %1\n\n"
                                     "Employés exportés : %2\n"
                                     "Largeur colonnes : %3 pixels")
                                 .arg(fileName)
                                 .arg(ui->tab_em->rowCount())
                                 .arg(columnWidthPx));

    qDebug() << "Export PDF terminé :" << fileName;
}


// === Statistiques de disponibilité ===
void pageemploye::afficherStatistiquesDisponibilite()
{
    qDebug() << "Génération des statistiques de disponibilité...";

    // Récupérer les statistiques depuis Employes
    QMap<QString, QVariant> stats = Employes::getStatistiquesDisponibilite();

    int disponibles = stats["disponibles"].toInt();
    int nonDisponibles = stats["nonDisponibles"].toInt();
    int totalEmployes = stats["totalEmployes"].toInt();
    double pourcentageDisponibles = stats["pourcentageDisponibles"].toDouble();
    double pourcentageNonDisponibles = stats["pourcentageNonDisponibles"].toDouble();

    qDebug() << "Statistiques - Disponibles:" << disponibles << "Non disponibles:" << nonDisponibles;

    // Vérifier s'il y a des données
    if (totalEmployes == 0) {
        QMessageBox::information(this, "Statistiques", "Aucun employé trouvé dans la base de données.");
        return;
    }

    // Créer la série de données pour le camembert
    QPieSeries *series = new QPieSeries();

    // Ajouter les données
    QPieSlice *sliceDisponibles = series->append("Disponibles", disponibles);
    QPieSlice *sliceNonDisponibles = series->append("Non disponibles", nonDisponibles);

    // Couleurs
    sliceDisponibles->setColor(QColor(76, 175, 80));    // Vert
    sliceNonDisponibles->setColor(QColor(244, 67, 54)); // Rouge

    // Effet de surbrillance
    sliceDisponibles->setExploded(true);
    sliceDisponibles->setLabelVisible(true);
    sliceNonDisponibles->setLabelVisible(true);

    // Format des labels avec pourcentages
    sliceDisponibles->setLabel(QString("Disponibles\n%1 (%2%)")
                                   .arg(disponibles)
                                   .arg(QString::number(pourcentageDisponibles, 'f', 1)));

    sliceNonDisponibles->setLabel(QString("Non disponibles\n%1 (%2%)")
                                      .arg(nonDisponibles)
                                      .arg(QString::number(pourcentageNonDisponibles, 'f', 1)));

    // Créer le chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques de Disponibilité des Employés");
    chart->setTitleFont(QFont("Arial", 12, QFont::Bold));
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Créer la vue du chart
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400, 300);

    // Nettoyer le groupBox avant d'ajouter le nouveau chart
    QLayout *layout = ui->groupBox_sattc->layout();
    if (layout) {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    } else {
        // Créer un layout si inexistant
        layout = new QVBoxLayout(ui->groupBox_sattc);
    }

    // Ajouter le chart au groupBox
    layout->addWidget(chartView);
    ui->groupBox_sattc->setLayout(layout);

    // Mettre à jour les labels d'information
    ui->groupBox_sattc->setTitle(QString("Statistiques - Total: %1 employés").arg(totalEmployes));

    qDebug() << "Statistiques affichées avec succès";
}

// === Slot pour le bouton statistiques ===
void pageemploye::on_stat_c_clicked()
{
    qDebug() << "Bouton statistiques cliqué";
    afficherStatistiquesDisponibilite();
}

void pageemploye::on_chatbot_clicked()
{
    QDialog *chatbotDialog = new QDialog(this);
    chatbotDialog->setWindowTitle("🤖 Assistant Chatbot - Gestion des Employés");
    chatbotDialog->setMinimumSize(500, 400);
    chatbotDialog->setStyleSheet("QDialog { background-color: #f5f5f5; }");
    QVBoxLayout *mainLayout = new QVBoxLayout(chatbotDialog);
    QLabel *title = new QLabel("🤖 Assistant Virtuel");
    title->setStyleSheet("font-size: 18px; font-weight: bold; color: #2c3e50; margin: 10px;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);
    QTextEdit *responseLabel = new QTextEdit();
    responseLabel->setReadOnly(true);
    responseLabel->setPlaceholderText("La réponse de l'assistant apparaîtra ici...");
    responseLabel->setStyleSheet("QTextEdit { "
                                 "background-color: white; "
                                 "border: 2px solid #3498db; "
                                 "border-radius: 10px; "
                                 "padding: 10px; "
                                 "font-size: 12px; "
                                 "min-height: 200px;"
                                 "}");
    responseLabel->setHtml("<p style='color: #7f8c8d; text-align: center; margin-top: 80px;'>"
                           "Bonjour ! Je suis votre assistant pour la gestion des employés. "
                           "Posez-moi vos questions ci-dessous.</p>");
    mainLayout->addWidget(responseLabel);
    QLineEdit *questionInput = new QLineEdit();
    questionInput->setPlaceholderText("Posez votre question ici... (ex: Comment ajouter un employé ?)");
    questionInput->setStyleSheet("QLineEdit { "
                                 "padding: 10px; "
                                 "border: 2px solid #bdc3c7; "
                                 "border-radius: 5px; "
                                 "font-size: 12px; "
                                 "margin: 5px;"
                                 "}"
                                 "QLineEdit:focus { "
                                 "border-color: #3498db;"
                                 "}");
    mainLayout->addWidget(questionInput);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *confirmButton = new QPushButton("💬 Envoyer");
    confirmButton->setStyleSheet("QPushButton { "
                                 "background-color: #3498db; "
                                 "color: white; "
                                 "border: none; "
                                 "padding: 10px 20px; "
                                 "border-radius: 5px; "
                                 "font-weight: bold;"
                                 "}"
                                 "QPushButton:hover { "
                                 "background-color: #2980b9;"
                                 "}"
                                 "QPushButton:disabled { "
                                 "background-color: #bdc3c7;"
                                 "}");

    QPushButton *clearButton = new QPushButton("🗑️ Effacer");
    clearButton->setStyleSheet("QPushButton { "
                               "background-color: #e74c3c; "
                               "color: white; "
                               "border: none; "
                               "padding: 10px 20px; "
                               "border-radius: 5px; "
                               "font-weight: bold;"
                               "}"
                               "QPushButton:hover { "
                               "background-color: #c0392b;"
                               "}");

    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(confirmButton);
    mainLayout->addLayout(buttonLayout);
    QLabel *loadingLabel = new QLabel("⏳ Envoi en cours...");
    loadingLabel->setStyleSheet("color: #f39c12; font-style: italic;");
    loadingLabel->setAlignment(Qt::AlignCenter);
    loadingLabel->hide();
    mainLayout->addWidget(loadingLabel);
    connect(confirmButton, &QPushButton::clicked, chatbotDialog, [=]() {
        QString question = questionInput->text().trimmed();
        if (question.isEmpty()) {
            QMessageBox::warning(chatbotDialog, "Question vide", "Veuillez saisir une question.");
            return;
        }
        confirmButton->setEnabled(false);
        loadingLabel->show();
        responseLabel->setHtml("<p style='color: #f39c12; text-align: center;'>⏳ Envoi de votre question...</p>");
        QString currentText = responseLabel->toHtml();
        QString userHtml = "<p style='color: #2c3e50; background-color: #ecf0f1; padding: 8px; border-radius: 10px; margin: 5px;'><strong>Vous:</strong> " + question + "</p>";
        responseLabel->setHtml(currentText + userHtml);
        sendToGeminiAPI(question);
        chatbotDialog->setProperty("responseLabel", QVariant::fromValue(responseLabel));
        chatbotDialog->setProperty("confirmButton", QVariant::fromValue(confirmButton));
        chatbotDialog->setProperty("loadingLabel", QVariant::fromValue(loadingLabel));
        chatbotDialog->setProperty("questionInput", QVariant::fromValue(questionInput));
        questionInput->clear();
    });
    connect(clearButton, &QPushButton::clicked, chatbotDialog, [=]() {
        responseLabel->setHtml("<p style='color: #7f8c8d; text-align: center; margin-top: 80px;'>"
                               "Bonjour ! Je suis votre assistant pour la gestion des employés. "
                               "Posez-moi vos questions ci-dessous.</p>");
        questionInput->clear();
    });
    connect(questionInput, &QLineEdit::returnPressed, confirmButton, &QPushButton::click);
    chatbotDialog->exec();
}
void pageemploye::sendToGeminiAPI(const QString &question)
{

    QString apiKey = "AIzaSyAmMXQoj-OMoFVGrHS_FW7QIAZgSOCjI98"; // Remember to use your actual key

    // Corrected URL with a current model and v1beta API
    QString url = QString("https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=%1").arg(apiKey);

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // The rest of your JSON construction code remains the same...
    QJsonObject contentObject;
    QJsonArray partsArray;
    QJsonObject partObject;
    partObject["text"] = question + " (Réponds en français s'il te plaît)";
    partsArray.append(partObject);

    QJsonObject contentItem;
    contentItem["parts"] = partsArray;

    QJsonArray contentsArray;
    contentsArray.append(contentItem);

    QJsonObject requestBody;
    requestBody["contents"] = contentsArray;

    QJsonDocument doc(requestBody);
    QByteArray data = doc.toJson();

    // Send the request (your existing code is fine here)
    QNetworkReply *reply = networkManager->post(request, data);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        onChatbotResponseReceived(reply);
    });
}

void pageemploye::onChatbotResponseReceived(QNetworkReply *reply)
{
    QDialog *chatbotDialog = findChild<QDialog*>();
    if (!chatbotDialog) {
        reply->deleteLater();
        return;
    }

    QTextEdit *responseLabel = chatbotDialog->property("responseLabel").value<QTextEdit*>();
    QPushButton *confirmButton = chatbotDialog->property("confirmButton").value<QPushButton*>();
    QLabel *loadingLabel = chatbotDialog->property("loadingLabel").value<QLabel*>();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response_data = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);
        QJsonObject jsonObject = jsonResponse.object();

        QString responseText = "Désolé, je n'ai pas pu traiter votre demande.";

        if (jsonObject.contains("candidates") && jsonObject["candidates"].isArray()) {
            QJsonArray candidates = jsonObject["candidates"].toArray();
            if (!candidates.isEmpty()) {
                QJsonObject candidate = candidates[0].toObject();
                if (candidate.contains("content") && candidate["content"].isObject()) {
                    QJsonObject content = candidate["content"].toObject();
                    if (content.contains("parts") && content["parts"].isArray()) {
                        QJsonArray parts = content["parts"].toArray();
                        if (!parts.isEmpty()) {
                            QJsonObject part = parts[0].toObject();
                            if (part.contains("text")) {
                                responseText = part["text"].toString();
                            }
                        }
                    }
                }
            }
        }

        QString formattedResponse = QString("<p style='color: #27ae60; background-color: #d5f4e6; padding: 8px; border-radius: 10px; margin: 5px;'>"
                                            "<strong>🤖 Assistant:</strong> %1</p>").arg(responseText);

        QString currentText = responseLabel->toHtml();
        responseLabel->setHtml(currentText + formattedResponse);

    } else {
        QString errorResponse = QString("<p style='color: #e74c3c; background-color: #fadbd8; padding: 8px; border-radius: 10px; margin: 5px;'>"
                                        "<strong>❌ Erreur:</strong> Impossible de contacter l'assistant. Vérifiez votre connexion internet et votre clé API.</p>");

        QString currentText = responseLabel->toHtml();
        responseLabel->setHtml(currentText + errorResponse);

        qDebug() << "API Error:" << reply->errorString();
    }

    if (loadingLabel) loadingLabel->hide();
    if (confirmButton) confirmButton->setEnabled(true);
    QTextCursor cursor = responseLabel->textCursor();
    cursor.movePosition(QTextCursor::End);
    responseLabel->setTextCursor(cursor);

    reply->deleteLater();
}

void pageemploye::on_candidat_7_clicked()
{

}


void pageemploye::on_planning_7_clicked()
{

}

void pageemploye::on_vehicule_7_clicked()
{

}
#include "navigation_constants.h"
void pageemploye::navigateToCandidat()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_CANDIDAT);
    }
}

void pageemploye::navigateToPlanning()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent()->parent());
    if (mainWindow) {
        mainWindow->findChild<QStackedWidget*>()->setCurrentIndex(Navigation::PAGE_PLANNING);
    }
}

void pageemploye::navigateToEmployee()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_EMPLOYES);
    }
}

void pageemploye::navigateToVehicule()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_VEHICULE);
    }
}

void pageemploye::navigateToExamen()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_EXAMEN);
    }
}

void pageemploye::navigateToEquipement()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent()->parent());
    if (mainWindow)
    {
        mainWindow->findChild<QStackedWidget *>()->setCurrentIndex(Navigation::PAGE_EQUIPEMENT);
    }
}
