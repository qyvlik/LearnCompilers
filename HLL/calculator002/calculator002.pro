QT += core
QT -= gui

TARGET = calculator002
CONFIG += console
CONFIG -= app_bundle

CONFIG += C++11

TEMPLATE = app

SOURCES += main.cpp \
    src/calculator.cpp \
    src/lexerstream.cpp \
    src/parser.cpp \
    src/ast.cpp

HEADERS += \
    src/calculator.h \
    src/utility.h \
    src/lexerstream.h \
    src/parser.h \
    src/ast.h

