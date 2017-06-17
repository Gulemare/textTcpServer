QT += core network
QT -= gui

CONFIG += c++11

#QMAKE_LFLAGS_RELEASE += -static -static-libgcc

TARGET = TextServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    textserver.cpp \
    textserver_thread.cpp

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    textserver.h \
    textserver_communication_protocol.h \
    textserver_thread.h
