#-------------------------------------------------
#
# Project created by QtCreator 2017-05-29T14:48:07
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QcanTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    bmsreceiver.cpp \
    ethernetcanreceiver.cpp \
    ethernetcansender.cpp

HEADERS  += mainwindow.h \
    bmsreceiver.h \
    ethernetcanreceiver.h \
    bmscanpacket.h \
    canpacket.h \
    canethernetframe.h \
    ethernetcansender.h

FORMS    += mainwindow.ui
