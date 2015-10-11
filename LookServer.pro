TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += C++11

SOURCES += main.cpp \
    httpserver.cpp \
    workqueue.cpp \
    client.cpp

LIBS += -levent
LIBS += -lpthread

HEADERS += \
    httpserver.h \
    workqueue.h \
    client.h
