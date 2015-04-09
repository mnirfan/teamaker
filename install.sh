#!/bin/bash
user=`whoami`
if [ $user == "root" ]; then
	echo -e "\033[31m All files in destination will be overwriten, continue?\e[00m \n1. Yes\nelse. No\n"
	read -r confirm
	if [ $confirm == '1' ]; then
		gcc `pkg-config --cflags gtk+-3.0` ./Files/main.c -o ./Files/teamaker `pkg-config --libs gtk+-3.0`
		cp -f ./Files/teamaker /usr/bin/
		chmod 755 /usr/bin/teamaker
		mkdir /usr/share/TEA/
		mkdir /usr/share/TEA/UI
		mkdir /usr/share/TEA/engine
		mkdir /usr/share/TEA/profiles
		cp -f Files/status/* /usr/share/TEA/profiles
		cp -f Files/tea_package_maker.ui /usr/share/TEA/UI
		cp -f Files/teamaker1 /usr/share/TEA/engine/
		cp -f Files/teamaker2 /usr/share/TEA/engine/
		cp -f Files/teamaker.desktop /usr/share/applications/
		cp -f Files/teamaker.svg /usr/share/icons/
		chmod 755 /usr/share/TEA/*
		chmod 755 /usr/share/TEA/engine/*
		chmod 755 /usr/share/applications/teamaker.desktop
		rm Files/teamaker
	else
		echo -e "operation aborted\n"
	fi
else
	echo -e "I think only root can run this script.\nwhy don't you try it?\n"
fi
