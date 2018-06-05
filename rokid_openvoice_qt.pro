TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

#QMAKE_RPATHDIR += $$PWD/lib/openvoice
LIBS +=   -lmad -lspeech -lPocoFoundation -lasound -lPocoNet -lPocoJSON -lpthread
#INCLUDEPATH += $$PWD/include/openvoice
INCLUDEPATH += $$PWD/include
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
    tools/http/httpdl.cpp \
    tools/json/jsonhandle.cpp \
    handlecallbacks.cpp \
    player/pcmplayer.cpp \
    netserver.cpp \
    net/netudpbase.cpp \
    responsehandle.cpp \
    httpplayer.cpp

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
    tools/json/jsonhandle.h \
    handlecallbacks.h \
    player/pcmplayer.h \
    netserver.h \
    wavfile.h \
    net/netudpbase.h \
    responsehandle.h \
    httpplayer.h

