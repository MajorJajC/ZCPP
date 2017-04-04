#-------------------------------------------------
#
# Project created by QtCreator 2016-02-26T11:43:03
#
#-------------------------------------------------

QT       += core gui script widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Zaawansowane_Cpp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    expressionbuilder.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    nodes.h \
    expressionbuilder.h \
    qcustomplot.h

FORMS    += mainwindow.ui
