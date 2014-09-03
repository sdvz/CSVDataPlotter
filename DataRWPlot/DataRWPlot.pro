#-------------------------------------------------
#
# Project created by QtCreator 2014-09-02T22:56:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = DataRWPlot
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    qcustomplot.cpp

HEADERS  += dialog.h \
    qcustomplot.h

FORMS    += dialog.ui
