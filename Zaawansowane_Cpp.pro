#-------------------------------------------------
#
# Project created by QtCreator 2017-03-12T18:28:15
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
