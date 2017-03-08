#-------------------------------------------------
#
# Project created by QtCreator 2017-03-07T22:07:16
#
#-------------------------------------------------

QT       += core gui

CONFIG   += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CmdRunner
TEMPLATE = app


SOURCES += main.cpp \
    maintray.cpp \
    command.cpp \
    mainwidget.cpp

HEADERS  += \
    maintray.h \
    command.h \
    mainwidget.h

FORMS    +=

RESOURCES += \
    logo.qrc
