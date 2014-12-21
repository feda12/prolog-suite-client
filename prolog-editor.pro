#-------------------------------------------------
#
# Project created by QtCreator 2014-12-18T00:04:44
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = prolog-editor
TEMPLATE = app

SOURCES += main.cpp\
        codeeditor.cpp \
    client.cpp \
    mainwindow.cpp

HEADERS  += codeeditor.h \
    client.h \
    mainwindow.h
