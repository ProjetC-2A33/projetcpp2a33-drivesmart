#include "Login.h"
#include "ui_Login.h"
#include "mainwindow.h"
#include <QDebug>
#include <QPixmap>
#include <QInputDialog>
#include <QtNetwork>
#include <QTextStream>

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
    , mainWindow(nullptr)
{
    ui->setupUi(this);

    connect(ui->signInButton, &QPushButton::clicked, this, &Login::on_signInButton_clicked);
    connect(ui->forgotpButton, &QPushButton::clicked, this, &Login::on_forgotpButton_clicked);

    setWindowTitle("DriveSmart - Login");
}

Login::~Login()
{
    delete ui;
    // Don't delete mainWindow here, let Qt handle it
}

void Login::on_signInButton_clicked()
{
    QString email = ui->emailInput->text();
    QString password = ui->passwordInput->text();

    if (email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill in all fields");
        return;
    }

    if(mainWindow){
        mainWindow->show();
        mainWindow->activateWindow();
        this->close();
        return;
    }

    // Create and show MainWindow
    mainWindow = new MainWindow();
    mainWindow->show();

    // Close the login window instead of just hiding it
    this->close();
}

void Login::on_forgotpButton_clicked()
{
    bool ok;
    QString email = QInputDialog::getText(this, "Forgot Password",
                                          "Enter your email address:",
                                          QLineEdit::Normal,
                                          "", &ok);

    if (!ok || email.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter your email address.");
        return;
    }

    // Step 1: Simulate "sending" a reset email
    QString resetCode = QString::number(QRandomGenerator::global()->bounded(100000, 999999)); // 6-digit code

    QMessageBox::information(this, "Password Reset",
                             QString("A password reset code has been sent to %1.\n\n(For demo: your code is %2)")
                                 .arg(email)
                                 .arg(resetCode));

    // Step 2: Ask the user to enter the code they "received"
    QString enteredCode = QInputDialog::getText(this, "Enter Reset Code",
                                                "Please enter the 6-digit code you received:",
                                                QLineEdit::Normal, "", &ok);

    if (!ok || enteredCode.isEmpty()) {
        QMessageBox::warning(this, "Error", "Reset cancelled.");
        return;
    }

    if (enteredCode != resetCode) {
        QMessageBox::critical(this, "Invalid Code", "The code you entered is incorrect.");
        return;
    }

    // Step 3: Ask for a new password
    QString newPassword = QInputDialog::getText(this, "Reset Password",
                                                "Enter your new password:",
                                                QLineEdit::Password, "", &ok);

    if (!ok || newPassword.isEmpty()) {
        QMessageBox::warning(this, "Error", "You must enter a new password.");
        return;
    }

    // Step 4: Confirm success (simulation only)
    QMessageBox::information(this, "Success",
                             QString("Your password for %1 has been successfully reset!\n\n(New password: %2)")
                                 .arg(email)
                                 .arg(newPassword));

    // Optionally: save the new password to a local file or variable
    // (For demo, just print it in console)
    qDebug() << "Password for" << email << "reset to:" << newPassword;
}
