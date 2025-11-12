QT += core gui widgets sql

CONFIG += c++17 console
CONFIG += qt

TARGET = DriveSmart
TEMPLATE = app

HEADERS += \
    Login.h \
    mainwindow.h \
    condidat.h \
    vehicule.h \
    examen.h \
    planning.h \
    gestion_equipement.h\
    connection.h \
    navigation_constants.h

SOURCES += \
    main.cpp \
    Login.cpp \
    mainwindow.cpp\
    condidat.cpp \
    vehicule.cpp \
    examen.cpp \
    planning.cpp \
    gestion_equipement.cpp\
    connection.cpp

FORMS += \
    Login.ui \
    condidat.ui \
    examen.ui \
    gestion_equipement.ui \
    mainwindow.ui \
    planning.ui \
    vehicule.ui

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
