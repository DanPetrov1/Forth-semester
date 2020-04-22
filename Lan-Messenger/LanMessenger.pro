QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LanMessenger
TEMPLATE = app

CONFIG += release
QMAKE_CXXFLAGS += -std=c++0x

# STATIC LINKING
CONFIG += static
QMAKE_CXXFLAGS += -static -static-libgcc -static-libstdc++
QMAKE_LFLAGS   += -static -static-libgcc -static-libstdc++

SOURCES += main.cpp\
        mainwindow.cpp \
    dialognickname.cpp \
    messenger.cpp \
    peer.cpp \
    pmwindow.cpp \
    roomwindow.cpp \
    dialogroom.cpp

HEADERS  += mainwindow.h \
    dialognickname.h \
    messenger.h \
    peer.h \
    pmwindow.h \
    roomwindow.h \
    dialogroom.h

FORMS    += mainwindow.ui \
    dialognickname.ui \
    pmwindow.ui \
    roomwindow.ui \
    dialogroom.ui

DISTFILES += \
    resources/kovalchuk.jpg \
    resources/kursach.jpg \
    resources/lick.jpg \
    resources/love-lick.jpg \
    resources/smile.jpg
