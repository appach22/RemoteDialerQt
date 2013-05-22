#-------------------------------------------------
#
# Project created by QtCreator 2013-03-19T01:36:04
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RemoteDialer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    remotedevice.cpp \
    remotedialerdevices.cpp \
    devicedelegate.cpp

HEADERS  += mainwindow.h \
    remotedevice.h \
    remotedialerdevices.h \
    devicedelegate.h

FORMS    += mainwindow.ui

OTHER_FILES +=
