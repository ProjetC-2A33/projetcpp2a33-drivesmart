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
    QRadioButton *libre;
    QRadioButton *reserve;
    QLabel *DispoLineEdit_2;
    QLabel *DateMLineEdit_2;
    QLabel *TypeEnergieLineEdit_2;
    QComboBox *type_energie_comboBox;
    QPushButton *AjouterButton;
    QPushButton *AnnulerButton;
    QTextEdit *MatriculeLineEdit_2;
    QTextEdit *KilomLineEdit_2;
    QRadioButton *enservice;
    QDateEdit *dat_maintenance;
    QLabel *DateMLineEdit_3;
    QTextEdit *etatmaintLineEdit;
    QGroupBox *groupBox_7;
    QTableWidget *tableWidget;
    QLabel *label_31;
    QTextEdit *recherche_4;
    QLabel *label_32;
    QComboBox *TriComboBox_2;
    QPushButton *supprimerButton;
    QPushButton *modifierButton;
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
    QLabel *label;

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
        libre = new QRadioButton(groupBox_10);
        libre->setObjectName("libre");
        libre->setGeometry(QRect(10, 350, 121, 31));
        libre->setFont(font1);
        reserve = new QRadioButton(groupBox_10);
        reserve->setObjectName("reserve");
        reserve->setGeometry(QRect(140, 350, 121, 31));
        reserve->setFont(font1);
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
        TypeEnergieLineEdit_2->setGeometry(QRect(20, 530, 181, 31));
        TypeEnergieLineEdit_2->setFont(font1);
        TypeEnergieLineEdit_2->setStyleSheet(QString::fromUtf8("color:#002157"));
        type_energie_comboBox = new QComboBox(groupBox_10);
        type_energie_comboBox->addItem(QString());
        type_energie_comboBox->addItem(QString());
        type_energie_comboBox->addItem(QString());
        type_energie_comboBox->addItem(QString());
        type_energie_comboBox->setObjectName("type_energie_comboBox");
        type_energie_comboBox->setGeometry(QRect(200, 530, 211, 41));
        type_energie_comboBox->setFont(font1);
        type_energie_comboBox->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
""));
        AjouterButton = new QPushButton(groupBox_10);
        AjouterButton->setObjectName("AjouterButton");
        AjouterButton->setGeometry(QRect(20, 600, 171, 61));
        QFont font2;
        font2.setPointSize(16);
        AjouterButton->setFont(font2);
        AjouterButton->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        AnnulerButton = new QPushButton(groupBox_10);
        AnnulerButton->setObjectName("AnnulerButton");
        AnnulerButton->setGeometry(QRect(220, 600, 171, 61));
        AnnulerButton->setFont(font2);
        AnnulerButton->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
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
        enservice = new QRadioButton(groupBox_10);
        enservice->setObjectName("enservice");
        enservice->setGeometry(QRect(270, 350, 131, 31));
        enservice->setFont(font1);
        dat_maintenance = new QDateEdit(groupBox_10);
        dat_maintenance->setObjectName("dat_maintenance");
        dat_maintenance->setGeometry(QRect(250, 400, 161, 31));
        QFont font3;
        font3.setPointSize(12);
        dat_maintenance->setFont(font3);
        DateMLineEdit_3 = new QLabel(groupBox_10);
        DateMLineEdit_3->setObjectName("DateMLineEdit_3");
        DateMLineEdit_3->setGeometry(QRect(0, 460, 221, 31));
        DateMLineEdit_3->setFont(font1);
        DateMLineEdit_3->setStyleSheet(QString::fromUtf8("color:#002157"));
        etatmaintLineEdit = new QTextEdit(groupBox_10);
        etatmaintLineEdit->setObjectName("etatmaintLineEdit");
        etatmaintLineEdit->setGeometry(QRect(220, 460, 191, 41));
        etatmaintLineEdit->setFont(font1);
        etatmaintLineEdit->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
"\n"
"\n"
""));
        etatmaintLineEdit->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        etatmaintLineEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        etatmaintLineEdit->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        groupBox_7 = new QGroupBox(Vehicule);
        groupBox_7->setObjectName("groupBox_7");
        groupBox_7->setGeometry(QRect(450, 78, 1041, 411));
        tableWidget = new QTableWidget(groupBox_7);
        if (tableWidget->columnCount() < 7)
            tableWidget->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font3);
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font3);
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setFont(font3);
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setFont(font3);
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setFont(font3);
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setFont(font3);
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        __qtablewidgetitem6->setFont(font3);
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        if (tableWidget->rowCount() < 15)
            tableWidget->setRowCount(15);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(10, 93, 1001, 301));
        tableWidget->setFont(font1);
        tableWidget->setAutoScrollMargin(20);
        tableWidget->setTabKeyNavigation(false);
        tableWidget->setProperty("showDropIndicator", QVariant(false));
        tableWidget->setDragEnabled(false);
        tableWidget->setDragDropOverwriteMode(true);
        tableWidget->setAlternatingRowColors(false);
        tableWidget->setShowGrid(true);
        tableWidget->setWordWrap(true);
        tableWidget->setRowCount(15);
        tableWidget->horizontalHeader()->setVisible(true);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(false);
        tableWidget->horizontalHeader()->setMinimumSectionSize(40);
        tableWidget->horizontalHeader()->setDefaultSectionSize(140);
        tableWidget->horizontalHeader()->setHighlightSections(true);
        tableWidget->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget->horizontalHeader()->setStretchLastSection(true);
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
        supprimerButton = new QPushButton(groupBox_7);
        supprimerButton->setObjectName("supprimerButton");
        supprimerButton->setGeometry(QRect(780, 30, 41, 31));
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::EditDelete));
        supprimerButton->setIcon(icon);
        modifierButton = new QPushButton(groupBox_7);
        modifierButton->setObjectName("modifierButton");
        modifierButton->setGeometry(QRect(830, 30, 41, 31));
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::MailMessageNew));
        modifierButton->setIcon(icon1);
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
        label_11->setGeometry(QRect(740, 180, 301, 151));
        label_11->setPixmap(QPixmap(QString::fromUtf8(":/images/msg.png")));
        label_11->setScaledContents(true);
        label_16 = new QLabel(groupBox_4);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(40, 20, 161, 31));
        label_16->setFont(font1);
        label_16->setStyleSheet(QString::fromUtf8("color:#002157"));
        tri_2 = new QComboBox(groupBox_4);
        tri_2->addItem(QString());
        tri_2->addItem(QString());
        tri_2->addItem(QString());
        tri_2->setObjectName("tri_2");
        tri_2->setGeometry(QRect(210, 10, 211, 41));
        tri_2->setFont(font1);
        tri_2->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
""));
        label = new QLabel(groupBox_4);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 70, 321, 201));
        label->setPixmap(QPixmap(QString::fromUtf8(":/images/stat_vehicule.png")));
        label->setScaledContents(true);

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
        libre->setText(QCoreApplication::translate("Vehicule", "Libre", nullptr));
        reserve->setText(QCoreApplication::translate("Vehicule", "R\303\251serv\303\251", nullptr));
        DispoLineEdit_2->setText(QCoreApplication::translate("Vehicule", "Disponibilit\303\251:", nullptr));
        DateMLineEdit_2->setText(QCoreApplication::translate("Vehicule", "date de maintenance:", nullptr));
        TypeEnergieLineEdit_2->setText(QCoreApplication::translate("Vehicule", "Type d'\303\251nergie :", nullptr));
        type_energie_comboBox->setItemText(0, QCoreApplication::translate("Vehicule", "--------------", nullptr));
        type_energie_comboBox->setItemText(1, QCoreApplication::translate("Vehicule", "Gaz", nullptr));
        type_energie_comboBox->setItemText(2, QCoreApplication::translate("Vehicule", "Essence", nullptr));
        type_energie_comboBox->setItemText(3, QCoreApplication::translate("Vehicule", "Hybride", nullptr));

        AjouterButton->setText(QCoreApplication::translate("Vehicule", "Ajouter", nullptr));
        AnnulerButton->setText(QCoreApplication::translate("Vehicule", "Annuler", nullptr));
        enservice->setText(QCoreApplication::translate("Vehicule", "En service", nullptr));
        DateMLineEdit_3->setText(QCoreApplication::translate("Vehicule", "Etat de maintenance:", nullptr));
        groupBox_7->setTitle(QString());
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Vehicule", "Modele", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Vehicule", "Matricule", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("Vehicule", "Kilometrage", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("Vehicule", "disponibilit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("Vehicule", "type d'energie", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("Vehicule", "Etat de maintenance", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("Vehicule", "date de maintenance", nullptr));
        label_31->setText(QCoreApplication::translate("Vehicule", "Tri par : ", nullptr));
        label_32->setText(QCoreApplication::translate("Vehicule", "chercher", nullptr));
        TriComboBox_2->setItemText(0, QCoreApplication::translate("Vehicule", "--------------", nullptr));
        TriComboBox_2->setItemText(1, QCoreApplication::translate("Vehicule", "Date de maintenance", nullptr));
        TriComboBox_2->setItemText(2, QCoreApplication::translate("Vehicule", "Kilom\303\251trage", nullptr));

        supprimerButton->setText(QString());
        modifierButton->setText(QString());
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
        label_16->setText(QCoreApplication::translate("Vehicule", "Statistique par :", nullptr));
        tri_2->setItemText(0, QCoreApplication::translate("Vehicule", "--------------", nullptr));
        tri_2->setItemText(1, QCoreApplication::translate("Vehicule", "Disponibilit\303\251", nullptr));
        tri_2->setItemText(2, QCoreApplication::translate("Vehicule", "type d'\303\251nergie", nullptr));

        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Vehicule: public Ui_Vehicule {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VEHICULE_H
