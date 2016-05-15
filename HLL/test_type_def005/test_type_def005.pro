QT += core
QT -= gui

TARGET = test_type_def
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    typeparser.cpp \
    throwable.cpp \
    token.cpp

CONFIG += C++11

HEADERS += \
    typeparser.h \
    throwable.h \
    token.h

