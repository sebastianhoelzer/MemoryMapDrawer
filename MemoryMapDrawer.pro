#-------------------------------------------------
#
# Project created by QtCreator 2015-06-23T20:13:09
#
#-------------------------------------------------

QT       += core gui
QT       += svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MemoryMapDrawer
TEMPLATE = app


SOURCES += main.cpp\
    MemoryRangesModel.cpp \
    MemoryRange.cpp \
    DrawingCanvas.cpp \
    MemoryRangeDialog.cpp \
    MapData.cpp \
    AboutDialog.cpp \
    EditorWindow.cpp \
    MapDataFileReadWriter.cpp

HEADERS  += \
    MemoryRangesModel.h \
    MemoryRange.h \
    DrawingCanvas.h \
    MemoryRangeDialog.h \
    MapData.h \
    AboutDialog.h \
    EditorWindow.h \
    MapDataFileReadWriter.h

FORMS    += mainwindow.ui \
    aboutdialog.ui

RESOURCES += \
    images.qrc
