QT += core

TEMPLATE = lib

CONFIG += staticlib c++17

include(geometry.pri)

# directory for executable file
DESTDIR = bin

# files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj
