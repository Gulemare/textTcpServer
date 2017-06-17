QT       += core gui network

#QMAKE_LFLAGS_RELEASE += -static -static-libgcc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TextClient
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        textclient.cpp

HEADERS  += textclient.h
FORMS    += textclient.ui
