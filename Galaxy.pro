######################################################################
# Automatically generated by qmake (2.01a) wo feb. 9 14:49:08 2011
######################################################################

TEMPLATE = app
TARGET = Galaxy
DEPENDPATH += .
INCLUDEPATH += .

QT += opengl
LIBS += -lOpenCL
DEFINES += GL_GLEXT_PROTOTYPES

OBJECTS_DIR = ./build
MOC_DIR = ./build
RCC_DIR = ./build
UI_DIR = ./build

# Input
HEADERS +=\
    main_window.h\
    gl_widget.h\
    camera.h\
    simulator.h\


SOURCES +=\
    main.cpp\
    main_window.cpp\
    gl_widget.cpp\
    camera.cpp\
    simulator.cpp\

FORMS += main_window.ui
