#!/bin/bash
cd "$(dirname "$0")"

cd .. # Go to root directory

NUM_THREADS=16

command_exists () {
    type "$1" &> /dev/null ;
}

BUILD_MODE=$1
if [ -z ${BUILD_MODE} ] 
then
	BUILD_MODE="EDITOR"
	echo "Compile mode not specified. Picking EDITOR mode (default)."
	echo "If you want to change it, specify as first argument: EDITOR or GAME"
	echo 
fi

# Make and run Bang
#echo "BUILD_MODE on compile.sh: '${BUILD_MODE}'"
qmake "BUILD_MODE=${BUILD_MODE}"
if [ $(command_exists colormake) ] && [ "${BUILD_MODE}" != "GAME" ] 
then
	colormake -j${NUM_THREADS}
else
	make -j${NUM_THREADS}
fi
