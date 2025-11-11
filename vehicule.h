#ifndef VEHICULE_H
#define VEHICULE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTableWidgetItem>

namespace Ui {
class Vehicule;
}

class Vehicule : public QWidget
{
    Q_OBJECT

public:
    explicit Vehicule(QWidget *parent = nullptr);
    ~Vehicule();

private slots:
    void on_AjouterButton_clicked();
    void on_Annuler_Button_clicked();
    void on_supprimerButton_clicked();
    void on_modifierButton_clicked();
    void on_tableWidget_itemClicked(QTableWidgetItem *item);
    void navigateToCandidat();
    void navigateToPlanning();
    void navigateToEmployee();
    void navigateToVehicule();
    void navigateToExamen();
    void navigateToEquipement();

private:
    Ui::Vehicule *ui;
    QSqlDatabase db;
    int selectedRow;
    
    // Fonctions CRUD
    bool ajouterVehicule();
    void afficherVehicules();
    bool modifierVehicule();
    bool supprimerVehicule();
    void viderChamps();
    void remplirChamps(int row);
    QString getDisponibilite(); // Récupère l'état de disponibilité sélectionné
};

#endif // VEHICULE_H
