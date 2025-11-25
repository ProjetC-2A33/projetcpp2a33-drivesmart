#ifndef EXAMEN_H
#define EXAMEN_H

#include <QWidget>
#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVariant>
#include <QPair>

namespace Ui
{
    class Examen;
}

class Examen : public QWidget
{
    Q_OBJECT

public:
    explicit Examen(QWidget *parent = nullptr);
    ~Examen();
    bool ajouter();
    QSqlQueryModel *afficher();
    bool supprimer(QString id_examen);
    bool modifier(QString originalId, QString newId, QString type_examen, QString type_permis, QDate date_examen);

private slots:
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_7_clicked();
    void navigateToCandidat();
    void navigateToPlanning();
    void navigateToEmployee();
    void navigateToVehicule();
    void navigateToExamen();
    void navigateToEquipement();

private:
    void refreshTable();
    QString selectedIdFromTable() const;
    void setFormFromRow(int row);
    QWidget *createActionsCell(int row, QString id);
    void setEditMode(bool enabled);
    QPair<QString, QString> fetchDefaultCandidat() const; // returns <cin, telephone>
    void prefillDefaultCandidat();
    QString id_examen;
    QString type_examen;
    QString type_permis;
    QDate date_examen;
    QString cin_candidat;
    QString telephone;
    QString currentEditingId = "";
    bool isEditMode = false;
    Ui::Examen *ui;
};

#endif // EXAMEN_H