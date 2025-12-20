QT += core gui widgets sql network printsupport charts serialport
# QT += multimedia  # Commented out - not available in this Qt installation

CONFIG += c++17 console
CONFIG -= c++1z
CONFIG += qt

QMAKE_CXXFLAGS += -std=c++17

TARGET = DriveSmart
TEMPLATE = app

HEADERS += \
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
    vendor/qrcodegen.hpp \
    arduino.h \
    cin_access_control.h \
    google_calendar_service.h \
    openstreetmap_service.h \
    local_calendar.h

SOURCES += \
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
    vendor/qrcodegen.cpp \
    arduino.cpp \
    cin_access_control.cpp \
    google_calendar_service.cpp \
    openstreetmap_service.cpp \
    local_calendar.cpp

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
