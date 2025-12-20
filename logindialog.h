#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    QString getEmail() const { return email; }
    QString getPassword() const { return mdp; }

private slots:
    void on_signInButton_2_clicked();
    void on_forgotpButton_2_clicked();
    void on_sendBtn_clicked();
    void on_verify_pb_clicked();
    void on_cont_pb_clicked();
    void on_backToLogin1_clicked();
    void on_backToLogin2_clicked();
    void on_backToLogin3_clicked();

private:
    Ui::Dialog *ui;
    QString email;
    QString mdp;
    QString sentVerificationCode;
    QString userEmail;
    bool isSendingEmail;

    QString generateVerificationCode();
    void sendMail(const QString &verificationCode);
    bool verifierConnexion();
};

#endif // LOGINDIALOG_H
