TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_RPATHDIR += $$PWD/lib/openvoice
QMAKE_RPATHDIR += $$PWD/lib/
LIBS +=   -lspeech -lPocoFoundation
INCLUDEPATH += $$PWD/include/openvoice
QMAKE_CXXFLAGS +=  -std=c++11
SOURCES += main.cpp \
    filehandler.cpp \
    speechsdk.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    filehandler.h \
    speechsdk.h

