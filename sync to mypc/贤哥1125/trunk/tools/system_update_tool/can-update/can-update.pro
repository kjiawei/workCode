TEMPLATE = app
TARGET = can-update
DEPENDPATH += .
INCLUDEPATH += .
LIBS += -lts -lz
#CONFIG += debug

# Input
HEADERS += can.h config_updatefw.h file_hex.h protocol.h updatefw.h scan_information.h
SOURCES += can.c file_hex.c updatefw.c main.c scan_information.c \
    sendUpdateSignal.c
