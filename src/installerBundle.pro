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

DEFINES += INSTALLERBUNDLE_LIBRARY

SOURCES += installerbundle.cpp

HEADERS += installerbundle.h


include(../plugin_template.pri)
