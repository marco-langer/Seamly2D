QT += testlib

CONFIG += testcase

CONFIG += no_testcase_installs

DESTDIR = bin

include(core_utils_tests.pri)

# core-utils library (header-only)
INCLUDEPATH += $$PWD/../../core_utils/include
DEPENDPATH += $$PWD/../../core_utils/include
