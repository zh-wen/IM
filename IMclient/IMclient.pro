#-------------------------------------------------
#
# Project created by QtCreator 2016-04-20T16:35:23
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IMclient
TEMPLATE = app


SOURCES += main.cpp\
        loginwindow.cpp \
    connect.cpp \
    window.cpp \
    friendbutton.cpp \
    chatwindow.cpp \
    registerwin.cpp

HEADERS  += loginwindow.h \
    constants.h \
    connect.h \
    window.h \
    friendbutton.h \
    chatwindow.h \
    registerwin.h

FORMS    += loginwindow.ui \
    window.ui \
    chatwindow.ui \
    registerwin.ui
