QT += testlib

CONFIG += testcase

CONFIG += no_testcase_installs

DESTDIR = bin

include(math_tests.pri)

# math library
unix|win32: LIBS += -L$${OUT_PWD}/../$${DESTDIR} -lmath

INCLUDEPATH += $${PWD}/../include
DEPENDPATH += $${PWD}/../

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../$${DESTDIR}/math.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../$${DESTDIR}/libmath.a
