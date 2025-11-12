#ifndef PLANNING_H
#define PLANNING_H

#include <QWidget>
#include <QString>
#include <QDate>
#include <QTime>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVariant>
#include <QComboBox>

namespace Ui {
class Planning;
}

class Planning : public QWidget
{
    Q_OBJECT

public:
    explicit Planning(QWidget *parent = nullptr);
    ~Planning();
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int id);
    bool modifier(int id, QDate date_seance, QString condidat, QString type, QTime debut, QTime fin, QString circuit, QString moniteur = "", QString vehicule = "");

private slots:
    void on_btn_ajout_3_clicked();
    void on_btn_reset_3_clicked();
    void on_supp_clicked();
    void on_edit_clicked();
    void navigateToCandidat();
    void navigateToPlanning();
    void navigateToEmployee();
    void navigateToVehicule();
    void navigateToExamen();
    void navigateToEquipement();

private:
    void refreshTable();
    int selectedIdFromTable() const;
    void setFormFromRow(int row);
    QWidget* createActionsCell(int row, int id);
    void setEditMode(bool enabled);
    void loadMoniteurs();
    void loadVehicules();
    void loadCondidats();
    QComboBox* getCondidatCombo(); // Fonction helper pour obtenir le champ condidat
    
    int currentEditingId = -1;
    bool isEditMode = false;
    
    // Planning data members
    QDate date_seance;
    QString condidat;
    QString nom_moniteur;
    QString vehicule;
    QString type_seance;
    QTime heure_debut;
    QTime heure_fin;
    QString circuit;
    
    Ui::Planning *ui;
};

#endif // PLANNING_H