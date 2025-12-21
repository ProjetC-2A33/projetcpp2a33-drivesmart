#ifndef GESTION_EQUIPEMENT_H
#define GESTION_EQUIPEMENT_H

#include <QWidget>
#include <QSqlDatabase>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPrinter>
#include <QPainter>
#include <QPagedPaintDevice>
#include <QTextDocument>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

QT_BEGIN_NAMESPACE
class QTableWidgetItem;
QT_END_NAMESPACE

namespace Ui
{
    class Gestion_Equipement;
}

class Gestion_Equipement : public QWidget
{
    Q_OBJECT

public:
    explicit Gestion_Equipement(QWidget *parent = nullptr);
    ~Gestion_Equipement();

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
    void exportToPDF();

    // Maintenance Prediction slots
    void on_pushButton_predict_maintenance_clicked();

    // Lifecycle & Depreciation slots
    void on_comboBox_lifecycle_equipment_currentIndexChanged(int index);

private:
    Ui::Gestion_Equipement *ui;
    QSqlDatabase db;
    int selectedRow;
    bool isEditMode;
    int currentEditingId;

    // Charts
    QChartView *categoryChartView;
    QChartView *valueChartView;

    // Maintenance Prediction
    QNetworkAccessManager *networkManager;
    void loadEquipmentForMaintenance();
    void updateMaintenancePrediction(int equipmentId);
    int getMaintenanceInterval(const QString &category);

    // Equipment Lifecycle & Depreciation
    struct LifecycleData
    {
        double originalCost;
        double currentValue;
        double depreciationAmount;
        double depreciationPercent;
        QString lifecycleStage;
        int ageYears;
        int ageMonths;
        int usefulLifeYears;
        double usefulLifePercent;
        int replacementYear;
        QString recommendation;
        double annualDepreciation;
        double monthlyDepreciation;
        double residualValue;
    };
    void loadEquipmentForLifecycle();
    void calculateLifecycle(int equipmentId);
    LifecycleData computeDepreciation(const QDate &acquisitionDate, const QString &category, double estimatedCost);
    int getUsefulLife(const QString &category);
    QString getLifecycleStage(double usefulLifePercent);
    double estimateEquipmentCost(const QString &nom, const QString &category);

    // CRUD methods
    bool ajouter(int id, const QString &nom, const QString &categorie,
                 int quantite_totale, int quantite_disponible, const QString &etat,
                 const QString &fournisseur, const QDate &date_acquisition,
                 const QString &cin_employee);
    bool modifier(int id, const QString &nom, const QString &categorie,
                  int quantite_totale, int quantite_disponible, const QString &etat,
                  const QString &fournisseur, const QDate &date_acquisition,
                  const QString &cin_employee);
    bool supprimer(int id);
    void afficher(QString orderBy = "");
    void rechercher(const QString &critere);

    // Helper methods
    void viderChamps();
    void remplirChamps(int row);
    void rafraichirTable();
    void chargerEmployees();
    int genererNouvelID();
    QWidget *createActionsCell(int row, int id);
    void setEditMode(bool enabled);
    void setFormFromRow(int row);

    // Statistics and Charts methods
    void updateStatistics();
    void updateCharts();
    void createCategoryPieChart();
    void createValueBarChart();
    void calculateTotalValue();
    void updateCategoryBreakdown();
};

#endif // GESTION_EQUIPEMENT_H