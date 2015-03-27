#-------------------------------------------------
#
# Project created by QtCreator 2013-01-03T13:31:41
#
#-------------------------------------------------

QT       -= gui

TARGET = installerBundle
TEMPLATE = lib

CONFIG += plugins
CONFIG += dll

CONFIG(release, debug|release) {
  QMAKE_CXXFLAGS += /Zi
  QMAKE_LFLAGS += /DEBUG
}

DEFINES += INSTALLERBUNDLE_LIBRARY

SOURCES += installerbundle.cpp

HEADERS += installerbundle.h


include(../plugin_template.pri)

OTHER_FILES += \
    installerbundle.json\
    SConscript
