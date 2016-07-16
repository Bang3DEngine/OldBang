#!/bin/bash

./updateUI.sh
echo 

if [ -z $1 ] 
then
	echo "[ ERROR ]: Please specify as first argument: EDITOR or GAME"
	echo 
	exit 1
fi

# Make and run BangPreprocessor
cd BangPreprocessor
qmake
(make -j4 && ./BangPreprocessor) || exit -1
cd ..

# Make and run Bang
qmake "BUILD_MODE=$1"
make -j4
