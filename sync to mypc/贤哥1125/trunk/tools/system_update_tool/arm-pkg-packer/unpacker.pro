QT      = 
TARGET  = unpacker

TEMPLATE = app

INCLUDEPATH += . 

HEADERS += ./public.h \
           ./crc32.h 

SOURCES += ./unpacker.c \
           ./crc32.c

LIBS  +=

CONFIG += warn_on release

CONFIG(debug, debug|release) {
    DESTDIR = ./debug
    OBJECTS_DIR = ./objs_debug
} 
else {
    DESTDIR = ./release
    OBJECTS_DIR = ./objs_release
}
