#!/bin/bash

./updateUI.sh
echo 

command_exists () {
    type "$1" &> /dev/null ;
}

if [ -z $1 ] 
then
	echo "[ ERROR ]: Please specify as first argument: EDITOR or GAME"
	echo 
	exit 1
fi

# Make and run Bang
qmake "BUILD_MODE=$1"
if command_exists colormake
then
	colormake -j8
else
	make -j8
fi
