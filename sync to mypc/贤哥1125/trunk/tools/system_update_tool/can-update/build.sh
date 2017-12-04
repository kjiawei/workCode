#!/bin/bash

# ----------------------------------------------------------
# Created by Jamesli 2014.08.11
# ChangeLog:
#
# ----------------------------------------------------------

echo "+----------------------------------------------------+"
echo "| can_update tool building...                   |"
echo "+----------------------------------------------------+"

#[[ -n $PATH_TOP ]]&&cd ${PATH_TOP}/app-software/trunk

#----------- app_test --------------------------------------
#[[ -n $PATH_TOP ]]&&cd ${PATH_TOP}/app-software/trunk/tools/system_update_tool

#cd can-update
make clean
qmake
make


#rm -rf ../../os-platform/filesystem/fs/usr/local/app/*
cp -r can-update  ../../../../../os-platform/filesystem/fs/bin/
echo "+----------------------------------------------------+"
echo "|   finished.                                        |"
echo "+----------------------------------------------------+"
