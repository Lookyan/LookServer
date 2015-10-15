TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += C++11

SOURCES += main.cpp \
    httpserver.cpp \
    workqueue.cpp \
    client.cpp \
    httpparser.cpp \
    response.cpp

LIBS += -lpthread
LIBS += -levent
LIBS += -levent_pthreads

HEADERS += \
    httpserver.h \
    workqueue.h \
    client.h \
    httpparser.h \
    response.h
