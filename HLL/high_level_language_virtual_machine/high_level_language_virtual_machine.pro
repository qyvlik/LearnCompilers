QT += core
QT -= gui

TARGET = high_level_language_virtual_machine
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    src/registerbasevirtualmachine.hpp \
    src/stackbasevirtualmachine.hpp


CONFIG += c++11
