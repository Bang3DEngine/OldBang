#!/bin/bash

./updateUI.sh
echo 

NUM_THREADS=16

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
	colormake -j${NUM_THREADS}
else
	make -j${NUM_THREADS}
fi
