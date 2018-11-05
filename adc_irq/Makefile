CC      = arm-linux-gcc
LD      = arm-linux-ld
AR      = arm-linux-ar
OBJCOPY = arm-linux-objcopy
OBJDUMP = arm-linux-objdump

INCLUDEDIR 	:= $(shell pwd)/include 
CFLAGS 		:= -Wall -fno-builtin
CPPFLAGS   	:= -nostdinc -I$(INCLUDEDIR) -nostdlib

export 	CC AR LD OBJCOPY OBJDUMP INCLUDEDIR CFLAGS CPPFLAGS 

objs := start.o main.o uart.o int.o clock.o rtc.o lib/libc.a

rtc.bin: $(objs)
	${LD} -Tadc.lds -o rtc.elf $^
	${OBJCOPY} -O binary -S rtc.elf $@
	${OBJDUMP} -D rtc.elf > rtc.dis
	
%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

.PHONY : lig/libc.a
lib/libc.a:
	cd lib; make; cd ..
	
clean:
	make -C ./lib
	rm -f *.bin *.elf *.dis *.o
	
