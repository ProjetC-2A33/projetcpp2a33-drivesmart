/********************************************************************************
** Form generated from reading UI file 'gestion_equipement.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GESTION_EQUIPEMENT_H
#define UI_GESTION_EQUIPEMENT_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Gestion_Equipement
{
public:
    QWidget *centralwidget;
    QGroupBox *groupBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QTextEdit *id_equipement;
    QTextEdit *nom_equipement;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QPushButton *btn_ajout;
    QPushButton *btn_reset;
    QComboBox *comboBox_equipement;
    QSpinBox *spinBox_equipement;
    QSpinBox *spinBox_equipement1;
    QComboBox *comboBox;
    QLabel *label_9;
    QDateEdit *dateEdit;
    QTextEdit *nom_equipement_2;
    QGroupBox *groupBox_2;
    QTableWidget *tab_2;
    QLabel *label_11;
    QComboBox *tri_2;
    QTextEdit *recherche_2;
    QLabel *label_14;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_17;
    QGroupBox *groupBox_3;
    QPushButton *btnUpdateMaintenance;
    QLabel *label_12;
    QProgressBar *progressBar;
    QGroupBox *groupBox_4;
    QPushButton *btnCheckWeather;
    QLabel *labelVille;
    QTextEdit *lineEditVille;
    QLabel *labelWeatherResult;
    QGroupBox *groupBox_5;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *candidat;
    QPushButton *planning;
    QPushButton *employee;
    QPushButton *vehicule;
    QPushButton *examen;
    QPushButton *equipement;
    QLabel *label_13;
    QStatusBar *statusbar;

    void setupUi(QWidget *Gestion_Equipement)
    {
        if (Gestion_Equipement->objectName().isEmpty())
            Gestion_Equipement->setObjectName("Gestion_Equipement");
        Gestion_Equipement->resize(1538, 903);
        centralwidget = new QWidget(Gestion_Equipement);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setGeometry(QRect(0, 0, 1581, 909));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(10, 78, 421, 761));
        groupBox->setStyleSheet(QString::fromUtf8("border-radius:50px;\n"
"background-color:#e3e2de;"));
        label = new QLabel(groupBox);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 10, 331, 61));
        QFont font;
        font.setPointSize(24);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("color:#002157\n"
""));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 80, 121, 31));
        QFont font1;
        font1.setPointSize(14);
        label_2->setFont(font1);
        label_2->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(20, 130, 121, 31));
        label_3->setFont(font1);
        label_3->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(20, 180, 121, 31));
        label_4->setFont(font1);
        label_4->setStyleSheet(QString::fromUtf8("color:#002157"));
        id_equipement = new QTextEdit(groupBox);
        id_equipement->setObjectName("id_equipement");
        id_equipement->setGeometry(QRect(200, 80, 211, 41));
        id_equipement->setFont(font1);
        id_equipement->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
"\n"
"\n"
""));
        id_equipement->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
        id_equipement->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
        id_equipement->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        id_equipement->setReadOnly(true);
        nom_equipement = new QTextEdit(groupBox);
        nom_equipement->setObjectName("nom_equipement");
        nom_equipement->setGeometry(QRect(200, 130, 211, 41));
        nom_equipement->setFont(font1);
        nom_equipement->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;"));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(20, 240, 151, 31));
        label_5->setFont(font1);
        label_5->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(20, 290, 211, 31));
        label_6->setFont(font1);
        label_6->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(20, 350, 141, 31));
        label_7->setFont(font1);
        label_7->setStyleSheet(QString::fromUtf8("color:#002157"));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(20, 400, 181, 31));
        label_8->setFont(font1);
        label_8->setStyleSheet(QString::fromUtf8("color:#002157"));
        btn_ajout = new QPushButton(groupBox);
        btn_ajout->setObjectName("btn_ajout");
        btn_ajout->setGeometry(QRect(30, 540, 171, 61));
        QFont font2;
        font2.setPointSize(16);
        btn_ajout->setFont(font2);
        btn_ajout->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        btn_reset = new QPushButton(groupBox);
        btn_reset->setObjectName("btn_reset");
        btn_reset->setGeometry(QRect(220, 540, 171, 61));
        btn_reset->setFont(font2);
        btn_reset->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        comboBox_equipement = new QComboBox(groupBox);
        comboBox_equipement->setObjectName("comboBox_equipement");
        comboBox_equipement->setGeometry(QRect(210, 180, 201, 31));
        spinBox_equipement = new QSpinBox(groupBox);
        spinBox_equipement->setObjectName("spinBox_equipement");
        spinBox_equipement->setGeometry(QRect(200, 240, 211, 31));
        spinBox_equipement->setMaximum(10);
        spinBox_equipement1 = new QSpinBox(groupBox);
        spinBox_equipement1->setObjectName("spinBox_equipement1");
        spinBox_equipement1->setGeometry(QRect(201, 290, 211, 31));
        comboBox = new QComboBox(groupBox);
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(200, 350, 211, 31));
        comboBox->setEditable(false);
        comboBox->setMaxVisibleItems(3);
        label_9 = new QLabel(groupBox);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(20, 460, 181, 31));
        label_9->setFont(font1);
        label_9->setStyleSheet(QString::fromUtf8("color:#002157"));
        dateEdit = new QDateEdit(groupBox);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setGeometry(QRect(200, 460, 211, 31));
        nom_equipement_2 = new QTextEdit(groupBox);
        nom_equipement_2->setObjectName("nom_equipement_2");
        nom_equipement_2->setGeometry(QRect(200, 400, 211, 41));
        nom_equipement_2->setFont(font1);
        nom_equipement_2->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;"));
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(450, 81, 1041, 411));
        tab_2 = new QTableWidget(groupBox_2);
        if (tab_2->columnCount() < 8)
            tab_2->setColumnCount(8);
        QFont font3;
        font3.setPointSize(12);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font3);
        tab_2->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font3);
        tab_2->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setFont(font3);
        tab_2->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setFont(font3);
        tab_2->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setFont(font3);
        tab_2->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setFont(font3);
        tab_2->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        __qtablewidgetitem6->setFont(font3);
        tab_2->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tab_2->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        if (tab_2->rowCount() < 15)
            tab_2->setRowCount(15);
        tab_2->setObjectName("tab_2");
        tab_2->setGeometry(QRect(10, 93, 1001, 301));
        tab_2->setFont(font1);
        tab_2->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
        tab_2->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        tab_2->setAutoScroll(true);
        tab_2->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
        tab_2->setVerticalScrollMode(QAbstractItemView::ScrollMode::ScrollPerItem);
        tab_2->setShowGrid(true);
        tab_2->setSortingEnabled(false);
        tab_2->setCornerButtonEnabled(true);
        tab_2->setRowCount(15);
        tab_2->horizontalHeader()->setVisible(true);
        tab_2->horizontalHeader()->setCascadingSectionResizes(false);
        tab_2->horizontalHeader()->setMinimumSectionSize(40);
        tab_2->horizontalHeader()->setDefaultSectionSize(125);
        tab_2->horizontalHeader()->setHighlightSections(true);
        tab_2->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tab_2->horizontalHeader()->setStretchLastSection(true);
        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(10, 25, 91, 31));
        label_11->setFont(font1);
        label_11->setStyleSheet(QString::fromUtf8("color:#002157"));
        tri_2 = new QComboBox(groupBox_2);
        tri_2->addItem(QString());
        tri_2->addItem(QString());
        tri_2->addItem(QString());
        tri_2->addItem(QString());
        tri_2->addItem(QString());
        tri_2->setObjectName("tri_2");
        tri_2->setGeometry(QRect(95, 20, 211, 41));
        tri_2->setFont(font1);
        tri_2->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
""));
        recherche_2 = new QTextEdit(groupBox_2);
        recherche_2->setObjectName("recherche_2");
        recherche_2->setGeometry(QRect(450, 20, 301, 41));
        recherche_2->setFont(font1);
        recherche_2->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;\n"
"\n"
"\n"
""));
        recherche_2->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
        recherche_2->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
        recherche_2->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        label_14 = new QLabel(groupBox_2);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(350, 20, 101, 31));
        label_14->setFont(font1);
        label_14->setStyleSheet(QString::fromUtf8("color:#002157"));
        pushButton_3 = new QPushButton(groupBox_2);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(780, 30, 41, 31));
        QIcon icon;
        if (QIcon::hasThemeIcon(QIcon::ThemeIcon::EditDelete)) {
            icon = QIcon::fromTheme(QIcon::ThemeIcon::EditDelete);
        } else {
            icon.addFile(QString::fromUtf8("E:/daraj/downloads"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        }
        pushButton_3->setIcon(icon);
        pushButton_4 = new QPushButton(groupBox_2);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(830, 30, 41, 31));
        QIcon icon1;
        if (QIcon::hasThemeIcon(QIcon::ThemeIcon::MailMessageNew)) {
            icon1 = QIcon::fromTheme(QIcon::ThemeIcon::MailMessageNew);
        } else {
            icon1.addFile(QString::fromUtf8("E:/daraj/downloads"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        }
        pushButton_4->setIcon(icon1);
        pushButton_17 = new QPushButton(groupBox_2);
        pushButton_17->setObjectName("pushButton_17");
        pushButton_17->setGeometry(QRect(890, 20, 141, 41));
        pushButton_17->setFont(font1);
        pushButton_17->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        QIcon icon2;
        if (QIcon::hasThemeIcon(QIcon::ThemeIcon::WindowNew)) {
            icon2 = QIcon::fromTheme(QIcon::ThemeIcon::WindowNew);
        } else {
            icon2.addFile(QString::fromUtf8("E:/daraj/downloads"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        }
        pushButton_17->setIcon(icon2);
        pushButton_17->setIconSize(QSize(20, 20));
        groupBox_3 = new QGroupBox(groupBox_2);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(90, 480, 311, 281));
        groupBox_3->setAlignment(Qt::AlignmentFlag::AlignCenter);
        groupBox_3->setFlat(false);
        groupBox_3->setCheckable(false);
        btnUpdateMaintenance = new QPushButton(groupBox_3);
        btnUpdateMaintenance->setObjectName("btnUpdateMaintenance");
        btnUpdateMaintenance->setGeometry(QRect(10, 60, 281, 61));
        btnUpdateMaintenance->setFont(font2);
        btnUpdateMaintenance->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(20, 150, 271, 31));
        QFont font4;
        font4.setFamilies({QString::fromUtf8("Montserrat Medium")});
        font4.setPointSize(14);
        font4.setBold(false);
        font4.setItalic(true);
        label_12->setFont(font4);
        label_12->setTextFormat(Qt::TextFormat::AutoText);
        label_12->setAlignment(Qt::AlignmentFlag::AlignCenter);
        progressBar = new QProgressBar(groupBox_3);
        progressBar->setObjectName("progressBar");
        progressBar->setGeometry(QRect(50, 210, 221, 31));
        progressBar->setValue(24);
        groupBox_4 = new QGroupBox(groupBox_2);
        groupBox_4->setObjectName("groupBox_4");
        groupBox_4->setGeometry(QRect(470, 480, 311, 281));
        groupBox_4->setAlignment(Qt::AlignmentFlag::AlignCenter);
        groupBox_4->setFlat(false);
        groupBox_4->setCheckable(false);
        btnCheckWeather = new QPushButton(groupBox_4);
        btnCheckWeather->setObjectName("btnCheckWeather");
        btnCheckWeather->setGeometry(QRect(70, 140, 171, 61));
        btnCheckWeather->setFont(font2);
        btnCheckWeather->setStyleSheet(QString::fromUtf8("background-color:#ffce00;\n"
"color:#002157;\n"
"border-radius:10px;"));
        labelVille = new QLabel(groupBox_4);
        labelVille->setObjectName("labelVille");
        labelVille->setGeometry(QRect(20, 30, 271, 31));
        labelVille->setFont(font4);
        labelVille->setTextFormat(Qt::TextFormat::AutoText);
        labelVille->setAlignment(Qt::AlignmentFlag::AlignCenter);
        lineEditVille = new QTextEdit(groupBox_4);
        lineEditVille->setObjectName("lineEditVille");
        lineEditVille->setGeometry(QRect(50, 80, 211, 41));
        lineEditVille->setFont(font1);
        lineEditVille->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"border-radius:5px;\n"
"border:2px solid #ffce00;"));
        labelWeatherResult = new QLabel(groupBox_4);
        labelWeatherResult->setObjectName("labelWeatherResult");
        labelWeatherResult->setGeometry(QRect(30, 220, 271, 31));
        labelWeatherResult->setFont(font4);
        labelWeatherResult->setTextFormat(Qt::TextFormat::AutoText);
        labelWeatherResult->setAlignment(Qt::AlignmentFlag::AlignCenter);
        groupBox_5 = new QGroupBox(centralwidget);
        groupBox_5->setObjectName("groupBox_5");
        groupBox_5->setGeometry(QRect(10, 10, 1471, 61));
        groupBox_5->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;\n"
"border-radius:10px;"));
        horizontalLayoutWidget = new QWidget(groupBox_5);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(440, 0, 931, 61));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        candidat = new QPushButton(horizontalLayoutWidget);
        candidat->setObjectName("candidat");
        QFont font5;
        font5.setPointSize(11);
        font5.setBold(true);
        candidat->setFont(font5);
        candidat->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(candidat);

        planning = new QPushButton(horizontalLayoutWidget);
        planning->setObjectName("planning");
        planning->setFont(font5);
        planning->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(planning);

        employee = new QPushButton(horizontalLayoutWidget);
        employee->setObjectName("employee");
        employee->setFont(font5);
        employee->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(employee);

        vehicule = new QPushButton(horizontalLayoutWidget);
        vehicule->setObjectName("vehicule");
        vehicule->setFont(font5);
        vehicule->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(vehicule);

        examen = new QPushButton(horizontalLayoutWidget);
        examen->setObjectName("examen");
        examen->setFont(font5);
        examen->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(examen);

        equipement = new QPushButton(horizontalLayoutWidget);
        equipement->setObjectName("equipement");
        equipement->setFont(font5);
        equipement->setStyleSheet(QString::fromUtf8("background-color:#e3e2de;"));

        horizontalLayout->addWidget(equipement);

        label_13 = new QLabel(groupBox_5);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(20, -10, 91, 91));
        label_13->setPixmap(QPixmap(QString::fromUtf8(":/images/logo.png")));
        label_13->setScaledContents(true);
        statusbar = new QStatusBar(Gestion_Equipement);
        statusbar->setObjectName("statusbar");
        statusbar->setGeometry(QRect(0, 0, 16, 21));

        retranslateUi(Gestion_Equipement);

        QMetaObject::connectSlotsByName(Gestion_Equipement);
    } // setupUi

    void retranslateUi(QWidget *Gestion_Equipement)
    {
        Gestion_Equipement->setWindowTitle(QCoreApplication::translate("Gestion_Equipement", "Gestion_Equipement", nullptr));
        groupBox->setTitle(QString());
        label->setText(QCoreApplication::translate("Gestion_Equipement", "Equipement", nullptr));
        label_2->setText(QCoreApplication::translate("Gestion_Equipement", "ID :", nullptr));
        label_3->setText(QCoreApplication::translate("Gestion_Equipement", "Nom :", nullptr));
        label_4->setText(QCoreApplication::translate("Gestion_Equipement", "cat\303\251gorie :", nullptr));
        label_5->setText(QCoreApplication::translate("Gestion_Equipement", "quantit\303\251 totale :", nullptr));
        label_6->setText(QCoreApplication::translate("Gestion_Equipement", "quantit\303\251 disponible :", nullptr));
        label_7->setText(QCoreApplication::translate("Gestion_Equipement", "\303\251tat :", nullptr));
        label_8->setText(QCoreApplication::translate("Gestion_Equipement", "Fournisseur  :", nullptr));
        btn_ajout->setText(QCoreApplication::translate("Gestion_Equipement", "Ajouter", nullptr));
        btn_reset->setText(QCoreApplication::translate("Gestion_Equipement", "Annuler", nullptr));
        comboBox->setCurrentText(QString());
        label_9->setText(QCoreApplication::translate("Gestion_Equipement", "Date acquisition :", nullptr));
        groupBox_2->setTitle(QString());
        QTableWidgetItem *___qtablewidgetitem = tab_2->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Gestion_Equipement", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tab_2->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Gestion_Equipement", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tab_2->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("Gestion_Equipement", "Cat\303\251gorie", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tab_2->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("Gestion_Equipement", "Quantit\303\251 totale", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tab_2->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("Gestion_Equipement", "Quantit\303\251 disponible", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tab_2->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("Gestion_Equipement", "\303\251tat", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tab_2->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("Gestion_Equipement", "Fournisseur", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tab_2->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("Gestion_Equipement", "Date acquisition", nullptr));
        label_11->setText(QCoreApplication::translate("Gestion_Equipement", "Tri par : ", nullptr));
        tri_2->setItemText(0, QCoreApplication::translate("Gestion_Equipement", "--------------", nullptr));
        tri_2->setItemText(1, QCoreApplication::translate("Gestion_Equipement", "nom A \303\240 Z ", nullptr));
        tri_2->setItemText(2, QCoreApplication::translate("Gestion_Equipement", "nom Z \303\240 A", nullptr));
        tri_2->setItemText(3, QCoreApplication::translate("Gestion_Equipement", "age croissant", nullptr));
        tri_2->setItemText(4, QCoreApplication::translate("Gestion_Equipement", "age  d\303\251croissant", nullptr));

        label_14->setText(QCoreApplication::translate("Gestion_Equipement", "chercher", nullptr));
        pushButton_3->setText(QString());
        pushButton_4->setText(QString());
        pushButton_17->setText(QCoreApplication::translate("Gestion_Equipement", "Exporter", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("Gestion_Equipement", "Statut Maintenance", nullptr));
        btnUpdateMaintenance->setText(QCoreApplication::translate("Gestion_Equipement", "Actualiser Statut Maintenance", nullptr));
        label_12->setText(QCoreApplication::translate("Gestion_Equipement", "Statut : \303\240 jour / \303\240 v\303\251rifier", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("Gestion_Equipement", "M\303\251t\303\251o actuelle", nullptr));
        btnCheckWeather->setText(QCoreApplication::translate("Gestion_Equipement", "V\303\251rifier M\303\251t\303\251o", nullptr));
        labelVille->setText(QCoreApplication::translate("Gestion_Equipement", "Ville ", nullptr));
        labelWeatherResult->setText(QCoreApplication::translate("Gestion_Equipement", "Temp\303\251rature actuelle :", nullptr));
        groupBox_5->setTitle(QString());
        candidat->setText(QCoreApplication::translate("Gestion_Equipement", "Candidat", nullptr));
        planning->setText(QCoreApplication::translate("Gestion_Equipement", "planning", nullptr));
        employee->setText(QCoreApplication::translate("Gestion_Equipement", "Employee", nullptr));
        vehicule->setText(QCoreApplication::translate("Gestion_Equipement", "vehicule", nullptr));
        examen->setText(QCoreApplication::translate("Gestion_Equipement", "Examen", nullptr));
        equipement->setText(QCoreApplication::translate("Gestion_Equipement", "equipement", nullptr));
        label_13->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Gestion_Equipement: public Ui_Gestion_Equipement {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GESTION_EQUIPEMENT_H
