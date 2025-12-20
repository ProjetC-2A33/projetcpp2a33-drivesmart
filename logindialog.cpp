#include "logindialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QTime>
#include <QTimer>
#include "smtp.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
    , isSendingEmail(false) // Initialize flag
{
    ui->setupUi(this);
    ui->emailInput_2->setPlaceholderText("Enter your email");
    ui->passwordInput_2->setPlaceholderText("Enter your password");
    ui->passwordInput_2->setEchoMode(QLineEdit::Password);
    ui->rcpt->setPlaceholderText("Enter your email");
    ui->codebt->setPlaceholderText("Enter verification code");
    ui->pas1->setPlaceholderText("Enter new password");
    ui->pas2->setPlaceholderText("Confirm new password");
    ui->pas1->setEchoMode(QLineEdit::Password);
    ui->pas2->setEchoMode(QLineEdit::Password);
    connect(ui->signInButton_2, &QPushButton::clicked, this, &LoginDialog::on_signInButton_2_clicked, Qt::UniqueConnection);
    connect(ui->forgotpButton_2, &QPushButton::clicked, this, &LoginDialog::on_forgotpButton_2_clicked, Qt::UniqueConnection);
    connect(ui->sendBtn, &QPushButton::clicked, this, &LoginDialog::on_sendBtn_clicked, Qt::UniqueConnection);
    connect(ui->verify_pb, &QPushButton::clicked, this, &LoginDialog::on_verify_pb_clicked, Qt::UniqueConnection);
    connect(ui->cont_pb, &QPushButton::clicked, this, &LoginDialog::on_cont_pb_clicked, Qt::UniqueConnection);
    connect(ui->backToLogin1, &QPushButton::clicked, this, &LoginDialog::on_backToLogin1_clicked, Qt::UniqueConnection);
    connect(ui->backToLogin2, &QPushButton::clicked, this, &LoginDialog::on_backToLogin2_clicked, Qt::UniqueConnection);
    connect(ui->backToLogin3, &QPushButton::clicked, this, &LoginDialog::on_backToLogin3_clicked, Qt::UniqueConnection);
}
LoginDialog::~LoginDialog()
{
    delete ui;
}

#include <QRandomGenerator>

QString LoginDialog::generateVerificationCode()
{
    int code = QRandomGenerator::global()->bounded(1000, 10000);
    return QString::number(code);
}

void LoginDialog::sendMail(const QString &verificationCode)
{
    if (isSendingEmail) {
        QMessageBox::warning(this, "Warning", "Please wait, email is being sent...");
        return;
    }

    isSendingEmail = true;

    QString user = "culeks.here@gmail.com";
    QString pass = "xxrzopxyytpgwcph";
    QString host = "smtp.gmail.com";
    int port = 465;
    int timeout = 30000;

    QString from = "culeks.here@gmail.com";
    QString to = userEmail;

    Smtp *smtp = new Smtp(user, pass, host, port, timeout);
    connect(smtp, &Smtp::status, this, [this, verificationCode, smtp](const QString &status) {
        // Reset flag first
        this->isSendingEmail = false;

        qDebug() << "SMTP Status:" << status;

        if (status == "Message sent") {
            this->sentVerificationCode = verificationCode;
            qDebug() << "Verification code stored:" << this->sentVerificationCode;

            QMessageBox::information(this, "Success", "Verification code sent to your email!");

            ui->stackedWidget->setCurrentIndex(2);
            qDebug() << "Navigated to verification page";
        } else {

            QMessageBox::warning(this, "Error", "Failed to send verification code: " + status);
        }


    });

    QString subject = "Password Reset Verification Code";
    QString body = QString("Your verification code is: %1\n\n")
                       .arg(verificationCode);
    body += "This code will expire in 10 minutes.\n";
    body += "If you didn't request this reset, please ignore this email.";

    // Send the email
    smtp->sendMail(from, to, subject, body);
}

void LoginDialog::on_signInButton_2_clicked()
{
    QString inputEmail = ui->emailInput_2->text().trimmed();
    QString inputMdp   = ui->passwordInput_2->text();

    if (inputEmail.isEmpty() || inputMdp.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }

    email = inputEmail.toLower();
    mdp   = inputMdp;

    if (verifierConnexion()) {
        accept();  // Login réussi
    } else {
        QMessageBox::warning(this, "Échec", "Email ou mot de passe incorrect.");
    }
}

bool LoginDialog::verifierConnexion()
{
    QSqlQuery query;
    query.prepare("SELECT 1 FROM EMPLOYEE WHERE LOWER(EMAIL) = :email AND MDP = :mdp");
    query.bindValue(":email", email);
    query.bindValue(":mdp", mdp);

    if (!query.exec()) {
        qDebug() << "Erreur BD :" << query.lastError().text();
        return false;
    }

    return query.next();
}

void LoginDialog::on_forgotpButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->rcpt->clear();
    ui->rcpt->setFocus();
}

void LoginDialog::on_sendBtn_clicked()
{
    userEmail = ui->rcpt->text().trimmed();

    if (userEmail.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter your email address.");
        return;
    }
    QRegularExpression emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    if (!emailRegex.match(userEmail).hasMatch()) {
        QMessageBox::warning(this, "Error", "Please enter a valid email address.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT 1 FROM EMPLOYEE WHERE EMAIL = :email");
    query.bindValue(":email", userEmail);
    if (!query.exec()) {
        QMessageBox::warning(this, "Error", "Database error occurred.");
        return;
    }
    if (!query.next()) {
        QMessageBox::warning(this, "Error", "Email not found in our system.");
        return;
    }
    QString verificationCode = generateVerificationCode();
    qDebug() << "Generated code:" << verificationCode;
    QMessageBox::information(this, "Sending", "Sending verification code...");

    sendMail(verificationCode);
}

void LoginDialog::on_verify_pb_clicked()
{
    QString enteredCode = ui->codebt->text().trimmed();

    if (enteredCode.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter the verification code.");
        return;
    }

    if (enteredCode == sentVerificationCode) {
        ui->stackedWidget->setCurrentIndex(3);
        ui->pas1->clear();
        ui->pas2->clear();
        ui->pas1->setFocus();
    } else {
        QMessageBox::warning(this, "Verification Failed", "Incorrect verification code. Please try again.");
        ui->codebt->clear();
        ui->codebt->setFocus();
    }
}

void LoginDialog::on_cont_pb_clicked()
{
    QString newPassword = ui->pas1->text().trimmed();
    QString confirmPassword = ui->pas2->text().trimmed();

    qDebug() << "Attempting password reset for email:" << userEmail;
    qDebug() << "New password length:" << newPassword.length();

    if (newPassword.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill in all password fields.");
        return;
    }

    if (newPassword != confirmPassword) {
        QMessageBox::warning(this, "Error", "Passwords do not match.");
        return;
    }
    if (newPassword.length() < 8) {
        QMessageBox::warning(this, "Error", "Password must be at least 8 characters long.");
        return;
    }

    QRegularExpression passwordRegex("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[@$!%*?&])[A-Za-z\\d@$!%*?&]{8,}$");
    if (!passwordRegex.match(newPassword).hasMatch()) {
        QMessageBox::warning(this, "Error",
                             "Password must contain:\n"
                             "• At least 1 uppercase letter\n"
                             "• At least 1 lowercase letter\n"
                             "• At least 1 digit\n"
                             "• At least 1 special character (@$!%*?&)");
        return;
    }
    if (userEmail.isEmpty()) {
        QMessageBox::warning(this, "Error", "No email address found. Please restart the password reset process.");
        return;
    }

    qDebug() << "Updating password for email:" << userEmail;
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYEE SET MDP = :newPassword WHERE EMAIL = :email");
    query.bindValue(":newPassword", newPassword);
    query.bindValue(":email", userEmail);

    if (query.exec()) {
        if (query.numRowsAffected() > 0) {
            QMessageBox::information(this, "Success", "Password updated successfully!");
            qDebug() << "Password updated successfully for:" << userEmail;

            // Clear all fields and return to login page
            ui->emailInput_2->clear();
            ui->passwordInput_2->clear();
            ui->rcpt->clear();
            ui->codebt->clear();
            ui->pas1->clear();
            ui->pas2->clear();
            ui->stackedWidget->setCurrentIndex(0);

            // Reset state
            sentVerificationCode.clear();
            userEmail.clear();
        } else {
            QMessageBox::warning(this, "Error", "No user found with the specified email address.");
            qDebug() << "No rows affected - email not found:" << userEmail;
        }
    } else {
        QString error = query.lastError().text();
        QMessageBox::warning(this, "Error", "Failed to update password: " + error);
        qDebug() << "Database error:" << error;
    }
}
void LoginDialog::on_backToLogin1_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->rcpt->clear();
}

void LoginDialog::on_backToLogin2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->codebt->clear();
}

void LoginDialog::on_backToLogin3_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->pas1->clear();
    ui->pas2->clear();
}
