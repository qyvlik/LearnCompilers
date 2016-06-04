QT += core
QT -= gui

TARGET = Language
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp


CONFIG += c++11

DISTFILES += \
    readme.md

HEADERS += \
    src/tokenstream.hpp \
    src/utility.hpp \
    src/parser.hpp \
    src/calleetracker.hpp \
    src/throwable.hpp
