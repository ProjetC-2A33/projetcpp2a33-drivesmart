#ifndef CONDIDAT_H
#define CONDIDAT_H

#include <QWidget>
#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVariant>

namespace Ui {
class Condidat;
}

class Condidat : public QWidget
{
    Q_OBJECT

public:
    explicit Condidat(QWidget *parent = nullptr);
    ~Condidat();
    Condidat(int cin_condidat,QString nom,QString prenom,QString sexe,QDate date_naissance,int tel,QString type_permis);
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int cin_condidat);
    bool modifier(int originalCin,int newCin,QString nom,QString prenom,QString sexe,QDate date_naissance,int tel,QString type_permis);

private slots:
    void on_btn_ajout_clicked();
    void on_btn_reset_clicked();
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
    int selectedCinFromTable() const;
    void setFormFromRow(int row);
    QWidget* createActionsCell(int row, int cin);
    int currentEditingCin = -1;
    bool isEditMode = false;
    void setEditMode(bool enabled);
    int cin_condidat,tel;
    QString nom,prenom,type_permis,sexe;
    QDate date_naissance;

    Ui::Condidat *ui;
};

#endif // CONDIDAT_H
