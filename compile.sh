#!/bin/bash

./updateUI.sh
echo 

NUM_THREADS=16

command_exists () {
    type "$1" &> /dev/null ;
}

BUILD_MODE=$1
if [ -z BUILD_MODE ] 
then
	echo "Compile mode not specified. Picking EDITOR mode (default). If you want to change it, specify as first argument: EDITOR or GAME"
	echo 
	BUILD_MODE="EDITOR"
	exit 1
fi

# Make and run Bang
qmake "BUILD_MODE=${BUILD_MODE}"
if command_exists colormake
then
	colormake -j${NUM_THREADS}
else
	make -j${NUM_THREADS}
fi
