#-------------------------------------------------
#
# Project created by QtCreator 2018-09-07T16:30:04
#
#-------------------------------------------------

QT       -= core gui

TARGET = rasterdata
TEMPLATE = lib
CONFIG += staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        rasterdata.cpp \
    pixel.cpp

HEADERS += \
        rasterdata.h \
    pixel.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32{
    INCLUDEPATH += C:/OSGeo4W64/include
    LIBS+= -LC:/OSGeo4W64/lib -lgdal_i
} else {
    LIBS+= -lgdal
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../point/release/ -lpoint
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../point/debug/ -lpoint
else:unix: LIBS += -L$$OUT_PWD/../point/ -lpoint

INCLUDEPATH += $$PWD/../point
DEPENDPATH += $$PWD/../point

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../point/release/libpoint.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../point/debug/libpoint.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../point/release/point.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../point/debug/point.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../point/libpoint.a
