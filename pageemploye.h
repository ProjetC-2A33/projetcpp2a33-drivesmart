

#ifndef pageemploye_H
#define pageemploye_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QDialog>


#include <QJsonObject>
QT_BEGIN_NAMESPACE
namespace Ui { class pageemploye; }
QT_END_NAMESPACE

class pageemploye : public QDialog
{
    Q_OBJECT

public:
    explicit pageemploye(QWidget *parent = nullptr);
    ~pageemploye();

private:
    Ui::pageemploye *ui;
    void afficherEmployes();
    void clearForm();
    bool validerCin(const QString &cin);
    bool validerNomPrenom(const QString &texte, const QString &champ);
    bool validerSalaire(const QString &salaire);
    bool validerTelephone(const QString &tel);
    bool validerEmail(const QString &email);
    bool validerMdp(const QString &mdp);
    void afficherStatistiquesDisponibilite();
    void lancerRecherche(const QString &texte);
    QNetworkAccessManager *networkManager;
    void sendToGeminiAPI(const QString &question);
private slots:
    void on_btn_reset_E_clicked();
    void on_btn_ajout_E_clicked();
    void on_tab_em_cellClicked(int row, int column);
    void on_modif_E_clicked();
    void on_sup_E_clicked();
    void updateButtonStates();
    void on_btn_c_clicked();  // Bouton recherche
    void on_recherche_c_textChanged(const QString &text);
    void on_trier_c_clicked();
    void on_pdf_c_clicked();
    void on_stat_c_clicked();
    void on_chatbot_clicked();
    void onChatbotResponseReceived(QNetworkReply *reply);

};

#endif // pageemploye_H
