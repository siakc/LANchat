#-------------------------------------------------
#
# Project created by QtCreator 2011-12-08T16:51:42
#
#-------------------------------------------------

QT       += core gui

TARGET = LANchat
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
        conmngr.cpp

HEADERS  += dialog.h \
    conmngr.h \
    buffer.h

FORMS    += dialog.ui

LIBS     += QtNetworkd4.lib \
        QtCored4.lib



