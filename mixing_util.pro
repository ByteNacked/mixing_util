#-------------------------------------------------
#
# Project created by QtCreator 2016-10-09T04:20:45
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mixing_util
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    loadfile.cpp \
    mixer.cpp \
    playsound.cpp \
    generator.cpp \
    decoder.cpp \
    createfile.cpp \
    utils.cpp \
    wavsaver.cpp

HEADERS  += mainwindow.h \
    loadfile.h \
    mixer.h \
    playsound.h \
    generator.h \
    decoder.h \
    createfile.h \
    utils.h \
    wavsaver.h

FORMS    += mainwindow.ui
CONFIG   += c++11
