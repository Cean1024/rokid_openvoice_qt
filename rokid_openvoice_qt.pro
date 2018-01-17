TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_RPATHDIR += $$PWD/lib/openvoice
LIBS +=  -L$$PWD/lib/ -lmad -lspeech -lPocoFoundation -lasound -lPocoNet -lPocoJSON
INCLUDEPATH += $$PWD/include/openvoice
QMAKE_CXXFLAGS +=  -std=c++11
SOURCES += main.cpp \
    filehandler.cpp \
    speechsdk.cpp \
    ttssdk.cpp \
    player/player.cpp \
    player/mp3decode.cpp \
    player/alsahandle.cpp \
    tools/linklist/linklist.cpp \
    player/siglelist.cpp \
    player/decodecallback.cpp \
    tools/http/httpdl.cpp \
    tools/json/jsonhandle.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    filehandler.h \
    speechsdk.h \
    ttssdk.h \
    common.h \
    player/player.h \
    player/mp3decode.h \
    player/alsahandle.h \
    tools/linklist/linklist.h \
    player/siglelist.h \
    tools/http/httpdl.h \
    tools/json/jsonhandle.h

