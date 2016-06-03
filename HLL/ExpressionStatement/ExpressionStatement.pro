QT += core
QT -= gui

TARGET = ExpressionStatement
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    src/expressionparser.cpp

HEADERS += \
    src/utility.hpp \
    src/tokenstream.hpp \
    src/expressionparser.h

DISTFILES += \
    readme.md

CONFIG += c++11
