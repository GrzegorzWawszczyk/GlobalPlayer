#-------------------------------------------------
#
# Project created by QtCreator 2016-09-29T08:58:17
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GlobalPlayer_GW
TEMPLATE = app


SOURCES += main.cpp\
    KeyObserver.cpp \
    Application.cpp \
    MainWindow.cpp \
    PlaylistModel.cpp \
    PlaylistView.cpp \
    VideoPlayer.cpp

HEADERS  += \
    KeyObserver.h \
    Application.h \
    MainWindow.h \
    PlaylistModel.h \
    PlaylistView.h \
    VideoPlayer.h

FORMS    += \
    mainwindow.ui
