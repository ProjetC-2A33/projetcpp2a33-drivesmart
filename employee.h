#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QWidget>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
class QTableWidgetItem;
QT_END_NAMESPACE

namespace Ui
{
    class Employee;
}

class Employee : public QWidget
{
    Q_OBJECT

public:
    explicit Employee(QWidget *parent = nullptr);
    ~Employee();

private slots:
    // Navigation slots
    void navigateToCandidat();
    void navigateToPlanning();
    void navigateToEmployee();
    void navigateToVehicule();
    void navigateToExamen();
    void navigateToEquipement();

    // CRUD operation slots
    void on_pushButton_ajouter_clicked();
    void on_pushButton_annuler_clicked();
    void on_tableWidget_itemClicked(QTableWidgetItem *item);
    void on_tri_currentIndexChanged(int index);
    void on_exporter_clicked();

private:
    Ui::Employee *ui;
    QSqlDatabase db;
    int selectedRow;
    bool isEditMode;
    QString currentEditingCin;

    // CRUD methods
    bool ajouter(const QString &cin, const QString &nom, const QString &prenom,
                 const QString &disponibilite, double salaire, const QString &telephone,
                 const QString &email, const QString &poste, const QString &mdp);
    bool modifier(const QString &cin, const QString &nom, const QString &prenom,
                  const QString &disponibilite, double salaire, const QString &telephone,
                  const QString &email, const QString &poste, const QString &mdp);
    bool supprimer(const QString &cin);
    void afficher(QString orderBy = "");
    void rechercher(const QString &critere);

    // Helper methods
    void viderChamps();
    void remplirChamps(int row);
    QString getDisponibilite();
    void rafraichirTable();
    QWidget *createActionsCell(int row, const QString &cin);
    void setEditMode(bool enabled);
    void setFormFromRow(int row);
};

#endif // EMPLOYEE_H
