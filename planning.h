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
#include <QLabel>
#include <QPixmap>
#include "google_calendar_service.h"
#include "openstreetmap_service.h"
#include "local_calendar.h"

namespace Ui
{
    class Planning;
}

class Planning : public QWidget
{
    Q_OBJECT

public:
    explicit Planning(QWidget *parent = nullptr);
    ~Planning();
    bool ajouter();
    QSqlQueryModel *afficher();
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
    
    // Search, Sort, Export functionality
    void on_recherche_3_textChanged();
    void on_tri_3_currentIndexChanged(int index);
    void on_pushButton_8_clicked(); // Export PDF
    
    // Statistics functionality
    void on_pushButton_10_clicked(); // Show statistics
    void afficherStatistiquesPlanning();
    
    // Map-related slots
    void onMapLocationSelected(double latitude, double longitude);
    void onGeocodeCompleted(double latitude, double longitude, const QString &formattedAddress);
    void onStaticMapReady(const QPixmap &mapImage);
    void onMapsError(const QString &error);
    
    // Calendar-related slots
    void onCalendarDateClicked(const QDate &date);
    void highlightDatesWithEvents();
    void refreshCalendarView();
    void onSearchLocationClicked();



private:
    void refreshTable();
    void refreshTableWithFilters(); // New method for filtered/sorted display
    int selectedIdFromTable() const;
    void setFormFromRow(int row);
    QWidget *createActionsCell(int row, int id);
    void setEditMode(bool enabled);
    void loadMoniteurs();
    void loadVehicules();
    void loadCondidats();
    QComboBox *getCondidatCombo(); // Fonction helper pour obtenir le champ condidat
    QString getCircuitAddress(const QString &circuitName); // Map circuit name to address

    int currentEditingId = -1;
    bool isEditMode = false;
    
    // Search and sort state
    QString currentSearchText;
    int currentSortIndex = 0;

    // Google Calendar integration
    GoogleCalendarService *calendarService;
    QMap<int, QString> eventIdMap; // Map planning ID to calendar event ID

    // OpenStreetMap integration (FREE - No API key needed!)
    OpenStreetMapService *mapService;
    LocalCalendar *localCalendar; // Local calendar service
    double currentLatitude;
    double currentLongitude;
    QString currentFormattedAddress;
    QLabel *mapDisplayLabel; // Label to display the static map image
    
    int pendingPlanningId; // Temporarily store planning ID while waiting for Calendar event creation

    // Planning data members
    QDate date_seance;
    QString condidat;
    QString nom_candidat;
    QString nom_moniteur;
    QString vehicule;
    QString type_seance;
    QTime heure_debut;
    QTime heure_fin;
    QString circuit;

    Ui::Planning *ui;
};

#endif // PLANNING_H
