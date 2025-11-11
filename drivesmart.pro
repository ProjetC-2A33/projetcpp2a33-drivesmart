QT += core gui sql widgets

# CONFIG += console  # Retiré pour éviter le conflit WinMain avec les applications GUI

TARGET = DriveSmart
TEMPLATE = app

HEADERS += \
    Login.h \
    connection.h \
    mainwindow.h \
    condidat.h \
    vehicule.h \
    examen.h \
    planning.h \
    gestion_equipement.h

SOURCES += \
    connection.cpp \
    main.cpp \
    Login.cpp \
    mainwindow.cpp \
    condidat.cpp \
    vehicule.cpp \
    examen.cpp \
    planning.cpp \
    gestion_equipement.cpp

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

# Configuration pour éviter les problèmes de répertoire de build
CONFIG += c++17

# Activer la console uniquement en mode debug pour voir les messages de connexion
debug {
    CONFIG += console
}

# Pour Oracle ODBC - ajustez ces chemins selon votre installation Oracle
# Décommentez et ajustez si nécessaire
# INCLUDEPATH += "C:/oracle/instantclient_XX_X/oci/include"
# LIBS += -L"C:/oracle/instantclient_XX_X/oci/lib/msvc" -loci
