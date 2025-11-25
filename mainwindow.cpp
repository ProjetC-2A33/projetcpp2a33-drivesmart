#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "condidat.h"
#include "employee.h"
#include "vehicule.h"
#include "examen.h"
#include "planning.h"
#include "gestion_equipement.h"
#include "navigation_constants.h"
#include "connection.h"
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QLineEdit>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow),
                                          condidatView(nullptr),
                                          vehiculeView(nullptr),
                                          examenView(nullptr),
                                          planningView(nullptr),
                                          equipementView(nullptr),
                                          employeeView(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("DriveSmart - Main Application");

    // Créer le QStackedWidget
    stackedWidget = new QStackedWidget(this);

    // Créer tous les widgets
    condidatView = new Condidat();
    planningView = new Planning();
    employeeView = new Employee();
    vehiculeView = new Vehicule();
    examenView = new Examen();
    equipementView = new Gestion_Equipement();

    // Ajouter les widgets dans l'ordre EXACT correspondant aux constantes Navigation
    // IMPORTANT: L'ordre d'ajout détermine les indices !
    // Index 0: Candidat
    stackedWidget->addWidget(condidatView);
    // Index 1: Planning
    stackedWidget->addWidget(planningView);
    // Index 2: Employee
    stackedWidget->addWidget(employeeView);
    // Index 3: Véhicule
    stackedWidget->addWidget(vehiculeView);
    // Index 4: Examen
    stackedWidget->addWidget(examenView);
    // Index 5: Équipement
    stackedWidget->addWidget(equipementView);

    // Maintenant définir le stackedWidget comme central widget
    // Cela retire automatiquement employeeView de son parent précédent
    setCentralWidget(stackedWidget);

    // Vérification de l'ordre
    qDebug() << "=== Ordre des pages dans QStackedWidget ===";
    qDebug() << "Index 0 (attendu: Candidat):" << (stackedWidget->widget(0) == condidatView ? "✓ Candidat" : QString("✗ ERREUR - widget: %1").arg(reinterpret_cast<quintptr>(stackedWidget->widget(0)), 0, 16));
    qDebug() << "Index 1 (attendu: Planning):" << (stackedWidget->widget(1) == planningView ? "✓ Planning" : QString("✗ ERREUR - widget: %1").arg(reinterpret_cast<quintptr>(stackedWidget->widget(1)), 0, 16));
    qDebug() << "Index 2 (attendu: Employee):" << (stackedWidget->widget(2) == employeeView ? "✓ Employee" : QString("✗ ERREUR - widget: %1").arg(reinterpret_cast<quintptr>(stackedWidget->widget(2)), 0, 16));
    qDebug() << "Index 3 (attendu: Véhicule):" << (stackedWidget->widget(3) == vehiculeView ? "✓ Véhicule" : QString("✗ ERREUR - widget: %1").arg(reinterpret_cast<quintptr>(stackedWidget->widget(3)), 0, 16));
    qDebug() << "Index 4 (attendu: Examen):" << (stackedWidget->widget(4) == examenView ? "✓ Examen" : QString("✗ ERREUR - widget: %1").arg(reinterpret_cast<quintptr>(stackedWidget->widget(4)), 0, 16));
    qDebug() << "Index 5 (attendu: Équipement):" << (stackedWidget->widget(5) == equipementView ? "✓ Équipement" : QString("✗ ERREUR - widget: %1").arg(reinterpret_cast<quintptr>(stackedWidget->widget(5)), 0, 16));
    qDebug() << "Total widgets:" << stackedWidget->count();
    qDebug() << "PAGE_CANDIDAT =" << Navigation::PAGE_CANDIDAT;
    qDebug() << "PAGE_PLANNING =" << Navigation::PAGE_PLANNING;
    qDebug() << "PAGE_EMPLOYEE =" << Navigation::PAGE_EMPLOYEE;
    qDebug() << "PAGE_VEHICULE =" << Navigation::PAGE_VEHICULE;
    qDebug() << "PAGE_EXAMEN =" << Navigation::PAGE_EXAMEN;
    qDebug() << "PAGE_EQUIPEMENT =" << Navigation::PAGE_EQUIPEMENT;

    // Démarrer sur la page Candidat (index 0)
    stackedWidget->setCurrentIndex(Navigation::PAGE_CANDIDAT);

    connect(ui->candidat_7, &QPushButton::clicked, this, &MainWindow::showCandidat);
    connect(ui->planning_7, &QPushButton::clicked, this, &MainWindow::showPlanning);
    connect(ui->employee_7, &QPushButton::clicked, this, &MainWindow::showEmployee);
    connect(ui->vehicule_7, &QPushButton::clicked, this, &MainWindow::showVehicule);
    connect(ui->examen_7, &QPushButton::clicked, this, &MainWindow::showExamen);

    // Connecter le bouton equipement s'il existe dans l'UI
    QPushButton *equipementBtn = findChild<QPushButton *>("equipement_7");
    if (!equipementBtn)
    {
        // Essayer d'autres noms possibles
        equipementBtn = findChild<QPushButton *>("equipment_7");
    }
    if (equipementBtn)
    {
        connect(equipementBtn, &QPushButton::clicked, this, &MainWindow::showEquipement);
    }

    // Connecter les boutons Employee manuellement pour s'assurer qu'ils fonctionnent
    QPushButton *btnAjoutE = employeeView->findChild<QPushButton *>("btn_ajout_E");
    if (btnAjoutE)
    {
        connect(btnAjoutE, &QPushButton::clicked, this, &MainWindow::on_btn_ajout_E_clicked);
    }

    QPushButton *btnResetE = employeeView->findChild<QPushButton *>("btn_reset_E");
    if (btnResetE)
    {
        connect(btnResetE, &QPushButton::clicked, this, &MainWindow::on_btn_reset_E_clicked);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showCandidat()
{
    if (stackedWidget && stackedWidget->count() > Navigation::PAGE_CANDIDAT)
    {
        stackedWidget->setCurrentIndex(Navigation::PAGE_CANDIDAT);
        qDebug() << "showCandidat: Index set to" << Navigation::PAGE_CANDIDAT << ", current index:" << stackedWidget->currentIndex();
    }
    else
    {
        qDebug() << "showCandidat: Error - stackedWidget is null or index out of range";
    }
}

void MainWindow::showVehicule()
{
    if (stackedWidget && stackedWidget->count() > Navigation::PAGE_VEHICULE)
    {
        stackedWidget->setCurrentIndex(Navigation::PAGE_VEHICULE);
    }
}

void MainWindow::showExamen()
{
    if (stackedWidget && stackedWidget->count() > Navigation::PAGE_EXAMEN)
    {
        int targetIndex = Navigation::PAGE_EXAMEN;
        QWidget *widgetAtTarget = stackedWidget->widget(targetIndex);
        qDebug() << "showExamen: Setting index to" << targetIndex;
        qDebug() << "Widget at index" << targetIndex << ":" << reinterpret_cast<void *>(widgetAtTarget);
        qDebug() << "Expected examenView:" << reinterpret_cast<void *>(examenView);
        qDebug() << "Match:" << (widgetAtTarget == examenView ? "YES" : "NO");
        stackedWidget->setCurrentIndex(targetIndex);
        qDebug() << "Current index after set:" << stackedWidget->currentIndex();
    }
}

void MainWindow::showPlanning()
{
    if (stackedWidget && stackedWidget->count() > Navigation::PAGE_PLANNING)
    {
        stackedWidget->setCurrentIndex(Navigation::PAGE_PLANNING);
    }
}

void MainWindow::showEmployee()
{
    if (stackedWidget && stackedWidget->count() > Navigation::PAGE_EMPLOYEE)
    {
        int targetIndex = Navigation::PAGE_EMPLOYEE;
        QWidget *widgetAtTarget = stackedWidget->widget(targetIndex);
        qDebug() << "showEmployee: Setting index to" << targetIndex;
        qDebug() << "Widget at index" << targetIndex << ":" << reinterpret_cast<void *>(widgetAtTarget);
        qDebug() << "Expected employeeView:" << reinterpret_cast<void *>(employeeView);
        qDebug() << "Match:" << (widgetAtTarget == employeeView ? "YES" : "NO");
        stackedWidget->setCurrentIndex(targetIndex);
        qDebug() << "Current index after set:" << stackedWidget->currentIndex();
    }
}

void MainWindow::showEquipement()
{
    if (stackedWidget && stackedWidget->count() > Navigation::PAGE_EQUIPEMENT)
    {
        stackedWidget->setCurrentIndex(Navigation::PAGE_EQUIPEMENT);
    }
}

void MainWindow::on_btn_ajout_E_clicked()
{
    // Récupérer les valeurs des champs
    QString cin = ui->cin_E->text().trimmed();
    QString nom = ui->nom_E->text().trimmed();
    QString prenom = ui->prenom_E_2->text().trimmed();
    QString tel = ui->tel_E->text().trimmed();
    QString email = ui->email_E->text().trimmed();
    QString salaire = ui->salaire_E->text().trimmed();
    QString mdp = ui->mdp_E->text().trimmed();
    QString poste = ui->poste_E->currentText();

    // Récupérer la disponibilité depuis le ComboBox
    QString disponibilite = ui->dispo_E->currentText();

    // Validation des champs obligatoires
    if (cin.isEmpty() || nom.isEmpty() || prenom.isEmpty() || tel.isEmpty() ||
        email.isEmpty() || salaire.isEmpty() || mdp.isEmpty() || disponibilite.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    // Convertir la disponibilité en nombre (si c'est un texte, utiliser 0 par défaut)
    // La disponibilité dans la base est un NUMBER, donc on peut utiliser 0 ou 1 selon le choix
    int disponibiliteValue = 0; // Par défaut
    if (disponibilite.toLower().contains("disponible") || disponibilite.toLower().contains("libre") || disponibilite == "1")
    {
        disponibiliteValue = 1;
    }
    else if (disponibilite.toLower().contains("non") || disponibilite.toLower().contains("occupé") || disponibilite == "0")
    {
        disponibiliteValue = 0;
    }
    else
    {
        // Essayer de convertir directement en nombre
        bool disponibiliteOk;
        disponibiliteValue = disponibilite.toInt(&disponibiliteOk);
        if (!disponibiliteOk)
        {
            disponibiliteValue = 0; // Valeur par défaut
        }
    }

    // Validation du salaire (doit être un nombre)
    bool salaireOk;
    double salaireValue = salaire.toDouble(&salaireOk);
    if (!salaireOk || salaireValue < 0)
    {
        QMessageBox::warning(this, "Erreur", "Le salaire doit être un nombre valide.");
        return;
    }

    // Connexion à la base de données
    Connection c;
    if (!c.createconnect())
    {
        QMessageBox::critical(this, "Erreur", "Impossible de se connecter à la base de données.");
        return;
    }

    QSqlQuery query;

    // Vérifier si le CIN existe déjà (utiliser CIN_EMPLOYEE)
    query.prepare("SELECT COUNT(*) FROM EMPLOYEE WHERE CIN_EMPLOYEE = :cin");
    query.bindValue(":cin", cin);
    if (!query.exec())
    {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la vérification du CIN : " + query.lastError().text());
        return;
    }
    if (query.next() && query.value(0).toInt() > 0)
    {
        QMessageBox::warning(this, "Erreur", "Un employé avec ce CIN existe déjà.");
        return;
    }

    // Insérer l'employé dans la base de données avec tous les champs obligatoires
    query.prepare("INSERT INTO EMPLOYEE (CIN_EMPLOYEE, NOM, PRENOM, DISPONIBILITE, SALAIRE, TELEPHONE, EMAIL, POSTE, MDP) "
                  "VALUES (:cin, :nom, :prenom, :disponibilite, :salaire, :telephone, :email, :poste, :mdp)");
    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":disponibilite", disponibiliteValue);
    query.bindValue(":salaire", salaireValue);
    query.bindValue(":telephone", tel);
    query.bindValue(":email", email);
    query.bindValue(":poste", poste);
    query.bindValue(":mdp", mdp);

    if (query.exec())
    {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès !");
        on_btn_reset_E_clicked(); // Réinitialiser le formulaire
    }
    else
    {
        QString errorMsg = query.lastError().text();
        if (errorMsg.contains("ORA-00001"))
        {
            QMessageBox::warning(this, "Erreur", "Un employé avec ce CIN existe déjà.");
        }
        else if (errorMsg.contains("ORA-00942"))
        {
            QMessageBox::critical(this, "Erreur", "La table EMPLOYEE n'existe pas. Veuillez exécuter le script create_tables_safe.sql.");
        }
        else
        {
            QMessageBox::critical(this, "Erreur", "Échec d'ajout de l'employé : " + errorMsg);
        }
    }
}

void MainWindow::on_btn_reset_E_clicked()
{
    // Réinitialiser tous les champs
    ui->cin_E->clear();
    ui->nom_E->clear();
    ui->prenom_E_2->clear();
    ui->tel_E->clear();
    ui->email_E->clear();
    ui->salaire_E->clear();
    ui->mdp_E->clear();
    ui->poste_E->setCurrentIndex(0);

    // Réinitialiser le champ disponibilité
    ui->dispo_E->setCurrentIndex(0);
}
