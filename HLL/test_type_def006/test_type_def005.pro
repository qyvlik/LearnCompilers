QT += core
QT -= gui

TARGET = test_type_def
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    typeparser.cpp \
    throwable.cpp \
    token.cpp \
    types/templatetypemetadatafactory.cpp \
    types/typeinstanciable.cpp \
    types/typemetadata.cpp \
    context/typesystem.cpp \
    types/buildin/stacktype.cpp \
    types/buildin/maptype.cpp \
    context/typesystemhelper.cpp

CONFIG += C++11

HEADERS += \
    typeparser.h \
    throwable.h \
    token.h \
    types/templatetypemetadatafactory.h \
    types/typeinstanciable.h \
    types/typemetadata.h \
    context/typesystem.h \
    types/buildin/stacktype.h \
    types/buildin/maptype.h \
    types/buildintype.h \
    context/typesystemhelper_p.h


