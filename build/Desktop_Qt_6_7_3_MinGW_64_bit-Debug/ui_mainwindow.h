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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *centralwidget_2;
    QGroupBox *groupBox_16;
    QLabel *label_57;
    QLabel *label_58;
    QLabel *label_59;
    QLabel *label_60;
    QLabel *label_61;
    QLabel *label_63;
    QLabel *label_64;
    QComboBox *poste_E;
    QPushButton *btn_ajout_E;
    QPushButton *btn_reset_E;
    QLabel *label_65;
    QLabel *label_67;
    QComboBox *dispo_E;
    QLabel *label_66;
    QLineEdit *cin_E;
    QLineEdit *nom_E;
    QLineEdit *prenom_E_2;
    QLineEdit *salaire_E;
    QLineEdit *tel_E;
    QLineEdit *email_E;
    QLineEdit *mdp_E;
    QGroupBox *groupBox_17;
    QWidget *horizontalLayoutWidget_7;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *candidat_7;
    QPushButton *planning_7;
    QPushButton *employee_7;
    QPushButton *vehicule_7;
    QPushButton *examen_7;
    QGroupBox *groupBox_18;
    QTableWidget *tab_em;
    QLabel *label_68;
    QComboBox *tri_6;
    QTextEdit *recherche_7;
    QLabel *label_69;
    QPushButton *modif_E;
    QPushButton *sup_E;
    QPushButton *pushButton_20;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1477, 782);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget_2 = new QWidget(centralwidget);
        centralwidget_2->setObjectName("centralwidget_2");
        centralwidget_2->setGeometry(QRect(0, 0, 1501, 871));
        groupBox_16 = new QGroupBox(centralwidget_2);
        groupBox_16->setObjectName("groupBox_16");
        groupBox_16->setGeometry(QRect(10, 10, 431, 851));
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
        label_61 = new QLabel(groupBox_16);
        label_61->setObjectName("label_61");
        label_61->setGeometry(QRect(20, 300, 141, 31));
        label_61->setFont(font1);
        label_61->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_63 = new QLabel(groupBox_16);
        label_63->setObjectName("label_63");
        label_63->setGeometry(QRect(20, 412, 141, 31));
        label_63->setFont(font1);
        label_63->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_64 = new QLabel(groupBox_16);
        label_64->setObjectName("label_64");
        label_64->setGeometry(QRect(20, 510, 181, 31));
        label_64->setFont(font1);
        label_64->setStyleSheet(QString::fromUtf8("color:#002157"));
        poste_E = new QComboBox(groupBox_16);
        poste_E->addItem(QString());
        poste_E->addItem(QString());
        poste_E->setObjectName("poste_E");
        poste_E->setGeometry(QRect(210, 510, 211, 41));
        poste_E->setFont(font1);
        poste_E->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
"color:black;\n"
""));
        btn_ajout_E = new QPushButton(groupBox_16);
        btn_ajout_E->setObjectName("btn_ajout_E");
        btn_ajout_E->setGeometry(QRect(30, 640, 171, 61));
        QFont font2;
        font2.setPointSize(16);
        btn_ajout_E->setFont(font2);
        btn_ajout_E->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        btn_reset_E = new QPushButton(groupBox_16);
        btn_reset_E->setObjectName("btn_reset_E");
        btn_reset_E->setGeometry(QRect(220, 640, 171, 61));
        btn_reset_E->setFont(font2);
        btn_reset_E->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        label_65 = new QLabel(groupBox_16);
        label_65->setObjectName("label_65");
        label_65->setGeometry(QRect(19, 464, 141, 31));
        label_65->setFont(font1);
        label_65->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_67 = new QLabel(groupBox_16);
        label_67->setObjectName("label_67");
        label_67->setGeometry(QRect(20, 360, 141, 31));
        label_67->setFont(font1);
        label_67->setStyleSheet(QString::fromUtf8("color:#002157"));
        dispo_E = new QComboBox(groupBox_16);
        dispo_E->addItem(QString());
        dispo_E->addItem(QString());
        dispo_E->setObjectName("dispo_E");
        dispo_E->setGeometry(QRect(210, 290, 211, 41));
        dispo_E->setFont(font1);
        dispo_E->setStyleSheet(QString::fromUtf8("color:black;\n"
"background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
""));
        label_66 = new QLabel(groupBox_16);
        label_66->setObjectName("label_66");
        label_66->setGeometry(QRect(20, 560, 141, 31));
        label_66->setFont(font1);
        label_66->setStyleSheet(QString::fromUtf8("color:#002157"));
        cin_E = new QLineEdit(groupBox_16);
        cin_E->setObjectName("cin_E");
        cin_E->setGeometry(QRect(210, 140, 211, 41));
        cin_E->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"color:black;\n"
"border:2px solid #ffce00;\n"
""));
        nom_E = new QLineEdit(groupBox_16);
        nom_E->setObjectName("nom_E");
        nom_E->setGeometry(QRect(210, 190, 211, 41));
        nom_E->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
"color:black;\n"
""));
        prenom_E_2 = new QLineEdit(groupBox_16);
        prenom_E_2->setObjectName("prenom_E_2");
        prenom_E_2->setGeometry(QRect(210, 240, 211, 41));
        prenom_E_2->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
"color:black;\n"
""));
        salaire_E = new QLineEdit(groupBox_16);
        salaire_E->setObjectName("salaire_E");
        salaire_E->setGeometry(QRect(210, 360, 211, 41));
        salaire_E->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
"color:black;\n"
""));
        tel_E = new QLineEdit(groupBox_16);
        tel_E->setObjectName("tel_E");
        tel_E->setGeometry(QRect(210, 410, 211, 41));
        tel_E->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
"color:black;\n"
""));
        email_E = new QLineEdit(groupBox_16);
        email_E->setObjectName("email_E");
        email_E->setGeometry(QRect(210, 460, 211, 41));
        email_E->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"color:black;\n"
"border:2px solid #ffce00;\n"
""));
        mdp_E = new QLineEdit(groupBox_16);
        mdp_E->setObjectName("mdp_E");
        mdp_E->setGeometry(QRect(210, 560, 211, 41));
        mdp_E->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
"color:black;\n"
""));
        groupBox_17 = new QGroupBox(centralwidget_2);
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
        QFont font3;
        font3.setPointSize(11);
        font3.setBold(true);
        candidat_7->setFont(font3);
        candidat_7->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout_7->addWidget(candidat_7);

        planning_7 = new QPushButton(horizontalLayoutWidget_7);
        planning_7->setObjectName("planning_7");
        planning_7->setFont(font3);
        planning_7->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout_7->addWidget(planning_7);

        employee_7 = new QPushButton(horizontalLayoutWidget_7);
        employee_7->setObjectName("employee_7");
        employee_7->setFont(font3);
        employee_7->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout_7->addWidget(employee_7);

        vehicule_7 = new QPushButton(horizontalLayoutWidget_7);
        vehicule_7->setObjectName("vehicule_7");
        vehicule_7->setFont(font3);
        vehicule_7->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout_7->addWidget(vehicule_7);

        examen_7 = new QPushButton(horizontalLayoutWidget_7);
        examen_7->setObjectName("examen_7");
        examen_7->setFont(font3);
        examen_7->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout_7->addWidget(examen_7);

        groupBox_18 = new QGroupBox(centralwidget_2);
        groupBox_18->setObjectName("groupBox_18");
        groupBox_18->setGeometry(QRect(460, 50, 1041, 801));
        tab_em = new QTableWidget(groupBox_18);
        if (tab_em->columnCount() < 8)
            tab_em->setColumnCount(8);
        QFont font4;
        font4.setPointSize(12);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font4);
        tab_em->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font4);
        tab_em->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setFont(font4);
        tab_em->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setFont(font4);
        tab_em->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setFont(font4);
        tab_em->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setFont(font4);
        tab_em->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        __qtablewidgetitem6->setFont(font4);
        tab_em->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        __qtablewidgetitem7->setFont(font4);
        tab_em->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        if (tab_em->rowCount() < 15)
            tab_em->setRowCount(15);
        tab_em->setObjectName("tab_em");
        tab_em->setGeometry(QRect(10, 93, 1011, 371));
        tab_em->setFont(font1);
        tab_em->setShowGrid(true);
        tab_em->setWordWrap(true);
        tab_em->setRowCount(15);
        tab_em->horizontalHeader()->setVisible(true);
        tab_em->horizontalHeader()->setCascadingSectionResizes(false);
        tab_em->horizontalHeader()->setMinimumSectionSize(40);
        tab_em->horizontalHeader()->setDefaultSectionSize(125);
        tab_em->horizontalHeader()->setHighlightSections(true);
        tab_em->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tab_em->horizontalHeader()->setStretchLastSection(true);
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
        modif_E = new QPushButton(groupBox_18);
        modif_E->setObjectName("modif_E");
        modif_E->setGeometry(QRect(830, 30, 41, 31));
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::MailMessageNew));
        modif_E->setIcon(icon);
        sup_E = new QPushButton(groupBox_18);
        sup_E->setObjectName("sup_E");
        sup_E->setGeometry(QRect(780, 30, 41, 31));
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::EditDelete));
        sup_E->setIcon(icon1);
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
        menubar->setGeometry(QRect(0, 0, 1477, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox_16->setTitle(QString());
        label_57->setText(QCoreApplication::translate("MainWindow", "Ajouter Employee", nullptr));
        label_58->setText(QCoreApplication::translate("MainWindow", "CIN :", nullptr));
        label_59->setText(QCoreApplication::translate("MainWindow", "Nom :", nullptr));
        label_60->setText(QCoreApplication::translate("MainWindow", "Pr\303\251nom :", nullptr));
        label_61->setText(QCoreApplication::translate("MainWindow", "Disponibilit\303\251:", nullptr));
        label_63->setText(QCoreApplication::translate("MainWindow", "T\303\251lephone :", nullptr));
        label_64->setText(QCoreApplication::translate("MainWindow", "Poste:", nullptr));
        poste_E->setItemText(0, QCoreApplication::translate("MainWindow", "manager", nullptr));
        poste_E->setItemText(1, QCoreApplication::translate("MainWindow", "ouvrier", nullptr));

        btn_ajout_E->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        btn_reset_E->setText(QCoreApplication::translate("MainWindow", "Annuler", nullptr));
        label_65->setText(QCoreApplication::translate("MainWindow", "Email:", nullptr));
        label_67->setText(QCoreApplication::translate("MainWindow", "salaire:", nullptr));
        dispo_E->setItemText(0, QCoreApplication::translate("MainWindow", "0", nullptr));
        dispo_E->setItemText(1, QCoreApplication::translate("MainWindow", "1", nullptr));

        label_66->setText(QCoreApplication::translate("MainWindow", "Mdp:", nullptr));
        groupBox_17->setTitle(QString());
        candidat_7->setText(QCoreApplication::translate("MainWindow", "Candidat", nullptr));
        planning_7->setText(QCoreApplication::translate("MainWindow", "planning", nullptr));
        employee_7->setText(QCoreApplication::translate("MainWindow", "Employee", nullptr));
        vehicule_7->setText(QCoreApplication::translate("MainWindow", "vehicule", nullptr));
        examen_7->setText(QCoreApplication::translate("MainWindow", "Examen", nullptr));
        groupBox_18->setTitle(QString());
        QTableWidgetItem *___qtablewidgetitem = tab_em->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "CIN", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tab_em->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tab_em->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Pr\303\251nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tab_em->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Sexe", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tab_em->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "date de naissance", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tab_em->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "T\303\251lephone", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tab_em->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tab_em->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "salaire", nullptr));
        label_68->setText(QCoreApplication::translate("MainWindow", "Tri par : ", nullptr));
        tri_6->setItemText(0, QCoreApplication::translate("MainWindow", "--------------", nullptr));
        tri_6->setItemText(1, QCoreApplication::translate("MainWindow", "nom A \303\240 Z ", nullptr));
        tri_6->setItemText(2, QCoreApplication::translate("MainWindow", "nom Z \303\240 A", nullptr));
        tri_6->setItemText(3, QCoreApplication::translate("MainWindow", "age croissant", nullptr));
        tri_6->setItemText(4, QCoreApplication::translate("MainWindow", "age  d\303\251croissant", nullptr));

        label_69->setText(QCoreApplication::translate("MainWindow", "chercher", nullptr));
        modif_E->setText(QString());
        sup_E->setText(QString());
        pushButton_20->setText(QCoreApplication::translate("MainWindow", "Exporter", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
