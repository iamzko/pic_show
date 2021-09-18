QT       += core gui network testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
QMAKE_CXXFLAGS +=-mwindows
CONFIG -= console
QT.testlib.CONFIG -= console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mainwindow_test.cpp

HEADERS += \
    lib/cximage/include/xfile.h \
    lib/cximage/include/ximabmp.h \
    lib/cximage/include/ximacfg.h \
    lib/cximage/include/ximadef.h \
    lib/cximage/include/ximage.h \
    lib/cximage/include/ximagif.h \
    lib/cximage/include/ximaico.h \
    lib/cximage/include/ximaiter.h \
    lib/cximage/include/ximajas.h \
    lib/cximage/include/ximajbg.h \
    lib/cximage/include/ximajpg.h \
    lib/cximage/include/ximamng.h \
    lib/cximage/include/ximapcx.h \
    lib/cximage/include/ximapng.h \
    lib/cximage/include/ximapsd.h \
    lib/cximage/include/ximaraw.h \
    lib/cximage/include/ximaska.h \
    lib/cximage/include/ximatga.h \
    lib/cximage/include/ximath.h \
    lib/cximage/include/ximatif.h \
    lib/cximage/include/ximawbmp.h \
    lib/cximage/include/ximawmf.h \
    lib/cximage/include/xiofile.h \
    lib/cximage/include/xmemfile.h \
    mainwindow.h \
    mainwindow_test.h

FORMS += \
    mainwindow.ui

CONFIG(debug,debug|release){
    LIBS += \
    $$PWD/lib/cximage/debug/cximage.lib
}
else{
    LIBS += \
    $$PWD/lib/cximage/release/cximage.lib
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
