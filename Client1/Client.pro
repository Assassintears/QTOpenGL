#-------------------------------------------------
#
# Project created by QtCreator 2019-09-11T11:03:08
#
#-------------------------------------------------

QT       += core gui widgets network sql

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        cdata.cpp \
        main.cpp \
        mainwindow.cpp \
        openglwidget.cpp \
        profile.cpp

HEADERS += \
    cdata.h \
        mainwindow.h \
        openglwidget.h \
        profile.h \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc \
    shaders.qrc


win32: LIBS += -L$$PWD/./ -lfreetype

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/./freetype.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/./libfreetype.a

INCLUDEPATH += $$PWD/./freetype
DEPENDPATH += $$PWD/./freetype

CONFIG += debug

DISTFILES +=
