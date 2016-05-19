QT += core
QT -= gui

TARGET = ast_001
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    throwable.cpp \
    token.cpp \
    typeparser.cpp \
    context/typesystem.cpp \
    context/typesystemhelper.cpp \
    types/templatetypemetadatafactory.cpp \
    types/typeinstanciable.cpp \
    types/typemetadata.cpp \
    types/buildin/maptype.cpp \
    types/buildin/stacktype.cpp \
    ast/visitor.cpp \
    ast/node.cpp \
    error/parsererror.cpp \
    error/typeerror.cpp \
    types/buildin/vectortype.cpp

CONFIG += C++11

HEADERS += \
    throwable.h \
    token.h \
    typeparser.h \
    context/typesystem.h \
    context/typesystemhelper_p.h \
    types/buildintype.h \
    types/templatetypemetadatafactory.h \
    types/typeinstanciable.h \
    types/typemetadata.h \
    types/buildin/maptype.h \
    types/buildin/stacktype.h \
    ast/visitor.h \
    ast/node.h \
    error/parsererror.h \
    error/typeerror.h \
    types/buildin/vectortype.h
