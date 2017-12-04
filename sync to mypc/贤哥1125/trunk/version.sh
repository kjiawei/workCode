#!/bin/bash

TARGET_FILE=include/version.h
__BUILD="BUILD_TIME "`date +\"%Y.%m.%d-%H:%M:%S\"`""
echo "sed -i \"s/BUILD_TIME .*$/__BUILD/g\" $TARGET_FILE"
sed -i "s/BUILD_TIME .*$/$__BUILD/g" $TARGET_FILE

__XML=`svn info --xml --incremental`
__REV=`echo "$__XML"|sed '/revision/!d'|sed '$d'`
__REV_STR="REV "`echo $__REV|sed 's/revision="\([0-9]\+\)">\?/\1/'`""
echo "sed -i \"s/REV .*$/__REV_STR/g\" $TARGET_FILE"
sed -i "s/REV .*$/$__REV_STR/g" $TARGET_FILE

