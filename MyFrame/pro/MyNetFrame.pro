TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpthread -ldl -lrt
QMAKE_CXXFLAGS += -fno-stack-protector -Wno-reorder

SOURCES += \
    ../src/Common.cpp \
    ../src/MyThread.cpp \
    ../src/MyEvent.cpp \
    ../src/MyApp.cpp \
    ../src/MyLog.cpp \
    ../src/MyHelp.cpp \
    ../src/MyTask.cpp \
    ../src/MyList.cpp \
    ../src/MyControls.cpp \
    ../src/MyNet.cpp \
    ../src/MySock.cpp \
    ../src/MyUdp.cpp \
    ../src/MyTcp.cpp \
    ../src/MyNormalEvent.cpp \
    ../src/MyTimer.cpp \
    ../src/MyAllEvent.cpp \
    ../src/MyBase64.cpp \
    ../src/MyMouseEvent.cpp \
    ../src/MyKeyEvent.cpp \
    ../src/MyTFTP.cpp \
    ../src/MyFileEvent.cpp \
    ../src/MyTest.cpp \
    ../src/MyICMP.cpp \
    ../src/MyRaw.cpp \
    ../src/MyWriteable.cpp \
    ../src/MySqlite3.cpp \
    ../../3rd_src/Sqlite3/sqlite3.c \
    ../src/main.cpp

HEADERS += \
    ../inc/Common.h \
    ../inc/MyHelp.h \
    ../inc/MyLog.h \
    ../inc/MyFrame.h \
    ../inc/MyApp.h \
    ../inc/MyList.h \
    ../inc/MyTask.h \
    ../inc/MyThread.h \
    ../inc/MyEvent.h \
    ../inc/MyTimer.h \
    ../inc/MySock.h \
    ../inc/MyNet.h \
    ../inc/MyNormalEvent.h \
    ../inc/MyControls.h \
    ../inc/MyUdp.h \
    ../inc/MyTcp.h \
    ../inc/MyVec.h \
    ../inc/MyBase64.h \
    ../inc/MyHeap.h \
    ../inc/MyHash.h \
    ../inc/MyWriteable.h \
    ../inc/MyMouseEvent.h \
    ../inc/MyKeyEvent.h \
    ../inc/MyAllEvent.h \
    ../inc/MyTFTP.h \
    ../inc/MyFileEvent.h \
    ../inc/MyTest.h \
    ../inc/MyICMP.h \
    ../inc/MyRaw.h \
    ../inc/MySqlite3.h \
    ../../3rd_src/Sqlite3/sqlite3.h \
    ../../3rd_src/Sqlite3/sqlite3ext.h \
DISTFILES +=
