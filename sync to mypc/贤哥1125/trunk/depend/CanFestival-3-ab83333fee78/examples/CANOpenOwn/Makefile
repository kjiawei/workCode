#! gmake

#
# Copyright (C) 2006 Laurent Bessard
# 
# This file is part of canfestival, a library implementing the canopen
# stack
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
# 

CC = arm-none-linux-gnueabi-gcc
CXX = arm-none-linux-gnueabi-g++
LD = arm-none-linux-gnueabi-gcc
OPT_CFLAGS = -O2
CFLAGS = 
PROG_CFLAGS = 
EXE_CFLAGS =  -lpthread -lrt -lgcc_s -ldl
OS_NAME = Linux
ARCH_NAME = arm
PREFIX = /home/developer/workspace/CODE/software/app-software/trunk/depend/CanFestival-3-ab83333fee78/install
TARGET = unix
CAN_DRIVER = can_mcp2515
TIMERS_DRIVER = timers_unix
CANOPENSHELL = CANOpenShell
CANOPENSHELL_LIB = libCANOpenShell.a


INCLUDES = -I../../include -I../../include/$(TARGET) -I../../include/$(CAN_DRIVER) -I../../include/$(TIMERS_DRIVER)

MASTER_MAIN = main.o
MASTER_OBJS = ObjDict.o CANOpenShell.o deal.o

OBJS = ../../src/libcanfestival.a ../../drivers/$(TARGET)/libcanfestival_$(TARGET).a

ifeq ($(TARGET),win32)
	CANOPENSHELL = CANOpenShell.exe
endif

ifeq ($(TIMERS_DRIVER),timers_win32)
	EXE_CFLAGS =
endif

ifeq ($(TIMERS_DRIVER),timers_xeno)
	PROGDEFINES = -DUSE_XENO
endif

ifeq ($(TIMERS_DRIVER),timers_rtai)
	PROGDEFINES = -DUSE_RTAI
endif

all: $(CANOPENSHELL)

../../drivers/$(TARGET)/libcanfestival_$(TARGET).a:
	$(MAKE) -C ../../drivers/$(TARGET) libcanfestival_$(TARGET).a

$(CANOPENSHELL): $(CANOPENSHELL_LIB) $(MASTER_MAIN) $(OBJS)
	$(LD) $(CFLAGS) $(PROG_CFLAGS) ${PROGDEFINES} $(INCLUDES) -o $@ $(MASTER_MAIN) $(CANOPENSHELL_LIB) $(OBJS) $(EXE_CFLAGS)

$(CANOPENSHELL_LIB): $(MASTER_OBJS)
	@echo " "
	@echo "*********************************************"
	@echo "**building [CANOpenShell.a]"
	@echo "*********************************************"
	$(binutils_prefix)ar rc $@ $(MASTER_OBJS)
	$(binutils_prefix)ranlib $@

ObjDict.c: ObjDict.od
	$(MAKE) -C ../../objdictgen gnosis
	python ../../objdictgen/objdictgen.py ObjDict.od ObjDict.c

%.o: %.c
	$(CC) $(CFLAGS) $(PROG_CFLAGS) ${PROGDEFINES} $(INCLUDES) -o $@ -c $<

%.o: %.cpp
	$(CXX) $(CFLAGS) $(PROG_CFLAGS) ${PROGDEFINES} $(INCLUDES) -o $@ -c $<


clean:
	rm -f $(MASTER_OBJS) $(MASTER_MAIN)
	rm -f $(CANOPENSHELL)
	rm -f ObjDict.c
		
mrproper: clean
	rm -f ObjDict.c

install: $(CANOPENSHELL) $(CANOPENSHELL_LIB) 
	mkdir -p $(PREFIX)/bin/
	mkdir -p $(PREFIX)/lib/
	cp $(CANOPENSHELL) $(PREFIX)/bin/
	cp $(CANOPENSHELL_LIB) $(PREFIX)/lib/
	cp CANOpenShell.h $(PREFIX)/include/canfestival/
	cp deal.h $(PREFIX)/include/canfestival/
	
uninstall:
	rm -f $(PREFIX)/bin/$(CANOPENSHELL)
