QT -= gui
CONFIG += console static
CONFIG -= app_bundle
TEMPLATE = app
TARGET = Launcher
SOURCES += main.cpp
LIBS += -lwinmm
QMAKE_CXXFLAGS += -std=c++17
