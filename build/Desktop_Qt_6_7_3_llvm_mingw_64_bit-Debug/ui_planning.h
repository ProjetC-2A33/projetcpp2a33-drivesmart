/********************************************************************************
** Form generated from reading UI file 'planning.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLANNING_H
#define UI_PLANNING_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCalendarWidget>
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
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Planning
{
public:
    QGroupBox *groupBox_4;
    QTableWidget *tab_3;
    QLabel *label_21;
    QComboBox *tri_3;
    QTextEdit *recherche_3;
    QLabel *label_22;
    QCalendarWidget *calendarWidget;
    QPushButton *pushButton_10;
    QPushButton *pushButton_11;
    QPushButton *pushButton_8;
    QWidget *widget;
    QLabel *label;
    QGroupBox *groupBox_6;
    QLabel *label_23;
    QLabel *label_24;
    QLabel *label_25;
    QLabel *label_26;
    QRadioButton *homme_3;
    QRadioButton *femme_3;
    QLabel *label_27;
    QLabel *label_29;
    QTextEdit *tel_3;
    QLabel *label_30;
    QComboBox *type_3;
    QPushButton *btn_ajout_3;
    QPushButton *btn_reset_3;
    QTextEdit *cin_4;
    QLabel *label_33;
    QTimeEdit *timeEdit;
    QTimeEdit *timeEdit_2;
    QDateEdit *dateEdit;
    QGroupBox *groupBox_3;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *candidat_3;
    QPushButton *planning_3;
    QPushButton *employee_3;
    QPushButton *vehicule_3;
    QPushButton *examen_3;
    QPushButton *equipement_3;
    QLabel *label_12;

    void setupUi(QWidget *Planning)
    {
        if (Planning->objectName().isEmpty())
            Planning->setObjectName("Planning");
        Planning->resize(1557, 876);
        groupBox_4 = new QGroupBox(Planning);
        groupBox_4->setObjectName("groupBox_4");
        groupBox_4->setGeometry(QRect(450, 78, 1041, 411));
        tab_3 = new QTableWidget(groupBox_4);
        if (tab_3->columnCount() < 7)
            tab_3->setColumnCount(7);
        QFont font;
        font.setPointSize(12);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font);
        tab_3->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font);
        tab_3->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setFont(font);
        tab_3->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setFont(font);
        tab_3->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setFont(font);
        tab_3->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setFont(font);
        tab_3->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        __qtablewidgetitem6->setFont(font);
        tab_3->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        if (tab_3->rowCount() < 15)
            tab_3->setRowCount(15);
        tab_3->setObjectName("tab_3");
        tab_3->setGeometry(QRect(10, 93, 1001, 301));
        QFont font1;
        font1.setPointSize(14);
        tab_3->setFont(font1);
        tab_3->setShowGrid(true);
        tab_3->setWordWrap(true);
        tab_3->setRowCount(15);
        tab_3->horizontalHeader()->setVisible(true);
        tab_3->horizontalHeader()->setCascadingSectionResizes(false);
        tab_3->horizontalHeader()->setMinimumSectionSize(40);
        tab_3->horizontalHeader()->setDefaultSectionSize(125);
        tab_3->horizontalHeader()->setHighlightSections(true);
        tab_3->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tab_3->horizontalHeader()->setStretchLastSection(true);
        label_21 = new QLabel(groupBox_4);
        label_21->setObjectName("label_21");
        label_21->setGeometry(QRect(10, 25, 91, 31));
        label_21->setFont(font1);
        label_21->setStyleSheet(QString::fromUtf8("color:#002157"));
        tri_3 = new QComboBox(groupBox_4);
        tri_3->addItem(QString());
        tri_3->addItem(QString());
        tri_3->addItem(QString());
        tri_3->addItem(QString());
        tri_3->addItem(QString());
        tri_3->setObjectName("tri_3");
        tri_3->setGeometry(QRect(95, 20, 211, 41));
        tri_3->setFont(font1);
        tri_3->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
""));
        recherche_3 = new QTextEdit(groupBox_4);
        recherche_3->setObjectName("recherche_3");
        recherche_3->setGeometry(QRect(450, 20, 301, 41));
        recherche_3->setFont(font1);
        recherche_3->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
"\n"
"\n"
""));
        recherche_3->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        recherche_3->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        recherche_3->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        label_22 = new QLabel(groupBox_4);
        label_22->setObjectName("label_22");
        label_22->setGeometry(QRect(350, 20, 101, 31));
        label_22->setFont(font1);
        label_22->setStyleSheet(QString::fromUtf8("color:#002157"));
        calendarWidget = new QCalendarWidget(groupBox_4);
        calendarWidget->setObjectName("calendarWidget");
        calendarWidget->setGeometry(QRect(10, 470, 491, 321));
        pushButton_10 = new QPushButton(groupBox_4);
        pushButton_10->setObjectName("pushButton_10");
        pushButton_10->setGeometry(QRect(780, 30, 41, 31));
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::EditDelete));
        pushButton_10->setIcon(icon);
        pushButton_11 = new QPushButton(groupBox_4);
        pushButton_11->setObjectName("pushButton_11");
        pushButton_11->setGeometry(QRect(830, 30, 41, 31));
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::MailMessageNew));
        pushButton_11->setIcon(icon1);
        pushButton_8 = new QPushButton(groupBox_4);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(890, 20, 141, 41));
        pushButton_8->setFont(font1);
        pushButton_8->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        QIcon icon2(QIcon::fromTheme(QIcon::ThemeIcon::WindowNew));
        pushButton_8->setIcon(icon2);
        pushButton_8->setIconSize(QSize(20, 20));
        widget = new QWidget(groupBox_4);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(510, 470, 121, 321));
        label = new QLabel(widget);
        label->setObjectName("label");
        label->setGeometry(QRect(0, -20, 121, 361));
        label->setPixmap(QPixmap(QString::fromUtf8("../Pictures/Screenshots/Capture d'\303\251cran 2025-10-07 191538.png")));
        groupBox_6 = new QGroupBox(Planning);
        groupBox_6->setObjectName("groupBox_6");
        groupBox_6->setGeometry(QRect(10, 78, 421, 761));
        groupBox_6->setStyleSheet(QString::fromUtf8("border-radius:50px;\n"
"background-color:#e3e2de;"));
        label_23 = new QLabel(groupBox_6);
        label_23->setObjectName("label_23");
        label_23->setGeometry(QRect(30, 10, 341, 61));
        QFont font2;
        font2.setPointSize(18);
        label_23->setFont(font2);
        label_23->setStyleSheet(QString::fromUtf8("color:#002157\n"
""));
        label_24 = new QLabel(groupBox_6);
        label_24->setObjectName("label_24");
        label_24->setGeometry(QRect(20, 139, 141, 31));
        label_24->setFont(font1);
        label_24->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_25 = new QLabel(groupBox_6);
        label_25->setObjectName("label_25");
        label_25->setGeometry(QRect(21, 233, 141, 31));
        label_25->setFont(font1);
        label_25->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_26 = new QLabel(groupBox_6);
        label_26->setObjectName("label_26");
        label_26->setGeometry(QRect(23, 284, 151, 40));
        label_26->setFont(font1);
        label_26->setStyleSheet(QString::fromUtf8("color:#002157"));
        homme_3 = new QRadioButton(groupBox_6);
        homme_3->setObjectName("homme_3");
        homme_3->setGeometry(QRect(170, 237, 121, 30));
        homme_3->setFont(font1);
        femme_3 = new QRadioButton(groupBox_6);
        femme_3->setObjectName("femme_3");
        femme_3->setGeometry(QRect(290, 234, 121, 40));
        femme_3->setFont(font1);
        label_27 = new QLabel(groupBox_6);
        label_27->setObjectName("label_27");
        label_27->setGeometry(QRect(20, 189, 171, 31));
        label_27->setFont(font1);
        label_27->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_29 = new QLabel(groupBox_6);
        label_29->setObjectName("label_29");
        label_29->setGeometry(QRect(20, 430, 171, 31));
        label_29->setFont(font1);
        label_29->setStyleSheet(QString::fromUtf8("color:#002157"));
        tel_3 = new QTextEdit(groupBox_6);
        tel_3->setObjectName("tel_3");
        tel_3->setGeometry(QRect(200, 426, 211, 41));
        tel_3->setFont(font1);
        tel_3->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;"));
        label_30 = new QLabel(groupBox_6);
        label_30->setObjectName("label_30");
        label_30->setGeometry(QRect(20, 372, 181, 31));
        label_30->setFont(font1);
        label_30->setStyleSheet(QString::fromUtf8("color:#002157"));
        type_3 = new QComboBox(groupBox_6);
        type_3->addItem(QString());
        type_3->addItem(QString());
        type_3->addItem(QString());
        type_3->setObjectName("type_3");
        type_3->setGeometry(QRect(200, 369, 201, 41));
        type_3->setFont(font1);
        type_3->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
""));
        btn_ajout_3 = new QPushButton(groupBox_6);
        btn_ajout_3->setObjectName("btn_ajout_3");
        btn_ajout_3->setGeometry(QRect(30, 509, 171, 61));
        QFont font3;
        font3.setPointSize(16);
        btn_ajout_3->setFont(font3);
        btn_ajout_3->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        btn_reset_3 = new QPushButton(groupBox_6);
        btn_reset_3->setObjectName("btn_reset_3");
        btn_reset_3->setGeometry(QRect(220, 509, 171, 61));
        btn_reset_3->setFont(font3);
        btn_reset_3->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        cin_4 = new QTextEdit(groupBox_6);
        cin_4->setObjectName("cin_4");
        cin_4->setGeometry(QRect(200, 189, 211, 41));
        cin_4->setFont(font1);
        cin_4->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
"\n"
"\n"
""));
        cin_4->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        cin_4->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        cin_4->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        label_33 = new QLabel(groupBox_6);
        label_33->setObjectName("label_33");
        label_33->setGeometry(QRect(22, 325, 121, 40));
        label_33->setFont(font1);
        label_33->setStyleSheet(QString::fromUtf8("color:#002157"));
        timeEdit = new QTimeEdit(groupBox_6);
        timeEdit->setObjectName("timeEdit");
        timeEdit->setGeometry(QRect(210, 284, 118, 35));
        QFont font4;
        font4.setPointSize(11);
        timeEdit->setFont(font4);
        timeEdit_2 = new QTimeEdit(groupBox_6);
        timeEdit_2->setObjectName("timeEdit_2");
        timeEdit_2->setGeometry(QRect(210, 324, 118, 35));
        timeEdit_2->setFont(font4);
        dateEdit = new QDateEdit(groupBox_6);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setGeometry(QRect(220, 140, 181, 31));
        dateEdit->setFont(font1);
        groupBox_3 = new QGroupBox(Planning);
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
        candidat_3 = new QPushButton(horizontalLayoutWidget);
        candidat_3->setObjectName("candidat_3");
        QFont font5;
        font5.setPointSize(11);
        font5.setBold(true);
        candidat_3->setFont(font5);
        candidat_3->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(candidat_3);

        planning_3 = new QPushButton(horizontalLayoutWidget);
        planning_3->setObjectName("planning_3");
        planning_3->setFont(font5);
        planning_3->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(planning_3);

        employee_3 = new QPushButton(horizontalLayoutWidget);
        employee_3->setObjectName("employee_3");
        employee_3->setFont(font5);
        employee_3->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(employee_3);

        vehicule_3 = new QPushButton(horizontalLayoutWidget);
        vehicule_3->setObjectName("vehicule_3");
        vehicule_3->setFont(font5);
        vehicule_3->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(vehicule_3);

        examen_3 = new QPushButton(horizontalLayoutWidget);
        examen_3->setObjectName("examen_3");
        examen_3->setFont(font5);
        examen_3->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(examen_3);

        equipement_3 = new QPushButton(horizontalLayoutWidget);
        equipement_3->setObjectName("equipement_3");
        equipement_3->setFont(font5);
        equipement_3->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(equipement_3);

        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(20, -10, 91, 91));
        label_12->setPixmap(QPixmap(QString::fromUtf8(":/images/logo.png")));
        label_12->setScaledContents(true);

        retranslateUi(Planning);

        QMetaObject::connectSlotsByName(Planning);
    } // setupUi

    void retranslateUi(QWidget *Planning)
    {
        Planning->setWindowTitle(QCoreApplication::translate("Planning", "Form", nullptr));
        groupBox_4->setTitle(QString());
        QTableWidgetItem *___qtablewidgetitem = tab_3->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Planning", "Date s\303\251ance", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tab_3->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Planning", "condidat", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tab_3->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("Planning", "type ", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tab_3->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("Planning", "debut", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tab_3->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("Planning", "fin", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tab_3->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("Planning", "moniteur", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tab_3->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("Planning", "circuit", nullptr));
        label_21->setText(QCoreApplication::translate("Planning", "Tri par : ", nullptr));
        tri_3->setItemText(0, QCoreApplication::translate("Planning", "--------------", nullptr));
        tri_3->setItemText(1, QCoreApplication::translate("Planning", "nom A \303\240 Z ", nullptr));
        tri_3->setItemText(2, QCoreApplication::translate("Planning", "nom Z \303\240 A", nullptr));
        tri_3->setItemText(3, QCoreApplication::translate("Planning", "age croissant", nullptr));
        tri_3->setItemText(4, QCoreApplication::translate("Planning", "age  d\303\251croissant", nullptr));

        label_22->setText(QCoreApplication::translate("Planning", "chercher", nullptr));
        pushButton_10->setText(QString());
        pushButton_11->setText(QString());
        pushButton_8->setText(QCoreApplication::translate("Planning", "Exporter", nullptr));
        label->setText(QString());
        groupBox_6->setTitle(QString());
        label_23->setText(QCoreApplication::translate("Planning", "Planification des s\303\251ances", nullptr));
        label_24->setText(QCoreApplication::translate("Planning", "Date s\303\251ance", nullptr));
        label_25->setText(QCoreApplication::translate("Planning", "type seance: ", nullptr));
        label_26->setText(QCoreApplication::translate("Planning", "heure debut :", nullptr));
        homme_3->setText(QCoreApplication::translate("Planning", "conduit", nullptr));
        femme_3->setText(QCoreApplication::translate("Planning", "code", nullptr));
        label_27->setText(QCoreApplication::translate("Planning", "nom condidat :", nullptr));
        label_29->setText(QCoreApplication::translate("Planning", "Adresse circuit :", nullptr));
        label_30->setText(QCoreApplication::translate("Planning", "Moniteur", nullptr));
        type_3->setItemText(0, QCoreApplication::translate("Planning", "choisir moniteur", nullptr));
        type_3->setItemText(1, QCoreApplication::translate("Planning", "mohamed", nullptr));
        type_3->setItemText(2, QCoreApplication::translate("Planning", "ali", nullptr));

        btn_ajout_3->setText(QCoreApplication::translate("Planning", "Ajouter", nullptr));
        btn_reset_3->setText(QCoreApplication::translate("Planning", "Annuler", nullptr));
        label_33->setText(QCoreApplication::translate("Planning", "heure fin :", nullptr));
        groupBox_3->setTitle(QString());
        candidat_3->setText(QCoreApplication::translate("Planning", "Candidat", nullptr));
        planning_3->setText(QCoreApplication::translate("Planning", "planning", nullptr));
        employee_3->setText(QCoreApplication::translate("Planning", "Employee", nullptr));
        vehicule_3->setText(QCoreApplication::translate("Planning", "vehicule", nullptr));
        examen_3->setText(QCoreApplication::translate("Planning", "Examen", nullptr));
        equipement_3->setText(QCoreApplication::translate("Planning", "equipement", nullptr));
        label_12->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Planning: public Ui_Planning {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLANNING_H
