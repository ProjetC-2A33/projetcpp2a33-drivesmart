/********************************************************************************
** Form generated from reading UI file 'condidat.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONDIDAT_H
#define UI_CONDIDAT_H

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

class Ui_Condidat
{
public:
    QGroupBox *groupBox_2;
    QTableWidget *tab;
    QLabel *label_9;
    QComboBox *tri;
    QTextEdit *recherche;
    QLabel *label_13;
    QPushButton *exporter;
    QGroupBox *groupBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QTextEdit *cin;
    QTextEdit *prenom;
    QTextEdit *nom;
    QRadioButton *homme;
    QRadioButton *femme;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QTextEdit *tel;
    QLabel *label_8;
    QComboBox *type;
    QPushButton *btn_ajout;
    QPushButton *btn_reset;
    QDateEdit *date;
    QGroupBox *groupBox_3;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *candidat;
    QPushButton *planning;
    QPushButton *employee;
    QPushButton *vehicule;
    QPushButton *examen;
    QPushButton *equipement;
    QLabel *label_12;

    void setupUi(QWidget *Condidat)
    {
        if (Condidat->objectName().isEmpty())
            Condidat->setObjectName("Condidat");
        Condidat->resize(1557, 876);
        groupBox_2 = new QGroupBox(Condidat);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(450, 78, 1041, 411));
        tab = new QTableWidget(groupBox_2);
        if (tab->columnCount() < 7)
            tab->setColumnCount(7);
        QFont font;
        font.setPointSize(12);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font);
        tab->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font);
        tab->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setFont(font);
        tab->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setFont(font);
        tab->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setFont(font);
        tab->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setFont(font);
        tab->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        __qtablewidgetitem6->setFont(font);
        tab->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        if (tab->rowCount() < 15)
            tab->setRowCount(15);
        tab->setObjectName("tab");
        tab->setGeometry(QRect(10, 93, 1001, 301));
        QFont font1;
        font1.setPointSize(14);
        tab->setFont(font1);
        tab->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
        tab->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        tab->setAutoScroll(true);
        tab->setSelectionMode(QAbstractItemView::SelectionMode::ContiguousSelection);
        tab->setVerticalScrollMode(QAbstractItemView::ScrollMode::ScrollPerItem);
        tab->setShowGrid(true);
        tab->setSortingEnabled(false);
        tab->setCornerButtonEnabled(true);
        tab->setRowCount(15);
        tab->horizontalHeader()->setVisible(true);
        tab->horizontalHeader()->setCascadingSectionResizes(false);
        tab->horizontalHeader()->setMinimumSectionSize(40);
        tab->horizontalHeader()->setDefaultSectionSize(125);
        tab->horizontalHeader()->setHighlightSections(true);
        tab->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tab->horizontalHeader()->setStretchLastSection(true);
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(10, 25, 91, 31));
        label_9->setFont(font1);
        label_9->setStyleSheet(QString::fromUtf8("color:#002157"));
        tri = new QComboBox(groupBox_2);
        tri->addItem(QString());
        tri->addItem(QString());
        tri->addItem(QString());
        tri->addItem(QString());
        tri->addItem(QString());
        tri->setObjectName("tri");
        tri->setGeometry(QRect(95, 20, 211, 41));
        tri->setFont(font1);
        tri->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
""));
        recherche = new QTextEdit(groupBox_2);
        recherche->setObjectName("recherche");
        recherche->setGeometry(QRect(450, 20, 301, 41));
        recherche->setFont(font1);
        recherche->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
""));
        recherche->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        recherche->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        recherche->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(350, 20, 101, 31));
        label_13->setFont(font1);
        label_13->setStyleSheet(QString::fromUtf8("color:#002157"));
        exporter = new QPushButton(groupBox_2);
        exporter->setObjectName("exporter");
        exporter->setGeometry(QRect(790, 20, 141, 41));
        exporter->setFont(font1);
        exporter->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::WindowNew));
        exporter->setIcon(icon);
        exporter->setIconSize(QSize(20, 20));
        groupBox = new QGroupBox(Condidat);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(10, 78, 421, 761));
        groupBox->setStyleSheet(QString::fromUtf8("border-radius:50px;\n"
"background-color:#e3e2de;"));
        label = new QLabel(groupBox);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 10, 331, 61));
        QFont font2;
        font2.setPointSize(24);
        label->setFont(font2);
        label->setStyleSheet(QString::fromUtf8("color:#002157\n"
""));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 140, 121, 31));
        label_2->setFont(font1);
        label_2->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(20, 190, 121, 31));
        label_3->setFont(font1);
        label_3->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(20, 240, 121, 31));
        label_4->setFont(font1);
        label_4->setStyleSheet(QString::fromUtf8("color:#002157"));
        cin = new QTextEdit(groupBox);
        cin->setObjectName("cin");
        cin->setGeometry(QRect(200, 140, 211, 41));
        cin->setFont(font1);
        cin->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
""));
        cin->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        cin->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        cin->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        prenom = new QTextEdit(groupBox);
        prenom->setObjectName("prenom");
        prenom->setGeometry(QRect(200, 240, 211, 41));
        prenom->setFont(font1);
        prenom->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;"));
        nom = new QTextEdit(groupBox);
        nom->setObjectName("nom");
        nom->setGeometry(QRect(200, 190, 211, 41));
        nom->setFont(font1);
        nom->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;"));
        homme = new QRadioButton(groupBox);
        homme->setObjectName("homme");
        homme->setGeometry(QRect(150, 310, 121, 31));
        homme->setFont(font1);
        femme = new QRadioButton(groupBox);
        femme->setObjectName("femme");
        femme->setGeometry(QRect(280, 306, 121, 40));
        femme->setFont(font1);
        label_5 = new QLabel(groupBox);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(20, 300, 121, 31));
        label_5->setFont(font1);
        label_5->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(20, 350, 211, 31));
        label_6->setFont(font1);
        label_6->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(20, 410, 141, 31));
        label_7->setFont(font1);
        label_7->setStyleSheet(QString::fromUtf8("color:#002157"));
        tel = new QTextEdit(groupBox);
        tel->setObjectName("tel");
        tel->setGeometry(QRect(200, 400, 211, 41));
        tel->setFont(font1);
        tel->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;"));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(20, 460, 181, 31));
        label_8->setFont(font1);
        label_8->setStyleSheet(QString::fromUtf8("color:#002157"));
        type = new QComboBox(groupBox);
        type->addItem(QString());
        type->addItem(QString());
        type->addItem(QString());
        type->setObjectName("type");
        type->setGeometry(QRect(200, 460, 211, 41));
        type->setFont(font1);
        type->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
""));
        btn_ajout = new QPushButton(groupBox);
        btn_ajout->setObjectName("btn_ajout");
        btn_ajout->setGeometry(QRect(30, 540, 171, 61));
        QFont font3;
        font3.setPointSize(16);
        btn_ajout->setFont(font3);
        btn_ajout->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        btn_reset = new QPushButton(groupBox);
        btn_reset->setObjectName("btn_reset");
        btn_reset->setGeometry(QRect(220, 540, 171, 61));
        btn_reset->setFont(font3);
        btn_reset->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        date = new QDateEdit(groupBox);
        date->setObjectName("date");
        date->setGeometry(QRect(250, 355, 161, 31));
        date->setFont(font);
        groupBox_3 = new QGroupBox(Condidat);
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
        candidat = new QPushButton(horizontalLayoutWidget);
        candidat->setObjectName("candidat");
        QFont font4;
        font4.setPointSize(11);
        font4.setBold(true);
        candidat->setFont(font4);
        candidat->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(candidat);

        planning = new QPushButton(horizontalLayoutWidget);
        planning->setObjectName("planning");
        planning->setFont(font4);
        planning->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(planning);

        employee = new QPushButton(horizontalLayoutWidget);
        employee->setObjectName("employee");
        employee->setFont(font4);
        employee->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(employee);

        vehicule = new QPushButton(horizontalLayoutWidget);
        vehicule->setObjectName("vehicule");
        vehicule->setFont(font4);
        vehicule->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(vehicule);

        examen = new QPushButton(horizontalLayoutWidget);
        examen->setObjectName("examen");
        examen->setFont(font4);
        examen->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(examen);

        equipement = new QPushButton(horizontalLayoutWidget);
        equipement->setObjectName("equipement");
        equipement->setFont(font4);
        equipement->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(equipement);

        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(20, -10, 91, 91));
        label_12->setPixmap(QPixmap(QString::fromUtf8(":/images/logo.png")));
        label_12->setScaledContents(true);

        retranslateUi(Condidat);

        QMetaObject::connectSlotsByName(Condidat);
    } // setupUi

    void retranslateUi(QWidget *Condidat)
    {
        Condidat->setWindowTitle(QCoreApplication::translate("Condidat", "Form", nullptr));
        groupBox_2->setTitle(QString());
        QTableWidgetItem *___qtablewidgetitem = tab->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Condidat", "CIN", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tab->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Condidat", "nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tab->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("Condidat", "Pr\303\251nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tab->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("Condidat", "Sexe", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tab->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("Condidat", "date de naissance", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tab->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("Condidat", "T\303\251lephone", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tab->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("Condidat", "type de permis", nullptr));
        label_9->setText(QCoreApplication::translate("Condidat", "Tri par : ", nullptr));
        tri->setItemText(0, QCoreApplication::translate("Condidat", "--------------", nullptr));
        tri->setItemText(1, QCoreApplication::translate("Condidat", "nom A \303\240 Z ", nullptr));
        tri->setItemText(2, QCoreApplication::translate("Condidat", "nom Z \303\240 A", nullptr));
        tri->setItemText(3, QCoreApplication::translate("Condidat", "age croissant", nullptr));
        tri->setItemText(4, QCoreApplication::translate("Condidat", "age  d\303\251croissant", nullptr));

        label_13->setText(QCoreApplication::translate("Condidat", "chercher", nullptr));
        exporter->setText(QCoreApplication::translate("Condidat", "Exporter", nullptr));
        groupBox->setTitle(QString());
        label->setText(QCoreApplication::translate("Condidat", "Ajouter Candidat", nullptr));
        label_2->setText(QCoreApplication::translate("Condidat", "CIN :", nullptr));
        label_3->setText(QCoreApplication::translate("Condidat", "Nom :", nullptr));
        label_4->setText(QCoreApplication::translate("Condidat", "Pr\303\251nom :", nullptr));
        homme->setText(QCoreApplication::translate("Condidat", "homme", nullptr));
        femme->setText(QCoreApplication::translate("Condidat", "femme", nullptr));
        label_5->setText(QCoreApplication::translate("Condidat", "Sexe :", nullptr));
        label_6->setText(QCoreApplication::translate("Condidat", "date de naissance  :", nullptr));
        label_7->setText(QCoreApplication::translate("Condidat", "T\303\251lephone :", nullptr));
        label_8->setText(QCoreApplication::translate("Condidat", "Type de permis :", nullptr));
        type->setItemText(0, QCoreApplication::translate("Condidat", "voiture", nullptr));
        type->setItemText(1, QCoreApplication::translate("Condidat", "moto", nullptr));
        type->setItemText(2, QCoreApplication::translate("Condidat", "poids lourd", nullptr));

        btn_ajout->setText(QCoreApplication::translate("Condidat", "Ajouter", nullptr));
        btn_reset->setText(QCoreApplication::translate("Condidat", "Annuler", nullptr));
        groupBox_3->setTitle(QString());
        candidat->setText(QCoreApplication::translate("Condidat", "Candidat", nullptr));
        planning->setText(QCoreApplication::translate("Condidat", "planning", nullptr));
        employee->setText(QCoreApplication::translate("Condidat", "Employee", nullptr));
        vehicule->setText(QCoreApplication::translate("Condidat", "vehicule", nullptr));
        examen->setText(QCoreApplication::translate("Condidat", "Examen", nullptr));
        equipement->setText(QCoreApplication::translate("Condidat", "equipement", nullptr));
        label_12->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Condidat: public Ui_Condidat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONDIDAT_H
