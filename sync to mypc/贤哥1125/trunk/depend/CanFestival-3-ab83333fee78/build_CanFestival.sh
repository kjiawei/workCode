#!/bin/bash

# ----------------------------------------------------------
# Created by  2012.08.06
# ChangeLog:
#
# ----------------------------------------------------------

echo "+----------------------------------------------------+"
echo "| CanFestival build...                               |"
echo "+----------------------------------------------------+"

if [ -z $1 ]
then
    exit 0
fi

if [ "HW_ARM"  ==  $1 ]; then
    ./configure --cc=arm-none-linux-gnueabi-gcc --cxx=arm-none-linux-gnueabi-g++ --ld=arm-none-linux-gnueabi-gcc --arch=arm --can=mcp2515 --disable-Ox --target=unix --timers=unix --prefix=`pwd`/install
    make clean
    make
    cp ./drivers/can_mcp2515/libcanfestival_can_mcp2515.so .
    cp -v ./drivers/can_mcp2515/libcanfestival_can_mcp2515.so ../../lib

elif [ "HW_X86"  ==  $1 ]; then
    ./configure --prefix=`pwd`/install
    make clean
    make
    cp ./drivers/can_virtual/libcanfestival_can_virtual.so .
    cp -v ./drivers/can_virtual/libcanfestival_can_virtual.so ../../lib
fi

echo "+----------------------------------------------------+"
echo "|   finished.                                        |"
echo "+----------------------------------------------------+"
