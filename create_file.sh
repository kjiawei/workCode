#!/bin/bash

#create c/cpp file with init include
#usage:./new.sh xxx.c/xxx.cpp

path=$1
filename=${path##[a-zA-Z0-9_]*.}
if [ $filename = "c" ] || [ $filename = "cpp" ] &&[ -e $1 ] 
then
    echo "file exist"
	vim $1
else 
    touch $1
	path2=$1
	filename2=${path##[a-zA-Z0-9_]*.}
	if [ $filename2 = "cpp" ] && [ -e $1 ]
	then
    echo "#include <iostream>" >$1
	echo " /*" >> $1
	echo " author:jerry" >> $1
	echo " date:`date`" >> $1
	echo " */" >> $1
	echo "int main()" >> $1   #add time    
	echo "{" >> $1
	echo " " >> $1
	echo "    return 0;" >> $1
	echo "}" >> $1
	elif [ $filename2 = "c" ] && [ -e $1 ] 	
	then
		echo "#include \"myhead.h\"" >$1
		echo " /*" >> $1
		echo " author:jerry" >> $1
		echo " date:`date`" >> $1
		echo " */" >> $1
		echo "int main()" >> $1
		echo "{" >> $1
		echo " " >> $1
		echo "    return 0;" >> $1
		echo "}" >> $1
	fi
vim $1
fi