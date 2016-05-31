QT += core
QT -= gui

TARGET = statement001
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    src/statementparser.cpp

CONFIG += c++11

HEADERS += \
    src/lexerstream.hpp \
    src/utility.hpp \
    src/statementparser.h
