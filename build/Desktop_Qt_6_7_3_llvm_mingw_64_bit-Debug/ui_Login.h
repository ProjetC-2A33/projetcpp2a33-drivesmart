/********************************************************************************
** Form generated from reading UI file 'Login.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *logoLayout;
    QLabel *logoLabel;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *formLayout;
    QLabel *emailLabel;
    QLineEdit *emailInput;
    QLabel *passwordLabel;
    QLineEdit *passwordInput;
    QPushButton *signInButton;
    QPushButton *signUpButton;
    QLabel *orLabel;
    QPushButton *forgotpButton;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName("Login");
        Login->resize(400, 600);
        Login->setStyleSheet(QString::fromUtf8("QWidget#Login {\n"
"    background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,\n"
"                                stop: 0 #1e3c72, stop: 0.5 #2a5298, stop: 1 #1e3c72);\n"
"}\n"
"\n"
"QLabel {\n"
"    background: transparent;\n"
"}\n"
"\n"
"QLabel#titleLabel {\n"
"    color: white;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    margin-top: 10px;\n"
"}\n"
"\n"
"QLabel#subtitleLabel {\n"
"    color: rgba(255, 255, 255, 0.8);\n"
"    font-size: 14px;\n"
"    margin-bottom: 20px;\n"
"}\n"
"\n"
"QPushButton#signInButton {\n"
"    background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0,\n"
"                                stop: 0 #1e3c72, stop: 1 #2a5298);\n"
"    color: white;\n"
"    border: 2px solid #FFD700;\n"
"    border-radius: 25px;\n"
"    padding: 12px;\n"
"    font-size: 16px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton#signInButton:hover {\n"
"    background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0,\n"
"                                stop: 0 #FFD700, stop: 1 #FFA500);\n"
"    c"
                        "olor: #1e3c72;\n"
"    border: 2px solid #FFA500;\n"
"}\n"
"\n"
"QPushButton#signInButton:pressed {\n"
"    background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0,\n"
"                                stop: 0 #FFA500, stop: 1 #FF8C00);\n"
"    color: white;\n"
"}\n"
"\n"
"QPushButton#signUpButton {\n"
"    background: rgba(255, 255, 255, 0.1);\n"
"    color: white;\n"
"    border: 2px solid rgba(255, 255, 255, 0.3);\n"
"    border-radius: 25px;\n"
"    padding: 12px;\n"
"    font-size: 16px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton#signUpButton:hover {\n"
"    background: rgba(255, 215, 0, 0.2);\n"
"    border: 2px solid #FFD700;\n"
"    color: #FFD700;\n"
"}\n"
"\n"
"QPushButton#signUpButton:pressed {\n"
"    background: rgba(255, 215, 0, 0.3);\n"
"    border: 2px solid #FFA500;\n"
"}\n"
"\n"
"QPushButton#forgotpButton {\n"
"    background: transparent;\n"
"    color: #FFD700;\n"
"    border: 2px solid transparent;\n"
"    border-radius: 25px;\n"
"    padding: 8px;\n"
"    font-size: 14px;\n"
"   "
                        " font-weight: bold;\n"
"    text-decoration: underline;\n"
"}\n"
"\n"
"QPushButton#forgotpButton:hover {\n"
"    background: rgba(255, 215, 0, 0.1);\n"
"    color: #FFA500;\n"
"    border: 2px solid rgba(255, 215, 0, 0.3);\n"
"    text-decoration: none;\n"
"}\n"
"\n"
"QPushButton#forgotpButton:pressed {\n"
"    background: rgba(255, 215, 0, 0.2);\n"
"    color: #FF8C00;\n"
"    border: 2px solid rgba(255, 215, 0, 0.5);\n"
"}\n"
"\n"
"QLineEdit {\n"
"    background: rgba(255, 255, 255, 0.15);\n"
"    border: 2px solid rgba(255, 255, 255, 0.3);\n"
"    border-radius: 25px;\n"
"    padding: 12px 20px;\n"
"    color: white;\n"
"    font-size: 14px;\n"
"    selection-background-color: #FFD700;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 2px solid #FFD700;\n"
"    background: rgba(255, 255, 255, 0.2);\n"
"}\n"
"\n"
"QLineEdit::placeholder {\n"
"    color: rgba(255, 255, 255, 0.6);\n"
"}\n"
"\n"
"QLineEdit:hover {\n"
"    border: 2px solid rgba(255, 215, 0, 0.5);\n"
"}\n"
"\n"
"QLabel#logoLabel {\n"
"    backgro"
                        "und: transparent;\n"
"    border: none;\n"
"}"));
        verticalLayout = new QVBoxLayout(Login);
        verticalLayout->setObjectName("verticalLayout");
        logoLayout = new QHBoxLayout();
        logoLayout->setObjectName("logoLayout");
        logoLabel = new QLabel(Login);
        logoLabel->setObjectName("logoLabel");
        logoLabel->setMinimumSize(QSize(80, 80));
        logoLabel->setMaximumSize(QSize(60, 60));
        logoLabel->setPixmap(QPixmap(QString::fromUtf8("images/logo.png")));
        logoLabel->setScaledContents(true);

        logoLayout->addWidget(logoLabel);

        horizontalSpacer = new QSpacerItem(30, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        logoLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(logoLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        titleLabel = new QLabel(Login);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(QString::fromUtf8("font: 700 italic 25pt \"Segoe UI\";"));
        titleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(titleLabel);

        subtitleLabel = new QLabel(Login);
        subtitleLabel->setObjectName("subtitleLabel");
        subtitleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(subtitleLabel);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        formLayout = new QVBoxLayout();
        formLayout->setSpacing(15);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(30, -1, 30, -1);
        emailLabel = new QLabel(Login);
        emailLabel->setObjectName("emailLabel");
        emailLabel->setStyleSheet(QString::fromUtf8("color: #FFD700; font-size: 12px; font-weight: bold; padding-left: 20px;"));

        formLayout->addWidget(emailLabel);

        emailInput = new QLineEdit(Login);
        emailInput->setObjectName("emailInput");

        formLayout->addWidget(emailInput);

        passwordLabel = new QLabel(Login);
        passwordLabel->setObjectName("passwordLabel");
        passwordLabel->setStyleSheet(QString::fromUtf8("color: #FFD700; font-size: 12px; font-weight: bold; padding-left: 20px;"));

        formLayout->addWidget(passwordLabel);

        passwordInput = new QLineEdit(Login);
        passwordInput->setObjectName("passwordInput");
        passwordInput->setEchoMode(QLineEdit::EchoMode::Password);

        formLayout->addWidget(passwordInput);

        signInButton = new QPushButton(Login);
        signInButton->setObjectName("signInButton");

        formLayout->addWidget(signInButton);

        signUpButton = new QPushButton(Login);
        signUpButton->setObjectName("signUpButton");

        formLayout->addWidget(signUpButton);

        orLabel = new QLabel(Login);
        orLabel->setObjectName("orLabel");
        orLabel->setStyleSheet(QString::fromUtf8("color: rgba(255, 215, 0, 0.7); font-size: 12px;"));
        orLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        formLayout->addWidget(orLabel);

        forgotpButton = new QPushButton(Login);
        forgotpButton->setObjectName("forgotpButton");

        formLayout->addWidget(forgotpButton);


        verticalLayout->addLayout(formLayout);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QWidget *Login)
    {
        Login->setWindowTitle(QCoreApplication::translate("Login", "Login", nullptr));
        logoLabel->setText(QString());
        titleLabel->setText(QCoreApplication::translate("Login", "Welcome Back", nullptr));
        subtitleLabel->setText(QCoreApplication::translate("Login", "Sign in to continue your journey", nullptr));
        emailLabel->setText(QCoreApplication::translate("Login", "EMAIL", nullptr));
        emailInput->setPlaceholderText(QCoreApplication::translate("Login", "Enter your email", nullptr));
        passwordLabel->setText(QCoreApplication::translate("Login", "PASSWORD", nullptr));
        passwordInput->setPlaceholderText(QCoreApplication::translate("Login", "Enter your password", nullptr));
        signInButton->setText(QCoreApplication::translate("Login", "Sign In", nullptr));
        signUpButton->setText(QCoreApplication::translate("Login", "Create Account", nullptr));
        orLabel->setText(QCoreApplication::translate("Login", "or", nullptr));
        forgotpButton->setText(QCoreApplication::translate("Login", "Forgot Password?", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
