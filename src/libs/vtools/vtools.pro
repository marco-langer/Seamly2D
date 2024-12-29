#-------------------------------------------------
#
# Project created by QtCreator 2015-06-16T18:05:56
#
#-------------------------------------------------

# File with common stuff for whole project
message("Entering vtools.pro")
include(../../../common.pri)

QT       += widgets xml multimedia

# Name of the library
TARGET = vtools

# We want create library
TEMPLATE = lib

CONFIG += staticlib

include(vtools.pri)

# This is static library so no need in "make install"

# directory for executable file
DESTDIR = bin

# files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

# Directory for files created rcc
#RCC_DIR = rcc

# Directory for files created uic
UI_DIR = uic

INCLUDEPATH += $$PWD/../vpatterndb

include(warnings.pri)

include (../libs.pri)

# math library
unix|win32: LIBS += -L$${OUT_PWD}/../../libs/math/$${DESTDIR} -lmath

INCLUDEPATH += $${PWD}/../../libs/math/include
DEPENDPATH += $${PWD}/../../libs/math

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/math/$${DESTDIR}/math.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/math/$${DESTDIR}/libmath.a
