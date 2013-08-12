#-------------------------------------------------
#
# Project created by QtCreator 2013-07-11T09:08:34
#
#-------------------------------------------------

QT       += core gui webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pttreader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dirwidget.cpp \
    bbs2html.cpp \
    mywebview.cpp

HEADERS  += mainwindow.h pttstruct.h \
    fileheader.h \
    dirwidget.h \
    bbsrecord.h \
    bbs2html.h \
    mywebview.h \
    settings.h

FORMS    += mainwindow.ui
