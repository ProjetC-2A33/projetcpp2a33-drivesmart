/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *groupBox_16;
    QLabel *label_57;
    QLabel *label_58;
    QLabel *label_59;
    QLabel *label_60;
    QTextEdit *cin_7;
    QTextEdit *prenom_4;
    QTextEdit *nom_5;
    QRadioButton *homme_6;
    QRadioButton *femme_6;
    QLabel *label_61;
    QLabel *label_62;
    QLabel *label_63;
    QTextEdit *tel_7;
    QLabel *label_64;
    QComboBox *type_6;
    QPushButton *btn_ajout_6;
    QPushButton *btn_reset_6;
    QLabel *label_65;
    QTextEdit *tel_8;
    QLabel *label_67;
    QTextEdit *tel_10;
    QDateEdit *dateEdit_6;
    QGroupBox *groupBox_17;
    QWidget *horizontalLayoutWidget_7;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *candidat_7;
    QPushButton *planning_7;
    QPushButton *employee_7;
    QPushButton *vehicule_7;
    QPushButton *examen_7;
    QGroupBox *groupBox_18;
    QTableWidget *tab_7;
    QLabel *label_68;
    QComboBox *tri_6;
    QTextEdit *recherche_7;
    QLabel *label_69;
    QPushButton *pushButton_19;
    QPushButton *pushButton_5;
    QPushButton *pushButton_20;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1557, 981);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        groupBox_16 = new QGroupBox(centralwidget);
        groupBox_16->setObjectName("groupBox_16");
        groupBox_16->setGeometry(QRect(10, 10, 421, 851));
        groupBox_16->setStyleSheet(QString::fromUtf8("border-radius:50px;\n"
"background-color:#e3e2de;"));
        label_57 = new QLabel(groupBox_16);
        label_57->setObjectName("label_57");
        label_57->setGeometry(QRect(30, 10, 331, 61));
        QFont font;
        font.setPointSize(24);
        label_57->setFont(font);
        label_57->setStyleSheet(QString::fromUtf8("color:#002157\n"
""));
        label_58 = new QLabel(groupBox_16);
        label_58->setObjectName("label_58");
        label_58->setGeometry(QRect(20, 140, 121, 31));
        QFont font1;
        font1.setPointSize(14);
        label_58->setFont(font1);
        label_58->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_59 = new QLabel(groupBox_16);
        label_59->setObjectName("label_59");
        label_59->setGeometry(QRect(20, 190, 121, 31));
        label_59->setFont(font1);
        label_59->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_60 = new QLabel(groupBox_16);
        label_60->setObjectName("label_60");
        label_60->setGeometry(QRect(20, 240, 121, 31));
        label_60->setFont(font1);
        label_60->setStyleSheet(QString::fromUtf8("color:#002157"));
        cin_7 = new QTextEdit(groupBox_16);
        cin_7->setObjectName("cin_7");
        cin_7->setGeometry(QRect(200, 140, 211, 41));
        cin_7->setFont(font1);
        cin_7->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
""));
        cin_7->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        cin_7->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        cin_7->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        prenom_4 = new QTextEdit(groupBox_16);
        prenom_4->setObjectName("prenom_4");
        prenom_4->setGeometry(QRect(200, 240, 211, 41));
        prenom_4->setFont(font1);
        prenom_4->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;"));
        nom_5 = new QTextEdit(groupBox_16);
        nom_5->setObjectName("nom_5");
        nom_5->setGeometry(QRect(200, 190, 211, 41));
        nom_5->setFont(font1);
        nom_5->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;"));
        homme_6 = new QRadioButton(groupBox_16);
        homme_6->setObjectName("homme_6");
        homme_6->setGeometry(QRect(160, 300, 121, 31));
        homme_6->setFont(font1);
        femme_6 = new QRadioButton(groupBox_16);
        femme_6->setObjectName("femme_6");
        femme_6->setGeometry(QRect(290, 300, 121, 30));
        femme_6->setFont(font1);
        label_61 = new QLabel(groupBox_16);
        label_61->setObjectName("label_61");
        label_61->setGeometry(QRect(20, 300, 121, 31));
        label_61->setFont(font1);
        label_61->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_62 = new QLabel(groupBox_16);
        label_62->setObjectName("label_62");
        label_62->setGeometry(QRect(20, 350, 211, 31));
        label_62->setFont(font1);
        label_62->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_63 = new QLabel(groupBox_16);
        label_63->setObjectName("label_63");
        label_63->setGeometry(QRect(20, 412, 141, 31));
        label_63->setFont(font1);
        label_63->setStyleSheet(QString::fromUtf8("color:#002157"));
        tel_7 = new QTextEdit(groupBox_16);
        tel_7->setObjectName("tel_7");
        tel_7->setGeometry(QRect(200, 408, 211, 41));
        tel_7->setFont(font1);
        tel_7->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;"));
        label_64 = new QLabel(groupBox_16);
        label_64->setObjectName("label_64");
        label_64->setGeometry(QRect(20, 510, 181, 31));
        label_64->setFont(font1);
        label_64->setStyleSheet(QString::fromUtf8("color:#002157"));
        type_6 = new QComboBox(groupBox_16);
        type_6->addItem(QString());
        type_6->addItem(QString());
        type_6->addItem(QString());
        type_6->addItem(QString());
        type_6->setObjectName("type_6");
        type_6->setGeometry(QRect(200, 510, 211, 41));
        type_6->setFont(font1);
        type_6->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
""));
        btn_ajout_6 = new QPushButton(groupBox_16);
        btn_ajout_6->setObjectName("btn_ajout_6");
        btn_ajout_6->setGeometry(QRect(30, 640, 171, 61));
        QFont font2;
        font2.setPointSize(16);
        btn_ajout_6->setFont(font2);
        btn_ajout_6->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        btn_reset_6 = new QPushButton(groupBox_16);
        btn_reset_6->setObjectName("btn_reset_6");
        btn_reset_6->setGeometry(QRect(220, 640, 171, 61));
        btn_reset_6->setFont(font2);
        btn_reset_6->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        label_65 = new QLabel(groupBox_16);
        label_65->setObjectName("label_65");
        label_65->setGeometry(QRect(19, 464, 141, 31));
        label_65->setFont(font1);
        label_65->setStyleSheet(QString::fromUtf8("color:#002157"));
        tel_8 = new QTextEdit(groupBox_16);
        tel_8->setObjectName("tel_8");
        tel_8->setGeometry(QRect(199, 460, 211, 41));
        tel_8->setFont(font1);
        tel_8->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;"));
        label_67 = new QLabel(groupBox_16);
        label_67->setObjectName("label_67");
        label_67->setGeometry(QRect(20, 574, 141, 31));
        label_67->setFont(font1);
        label_67->setStyleSheet(QString::fromUtf8("color:#002157"));
        tel_10 = new QTextEdit(groupBox_16);
        tel_10->setObjectName("tel_10");
        tel_10->setGeometry(QRect(200, 570, 211, 41));
        tel_10->setFont(font1);
        tel_10->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;"));
        dateEdit_6 = new QDateEdit(groupBox_16);
        dateEdit_6->setObjectName("dateEdit_6");
        dateEdit_6->setGeometry(QRect(240, 354, 161, 31));
        QFont font3;
        font3.setPointSize(12);
        dateEdit_6->setFont(font3);
        groupBox_17 = new QGroupBox(centralwidget);
        groupBox_17->setObjectName("groupBox_17");
        groupBox_17->setGeometry(QRect(450, 10, 1041, 41));
        groupBox_17->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;\n"
"border-radius:10px;"));
        horizontalLayoutWidget_7 = new QWidget(groupBox_17);
        horizontalLayoutWidget_7->setObjectName("horizontalLayoutWidget_7");
        horizontalLayoutWidget_7->setGeometry(QRect(10, 3, 1021, 38));
        horizontalLayout_7 = new QHBoxLayout(horizontalLayoutWidget_7);
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        candidat_7 = new QPushButton(horizontalLayoutWidget_7);
        candidat_7->setObjectName("candidat_7");
        QFont font4;
        font4.setPointSize(11);
        font4.setBold(true);
        candidat_7->setFont(font4);
        candidat_7->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout_7->addWidget(candidat_7);

        planning_7 = new QPushButton(horizontalLayoutWidget_7);
        planning_7->setObjectName("planning_7");
        planning_7->setFont(font4);
        planning_7->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout_7->addWidget(planning_7);

        employee_7 = new QPushButton(horizontalLayoutWidget_7);
        employee_7->setObjectName("employee_7");
        employee_7->setFont(font4);
        employee_7->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout_7->addWidget(employee_7);

        vehicule_7 = new QPushButton(horizontalLayoutWidget_7);
        vehicule_7->setObjectName("vehicule_7");
        vehicule_7->setFont(font4);
        vehicule_7->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout_7->addWidget(vehicule_7);

        examen_7 = new QPushButton(horizontalLayoutWidget_7);
        examen_7->setObjectName("examen_7");
        examen_7->setFont(font4);
        examen_7->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout_7->addWidget(examen_7);

        groupBox_18 = new QGroupBox(centralwidget);
        groupBox_18->setObjectName("groupBox_18");
        groupBox_18->setGeometry(QRect(450, 55, 1041, 801));
        tab_7 = new QTableWidget(groupBox_18);
        if (tab_7->columnCount() < 8)
            tab_7->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font3);
        tab_7->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font3);
        tab_7->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setFont(font3);
        tab_7->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setFont(font3);
        tab_7->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setFont(font3);
        tab_7->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setFont(font3);
        tab_7->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        __qtablewidgetitem6->setFont(font3);
        tab_7->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        __qtablewidgetitem7->setFont(font3);
        tab_7->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        if (tab_7->rowCount() < 15)
            tab_7->setRowCount(15);
        tab_7->setObjectName("tab_7");
        tab_7->setGeometry(QRect(10, 93, 1011, 371));
        tab_7->setFont(font1);
        tab_7->setShowGrid(true);
        tab_7->setWordWrap(true);
        tab_7->setRowCount(15);
        tab_7->horizontalHeader()->setVisible(true);
        tab_7->horizontalHeader()->setCascadingSectionResizes(false);
        tab_7->horizontalHeader()->setMinimumSectionSize(40);
        tab_7->horizontalHeader()->setDefaultSectionSize(125);
        tab_7->horizontalHeader()->setHighlightSections(true);
        tab_7->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tab_7->horizontalHeader()->setStretchLastSection(true);
        label_68 = new QLabel(groupBox_18);
        label_68->setObjectName("label_68");
        label_68->setGeometry(QRect(10, 25, 91, 31));
        label_68->setFont(font1);
        label_68->setStyleSheet(QString::fromUtf8("color:#002157"));
        tri_6 = new QComboBox(groupBox_18);
        tri_6->addItem(QString());
        tri_6->addItem(QString());
        tri_6->addItem(QString());
        tri_6->addItem(QString());
        tri_6->addItem(QString());
        tri_6->setObjectName("tri_6");
        tri_6->setGeometry(QRect(95, 20, 211, 41));
        tri_6->setFont(font1);
        tri_6->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
""));
        recherche_7 = new QTextEdit(groupBox_18);
        recherche_7->setObjectName("recherche_7");
        recherche_7->setGeometry(QRect(450, 20, 301, 41));
        recherche_7->setFont(font1);
        recherche_7->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
""));
        recherche_7->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        recherche_7->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        recherche_7->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        label_69 = new QLabel(groupBox_18);
        label_69->setObjectName("label_69");
        label_69->setGeometry(QRect(350, 20, 101, 31));
        label_69->setFont(font1);
        label_69->setStyleSheet(QString::fromUtf8("color:#002157"));
        pushButton_19 = new QPushButton(groupBox_18);
        pushButton_19->setObjectName("pushButton_19");
        pushButton_19->setGeometry(QRect(830, 30, 41, 31));
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::MailMessageNew));
        pushButton_19->setIcon(icon);
        pushButton_5 = new QPushButton(groupBox_18);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(780, 30, 41, 31));
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::EditDelete));
        pushButton_5->setIcon(icon1);
        pushButton_20 = new QPushButton(groupBox_18);
        pushButton_20->setObjectName("pushButton_20");
        pushButton_20->setGeometry(QRect(890, 20, 141, 41));
        pushButton_20->setFont(font1);
        pushButton_20->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        QIcon icon2(QIcon::fromTheme(QIcon::ThemeIcon::WindowNew));
        pushButton_20->setIcon(icon2);
        pushButton_20->setIconSize(QSize(20, 20));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1557, 30));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "DriveSmart - Main Application", nullptr));
        groupBox_16->setTitle(QString());
        label_57->setText(QCoreApplication::translate("MainWindow", "Ajouter Employee", nullptr));
        label_58->setText(QCoreApplication::translate("MainWindow", "CIN :", nullptr));
        label_59->setText(QCoreApplication::translate("MainWindow", "Nom :", nullptr));
        label_60->setText(QCoreApplication::translate("MainWindow", "Pr\303\251nom :", nullptr));
        homme_6->setText(QCoreApplication::translate("MainWindow", "homme", nullptr));
        femme_6->setText(QCoreApplication::translate("MainWindow", "femme", nullptr));
        label_61->setText(QCoreApplication::translate("MainWindow", "Sexe :", nullptr));
        label_62->setText(QCoreApplication::translate("MainWindow", "date de naissance  :", nullptr));
        label_63->setText(QCoreApplication::translate("MainWindow", "T\303\251lephone :", nullptr));
        label_64->setText(QCoreApplication::translate("MainWindow", "Sp\303\251cialit\303\251 :", nullptr));
        type_6->setItemText(0, QCoreApplication::translate("MainWindow", "------------------", nullptr));
        type_6->setItemText(1, QCoreApplication::translate("MainWindow", "voiture", nullptr));
        type_6->setItemText(2, QCoreApplication::translate("MainWindow", "moto", nullptr));
        type_6->setItemText(3, QCoreApplication::translate("MainWindow", "poids lourd", nullptr));

        btn_ajout_6->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        btn_reset_6->setText(QCoreApplication::translate("MainWindow", "Annuler", nullptr));
        label_65->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        label_67->setText(QCoreApplication::translate("MainWindow", "salaire", nullptr));
        groupBox_17->setTitle(QString());
        candidat_7->setText(QCoreApplication::translate("MainWindow", "Candidat", nullptr));
        planning_7->setText(QCoreApplication::translate("MainWindow", "planning", nullptr));
        employee_7->setText(QCoreApplication::translate("MainWindow", "Employee", nullptr));
        vehicule_7->setText(QCoreApplication::translate("MainWindow", "vehicule", nullptr));
        examen_7->setText(QCoreApplication::translate("MainWindow", "Examen", nullptr));
        groupBox_18->setTitle(QString());
        QTableWidgetItem *___qtablewidgetitem = tab_7->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "CIN", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tab_7->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tab_7->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Pr\303\251nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tab_7->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Sexe", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tab_7->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "date de naissance", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tab_7->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "T\303\251lephone", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tab_7->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tab_7->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "salaire", nullptr));
        label_68->setText(QCoreApplication::translate("MainWindow", "Tri par : ", nullptr));
        tri_6->setItemText(0, QCoreApplication::translate("MainWindow", "--------------", nullptr));
        tri_6->setItemText(1, QCoreApplication::translate("MainWindow", "nom A \303\240 Z ", nullptr));
        tri_6->setItemText(2, QCoreApplication::translate("MainWindow", "nom Z \303\240 A", nullptr));
        tri_6->setItemText(3, QCoreApplication::translate("MainWindow", "age croissant", nullptr));
        tri_6->setItemText(4, QCoreApplication::translate("MainWindow", "age  d\303\251croissant", nullptr));

        label_69->setText(QCoreApplication::translate("MainWindow", "chercher", nullptr));
        pushButton_19->setText(QString());
        pushButton_5->setText(QString());
        pushButton_20->setText(QCoreApplication::translate("MainWindow", "Exporter", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
