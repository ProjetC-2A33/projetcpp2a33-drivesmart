#include "condidat.h"
#include "ui_condidat.h"
#include "navigation_constants.h"
#include <QMessageBox>
#include <QMainWindow>
#include <QStackedWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QTableWidgetItem>
#include <QDate>
#include <memory>
#include <QPushButton>
#include <QHBoxLayout>
#include <QWidget>
#include <QHeaderView>
#include <QStyle>
#include <QRegularExpression>
#include <QFileDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QTextStream>
#include <QDesktopServices>
#include <QUrl>
#include <QDateTime>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QStringConverter>
#endif
#include <QPixmap>
#include <QPainter>
#include <QMap>
#include <QApplication>
#include <QFont>
#include <QFontMetrics>
#include <QBrush>
#include <QPen>
#include <cmath>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QAudioSource>
#include <QAudioDevice>
#include <QMediaDevices>
#else
#include <QAudioInput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#endif
#include <QIODevice>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QBuffer>
#include <QRegularExpression>
#include <QDebug>
#include <QInputDialog>
#include <QDialog>
#include <QHttpMultiPart>
#include <QProcessEnvironment>
#ifdef USE_VOSK
#include <vosk_api.h>
#endif
#include <QDir>
#include <QKeyEvent>
#include <QCoreApplication>

Condidat::Condidat(QWidget *parent) : QWidget(parent),
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    audioSource(nullptr),
#else
    audioInput(nullptr),
#endif
    audioBuffer(nullptr),
    networkManager(nullptr),
    recordingTimer(nullptr),
    isRecording(false),
    ui(new Ui::Condidat)
{
    ui->setupUi(this);

    // Initialize network manager for potential future use
    networkManager = new QNetworkAccessManager(this);

    // Initialize recording timer
    recordingTimer = new QTimer(this);
    recordingTimer->setSingleShot(true);
    connect(recordingTimer, &QTimer::timeout, this, &Condidat::stopVoiceRecording);

    // Resolve voice button even if not present in generated UI header
    voiceButton = ui->btn_vocal ? ui->btn_vocal : this->findChild<QPushButton*>("btn_vocal");
    if (!voiceButton)
    {
        voiceButton = new QPushButton(this);
        voiceButton->setObjectName("btn_vocal");
        voiceButton->setText("🎤 Remplir par voix");
        voiceButton->setStyleSheet("background-color:#002157;color:#ffce00;border-radius:10px;");
        voiceButton->setGeometry(QRect(50, 710, 271, 31));
        voiceButton->raise();
    }

#ifdef USE_VOSK
    voiceButton->setText("🎤 Remplir par voix");
    voiceButton->setStyleSheet("background-color:#002157;color:#ffce00;border-radius:10px;");
    voiceButton->setToolTip("Cliquez pour parler");
    voiceButton->show();
    if (this->findChild<QLabel*>("label_10")) this->findChild<QLabel*>("label_10")->show();
#else
    // Masquer le bouton vocal si Vosk n'est pas disponible
    voiceButton->hide();
    if (this->findChild<QLabel*>("label_10")) this->findChild<QLabel*>("label_10")->hide();
#endif

    // Connect navigation buttons
    connect(ui->candidat, &QPushButton::clicked, this, &Condidat::navigateToCandidat);
    connect(ui->planning, &QPushButton::clicked, this, &Condidat::navigateToPlanning);
    connect(ui->employee, &QPushButton::clicked, this, &Condidat::navigateToEmployee);
    connect(ui->vehicule, &QPushButton::clicked, this, &Condidat::navigateToVehicule);
    connect(ui->examen, &QPushButton::clicked, this, &Condidat::navigateToExamen);
    connect(ui->equipement, &QPushButton::clicked, this, &Condidat::navigateToEquipement);

    // Connect tri combo box
    connect(ui->tri, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Condidat::on_tri_currentIndexChanged);

    // Connect recherche text changed
    connect(ui->recherche, &QTextEdit::textChanged, this, &Condidat::on_recherche_textChanged);

    // Connect exporter button
    connect(ui->exporter, &QPushButton::clicked, this, &Condidat::on_exporter_clicked);

    // Connect statistics combo box
    connect(ui->tri_2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Condidat::on_tri_2_currentIndexChanged);

    // Connect voice button only if Vosk is available
#ifdef USE_VOSK
    connect(voiceButton, &QPushButton::clicked, this, &Condidat::on_btn_vocal_clicked);
#endif

    // Initialize sort and filter variables
    currentFilter = QString();
    currentSortColumn = QString();
    currentSortOrder = Qt::AscendingOrder;

    // Note: slots on_supp_clicked and on_edit_clicked are auto-connected by Qt
    // via connectSlotsByName(this) inside setupUi, due to their naming.
    // No manual connects needed to avoid duplicate signal handling.

    // Initialize table headers (ensure 8 columns: 7 data + 1 actions)
    ui->tab->setColumnCount(8);
    QStringList headers;
    headers << "CIN" << "nom" << "Prénom" << "Sexe" << "date de naissance" << "Télephone" << "type de permis" << "Actions";
    ui->tab->setHorizontalHeaderLabels(headers);

    // Table visual/interaction tuning
    ui->tab->setShowGrid(true);
    ui->tab->setAlternatingRowColors(true);
    ui->tab->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tab->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tab->horizontalHeader()->setVisible(true);
    ui->tab->verticalHeader()->setVisible(true); // shows row numbers
    ui->tab->horizontalHeader()->setStretchLastSection(true);
    ui->tab->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tab->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // disable bottom scrollbar only
    ui->tab->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Load initial data
    refreshTable();

    // Initialize with empty chart
    updateStatisticsChart(0);

#ifdef USE_VOSK
    initVosk();
#endif
    qApp->installEventFilter(this);
}

Condidat::~Condidat()
{
#ifdef USE_VOSK
    freeVosk();
#endif
    qApp->removeEventFilter(this);
    delete ui;
}

// ... (toutes les autres fonctions restent identiques jusqu'à on_btn_vocal_clicked)

void Condidat::on_btn_vocal_clicked()
{
#ifndef USE_VOSK
    QMessageBox::information(this, "Fonctionnalité désactivée",
                             "La reconnaissance vocale n'est pas disponible dans cette version.\n"
                             "Veuillez compiler avec la bibliothèque Vosk pour activer cette fonctionnalité.");
    return;
#else
    if (isRecording)
    {
        stopVoiceRecording();
        return;
    }
    if (!ui->cin->toPlainText().isEmpty() ||
        !ui->nom->toPlainText().isEmpty() ||
        !ui->prenom->toPlainText().isEmpty())
    {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Formulaire déjà rempli",
            "Le formulaire contient déjà des données. Voulez-vous les remplacer par la saisie vocale?",
            QMessageBox::Yes | QMessageBox::No
            );
        if (reply == QMessageBox::No)
            return;
    }
    startVoiceRecording();
#endif
}

void Condidat::startVoiceRecording()
{
#ifdef USE_VOSK
    if (!voskReady) {
        QMessageBox::warning(this, "Erreur Vosk", "Le modèle de reconnaissance vocale n'est pas initialisé.");
        return;
    }
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QMediaDevices devices;
    QAudioDevice device = devices.defaultAudioInput();
    if (device.isNull())
    {
        QMessageBox::warning(this, "Erreur", "Aucun microphone détecté.");
        return;
    }
    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);
    if (!device.isFormatSupported(format))
        format = device.preferredFormat();
    audioSource = new QAudioSource(device, format, this);
#else
    QAudioDeviceInfo deviceInfo = QAudioDeviceInfo::defaultInputDevice();
    if (deviceInfo.isNull())
    {
        QMessageBox::warning(this, "Erreur", "Aucun microphone détecté.");
        return;
    }
    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
    if (!deviceInfo.isFormatSupported(format))
        format = deviceInfo.nearestFormat(format);
    audioInput = new QAudioInput(format, this);
#endif

    if (audioBuffer)
    {
        audioBuffer->close();
        audioBuffer->deleteLater();
    }
    audioBuffer = new QBuffer(this);
    audioBuffer->open(QIODevice::ReadWrite);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    audioSource->start(audioBuffer);
#else
    audioInput->start(audioBuffer);
#endif

#ifdef USE_VOSK
    // Prepare Vosk recognizer for streaming
    if (voskReady && voskModel)
    {
        // free any previous recognizer
        if (voskRecognizer) {
            vosk_recognizer_free(voskRecognizer);
            voskRecognizer = nullptr;
            voskProcessedBytes = 0;
        }

        voskRecognizer = vosk_recognizer_new(voskModel, 16000.0);
        if (!voskRecognizer) {
            QMessageBox::warning(this, "Erreur Vosk", "Impossible de créer le reconnaisseur vocal.");
            return;
        }

        if (voskPollTimer) {
            voskPollTimer->stop();
            voskPollTimer->deleteLater();
            voskPollTimer = nullptr;
        }
        voskPollTimer = new QTimer(this);
        voskPollTimer->setInterval(100);
        connect(voskPollTimer, &QTimer::timeout, this, [this]() {
            if (!audioBuffer || !voskRecognizer) return;
            qint64 available = audioBuffer->size() - voskProcessedBytes;
            if (available <= 0) return;
            audioBuffer->seek(voskProcessedBytes);
            QByteArray chunk = audioBuffer->read(static_cast<int>(available));
            voskProcessedBytes += chunk.size();
            int accepted = vosk_recognizer_accept_waveform(voskRecognizer, chunk.constData(), chunk.size());
            if (accepted) {
                QByteArray res(vosk_recognizer_result(voskRecognizer));
                QJsonDocument doc = QJsonDocument::fromJson(res);
                QString text = doc.isObject() ? doc.object().value("text").toString() : QString();
                if (!text.isEmpty()) processVoiceText(text);
            } else {
                const char* pres = vosk_recognizer_partial_result(voskRecognizer);
                if (pres) {
                    QString partial = QString::fromUtf8(pres);
                    // optional: show partial in UI
                    qDebug() << "Vosk partial:" << partial;
                }
            }
        });
        voskPollTimer->start();
    }
#endif

    isRecording = true;
    voiceButton->setText("● Enregistrement...");
    voiceButton->setStyleSheet("background-color:#FF5722;color:white;border-radius:10px;font-weight:bold;");
    voiceButton->setEnabled(true);
    recordingTimer->start(10000);
    voiceButton->setToolTip("Enregistrement en cours... Cliquez pour arrêter");
}

void Condidat::stopVoiceRecording()
{
    isRecording = false;
    if (recordingTimer)
        recordingTimer->stop();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if (audioSource) {
        audioSource->stop();
        audioSource->deleteLater();
        audioSource = nullptr;
    }
#else
    if (audioInput) {
        audioInput->stop();
        audioInput->deleteLater();
        audioInput = nullptr;
    }
#endif
    QByteArray pcmData;
    if (audioBuffer)
    {
        pcmData = audioBuffer->buffer();
        audioBuffer->close();
        audioBuffer->deleteLater();
        audioBuffer = nullptr;
    }

#ifdef USE_VOSK
    if (voskPollTimer) {
        voskPollTimer->stop();
        voskPollTimer->deleteLater();
        voskPollTimer = nullptr;
    }
    if (voskRecognizer)
    {
        // feed remaining bytes
        if (!pcmData.isEmpty()) {
            vosk_recognizer_accept_waveform(voskRecognizer, pcmData.constData(), pcmData.size());
        }
        QByteArray finalRes(vosk_recognizer_final_result(voskRecognizer));
        vosk_recognizer_free(voskRecognizer);
        voskRecognizer = nullptr;
        QJsonDocument doc = QJsonDocument::fromJson(finalRes);
        QString text = doc.isObject() ? doc.object().value("text").toString() : QString();
        if (!text.isEmpty()) {
            processVoiceText(text);
        } else {
            voiceButton->setText("🎤 Remplir par voix");
            voiceButton->setStyleSheet("background-color:#002157;color:#ffce00;border-radius:10px;");
            voiceButton->setEnabled(true);
            voiceButton->setToolTip("Cliquez pour parler");
            QMessageBox::information(this, "Reconnaissance vocale", "Aucune parole détectée.");
        }
    }
#else
    voiceButton->setText("🎤 Remplir par voix");
    voiceButton->setStyleSheet("background-color:#002157;color:#ffce00;border-radius:10px;");
    voiceButton->setEnabled(true);
    voiceButton->setToolTip("Cliquez pour parler");
#endif
}

// ... (toutes les autres fonctions restent identiques)

#ifdef USE_VOSK
QString Condidat::locateVoskModelPath() const
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString path = env.value("VOSK_MODEL_PATH").trimmed();
    if (!path.isEmpty() && QDir(path).exists()) {
        qDebug() << "Using VOSK_MODEL_PATH:" << path;
        return path;
    }

    // Check in application directory
    QString appDir = QCoreApplication::applicationDirPath();
    QString localPath = appDir + "/models/fr";
    if (QDir(localPath).exists()) {
        qDebug() << "Using local model path:" << localPath;
        return localPath;
    }

    // Check in source directory (for development)
    localPath = QDir::currentPath() + "/models/fr";
    if (QDir(localPath).exists()) {
        qDebug() << "Using development model path:" << localPath;
        return localPath;
    }

    qDebug() << "Vosk model not found in any location";
    return QString();
}

void Condidat::initVosk()
{
    QString modelPath = locateVoskModelPath();
    if (modelPath.isEmpty())
    {
        voskReady = false;
        qDebug() << "Vosk model not found";
        QMessageBox::warning(this, "Configuration Vosk",
                             "Modèle Vosk non trouvé.\n\n"
                             "Veuillez placer le modèle français dans :\n"
                             "• models/fr/ dans le répertoire de l'application\n"
                             "• Ou définir la variable VOSK_MODEL_PATH");
        return;
    }

    voskModel = vosk_model_new(modelPath.toUtf8().constData());
    if (!voskModel)
    {
        voskReady = false;
        qDebug() << "Failed to load Vosk model at" << modelPath;
        QMessageBox::warning(this, "Erreur Vosk",
                             "Impossible de charger le modèle Vosk.\n"
                             "Vérifiez que le modèle est valide et complet.");
        return;
    }

    voskReady = true;
    qDebug() << "Vosk initialized successfully with model:" << modelPath;
}

void Condidat::freeVosk()
{
    if (voskPollTimer) {
        voskPollTimer->stop();
        voskPollTimer->deleteLater();
        voskPollTimer = nullptr;
    }

    if (voskRecognizer) {
        vosk_recognizer_free(voskRecognizer);
        voskRecognizer = nullptr;
    }

    if (voskModel) {
        vosk_model_free(voskModel);
        voskModel = nullptr;
    }

    voskReady = false;
}

void Condidat::transcribeWithVosk(const QByteArray &pcm)
{
    if (!voskReady || !voskModel) {
        if (voiceButton) {
            voiceButton->setText("🎤 Remplir par voix");
            voiceButton->setStyleSheet("background-color:#002157;color:#ffce00;border-radius:10px;");
            voiceButton->setEnabled(true);
        }
        return;
    }

    VoskRecognizer *rec = vosk_recognizer_new(voskModel, 16000.0);
    if (!rec) {
        qDebug() << "Failed to create Vosk recognizer";
        if (voiceButton) {
            voiceButton->setText("🎤 Remplir par voix");
            voiceButton->setStyleSheet("background-color:#002157;color:#ffce00;border-radius:10px;");
            voiceButton->setEnabled(true);
        }
        return;
    }

    vosk_recognizer_accept_waveform(rec, pcm.constData(), pcm.size());
    QByteArray res(vosk_recognizer_result(rec));
    vosk_recognizer_free(rec);
    QJsonDocument doc = QJsonDocument::fromJson(res);
    QString text = doc.isObject() ? doc.object().value("text").toString() : QString();
    processVoiceText(text);
}
#endif

void Condidat::onVoicePressed()
{
    isPressing = true;
    if (holdTimer) {
        holdTimer->start(400);
    }
}

void Condidat::onVoiceReleased()
{
    isPressing = false;
    if (holdTimer && holdTimer->isActive()) {
        holdTimer->stop();
    }
    if (isRecording) {
        stopVoiceRecording();
    }
}

bool Condidat::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj);
    if (!this->isVisible()) return false;
#ifdef USE_VOSK
    if (event->type() == QEvent::KeyPress)
    {
        auto *ke = static_cast<QKeyEvent*>(event);
        if (ke->key() == Qt::Key_V && !ke->modifiers())
        {
            if (isRecording)
                stopVoiceRecording();
            else
                startVoiceRecording();
            return true;
        }
    }
#endif
    return false;
}

// Constructeur avec paramètres
Condidat::Condidat(int cin_condidat, QString nom, QString prenom, QString sexe, QDate date_naissance, int tel, QString type_permis)
    : cin_condidat(cin_condidat), nom(nom), prenom(prenom), sexe(sexe), date_naissance(date_naissance), tel(tel), type_permis(type_permis),
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    audioSource(nullptr),
#else
    audioInput(nullptr),
#endif
    audioBuffer(nullptr), networkManager(nullptr), recordingTimer(nullptr), isRecording(false), ui(new Ui::Condidat)
{
    ui->setupUi(this);
}
