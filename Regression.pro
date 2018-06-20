QT       -= gui

TARGET = autoOGS
CONFIG   += c++14
CONFIG   += warn_on
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    dataset.cpp

HEADERS += \
    dataset.h
