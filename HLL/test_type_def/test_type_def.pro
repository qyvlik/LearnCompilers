QT += core
QT -= gui

TARGET = test_type_def
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    typeparser.cpp \
    lexer.cpp \
    src/type/value/valueproxy.cpp \
    src/type/instantiable/typeinstantiable.cpp \
    src/type/instantiable/boolinstantiable.cpp \
    src/type/instantiable/int8instantiable.cpp \
    src/type/instantiable/int16instantiable.cpp \
    src/type/instantiable/int32instantiable.cpp \
    src/type/instantiable/int64instantiable.cpp \
    src/type/instantiable/doubleinstantiable.cpp \
    src/type/instantiable/stringinstantiable.cpp \
    src/type/instantiable/stackinstantiable.cpp \
    src/type/instantiable/vectorinstantiable.cpp \
    src/type/value/value.cpp \
    src/type/type/type.cpp \
    src/type/type/templatetype.cpp \
    src/type/type/int32type.cpp \
    src/type/type/vectortype.cpp \
    src/throwable/throwable.cpp \
    src/type/type/int8type.cpp \
    src/type/type/booltype.cpp \
    src/type/type/int16type.cpp \
    src/type/type/int64type.cpp \
    src/type/type/doubletype.cpp \
    src/type/type/stringtype.cpp \
    src/type/type/stacktype.cpp

CONFIG += C++11

HEADERS += \
    typeparser.h \
    lexer.h \
    src/type/value/valueproxy.h \
    src/type/instantiable/typeinstantiable.h \
    src/type/instantiable/boolinstantiable.h \
    src/type/instantiable/int8instantiable.h \
    src/type/instantiable/int16instantiable.h \
    src/type/instantiable/int32instantiable.h \
    src/type/instantiable/int64instantiable.h \
    src/type/instantiable/doubleinstantiable.h \
    src/type/instantiable/stringinstantiable.h \
    src/type/instantiable/stackinstantiable.h \
    src/type/instantiable/vectorinstantiable.h \
    src/type/value/value.h \
    src/type/type/type.h \
    src/type/type/templatetype.h \
    src/type/type/int32type.h \
    src/type/type/vectortype.h \
    src/throwable/throwable.h \
    src/type/type/int8type.h \
    src/type/type/booltype.h \
    src/type/type/int16type.h \
    src/type/type/int64type.h \
    src/type/type/doubletype.h \
    src/type/type/stringtype.h \
    src/type/type/stacktype.h

