#ifndef CONDIDAT_H
#define CONDIDAT_H

#include <QWidget>
#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVariant>
#include <QFileDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QTextStream>
#include <QHeaderView>
#include <QPixmap>
#include <QSize>
#include <QMap>
#include <QIODevice>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QBuffer>
#include <QPushButton>
#include <QKeyEvent>
// Audio features disabled - Qt Multimedia not available
/*
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QAudioSource>
#include <QAudioDevice>
#include <QMediaDevices>
#else
#include <QAudioInput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#endif
*/
#ifdef USE_VOSK
#include <vosk_api.h>
#endif

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
    void on_tri_currentIndexChanged(int index);
    void on_recherche_textChanged();
    void on_exporter_clicked();
    void on_tri_2_currentIndexChanged(int index);
    void on_btn_vocal_clicked();
    void on_cin_condidat_currentIndexChanged(int index);

private:
    void refreshTable();
    void refreshTableWithFilter(const QString &filter = QString());
    void refreshTableWithSort(const QString &sortColumn = QString(), Qt::SortOrder order = Qt::AscendingOrder);
    int selectedCinFromTable() const;
    void setFormFromRow(int row);
    QWidget* createActionsCell(int row, int cin);
    void exportToPDF();
    void exportToExcel();
    int calculateAge(const QDate &birthDate) const;
    void updateStatisticsChart(int index);
    void createSexeChart();
    void createLicenseTypeChart();
    QPixmap createPieChartPixmap(const QMap<QString, int> &data, const QString &title, const QSize &size);
    void startVoiceRecording();
    void stopVoiceRecording();
    void processVoiceText(const QString &text);
    void parseAndFillForm(const QString &text);
    QByteArray createWavFromPcm(const QByteArray &pcm, int sampleRate, int channels, int sampleSizeBits);
    QString extractCIN(const QString &text);
    QString extractName(const QString &text, bool isFirstName = false);
    QString extractSexe(const QString &text);
    QString extractPhone(const QString &text);
    QString extractLicenseType(const QString &text);
    QDate extractDate(const QString &text);
    void updatePrediction(int age, QString sexe, QString type_permis);
    void populateCandidateComboBox();
    int currentEditingCin = -1;
    bool isEditMode = false;
    void setEditMode(bool enabled);
    int cin_condidat,tel;
    QString nom,prenom,type_permis,sexe;
    QDate date_naissance;
    QString currentFilter;
    QString currentSortColumn;
    Qt::SortOrder currentSortOrder;

// Audio features disabled - Qt Multimedia not available
/*
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QAudioSource *audioSource;
#else
    QAudioInput *audioInput;
#endif
*/
    QBuffer *audioBuffer;
    QNetworkAccessManager *networkManager;
    QTimer *recordingTimer;
    bool isRecording;
    QTimer *holdTimer;
    bool isPressing = false;

    // Vosk
    bool voskReady = false;
#ifdef USE_VOSK
    void initVosk();
    void freeVosk();
    void transcribeWithVosk(const QByteArray &pcm);
    QString locateVoskModelPath() const;

    // Runtime Vosk objects used when streaming audio
    VoskRecognizer *voskRecognizer = nullptr;
    QTimer *voskPollTimer = nullptr;
    qint64 voskProcessedBytes = 0;
#endif

    Ui::Condidat *ui;
    QPushButton *voiceButton = nullptr;

private slots:
    void onVoicePressed();
    void onVoiceReleased();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // CONDIDAT_H
