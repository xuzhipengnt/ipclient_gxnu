#-------------------------------------------------
#
# Project created by QtCreator 2016-08-11T20:48:05
#
#-------------------------------------------------

QT       += core gui
QT      += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = macopen
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
 win32{
RC_FILE += icon.rc
}

RESOURCES += \
    icon.qrc


