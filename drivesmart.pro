QT += core gui widgets sql network printsupport charts multimedia serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17 console
CONFIG -= c++1z
CONFIG += qt
CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++17

TARGET = DriveSmart
TEMPLATE = app

HEADERS += \
    ../push metier avancee/qrcodegen.h \
    arduino-integ.h \
    employes.h \
    logindialog.h \
    mainwindow.h \
    condidat.h \
    pageemploye.h \
    qrcodegen.h \
    randomstringgenerator.h \
    smtp.h \
    vehicule.h \
    examen.h \
    planning.h \
    gestion_equipement.h\
    connection.h \
    navigation_constants.h \
    vendor/qrcodegen.hpp

SOURCES += \
    arduino-integ.cpp \
    employes.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp\
    condidat.cpp \
    pageemploye.cpp \
    smtp.cpp \
    vehicule.cpp \
    examen.cpp \
    planning.cpp \
    gestion_equipement.cpp\
    connection.cpp \
    vendor/qrcodegen.cpp

FORMS += \
    Dialog.ui \
    condidat.ui \
    examen.ui \
    gestion_equipement.ui \
    mainwindow.ui \
    pageemploye.ui \
    planning.ui \
    vehicule.ui

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
