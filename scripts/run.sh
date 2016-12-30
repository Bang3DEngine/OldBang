#!/bin/bash
cd "$(dirname "$0")"	

BUILD_MODE=$1

./compile.sh $1


if [ $? -ne 0 ]; then
	exit 1
fi

if [ "$BUILD_MODE" = "GAME" ]
then
	gdb -ex run ../bin/Game.exe
else
	gdb -ex run ../bin/BangEditor.exe
fi
