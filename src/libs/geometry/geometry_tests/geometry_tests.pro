QT += testlib

CONFIG += testcase

CONFIG += no_testcase_installs

DESTDIR = bin

include(geometry_tests.pri)

# geometry library
unix|win32: LIBS += -L$${OUT_PWD}/../$${DESTDIR} -lgeometry

INCLUDEPATH += $${PWD}/../include
DEPENDPATH += $${PWD}/../

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../$${DESTDIR}/geometry.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../$${DESTDIR}/libgeometry.a
