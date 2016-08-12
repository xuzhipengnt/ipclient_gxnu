#-------------------------------------------------
#
# Project created by QtCreator 2014-11-16T20:51:56
#
#-------------------------------------------------

QT       += core gui
QT      += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ipclient_qt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cafesclient.cpp

HEADERS  += mainwindow.h \
    cafesclient.h

FORMS    += mainwindow.ui

OTHER_FILES +=

RESOURCES += \
    back.qrc
win32{
RC_FILE += icon.rc
}
