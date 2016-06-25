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
    src/parser/lexer.hpp \
    src/parser/parser.hpp \
    src/parser/parsercontext.hpp \
    src/parser/token.hpp \
    src/parser/tokenstream.hpp \
    src/throwable/throwable.hpp \
    src/utility/calleetracker.hpp \
    src/utility/utility.hpp
