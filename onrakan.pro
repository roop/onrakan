TEMPLATE = app
QT += core gui

TARGET = onrakan
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
RCC_DIR = build

SOURCES += src/main.cpp\
    src/mainwindow.cpp \
    src/optionsdialog.cpp \
    src/stereogram.cpp \
    src/stereogramparameters.cpp

HEADERS  += src/mainwindow.h \
    src/optionsdialog.h \
    src/stereogram.h \
    src/stereogramparameters.h

FORMS    += src/mainwindow.ui \
    src/optionsdialog.ui

RESOURCES += \
    onrakan.qrc
