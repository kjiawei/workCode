#!/bin/bash

# ----------------------------------------------------------
# Created by Jamesli 2014.08.11
# ChangeLog:
#
# ----------------------------------------------------------

echo "+----------------------------------------------------+"
echo "| update_ui tool building...                   |"
echo "+----------------------------------------------------+"

#[[ -n $PATH_TOP ]]&&cd ${PATH_TOP}/app-software/trunk

#----------- app_test --------------------------------------
#[[ -n $PATH_TOP ]]&&cd ${PATH_TOP}/app-software/trunk/tools/system_update_tool

#cd can-update
make clean
qmake
make

rm -rf target
mkdir -p target

cp  update-ui target


#rm -rf ../../os-platform/filesystem/fs/usr/local/app/*
cp -r target/* ../../../../../os-platform/filesystem/fs/bin/
cp -r target/* ../../../../../os-platform/filesystem/fs/usr/bin/
rm -rf target
echo "+----------------------------------------------------+"
echo "|   finished.                                        |"
echo "+----------------------------------------------------+"
