QT += core

TEMPLATE = lib

CONFIG += staticlib

include(geometry.pri)

# directory for executable file
DESTDIR = bin

# files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

# math library
unix|win32: LIBS += -L$$OUT_PWD/../math/$${DESTDIR}/ -lmath

INCLUDEPATH += $$PWD/../math/include
DEPENDPATH += $$PWD/../math

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../math/$${DESTDIR}/math.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../math/$${DESTDIR}/libmath.a
