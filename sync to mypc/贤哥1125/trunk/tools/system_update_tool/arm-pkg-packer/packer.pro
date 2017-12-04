QT      = 
TARGET  = packer

TEMPLATE = app

INCLUDEPATH += . 

HEADERS += ./cmd.h \
           ./crc32.h \
             ./public.h 

SOURCES += ./cmd.c \
           ./crc32.c \ 
           ./packer.c

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
