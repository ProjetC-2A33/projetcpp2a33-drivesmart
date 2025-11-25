#ifndef VEHICULE_H
#define VEHICULE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTableWidgetItem>
#include <QEvent>
#include <QMap>
#include <QVariant>
#include <QLabel>
#include <QTimer>
#include <QDate>
#include <QVBoxLayout>
#include <QImage>
#include <QPainter>
#include <QUrl>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRandomGenerator>

namespace Ui {
class Vehicule;
}

class QTcpServer;

class Vehicule : public QWidget
{
    Q_OBJECT

public:
    explicit Vehicule(QWidget *parent = nullptr);
    ~Vehicule();

private:
    // Classe pour générer des QR codes simples
    class QRCodeGenerator {
    public:
        static QImage generateQRCode(const QString &data, int size) {
            // Créer une image vide
            QImage qrImage(size, size, QImage::Format_RGB32);
            qrImage.fill(Qt::white);
            QPainter painter(&qrImage);

            // Dessiner un QR code simple
            int margin = size / 10;
            int moduleSize = (size - 2 * margin) / 21; // 21x21 modules pour un QR code simple

            // Dessiner les marqueurs de position (les trois carrés dans les coins)
            drawPositionMarker(painter, margin, margin, moduleSize * 7);
            drawPositionMarker(painter, size - margin - moduleSize * 7, margin, moduleSize * 7);
            drawPositionMarker(painter, margin, size - margin - moduleSize * 7, moduleSize * 7);

            // Dessiner des motifs aléatoires pour simuler un QR code
            QRandomGenerator *generator = QRandomGenerator::global();
            painter.setPen(Qt::NoPen);
            painter.setBrush(Qt::black);

            for (int y = 0; y < 21; y++) {
                for (int x = 0; x < 21; x++) {
                    // Ne pas écraser les marqueurs de position
                    if ((x < 7 && y < 7) || (x > 13 && y < 7) || (x < 7 && y > 13)) {
                        continue;
                    }

                    if (generator->bounded(100) > 50) {
                        int px = margin + x * moduleSize;
                        int py = margin + y * moduleSize;
                        painter.drawRect(px, py, moduleSize, moduleSize);
                    }
                }
            }

            return qrImage;
        }

    private:
        static void drawPositionMarker(QPainter &painter, int x, int y, int size) {
            // Dessiner le carré extérieur noir
            painter.setPen(Qt::NoPen);
            painter.setBrush(Qt::black);
            painter.drawRect(x, y, size, size);

            // Dessiner le carré intérieur blanc
            int innerMargin = size / 7;
            painter.setBrush(Qt::white);
            painter.drawRect(x + innerMargin, y + innerMargin,
                             size - 2 * innerMargin, size - 2 * innerMargin);

            // Dessiner le point central noir
            int dotSize = size / 3;
            int dotOffset = (size - dotSize) / 2;
            painter.setBrush(Qt::black);
            painter.drawRect(x + dotOffset, y + dotOffset, dotSize, dotSize);
        }
    };

private slots:
    void on_AjouterButton_clicked();
    void on_Annuler_Button_clicked();
    void on_supprimerButton_clicked();
    void on_modifierButton_clicked();
    void on_tableWidget_itemClicked(QTableWidgetItem *item);
    void navigateToCandidat();
    void navigateToPlanning();
    void navigateToEmployee();
    void navigateToVehicule();
    void navigateToExamen();
    void navigateToEquipement();
    void on_recherche_4_returnPressed();
    void on_TriComboBox_2_currentIndexChanged(int index);
    void on_pushButton_18_clicked();
    void on_tri_2_currentIndexChanged(int index);
    void on_QR_CODE_2_clicked();

private:
    Ui::Vehicule *ui;
    QSqlDatabase db;
    int selectedRow;
    QLabel *notificationLabel; // Label pour afficher les notifications en bas à gauche
    QTimer *notificationTimer; // Timer pour faire disparaître la notification

    QTcpServer *httpServer;
    QTcpServer *httpServerAlt;
    quint16 httpPort;
    QString httpRootDir;

    // Fonctions CRUD
    bool ajouterVehicule();
    void afficherVehicules();
    bool modifierVehicule();
    bool supprimerVehicule();
    void viderChamps();
    void remplirChamps(int row);
    QString getDisponibilite(); // Récupère l'état de disponibilité sélectionné

    // Fonctions de statistiques
    QMap<QString, QVariant> getStatistiquesDisponibilite();
    QMap<QString, QVariant> getStatistiquesTypeEnergie();

    // Fonction pour intercepter les événements clavier
    bool eventFilter(QObject *obj, QEvent *event) override;

    // Gestionnaire de redimensionnement pour repositionner la notification
    void resizeEvent(QResizeEvent *event) override;

    // Fonction métier avancée : calcul des jours restants pour la maintenance
    void afficherJoursRestantsMaintenance(const QDate& dateMaintenance, const QString& matricule);

    // Fonctions pour la génération de QR code
    QString genererTexteQRCode(const QString& matricule);
    QString predirePanne(const QString& matricule, int kilometrage, const QDate& dateMaintenance, const QString& etatMaintenance, const QString& typeEnergie);
    QString genererFichierHTML(const QString& matricule, const QString& modele, int kilometrage, const QString& disponibilite, const QString& typeEnergie, const QString& etatMaintenance, const QDate& dateMaintenance, const QString& prediction);

    // Fonction utilitaire pour ajouter une ligne d'information dans la boîte de dialogue
    void addInfoRow(QVBoxLayout *layout, const QString &label, const QString &value);

    bool ensureHttpServer();
    void handleHttpConnection();
    QString selectBestLanIp();
};

#endif // VEHICULE_H
