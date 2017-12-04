TEMPLATE = app
TARGET = gsnap
DEPENDPATH += .
#INCLUDEPATH += . (...)/libpng-1.5.13/install/include (...)/jpeg-6b/install/include
CONFIG -= qt
LIBS += -lz -lpng -ljpeg
#LIBS += -L(...)/jpeg-6b/install/lib -ljpeg -L(...)/libpng-1.5.13/install/lib -lpng

# Input
SOURCES += gsnap.c
