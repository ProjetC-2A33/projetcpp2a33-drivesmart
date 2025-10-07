#include "Login.h"
#include "ui_Login.h"
#include "mainwindow.h"
#include <QDebug>
#include <QPixmap>
#include <QInputDialog>

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
    // Ask for email
    bool ok;
    QString email = QInputDialog::getText(this, "Forgot Password",
                                          "Enter your email address:",
                                          QLineEdit::Normal,
                                          "", &ok);

    if (ok && !email.isEmpty()) {
        // Simple password recovery logic
        // In a real app, you would check against a database
        QString recoveredPassword;

        if (email == "admin@drivesmart.com") {
            recoveredPassword = "admin";
        } else if (email == "user@example.com") {
            recoveredPassword = "user";
        } else if (email == "test@test.com") {
            recoveredPassword = "test";
        } else {
            // For any other email, generate a simple password
            recoveredPassword = "password123";
        }

        // Show the recovered password
        QMessageBox::information(this, "Password Recovery",
                                 QString("Email: %1\nYour password is: %2")
                                     .arg(email)
                                     .arg(recoveredPassword));
    } else if (ok && email.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter your email address.");
    }
}
