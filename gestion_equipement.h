#ifndef GESTION_EQUIPEMENT_H
#define GESTION_EQUIPEMENT_H

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class Gestion_Equipement;
}

class Gestion_Equipement : public QWidget
{
    Q_OBJECT

public:
    explicit Gestion_Equipement(QWidget *parent = nullptr);
    ~Gestion_Equipement();

private slots:
    // CRUD operations
    void on_btn_ajout_clicked();
    void on_btn_reset_clicked();
    void on_pushButton_3_clicked(); // Delete button
    void on_pushButton_4_clicked(); // Update button
    void on_pushButton_17_clicked(); // Export button
    
    // Table interaction
    void on_tab_2_clicked(const QModelIndex &index);
    
    // Search and filter
    void on_recherche_2_textChanged();
    void on_tri_2_currentIndexChanged(int index);
    
    // Additional features
    void on_btnUpdateMaintenance_clicked();
    void on_btnCheckWeather_clicked();
    
    // Navigation
    void navigateToCandidat();
    void navigateToPlanning();
    void navigateToEmployee();
    void navigateToVehicule();
    void navigateToExamen();
    void navigateToEquipement();

private:
    Ui::Gestion_Equipement *ui;
    QSqlQueryModel *model;
    
    // Helper functions
    void loadTableData();
    void clearForm();
    bool validateInput();
    int generateNextId();
};

#endif // GESTION_EQUIPEMENT_H