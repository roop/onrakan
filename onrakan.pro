QT       += core gui

TARGET = onrakan
TEMPLATE = app
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
RCC_DIR = build

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    onrakan.qrc
