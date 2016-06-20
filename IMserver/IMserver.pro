#-------------------------------------------------
#
# Project created by QtCreator 2016-04-19T22:44:00
#
#-------------------------------------------------

QT    +=  core
QT    +=  gui
QT    += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IMserver
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    clientsocket.cpp \
    server.cpp \
    filedata.cpp

HEADERS  += widget.h \
    clientsocket.h \
    server.h \
    constants.h \
    filedata.h

FORMS    += widget.ui

OTHER_FILES += \
    AccountInfo
