/********************************************************************************
** Form generated from reading UI file 'examen.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXAMEN_H
#define UI_EXAMEN_H

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
#include "QtWidgets/QWidget"

QT_BEGIN_NAMESPACE

class Ui_Examen
{
public:
    QWidget *centralwidget;
    QWidget *widget;
    QGroupBox *groupBox_2;
    QLabel *label_20;
    QComboBox *comboBox_4;
    QTextEdit *textEdit_16;
    QLabel *label_21;
    QPushButton *pushButton_14;
    QPushButton *pushButton_15;
    QPushButton *pushButton_8;
    QTableWidget *tab_5;
    QGroupBox *groupBox;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_14;
    QTextEdit *textEdit_6;
    QRadioButton *radioButton_3;
    QRadioButton *radioButton_4;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QTextEdit *textEdit_11;
    QPushButton *pushButton_7;
    QTextEdit *textEdit_12;
    QLabel *label_19;
    QDateEdit *dateEdit_5;
    QTextEdit *textEdit_13;
    QLabel *label_13;
    QLabel *label_15;
    QLabel *label;
    QLabel *label_22;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QGroupBox *groupBox_3;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *condidat_5;
    QPushButton *planning_5;
    QPushButton *employee_5;
    QPushButton *vehicule_5;
    QPushButton *examen_5;
    QPushButton *equipment_5;
    QLabel *label_23;

    void setupUi(QWidget *Examen)
    {
        if (Examen->objectName().isEmpty())
            Examen->setObjectName("Examen");
        Examen->resize(1557, 876);
        centralwidget = new QWidget(Examen);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setGeometry(QRect(0, 0, 1500, 850));
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(0, 0, 1500, 850));
        widget->setStyleSheet(QString::fromUtf8(""));
        groupBox_2 = new QGroupBox(widget);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(450, 78, 1041, 411));
        label_20 = new QLabel(groupBox_2);
        label_20->setObjectName("label_20");
        label_20->setGeometry(QRect(10, 25, 91, 31));
        QFont font;
        font.setPointSize(14);
        label_20->setFont(font);
        label_20->setStyleSheet(QString::fromUtf8("color:#002157"));
        comboBox_4 = new QComboBox(groupBox_2);
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->setObjectName("comboBox_4");
        comboBox_4->setGeometry(QRect(95, 20, 211, 41));
        comboBox_4->setFont(font);
        comboBox_4->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
""));
        textEdit_16 = new QTextEdit(groupBox_2);
        textEdit_16->setObjectName("textEdit_16");
        textEdit_16->setGeometry(QRect(460, 20, 301, 41));
        textEdit_16->setFont(font);
        textEdit_16->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
"\n"
"\n"
""));
        textEdit_16->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        textEdit_16->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        textEdit_16->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        label_21 = new QLabel(groupBox_2);
        label_21->setObjectName("label_21");
        label_21->setGeometry(QRect(320, 25, 131, 31));
        QFont font1;
        font1.setPointSize(14);
        font1.setBold(false);
        label_21->setFont(font1);
        label_21->setStyleSheet(QString::fromUtf8("color:#002157"));
        pushButton_14 = new QPushButton(groupBox_2);
        pushButton_14->setObjectName("pushButton_14");
        pushButton_14->setGeometry(QRect(790, 30, 41, 31));
        pushButton_14->setStyleSheet(QString::fromUtf8("color:bleu;"));
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::EditDelete));
        pushButton_14->setIcon(icon);
        pushButton_15 = new QPushButton(groupBox_2);
        pushButton_15->setObjectName("pushButton_15");
        pushButton_15->setGeometry(QRect(840, 30, 41, 31));
        pushButton_15->setStyleSheet(QString::fromUtf8("color:#002157"));
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::MailMessageNew));
        pushButton_15->setIcon(icon1);
        pushButton_8 = new QPushButton(groupBox_2);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(900, 20, 121, 41));
        pushButton_8->setFont(font);
        pushButton_8->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        QIcon icon2(QIcon::fromTheme(QIcon::ThemeIcon::WindowNew));
        pushButton_8->setIcon(icon2);
        pushButton_8->setIconSize(QSize(20, 20));
        tab_5 = new QTableWidget(groupBox_2);
        if (tab_5->columnCount() < 6)
            tab_5->setColumnCount(6);
        QFont font2;
        font2.setPointSize(12);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font2);
        tab_5->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font2);
        tab_5->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setFont(font2);
        tab_5->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setFont(font2);
        tab_5->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setFont(font2);
        tab_5->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setFont(font2);
        tab_5->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        if (tab_5->rowCount() < 15)
            tab_5->setRowCount(15);
        tab_5->setObjectName("tab_5");
        tab_5->setGeometry(QRect(10, 93, 1001, 301));
        tab_5->setFont(font);
        tab_5->setStyleSheet(QString::fromUtf8("color:bleu;\n"
""));
        tab_5->setShowGrid(true);
        tab_5->setWordWrap(true);
        tab_5->setRowCount(15);
        tab_5->horizontalHeader()->setVisible(true);
        tab_5->horizontalHeader()->setCascadingSectionResizes(false);
        tab_5->horizontalHeader()->setMinimumSectionSize(40);
        tab_5->horizontalHeader()->setDefaultSectionSize(125);
        tab_5->horizontalHeader()->setHighlightSections(true);
        tab_5->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tab_5->horizontalHeader()->setStretchLastSection(true);
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(10, 78, 421, 761));
        groupBox->setStyleSheet(QString::fromUtf8("border-radius:50px;\n"
"background-color:#e3e2de;"));
        label_11 = new QLabel(groupBox);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(30, 10, 331, 61));
        QFont font3;
        font3.setPointSize(24);
        label_11->setFont(font3);
        label_11->setStyleSheet(QString::fromUtf8("color:#002157\n"
""));
        label_12 = new QLabel(groupBox);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(20, 130, 121, 31));
        label_12->setFont(font);
        label_12->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_14 = new QLabel(groupBox);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(20, 180, 121, 31));
        label_14->setFont(font);
        label_14->setStyleSheet(QString::fromUtf8("color:#002157"));
        textEdit_6 = new QTextEdit(groupBox);
        textEdit_6->setObjectName("textEdit_6");
        textEdit_6->setGeometry(QRect(200, 130, 211, 41));
        textEdit_6->setFont(font);
        textEdit_6->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
"\n"
"\n"
""));
        textEdit_6->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        textEdit_6->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        textEdit_6->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        radioButton_3 = new QRadioButton(groupBox);
        radioButton_3->setObjectName("radioButton_3");
        radioButton_3->setGeometry(QRect(200, 240, 121, 31));
        radioButton_3->setFont(font);
        radioButton_3->setStyleSheet(QString::fromUtf8("color:#002157"));
        radioButton_4 = new QRadioButton(groupBox);
        radioButton_4->setObjectName("radioButton_4");
        radioButton_4->setGeometry(QRect(290, 240, 121, 31));
        radioButton_4->setFont(font);
        radioButton_4->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_16 = new QLabel(groupBox);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(20, 240, 171, 31));
        label_16->setFont(font);
        label_16->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_17 = new QLabel(groupBox);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(20, 290, 211, 31));
        label_17->setFont(font);
        label_17->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_18 = new QLabel(groupBox);
        label_18->setObjectName("label_18");
        label_18->setGeometry(QRect(20, 400, 181, 31));
        label_18->setFont(font);
        label_18->setStyleSheet(QString::fromUtf8("color:#002157"));
        pushButton_4 = new QPushButton(groupBox);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(20, 480, 171, 61));
        QFont font4;
        font4.setPointSize(16);
        pushButton_4->setFont(font4);
        pushButton_4->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        pushButton_5 = new QPushButton(groupBox);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(230, 480, 171, 61));
        pushButton_5->setFont(font4);
        pushButton_5->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        textEdit_11 = new QTextEdit(groupBox);
        textEdit_11->setObjectName("textEdit_11");
        textEdit_11->setGeometry(QRect(200, 180, 211, 41));
        textEdit_11->setFont(font);
        textEdit_11->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
"\n"
"\n"
""));
        textEdit_11->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        textEdit_11->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        textEdit_11->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        pushButton_7 = new QPushButton(groupBox);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(120, 580, 171, 61));
        pushButton_7->setFont(font4);
        pushButton_7->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        textEdit_12 = new QTextEdit(groupBox);
        textEdit_12->setObjectName("textEdit_12");
        textEdit_12->setGeometry(QRect(200, 390, 211, 41));
        textEdit_12->setFont(font);
        textEdit_12->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
"\n"
"\n"
""));
        textEdit_12->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        textEdit_12->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        textEdit_12->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        label_19 = new QLabel(groupBox);
        label_19->setObjectName("label_19");
        label_19->setGeometry(QRect(20, 350, 171, 31));
        label_19->setFont(font);
        label_19->setStyleSheet(QString::fromUtf8("color:#002157"));
        dateEdit_5 = new QDateEdit(groupBox);
        dateEdit_5->setObjectName("dateEdit_5");
        dateEdit_5->setGeometry(QRect(220, 290, 161, 31));
        dateEdit_5->setFont(font2);
        dateEdit_5->setStyleSheet(QString::fromUtf8("color:#002157"));
        textEdit_13 = new QTextEdit(groupBox);
        textEdit_13->setObjectName("textEdit_13");
        textEdit_13->setGeometry(QRect(200, 340, 211, 41));
        textEdit_13->setFont(font);
        textEdit_13->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
"\n"
"\n"
""));
        textEdit_13->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        textEdit_13->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        textEdit_13->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        label_13 = new QLabel(groupBox);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(510, 420, 121, 31));
        label_13->setFont(font);
        label_13->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_15 = new QLabel(groupBox);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(490, 440, 121, 31));
        label_15->setFont(font);
        label_15->setStyleSheet(QString::fromUtf8("color:#002157"));
        label = new QLabel(widget);
        label->setObjectName("label");
        label->setGeometry(QRect(1400, 740, 71, 81));
        label->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/SMS.png")));
        label_22 = new QLabel(widget);
        label_22->setObjectName("label_22");
        label_22->setGeometry(QRect(1290, 760, 111, 31));
        label_22->setFont(font);
        label_22->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_2 = new QLabel(widget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(450, 460, 63, 20));
        label_3 = new QLabel(widget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(460, 450, 331, 301));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/stat.png")));
        label_4 = new QLabel(widget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(520, 590, 63, 20));
        label_4->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
""));
        label_5 = new QLabel(widget);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(670, 590, 63, 20));
        label_5->setStyleSheet(QString::fromUtf8("background-color:rgb(85, 0, 255);\n"
"color:#002157;\n"
""));
        groupBox_3 = new QGroupBox(widget);
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
        condidat_5 = new QPushButton(horizontalLayoutWidget);
        condidat_5->setObjectName("condidat_5");
        QFont font5;
        font5.setPointSize(11);
        font5.setBold(true);
        condidat_5->setFont(font5);
        condidat_5->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(condidat_5);

        planning_5 = new QPushButton(horizontalLayoutWidget);
        planning_5->setObjectName("planning_5");
        planning_5->setFont(font5);
        planning_5->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(planning_5);

        employee_5 = new QPushButton(horizontalLayoutWidget);
        employee_5->setObjectName("employee_5");
        employee_5->setFont(font5);
        employee_5->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(employee_5);

        vehicule_5 = new QPushButton(horizontalLayoutWidget);
        vehicule_5->setObjectName("vehicule_5");
        vehicule_5->setFont(font5);
        vehicule_5->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(vehicule_5);

        examen_5 = new QPushButton(horizontalLayoutWidget);
        examen_5->setObjectName("examen_5");
        examen_5->setFont(font5);
        examen_5->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(examen_5);

        equipment_5 = new QPushButton(horizontalLayoutWidget);
        equipment_5->setObjectName("equipment_5");
        equipment_5->setFont(font5);
        equipment_5->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(equipment_5);

        label_23 = new QLabel(groupBox_3);
        label_23->setObjectName("label_23");
        label_23->setGeometry(QRect(20, -10, 91, 91));
        label_23->setPixmap(QPixmap(QString::fromUtf8(":/images/logo.png")));
        label_23->setScaledContents(true);

        retranslateUi(Examen);

        QMetaObject::connectSlotsByName(Examen);
    } // setupUi

    void retranslateUi(QWidget *Examen)
    {
        Examen->setWindowTitle(QCoreApplication::translate("Examen", "MainWindow", nullptr));
        groupBox_2->setTitle(QString());
        label_20->setText(QCoreApplication::translate("Examen", "Tri par : ", nullptr));
        comboBox_4->setItemText(0, QCoreApplication::translate("Examen", "--------------", nullptr));
        comboBox_4->setItemText(1, QCoreApplication::translate("Examen", "nom A \303\240 Z ", nullptr));
        comboBox_4->setItemText(2, QCoreApplication::translate("Examen", "nom Z \303\240 A", nullptr));
        comboBox_4->setItemText(3, QCoreApplication::translate("Examen", "age croissant", nullptr));
        comboBox_4->setItemText(4, QCoreApplication::translate("Examen", "age  d\303\251croissant", nullptr));

        label_21->setText(QCoreApplication::translate("Examen", " Chercher  :", nullptr));
        pushButton_14->setText(QString());
        pushButton_15->setText(QString());
        pushButton_8->setText(QCoreApplication::translate("Examen", "Exporter", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tab_5->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Examen", "CIN", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tab_5->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Examen", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tab_5->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("Examen", "type examen", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tab_5->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("Examen", "Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tab_5->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("Examen", "T\303\251l", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tab_5->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("Examen", "type de permis", nullptr));
        groupBox->setTitle(QString());
        label_11->setText(QCoreApplication::translate("Examen", "Ajouter Examen", nullptr));
        label_12->setText(QCoreApplication::translate("Examen", "CIN :", nullptr));
        label_14->setText(QCoreApplication::translate("Examen", "Nom :", nullptr));
        radioButton_3->setText(QCoreApplication::translate("Examen", "code", nullptr));
        radioButton_4->setText(QCoreApplication::translate("Examen", "conduit", nullptr));
        label_16->setText(QCoreApplication::translate("Examen", "Type d'examen :", nullptr));
        label_17->setText(QCoreApplication::translate("Examen", "Date d'examen  :", nullptr));
        label_18->setText(QCoreApplication::translate("Examen", "Type de permis :", nullptr));
        pushButton_4->setText(QCoreApplication::translate("Examen", "Ajouter", nullptr));
        pushButton_5->setText(QCoreApplication::translate("Examen", "Modifier", nullptr));
        pushButton_7->setText(QCoreApplication::translate("Examen", "Annuler", nullptr));
        label_19->setText(QCoreApplication::translate("Examen", "T\303\251lephone :", nullptr));
        label_13->setText(QCoreApplication::translate("Examen", "CIN :", nullptr));
        label_15->setText(QCoreApplication::translate("Examen", "CIN :", nullptr));
        label->setText(QString());
        label_22->setText(QCoreApplication::translate("Examen", "Envoyer:", nullptr));
        label_2->setText(QString());
        label_3->setText(QString());
        label_4->setText(QCoreApplication::translate("Examen", "code", nullptr));
        label_5->setText(QCoreApplication::translate("Examen", "conduit", nullptr));
        groupBox_3->setTitle(QString());
        condidat_5->setText(QCoreApplication::translate("Examen", "Candidat", nullptr));
        planning_5->setText(QCoreApplication::translate("Examen", "planning", nullptr));
        employee_5->setText(QCoreApplication::translate("Examen", "Employee", nullptr));
        vehicule_5->setText(QCoreApplication::translate("Examen", "vehicule", nullptr));
        examen_5->setText(QCoreApplication::translate("Examen", "Examen", nullptr));
        equipment_5->setText(QCoreApplication::translate("Examen", "equipement", nullptr));
        label_23->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Examen: public Ui_Examen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXAMEN_H
