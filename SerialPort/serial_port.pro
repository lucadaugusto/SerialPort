TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH +=/usr/include
LIBS += \
        -lboost_system\
        -lboost_thread\

SOURCES += main.cpp \
    serial.cpp \
    easylogging++.cpp \
    device.cpp

HEADERS += \
    serial.h \
    defs.h \
    easylogging++.h \
    device.h
