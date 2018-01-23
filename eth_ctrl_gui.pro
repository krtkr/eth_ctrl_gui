#-------------------------------------------------
#
# Project created by QtCreator 2016-12-04T16:11:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = eth_ctrl_gui
TEMPLATE = app

INCLUDEPATH += include

SOURCES += main.cpp\
    mainwindow.cpp \
    eth_ctrl.c \
    OS/linux/Mutex.cpp \
    OS/linux/Thread.cpp \
    OS/linux/Timer.cpp \
    ethctrl.cpp

HEADERS  += mainwindow.h \
    eth_ctrl.h \
    include/OS/Mutex.hpp \
    include/OS/Thread.hpp \
    include/OS/Timer.hpp \
    include/Common/SharedBuffer.hpp \
    ethctrl.hpp \
    include/Common/AbstractModule.hpp

FORMS    += mainwindow.ui
