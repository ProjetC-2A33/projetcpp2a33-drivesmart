/********************************************************************************
** Form generated from reading UI file 'vehicule.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VEHICULE_H
#define UI_VEHICULE_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Vehicule
{
public:
    QGroupBox *groupBox_10;
    QLabel *label_41;
    QLabel *label_42;
    QLabel *label_43;
    QLabel *label_44;
    QTextEdit *ModeleLineEdit_2;
    QRadioButton *radioButton_4;
    QRadioButton *radioButton_5;
    QLabel *DispoLineEdit_2;
    QLabel *DateMLineEdit_2;
    QLabel *TypeEnergieLineEdit_2;
    QComboBox *comboBox_2;
    QPushButton *AjouterButton_2;
    QPushButton *AnnulerButton_2;
    QTextEdit *MatriculeLineEdit_2;
    QTextEdit *KilomLineEdit_2;
    QRadioButton *radioButton_6;
    QDateEdit *dateEdit_4;
    QGroupBox *groupBox_7;
    QTableWidget *tab_4;
    QLabel *label_31;
    QTextEdit *recherche_4;
    QLabel *label_32;
    QComboBox *TriComboBox_2;
    QPushButton *pushButton_12;
    QPushButton *pushButton_13;
    QPushButton *pushButton_18;
    QGroupBox *groupBox_3;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *candidat_4;
    QPushButton *planning_4;
    QPushButton *employee_4;
    QPushButton *vehicule_4;
    QPushButton *examen_4;
    QPushButton *equipement_4;
    QLabel *label_12;
    QGroupBox *groupBox_4;
    QLabel *label_11;
    QLabel *label_16;
    QComboBox *tri_2;

    void setupUi(QWidget *Vehicule)
    {
        if (Vehicule->objectName().isEmpty())
            Vehicule->setObjectName("Vehicule");
        Vehicule->resize(1557, 876);
        groupBox_10 = new QGroupBox(Vehicule);
        groupBox_10->setObjectName("groupBox_10");
        groupBox_10->setGeometry(QRect(10, 78, 421, 761));
        groupBox_10->setStyleSheet(QString::fromUtf8("border-radius:50px;\n"
"background-color:#e3e2de;"));
        label_41 = new QLabel(groupBox_10);
        label_41->setObjectName("label_41");
        label_41->setGeometry(QRect(30, 10, 341, 61));
        QFont font;
        font.setPointSize(24);
        label_41->setFont(font);
        label_41->setStyleSheet(QString::fromUtf8("color:#002157\n"
""));
        label_42 = new QLabel(groupBox_10);
        label_42->setObjectName("label_42");
        label_42->setGeometry(QRect(20, 150, 121, 31));
        QFont font1;
        font1.setPointSize(14);
        label_42->setFont(font1);
        label_42->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_43 = new QLabel(groupBox_10);
        label_43->setObjectName("label_43");
        label_43->setGeometry(QRect(20, 200, 121, 31));
        label_43->setFont(font1);
        label_43->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_44 = new QLabel(groupBox_10);
        label_44->setObjectName("label_44");
        label_44->setGeometry(QRect(20, 250, 131, 31));
        label_44->setFont(font1);
        label_44->setStyleSheet(QString::fromUtf8("color:#002157"));
        ModeleLineEdit_2 = new QTextEdit(groupBox_10);
        ModeleLineEdit_2->setObjectName("ModeleLineEdit_2");
        ModeleLineEdit_2->setGeometry(QRect(200, 140, 211, 41));
        ModeleLineEdit_2->setFont(font1);
        ModeleLineEdit_2->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
"\n"
"\n"
""));
        ModeleLineEdit_2->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        ModeleLineEdit_2->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        ModeleLineEdit_2->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        radioButton_4 = new QRadioButton(groupBox_10);
        radioButton_4->setObjectName("radioButton_4");
        radioButton_4->setGeometry(QRect(10, 350, 121, 31));
        radioButton_4->setFont(font1);
        radioButton_5 = new QRadioButton(groupBox_10);
        radioButton_5->setObjectName("radioButton_5");
        radioButton_5->setGeometry(QRect(140, 350, 121, 31));
        radioButton_5->setFont(font1);
        DispoLineEdit_2 = new QLabel(groupBox_10);
        DispoLineEdit_2->setObjectName("DispoLineEdit_2");
        DispoLineEdit_2->setGeometry(QRect(20, 310, 141, 31));
        DispoLineEdit_2->setFont(font1);
        DispoLineEdit_2->setStyleSheet(QString::fromUtf8("color:#002157"));
        DateMLineEdit_2 = new QLabel(groupBox_10);
        DateMLineEdit_2->setObjectName("DateMLineEdit_2");
        DateMLineEdit_2->setGeometry(QRect(10, 400, 221, 31));
        DateMLineEdit_2->setFont(font1);
        DateMLineEdit_2->setStyleSheet(QString::fromUtf8("color:#002157"));
        TypeEnergieLineEdit_2 = new QLabel(groupBox_10);
        TypeEnergieLineEdit_2->setObjectName("TypeEnergieLineEdit_2");
        TypeEnergieLineEdit_2->setGeometry(QRect(10, 460, 181, 31));
        TypeEnergieLineEdit_2->setFont(font1);
        TypeEnergieLineEdit_2->setStyleSheet(QString::fromUtf8("color:#002157"));
        comboBox_2 = new QComboBox(groupBox_10);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName("comboBox_2");
        comboBox_2->setGeometry(QRect(190, 460, 211, 41));
        comboBox_2->setFont(font1);
        comboBox_2->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
""));
        AjouterButton_2 = new QPushButton(groupBox_10);
        AjouterButton_2->setObjectName("AjouterButton_2");
        AjouterButton_2->setGeometry(QRect(30, 557, 171, 61));
        QFont font2;
        font2.setPointSize(16);
        AjouterButton_2->setFont(font2);
        AjouterButton_2->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        AnnulerButton_2 = new QPushButton(groupBox_10);
        AnnulerButton_2->setObjectName("AnnulerButton_2");
        AnnulerButton_2->setGeometry(QRect(220, 557, 171, 61));
        AnnulerButton_2->setFont(font2);
        AnnulerButton_2->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        MatriculeLineEdit_2 = new QTextEdit(groupBox_10);
        MatriculeLineEdit_2->setObjectName("MatriculeLineEdit_2");
        MatriculeLineEdit_2->setGeometry(QRect(200, 190, 211, 41));
        MatriculeLineEdit_2->setFont(font1);
        MatriculeLineEdit_2->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
"\n"
"\n"
""));
        MatriculeLineEdit_2->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        MatriculeLineEdit_2->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        MatriculeLineEdit_2->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        KilomLineEdit_2 = new QTextEdit(groupBox_10);
        KilomLineEdit_2->setObjectName("KilomLineEdit_2");
        KilomLineEdit_2->setGeometry(QRect(200, 250, 211, 41));
        KilomLineEdit_2->setFont(font1);
        KilomLineEdit_2->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
"\n"
"\n"
""));
        KilomLineEdit_2->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        KilomLineEdit_2->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        KilomLineEdit_2->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        radioButton_6 = new QRadioButton(groupBox_10);
        radioButton_6->setObjectName("radioButton_6");
        radioButton_6->setGeometry(QRect(270, 350, 131, 31));
        radioButton_6->setFont(font1);
        dateEdit_4 = new QDateEdit(groupBox_10);
        dateEdit_4->setObjectName("dateEdit_4");
        dateEdit_4->setGeometry(QRect(250, 400, 161, 31));
        QFont font3;
        font3.setPointSize(12);
        dateEdit_4->setFont(font3);
        groupBox_7 = new QGroupBox(Vehicule);
        groupBox_7->setObjectName("groupBox_7");
        groupBox_7->setGeometry(QRect(450, 78, 1041, 411));
        tab_4 = new QTableWidget(groupBox_7);
        if (tab_4->columnCount() < 6)
            tab_4->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font3);
        tab_4->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font3);
        tab_4->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setFont(font3);
        tab_4->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setFont(font3);
        tab_4->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setFont(font3);
        tab_4->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setFont(font3);
        tab_4->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        if (tab_4->rowCount() < 15)
            tab_4->setRowCount(15);
        tab_4->setObjectName("tab_4");
        tab_4->setGeometry(QRect(10, 93, 1001, 301));
        tab_4->setFont(font1);
        tab_4->setAutoScrollMargin(20);
        tab_4->setTabKeyNavigation(false);
        tab_4->setProperty("showDropIndicator", QVariant(false));
        tab_4->setDragEnabled(false);
        tab_4->setDragDropOverwriteMode(true);
        tab_4->setAlternatingRowColors(false);
        tab_4->setShowGrid(true);
        tab_4->setWordWrap(true);
        tab_4->setRowCount(15);
        tab_4->horizontalHeader()->setVisible(true);
        tab_4->horizontalHeader()->setCascadingSectionResizes(false);
        tab_4->horizontalHeader()->setMinimumSectionSize(40);
        tab_4->horizontalHeader()->setDefaultSectionSize(140);
        tab_4->horizontalHeader()->setHighlightSections(true);
        tab_4->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tab_4->horizontalHeader()->setStretchLastSection(true);
        label_31 = new QLabel(groupBox_7);
        label_31->setObjectName("label_31");
        label_31->setGeometry(QRect(10, 25, 91, 31));
        label_31->setFont(font1);
        label_31->setStyleSheet(QString::fromUtf8("color:#002157"));
        recherche_4 = new QTextEdit(groupBox_7);
        recherche_4->setObjectName("recherche_4");
        recherche_4->setGeometry(QRect(450, 20, 301, 41));
        recherche_4->setFont(font1);
        recherche_4->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
"\n"
"\n"
""));
        recherche_4->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        recherche_4->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        recherche_4->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        label_32 = new QLabel(groupBox_7);
        label_32->setObjectName("label_32");
        label_32->setGeometry(QRect(350, 20, 101, 31));
        label_32->setFont(font1);
        label_32->setStyleSheet(QString::fromUtf8("color:#002157"));
        TriComboBox_2 = new QComboBox(groupBox_7);
        TriComboBox_2->addItem(QString());
        TriComboBox_2->addItem(QString());
        TriComboBox_2->addItem(QString());
        TriComboBox_2->setObjectName("TriComboBox_2");
        TriComboBox_2->setGeometry(QRect(97, 20, 211, 41));
        TriComboBox_2->setFont(font1);
        TriComboBox_2->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
""));
        pushButton_12 = new QPushButton(groupBox_7);
        pushButton_12->setObjectName("pushButton_12");
        pushButton_12->setGeometry(QRect(780, 30, 41, 31));
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::EditDelete));
        pushButton_12->setIcon(icon);
        pushButton_13 = new QPushButton(groupBox_7);
        pushButton_13->setObjectName("pushButton_13");
        pushButton_13->setGeometry(QRect(830, 30, 41, 31));
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::MailMessageNew));
        pushButton_13->setIcon(icon1);
        pushButton_18 = new QPushButton(groupBox_7);
        pushButton_18->setObjectName("pushButton_18");
        pushButton_18->setGeometry(QRect(890, 20, 141, 41));
        pushButton_18->setFont(font1);
        pushButton_18->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        QIcon icon2(QIcon::fromTheme(QIcon::ThemeIcon::WindowNew));
        pushButton_18->setIcon(icon2);
        pushButton_18->setIconSize(QSize(20, 20));
        groupBox_3 = new QGroupBox(Vehicule);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(10, 10, 1471, 61));
        groupBox_3->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;\n"
"border-radius:10px;"));
        horizontalLayoutWidget = new QWidget(groupBox_3);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(440, 0, 931, 61));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        candidat_4 = new QPushButton(horizontalLayoutWidget);
        candidat_4->setObjectName("candidat_4");
        QFont font4;
        font4.setPointSize(11);
        font4.setBold(true);
        candidat_4->setFont(font4);
        candidat_4->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(candidat_4);

        planning_4 = new QPushButton(horizontalLayoutWidget);
        planning_4->setObjectName("planning_4");
        planning_4->setFont(font4);
        planning_4->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(planning_4);

        employee_4 = new QPushButton(horizontalLayoutWidget);
        employee_4->setObjectName("employee_4");
        employee_4->setFont(font4);
        employee_4->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(employee_4);

        vehicule_4 = new QPushButton(horizontalLayoutWidget);
        vehicule_4->setObjectName("vehicule_4");
        vehicule_4->setFont(font4);
        vehicule_4->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(vehicule_4);

        examen_4 = new QPushButton(horizontalLayoutWidget);
        examen_4->setObjectName("examen_4");
        examen_4->setFont(font4);
        examen_4->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(examen_4);

        equipement_4 = new QPushButton(horizontalLayoutWidget);
        equipement_4->setObjectName("equipement_4");
        equipement_4->setFont(font4);
        equipement_4->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(equipement_4);

        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(20, -10, 91, 91));
        label_12->setPixmap(QPixmap(QString::fromUtf8(":/images/logo.png")));
        label_12->setScaledContents(true);
        groupBox_4 = new QGroupBox(Vehicule);
        groupBox_4->setObjectName("groupBox_4");
        groupBox_4->setGeometry(QRect(449, 510, 1041, 331));
        label_11 = new QLabel(groupBox_4);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(350, 60, 301, 261));
        label_11->setPixmap(QPixmap(QString::fromUtf8(":/images/statistique.png")));
        label_11->setScaledContents(true);
        label_16 = new QLabel(groupBox_4);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(320, 20, 241, 31));
        label_16->setFont(font1);
        label_16->setStyleSheet(QString::fromUtf8("color:#002157"));
        tri_2 = new QComboBox(groupBox_4);
        tri_2->addItem(QString());
        tri_2->addItem(QString());
        tri_2->addItem(QString());
        tri_2->setObjectName("tri_2");
        tri_2->setGeometry(QRect(490, 10, 211, 41));
        tri_2->setFont(font1);
        tri_2->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
""));

        retranslateUi(Vehicule);

        QMetaObject::connectSlotsByName(Vehicule);
    } // setupUi

    void retranslateUi(QWidget *Vehicule)
    {
        Vehicule->setWindowTitle(QCoreApplication::translate("Vehicule", "Form", nullptr));
        groupBox_10->setTitle(QString());
        label_41->setText(QCoreApplication::translate("Vehicule", "Ajout de v\303\251hicules", nullptr));
        label_42->setText(QCoreApplication::translate("Vehicule", "Mod\303\250le:", nullptr));
        label_43->setText(QCoreApplication::translate("Vehicule", "Matricule:", nullptr));
        label_44->setText(QCoreApplication::translate("Vehicule", "Kilom\303\251trage:", nullptr));
        radioButton_4->setText(QCoreApplication::translate("Vehicule", "Libre", nullptr));
        radioButton_5->setText(QCoreApplication::translate("Vehicule", "R\303\251serv\303\251", nullptr));
        DispoLineEdit_2->setText(QCoreApplication::translate("Vehicule", "Disponibilit\303\251:", nullptr));
        DateMLineEdit_2->setText(QCoreApplication::translate("Vehicule", "date de maintenance:", nullptr));
        TypeEnergieLineEdit_2->setText(QCoreApplication::translate("Vehicule", "Type d'\303\251nergie :", nullptr));
        comboBox_2->setItemText(0, QCoreApplication::translate("Vehicule", "Gaz", nullptr));
        comboBox_2->setItemText(1, QCoreApplication::translate("Vehicule", "Essence", nullptr));
        comboBox_2->setItemText(2, QCoreApplication::translate("Vehicule", "Hybride", nullptr));

        AjouterButton_2->setText(QCoreApplication::translate("Vehicule", "Ajouter", nullptr));
        AnnulerButton_2->setText(QCoreApplication::translate("Vehicule", "Annuler", nullptr));
        radioButton_6->setText(QCoreApplication::translate("Vehicule", "En service", nullptr));
        groupBox_7->setTitle(QString());
        QTableWidgetItem *___qtablewidgetitem = tab_4->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Vehicule", "Modele", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tab_4->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Vehicule", "Matricule", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tab_4->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("Vehicule", "Kilometrage", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tab_4->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("Vehicule", "disponibilit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tab_4->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("Vehicule", "date de maintenance", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tab_4->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("Vehicule", "type d'energie", nullptr));
        label_31->setText(QCoreApplication::translate("Vehicule", "Tri par : ", nullptr));
        label_32->setText(QCoreApplication::translate("Vehicule", "chercher", nullptr));
        TriComboBox_2->setItemText(0, QCoreApplication::translate("Vehicule", "--------------", nullptr));
        TriComboBox_2->setItemText(1, QCoreApplication::translate("Vehicule", "Date de maintenance", nullptr));
        TriComboBox_2->setItemText(2, QCoreApplication::translate("Vehicule", "Kilom\303\251trage", nullptr));

        pushButton_12->setText(QString());
        pushButton_13->setText(QString());
        pushButton_18->setText(QCoreApplication::translate("Vehicule", "Exporter", nullptr));
        groupBox_3->setTitle(QString());
        candidat_4->setText(QCoreApplication::translate("Vehicule", "Candidat", nullptr));
        planning_4->setText(QCoreApplication::translate("Vehicule", "planning", nullptr));
        employee_4->setText(QCoreApplication::translate("Vehicule", "Employee", nullptr));
        vehicule_4->setText(QCoreApplication::translate("Vehicule", "vehicule", nullptr));
        examen_4->setText(QCoreApplication::translate("Vehicule", "Examen", nullptr));
        equipement_4->setText(QCoreApplication::translate("Vehicule", "equipement", nullptr));
        label_12->setText(QString());
        groupBox_4->setTitle(QString());
        label_11->setText(QString());
        label_16->setText(QCoreApplication::translate("Vehicule", "statiqtique par : ", nullptr));
        tri_2->setItemText(0, QCoreApplication::translate("Vehicule", "--------------", nullptr));
        tri_2->setItemText(1, QCoreApplication::translate("Vehicule", "age", nullptr));
        tri_2->setItemText(2, QCoreApplication::translate("Vehicule", "type de permis", nullptr));

    } // retranslateUi

};

namespace Ui {
    class Vehicule: public Ui_Vehicule {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VEHICULE_H
