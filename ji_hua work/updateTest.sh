#!/bin/bash
#$0执行的文件名 #$1第一个参数
if [ ! "$1" ];then
	echo "usage:"$0" [1:update;2:test]"
	exit
else 
	if [ "$1" == "1" ];then
			ps=`pgrep hemodialysis` #是否在运行中
			[[ -z ${ps} ]]||killall -9 hemodialysis
			echo "finish kill hemodialysis"
			if [[ ! -d "/mnt/nfs/hemodialysis/" ]];then #是否已挂载
				echo "mounting directory..."
				mount -o nolock -t nfs 192.168.123.245:/work/nfs /mnt/nfs
				echo "finish mounting directory" 
				#umount -v /mnt/nfs/
			fi
			echo "start copy new software from virtual..."
			cp /mnt/nfs/hemodialysis/hemodialysis /usr/local/app/
			sleep 5s
			echo "finish copy software to target directory"
			
			if [ $(cd `dirname $0`; pwd) != "/usr/local/app" ];then
				#echo `pwd`
				cd /usr/local/app/
				echo "entering /usr/local/app/ ..."
			fi
			./hemodialysis -qws &
			echo "copy new finish...restarting..."
	else
		if [  "$1" == "2"  ];then
			echo "test"
		fi
	fi
fi

