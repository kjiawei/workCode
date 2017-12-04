#!/bin/sh
arm-none-linux-gnueabi-gcc  arm-update.c crc32.c signalProgress.c unpack.c can.c file_hex.c updatefw.c -o arm-update

