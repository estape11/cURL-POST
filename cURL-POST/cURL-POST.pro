QT += core
QT -= gui
QT += network
CONFIG += c++11

TARGET = TestCURL
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp
INCLUDEPATH += /usr/include/curl
LIBS +=/usr/local/lib/libcurl.so
