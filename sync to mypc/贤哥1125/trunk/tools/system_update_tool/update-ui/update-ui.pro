#-------------------------------------------------
#
# Project created by QtCreator 2014-01-18T00:45:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = update-ui
TEMPLATE = app
LIBS += -lts -lz

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    public.h

FORMS    += mainwindow.ui
