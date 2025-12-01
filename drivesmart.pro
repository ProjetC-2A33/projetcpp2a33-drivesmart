QT += core gui widgets sql network printsupport charts

CONFIG += c++17 console
CONFIG -= c++1z
CONFIG += qt

QMAKE_CXXFLAGS += -std=c++17

TARGET = DriveSmart
TEMPLATE = app

HEADERS += \
    Login.h \
    mainwindow.h \
    condidat.h \
    employee.h \
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
    employee.cpp \
    vehicule.cpp \
    examen.cpp \
    planning.cpp \
    gestion_equipement.cpp\
    connection.cpp

FORMS += \
    Login.ui \
    condidat.ui \
    employee.ui \
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
