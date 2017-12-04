#!/bin/bash


function build(){
	gcc -o signalRecvService signalRecvService.c
	gcc -o sendUpdateSignal sendUpdateSignal.c
#	cp signalRecvService /home/harry/soc/code/software/os-platform/filesystem/fs/bin
#	cp signalRecvService /home/harry/soc/linux_share/out_file
#	cp sendUpdateSignal /home/harry/soc/linux_share/out_file
#	cp sendUpdateSignal /home/harry/soc/code/software/os-platform/filesystem/fs/bin
#	sleep 1
}

build
