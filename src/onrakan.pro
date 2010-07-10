QT       += core gui

TARGET = onrakan
TEMPLATE = app
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
RCC_DIR = build

SOURCES += main.cpp\
        mainwindow.cpp \
    optionsdialog.cpp \
    stereogram.cpp \
    stereogramparameters.cpp

HEADERS  += mainwindow.h \
    optionsdialog.h \
    stereogram.h \
    stereogramparameters.h

FORMS    += mainwindow.ui \
    optionsdialog.ui

RESOURCES += \
    onrakan.qrc
