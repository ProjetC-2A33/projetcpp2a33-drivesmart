#ifndef PLANNING_H
#define PLANNING_H

#include <QWidget>
<<<<<<< HEAD
=======
#include <QString>
#include <QDate>
#include <QTime>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVariant>
>>>>>>> 968ef59aaf1cad56730d7a2f9673636ba3a6d6e2

namespace Ui {
class Planning;
}

class Planning : public QWidget
{
    Q_OBJECT

public:
    explicit Planning(QWidget *parent = nullptr);
    ~Planning();
<<<<<<< HEAD
=======
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int id);
    bool modifier(int id, QDate date_seance, QString condidat, QString type, QTime debut, QTime fin, QString circuit);
>>>>>>> 968ef59aaf1cad56730d7a2f9673636ba3a6d6e2

private slots:
    void on_btn_ajout_3_clicked();
    void on_btn_reset_3_clicked();
<<<<<<< HEAD
=======
    void on_supp_clicked();
    void on_edit_clicked();
>>>>>>> 968ef59aaf1cad56730d7a2f9673636ba3a6d6e2
    void navigateToCandidat();
    void navigateToPlanning();
    void navigateToEmployee();
    void navigateToVehicule();
    void navigateToExamen();
    void navigateToEquipement();

private:
<<<<<<< HEAD
=======
    void refreshTable();
    int selectedIdFromTable() const;
    void setFormFromRow(int row);
    QWidget* createActionsCell(int row, int id);
    void setEditMode(bool enabled);
    
    int currentEditingId = -1;
    bool isEditMode = false;
    
    // Planning data members
    QDate date_seance;
    QString condidat;
    QString type_seance;
    QTime heure_debut;
    QTime heure_fin;
    QString circuit;

>>>>>>> 968ef59aaf1cad56730d7a2f9673636ba3a6d6e2
    Ui::Planning *ui;
};

#endif // PLANNING_H