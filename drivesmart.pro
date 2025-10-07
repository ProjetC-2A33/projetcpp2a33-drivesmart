QT += core gui widgets

TARGET = DriveSmart
TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp \
           Login.cpp

HEADERS += mainwindow.h \
           Login.h

FORMS += mainwindow.ui \
         Login.ui

RESOURCES += resources.qrc \

CONFIG += console
